#include "FuzRoBorkInternals.h"
#include "Hooks.h"
#include "VersionInfo.h"
#include <ShlObj.h>
#include "skse64/ScaleformValue.h"
#include "skse64/ScaleformCallbacks.h"
#include "include/detour.h"

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
		{ RUNTIME_VERSION_1_6_353, 0 },
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

		if (a[1].GetType() == GFxValue::kType_String) // text to speak
			sspeech = a[0].GetString();

		wstring_convert<codecvt_utf8_utf16<wchar_t>> converter;
		wstring wspeech = converter.from_bytes(sspeech).c_str();
		const wchar_t* speech = wspeech.c_str();

		string type = "";

		if (a[1].GetType() == GFxValue::kType_String) // call type
			type = a[1].GetString();

		if (type != "CHECK_DONE")
			_MESSAGE(("SKSEScaleform_BorkFunction received " + type).c_str());

		if (type == "BOOK_READ" && kPlayBookPages.GetData().i == 1) {
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
			if (isDone) {
				_MESSAGE("Done speaking");
			}
			else {
				_MESSAGE("Still speaking");
			}
			args->result->type = GFxValue::kType_String;
			args->result->data.string = isDone ? "YES" : "NO";
		}
		else if (type == "STOP") {
			FuzRoBorkNamespace::stopSpeaking();
		}
	}

	// from https://github.com/guitarino/skyrim-tts-voiced-player/blob/master/main.cpp

	BYTE* gOnTopicSetter = (BYTE*)0x00674113;
	BYTE* gOnTopicSetterResume;
	BYTE* gOnDialogueSay = (BYTE*)0x006D397E;
	BYTE* gOnDialogueSayResume;
	BYTE* gOnDialogueSaySkip = (BYTE*)0x006D39C4;


	enum PlayerSpeechState {
		TOPIC_NOT_SELECTED = 0,
		TOPIC_SELECTED = 1,
		TOPIC_SPOKEN = 2
	};

	struct PlayerSpeech {
		PlayerSpeechState state;
		bool isNPCSpeechDelayed;
		UInt32 option;
	};

	PlayerSpeech* gPlayerSpeech = NULL;

	void initializePlayerSpeech() {
		if (gPlayerSpeech == NULL) {
			gPlayerSpeech = new PlayerSpeech();
			gPlayerSpeech->state = TOPIC_NOT_SELECTED;
			gPlayerSpeech->isNPCSpeechDelayed = false;
		}
	}

	bool __stdcall shouldDelayNPCSpeech() {


		// This is for when the user wants to skip the convo by (usually vigorously) clicking
		if (gPlayerSpeech->state == TOPIC_SELECTED && gPlayerSpeech->isNPCSpeechDelayed) {
			gPlayerSpeech->state = TOPIC_NOT_SELECTED;
			gPlayerSpeech->isNPCSpeechDelayed = false;
			FuzRoBorkNamespace::stopSpeaking();
		}

		else if (gPlayerSpeech->state == TOPIC_SELECTED) {
			gPlayerSpeech->isNPCSpeechDelayed = true;
			return true;
		}

		return false;
	}

	__declspec(naked) void onDialogueSayHooked() {
		__asm {
			pushad
			call shouldDelayNPCSpeech
			test al, al
			jnz DELAY_NPC_SPEECH // If should delay NPC speech, go to some code after
			popad
			jmp[gOnDialogueSayResume]

			DELAY_NPC_SPEECH:
			popad
				jmp[gOnDialogueSaySkip]
		}
	}

	struct ObjectWithMessage {
		const char* message;
	};

	struct ObjectWithObjectWithMessage {
		ObjectWithMessage* object;
	};

	void __stdcall onTopicSetterHook(ObjectWithObjectWithMessage* object, UInt32 option) {
		
		initializePlayerSpeech();

		if (gPlayerSpeech->state == TOPIC_NOT_SELECTED || gPlayerSpeech->state == TOPIC_SPOKEN) {
			gPlayerSpeech->state = TOPIC_SELECTED;
			gPlayerSpeech->isNPCSpeechDelayed = false;
			gPlayerSpeech->option = option;

			wstring_convert<codecvt_utf8_utf16<wchar_t>> converter;
			const wchar_t* speech = converter.from_bytes(object->object->message).c_str();

			FuzRoBorkNamespace::startPlayerSpeech(speech);
		}
	}

	__declspec(naked) void onTopicSetterHooked() {
		__asm {
			push edx
			mov edx, [esp + 0xC] // The selected option is an argument to the function, and is still on the stack there
			pushad
			push edx
			push esi           // `esi` register here contains a pointer to an object, containing
							   // a pointer to another object with a pointer to the string of a chosen topic
							   // (I didn't bother to figure out what this object is)
							   call onTopicSetterHook
							   popad
							   pop edx
							   jmp[gOnTopicSetterResume]
		}
	}


	// registration


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

		gOnTopicSetterResume = detourWithTrampoline(gOnTopicSetter, (BYTE*)onTopicSetterHooked, 5);
		if (!g_papyrus)
		{
			_FATALERROR("couldn't topic setter hook");
			return false;
		}

		gOnDialogueSayResume = detourWithTrampoline(gOnDialogueSay, (BYTE*)onDialogueSayHooked, 6);
		if (!g_papyrus)
		{
			_FATALERROR("couldn't dialogue say hook");
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


