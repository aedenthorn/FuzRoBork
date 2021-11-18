#include "FuzRoBorkInternals.h"
#include "Hooks.h"
#include "VersionInfo.h"
#include <ShlObj.h>
#include "skse64/ScaleformValue.h"
#include "skse64/ScaleformCallbacks.h"

#define RUNTIME_VERSION RUNTIME_VERSION_1_5_97

PluginHandle g_pluginHandle;

SKSEScaleformInterface* g_scaleform = NULL;
SKSESerializationInterface* g_serialization = NULL;
SKSEPapyrusInterface* g_papyrus = NULL;

extern "C"
{
	void MessageHandler(SKSEMessagingInterface::Message * msg)
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
				_MESSAGE("%s Initialized!", MakeSillyName().c_str());
				FuzRoBorkNamespace::LoadXML();
				FuzRoBorkNamespace::ImportTranslationFiles();
		}
			break;
		}
	}

	class SKSEScaleform_BorkFunction : public GFxFunctionHandler
	{
	public:
		virtual void	Invoke(Args* args);
	};

	void SKSEScaleform_BorkFunction::Invoke(Args* args)
	{


		GFxValue* a = args->args;

		const char* speech = "";

		if (a[1].GetType() == GFxValue::kType_String) // text to speak
			speech = a[0].GetString();

		std::string type = "";

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
		}
		else if (type == "STOP") {
			FuzRoBorkNamespace::stopSpeaking();
		}
	}

	bool RegisterScaleform(GFxMovieView* view, GFxValue* root)
	{
		RegisterFunction <SKSEScaleform_BorkFunction>(root, view, "BorkFunction");
		return true;
	}

	bool SKSEPlugin_Query(const SKSEInterface * skse, PluginInfo * info)
	{
		gLog.OpenRelative(CSIDL_MYDOCUMENTS, "\\My Games\\Skyrim Special Edition\\SKSE\\FuzRoBork.log");

		_MESSAGE("%s Initializing...", MakeSillyName().c_str());

		// populate info structure
		info->infoVersion = PluginInfo::kInfoVersion;
		info->name = "Fuz Ro D'oh";
		info->version = PACKED_SME_VERSION;

		interfaces::kPluginHandle = skse->GetPluginHandle();
		interfaces::kMsgInterface = (SKSEMessagingInterface*)skse->QueryInterface(kInterface_Messaging);

		if (skse->isEditor)
			return false;
		else if (skse->runtimeVersion != RUNTIME_VERSION)
		{
			_MESSAGE("Unsupported runtime version %08X", skse->runtimeVersion);
			return false;
		}
		else if (!interfaces::kMsgInterface)
		{
			_MESSAGE("Couldn't initialize messaging interface");
			return false;
		}
		else if (interfaces::kMsgInterface->interfaceVersion < 2)
		{
			_MESSAGE("Messaging interface too old (%d expected %d)", interfaces::kMsgInterface->interfaceVersion, 2);
			return false;
		}


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
		return true;
	}

	bool SKSEPlugin_Load(const SKSEInterface * skse)
	{
		bool btest = g_papyrus->Register(FuzRoBorkNamespace::RegisterFuncs);
		if (btest)
			_MESSAGE("Register papyrus methods Succeeded");

		btest = g_scaleform->Register("FuzRoBork", RegisterScaleform);
		if (btest)
			_MESSAGE("Register scaleform method Succeeded");



		_MESSAGE("Initializing INI Manager");
		FuzRoBorkINIManager::Instance.Initialize("Data\\SKSE\\Plugins\\FuzRoBork.ini", nullptr);

		if (interfaces::kMsgInterface->RegisterListener(interfaces::kPluginHandle, "SKSE", MessageHandler) == false)
		{
			_MESSAGE("Couldn't register message listener");
			return false;
		}
		else if (InstallHooks() == false)
			return false;

		return true;
	}

};


