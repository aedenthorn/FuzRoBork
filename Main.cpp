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

		/*
		if (type == "SET_VARS") {
		int x = 2; // start with third argument
		if (a[x].GetType() == GFxValue::kType_Number)
		kMsPerWordSilence = a[x++].GetNumber(); // msVal
		if (a[x].GetType() == GFxValue::kType_Bool)
		kSkipEmptyResponses = a[x++].GetBool(); // skipVal
		if (a[x].GetType() == GFxValue::kType_Bool)
		kPlayPlayerDialogue = a[x++].GetBool(); // playPlayerVal
		if (a[x].GetType() == GFxValue::kType_Bool)
		kPlayNPCDialogue = a[x++].GetBool(); // playNPCVal
		if (a[x].GetType() == GFxValue::kType_Bool)
		kPlayBookPages = a[x++].GetBool(); // playBookVal
		if (a[x].GetType() == GFxValue::kType_Bool)
		kPlayHUDMessages = a[x++].GetBool(); // playHUDVal
		if (a[x].GetType() == GFxValue::kType_Bool)
		kUseXMLOverrides = a[x++].GetBool(); // useXMLVal
		if (a[x].GetType() == GFxValue::kType_String)
		kPlayerLanguageLCID = a[x++].GetString(); // pLangVal
		if (a[x].GetType() == GFxValue::kType_String)
		kFemaleLanguageLCID = a[x++].GetString(); // fLangVal
		if (a[x].GetType() == GFxValue::kType_String)
		kMaleLanguageLCID = a[x++].GetString(); // mLangVal
		if (a[x].GetType() == GFxValue::kType_String)
		kNarratorLanguageLCID = a[x++].GetString(); // nLangVal
		if (a[x].GetType() == GFxValue::kType_Number)
		kPlayerVoiceRate = a[x++].GetNumber(); // pRateVal
		if (a[x].GetType() == GFxValue::kType_Number)
		kFemaleVoiceRate = a[x++].GetNumber(); // fRateVal
		if (a[x].GetType() == GFxValue::kType_Number)
		kMaleVoiceRate = a[x++].GetNumber(); // mRateVal
		if (a[x].GetType() == GFxValue::kType_Number)
		kNarratorVoiceRate = a[x++].GetNumber(); // nRateVal
		if (a[x].GetType() == GFxValue::kType_Number)
		kPlayerVoiceVolume = a[x++].GetNumber(); // pVolVal
		if (a[x].GetType() == GFxValue::kType_Number)
		kFemaleVoiceVolume = a[x++].GetNumber(); // fVolVal
		if (a[x].GetType() == GFxValue::kType_Number)
		kMaleVoiceVolume = a[x++].GetNumber(); // mVolVal
		if (a[x].GetType() == GFxValue::kType_Number)
		kNarratorVoiceVolume = a[x++].GetNumber(); // nVolVal
		if (a[x].GetType() == GFxValue::kType_Number)
		kNarratorSex = a[x++].GetNumber(); // nSexVal
		}
		*/

		_MESSAGE(("SKSEScaleform_BorkFunction received " + type).c_str());

		if (type == "DIALOGUE_CLICK") {
			FuzRoBorkNamespace::stopSpeaking();
			FuzRoBorkNamespace::startPlayerSpeech(speech);
		}
		else if (type == "DIALOGUE" && kPlayPlayerDialogue.GetData().i == 1)
			FuzRoBorkNamespace::startPlayerSpeech(speech);
		else if (type == "BOOK_PAGE" && kPlayBookPages.GetData().i == 1) {
			FuzRoBorkNamespace::stopSpeaking();
			FuzRoBorkNamespace::startBookSpeech(speech);
		}
		else if (type == "BOOK_CLICK") {
			FuzRoBorkNamespace::stopSpeaking();
			FuzRoBorkNamespace::startBookSpeech(speech);
		}
		if (type == "LOADING_SCREEN" && kPlayLoadingScreenText.GetData().i == 1) {
			FuzRoBorkNamespace::speakLoadingScreen(speech);
		}
		else if (type == "CHECK_DONE") { // check whether speech is finished
			bool isDone = !FuzRoBorkNamespace::isSpeaking();
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


