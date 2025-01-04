#include "FuzRoBorkInternals.h"
#include "Hooks.h"
#include "VersionInfo.h"
#include <ShlObj.h>
#include "skse64/ScaleformValue.h"
#include "skse64/ScaleformCallbacks.h"

PluginHandle g_pluginHandle;

SKSEScaleformInterface* g_scaleform = NULL;
SKSESerializationInterface* g_serialization = NULL;
SKSEPapyrusInterface* g_papyrus = NULL;

extern "C"
{
	__declspec(dllexport) SKSEPluginVersionData SKSEPlugin_Version =
	{
		SKSEPluginVersionData::kVersion,

		PACKED_SME_VERSION,
		"Fuz Ro Bork",
		"aedenthorn",
		"",
		0,	// Version-dependent
		0,
		{ RUNTIME_VERSION_1_6_1170, 0 },
		0,
	};

	class SKSEScaleform_BorkFunction : public GFxFunctionHandler
	{
	public:
		virtual void	Invoke(Args* args);
	};

	void SKSEScaleform_BorkFunction::Invoke(Args* args)
	{
		GFxValue* a = args->args;

		string sspeech = "";

		if (a[1].GetType() == GFxValue::kType_String) {// text to speak
			sspeech = string(a[0].GetString());

			//_MESSAGE("Raw speech: %s", sspeech.c_str());
		} 
		const char* speech = sspeech.c_str();
		string type = "";

		if (a[1].GetType() == GFxValue::kType_String) // call type
			type = a[1].GetString();

		if (type != "CHECK_DONE")
			_MESSAGE(("SKSEScaleform_BorkFunction received " + type).c_str());

		if (type == "DIALOGUE_CLICK") {
			FuzRoBorkNamespace::stopSpeaking();
			FuzRoBorkNamespace::startPlayerSpeech(speech);
		}
		else if (type == "DIALOGUE" && kPlayPlayerDialogue.GetData().i == 1)
			FuzRoBorkNamespace::startPlayerSpeech(speech);
		else if (type == "BOOK_READ" && kPlayBookPages.GetData().i == 1) {
			FuzRoBorkNamespace::stopSpeaking();
			FuzRoBorkNamespace::startBookSpeech(speech);
		}
		else if (type == "BOOK_BOOK") {
			FuzRoBorkNamespace::storeBookSpeech(speech);
		}
		else if (type == "BOOK_PAGES_FIRST") {
			FuzRoBorkNamespace::storeFirstPagesSpeech(speech);
		}
		else if (type == "BOOK_PAGES") {
			FuzRoBorkNamespace::storePagesSpeech(speech);
		}
		else if (type == "LOADING_SCREEN" && kPlayLoadingScreenText.GetData().i == 1) {
			FuzRoBorkNamespace::speakLoadingScreen(speech);
		}
		else if (type == "CHECK_DONE") { // check whether speech is finished
			bool isDone = !FuzRoBorkNamespace::isSpeaking() && !FuzRoBorkNamespace::isXVASpeaking();
			args->result->type = GFxValue::kType_String;
			args->result->data.string = isDone ? "YES" : "NO";
			if (isDone) {
				_MESSAGE("Done speaking");
			}
			else {
				_MESSAGE("Still speaking");
			}
		}
		else if (type == "STOP") {
			FuzRoBorkNamespace::stopSpeaking();
		}
	}

	__declspec(dllexport) bool RegisterScaleform(GFxMovieView* view, GFxValue* root)
	{
		RegisterFunction <SKSEScaleform_BorkFunction>(root, view, "BorkFunction");
		return true;
	}

	__declspec(dllexport) void MessageHandler(SKSEMessagingInterface::Message* msg)
	{
		switch (msg->type)
		{
			case SKSEMessagingInterface::kMessage_InputLoaded:
			{
				// schedule a cleanup thread for the subtitle hasher
				std::thread CleanupThread([]() {
					while (true)
					{
						std::this_thread::sleep_for(std::chrono::seconds(2));
						SubtitleHasher::Instance.Tick();
					}
				});
				CleanupThread.detach();

				_MESSAGE("Scheduled cleanup thread");
				
				std::thread CheckSpeechThread([]() {
					SpeakObj obj;
					while (true)
					{
						//_MESSAGE("checking for speech; speaking: %s, queue: %s", (!FuzRoBorkNamespace::isSpeaking() && !FuzRoBorkNamespace::isXVASpeaking()) ? "false" : "true", FuzRoBorkNamespace::GetSpeechFromQueue(obj) ? "yes" : "no");
						if (!FuzRoBorkNamespace::isSpeaking() && !FuzRoBorkNamespace::isXVASpeaking() && FuzRoBorkNamespace::GetSpeechFromQueue(obj)) {

							_MESSAGE("Speaking queued speech %s", obj.speech.c_str());
							actionSpeaking = true;
							thread(FuzRoBorkNamespace::speakTask, obj).detach();
							FuzRoBorkNamespace::EraseFromQueue();
						}
						std::this_thread::sleep_for(100ms);
					}
				});
				CheckSpeechThread.detach();

				_MESSAGE("%s Initialized!", MakeSillyName().c_str());
				FuzRoBorkNamespace::LoadXML();
				FuzRoBorkNamespace::ImportTranslationFiles();
			}
			break;
		}
	}

	__declspec(dllexport) bool SKSEPlugin_Load(const SKSEInterface* skse)
	{
		gLog.OpenRelative(CSIDL_MYDOCUMENTS, "\\My Games\\Skyrim Special Edition\\SKSE\\FuzRoBork.log");

		_MESSAGE("%s Initializing...", MakeSillyName().c_str());

		interfaces::kPluginHandle = skse->GetPluginHandle();
		interfaces::kMsgInterface = (SKSEMessagingInterface*)skse->QueryInterface(kInterface_Messaging);

		if (!interfaces::kMsgInterface)
		{
			_MESSAGE("Couldn't initialize messaging interface");
			return false;
		}
		else if (interfaces::kMsgInterface->interfaceVersion < 2)
		{
			_MESSAGE("Messaging interface too old (%d expected %d)", interfaces::kMsgInterface->interfaceVersion, 2);
			return false;
		}

		_MESSAGE("Initializing INI Manager");
		FuzRoBorkINIManager::Instance.Initialize("Data\\SKSE\\Plugins\\FuzRoBork.ini", nullptr);

		if (interfaces::kMsgInterface->RegisterListener(interfaces::kPluginHandle, "SKSE", MessageHandler) == false)
		{
			_MESSAGE("Couldn't register message listener");
			return false;
		}
		else if (InstallHooks() == false)
			return false;

		g_scaleform = static_cast<SKSEScaleformInterface*>(skse->QueryInterface(kInterface_Scaleform));
		if (!g_scaleform)
		{
			_FATALERROR("couldn't get scaleform interface");
			return false;
		}

		g_papyrus = static_cast<SKSEPapyrusInterface*>(skse->QueryInterface(kInterface_Papyrus));
		if (!g_papyrus)
		{
			_FATALERROR("couldn't get papyrus interface");
			return false;
		}

		bool btest = g_papyrus->Register(FuzRoBorkNamespace::RegisterFuncs);
		if (btest)
			_MESSAGE("Register papyrus methods Succeeded");
		else {
			_MESSAGE("Register papyrus methods failed");
			return false;
		}
		btest = g_scaleform->Register("FuzRoBork", RegisterScaleform);
		if (btest)
			_MESSAGE("Register scaleform method Succeeded");
		else {
			_MESSAGE("Register scaleform methods failed");
			return false;
		}

		return true;
	}
};


