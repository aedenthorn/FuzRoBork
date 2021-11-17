#include "FuzRoBorkInternals.h"

IDebugLog				gLog;

namespace interfaces
{
	PluginHandle				kPluginHandle = kPluginHandle_Invalid;
	SKSEMessagingInterface*		kMsgInterface = nullptr;
}

FuzRoBorkINIManager		FuzRoBorkINIManager::Instance;
SubtitleHasher			SubtitleHasher::Instance;
const double			SubtitleHasher::kPurgeInterval = 1000.0 * 60.0f;


SME::INI::INISetting	kWordsPerSecondSilence("WordsPerSecondSilence",
	"General",
	"Number of words a second of silent voice can \"hold\"",
	(SInt32)2);

SME::INI::INISetting	kSkipEmptyResponses("SkipEmptyResponses",
	"General",
	"Don't play back silent dialog for empty dialog responses",
	(SInt32)1);

SME::INI::INISetting	kPlayPlayerDialogue("PlayPlayerDialogue",
	"General",
	"BLEH",
	(SInt32)1);
SME::INI::INISetting	kPlayNPCDialogue("PlayNPCDialogue",
	"General",
	"BLEH",
	(SInt32)1);
SME::INI::INISetting	kPlayBookPages("PlayBookPages",
	"General",
	"BLEH",
	(SInt32)0);
SME::INI::INISetting	kPlayBooks("PlayBooks",
	"General",
	"BLEH",
	(SInt32)0);
SME::INI::INISetting	kPlayLoadingScreenText("PlayLoadingScreenText",
	"General",
	"BLEH",
	(SInt32)0);
SME::INI::INISetting	kVoicePlayerActions("VoicePlayerActions",
	"General",
	"BLEH",
	(SInt32)0);

SME::INI::INISetting	kSpeakParentheses("SpeakParentheses",
	"General",
	"BLEH",
	(SInt32)0);
SME::INI::INISetting	kEnableHotKeys("EnableHotKeys",
	"General",
	"BLEH",
	(SInt32)0);

SME::INI::INISetting	kMsPerWordSilence("MsPerWordSilence",
	"General",
	"BLEH",
	(float)-1);

SME::INI::INISetting	kPlayerLanguage("PlayerLanguage",
	"General",
	"BLEH",
	(char*)"");
SME::INI::INISetting	kFemaleLanguage("FemaleLanguage",
	"General",
	"BLEH",
	(char*)"");
SME::INI::INISetting	kMaleLanguage("MaleLanguage",
	"General",
	"BLEH",
	(char*)"");
SME::INI::INISetting	kNarratorLanguage("NarratorLanguage",
	"General",
	"BLEH",
	(char*)"");

SME::INI::INISetting	kPlayerVoiceRate("PlayerVoiceRate",
	"General",
	"BLEH",
	(float)-11);
SME::INI::INISetting	kFemaleVoiceRate("FemaleVoiceRate",
	"General",
	"BLEH",
	(float)-11);
SME::INI::INISetting	kMaleVoiceRate("MaleVoiceRate",
	"General",
	"BLEH",
	(float)-11);
SME::INI::INISetting	kNarratorVoiceRate("NarratorVoiceRate",
	"General",
	"BLEH",
	(float)-11);

SME::INI::INISetting	kPlayerVoiceVolume("PlayerVoiceVolume",
	"General",
	"BLEH",
	(float)-1);
SME::INI::INISetting	kFemaleVoiceVolume("FemaleVoiceVolume",
	"General",
	"BLEH",
	(float)-1);
SME::INI::INISetting	kMaleVoiceVolume("MaleVoiceVolume",
	"General",
	"BLEH",
	(float)-1);
SME::INI::INISetting	kNarratorVoiceVolume("NarratorVoiceVolume",
	"General",
	"BLEH",
	(float)-1);

SME::INI::INISetting	kPlayerVoicePitch("PlayerVoicePitch",
	"General",
	"BLEH",
	(SInt32)-99);
SME::INI::INISetting	kFemaleVoicePitch("FemaleVoicePitch",
	"General",
	"BLEH",
	(SInt32)-99);
SME::INI::INISetting	kMaleVoicePitch("MaleVoicePitch",
	"General",
	"BLEH",
	(SInt32)-99);
SME::INI::INISetting	kNarratorVoicePitch("NarratorVoicePitch",
	"General",
	"BLEH",
	(SInt32)-99);

SME::INI::INISetting	kxVASynthVoice("xVASynthVoice",
	"General",
	"BLEH",
	(UInt32)0);

void FuzRoBorkINIManager::Initialize(const char* INIPath, void* Paramenter)
{
	this->INIFilePath = INIPath;
	_MESSAGE("INI Path: %s", INIPath);

	std::fstream INIStream(INIPath, std::fstream::in);
	bool CreateINI = false;

	if (INIStream.fail())
	{
		_MESSAGE("INI File not found; Creating one...");
		CreateINI = true;
	}

	INIStream.close();
	INIStream.clear();

	RegisterSetting(&kWordsPerSecondSilence);
	RegisterSetting(&kSkipEmptyResponses);

	RegisterSetting(&kMsPerWordSilence);
	RegisterSetting(&kPlayPlayerDialogue);
	RegisterSetting(&kPlayNPCDialogue);
	RegisterSetting(&kPlayBookPages);
	RegisterSetting(&kPlayBooks);
	RegisterSetting(&kPlayLoadingScreenText);
	RegisterSetting(&kVoicePlayerActions);

	RegisterSetting(&kSpeakParentheses);
	RegisterSetting(&kEnableHotKeys);

	RegisterSetting(&kPlayerLanguage);
	RegisterSetting(&kFemaleLanguage);
	RegisterSetting(&kMaleLanguage);
	RegisterSetting(&kNarratorLanguage);

	RegisterSetting(&kPlayerVoiceRate);
	RegisterSetting(&kFemaleVoiceRate);
	RegisterSetting(&kMaleVoiceRate);
	RegisterSetting(&kNarratorVoiceRate);

	RegisterSetting(&kPlayerVoiceVolume);
	RegisterSetting(&kFemaleVoiceVolume);
	RegisterSetting(&kMaleVoiceVolume);
	RegisterSetting(&kNarratorVoiceVolume);

	RegisterSetting(&kPlayerVoicePitch);
	RegisterSetting(&kFemaleVoicePitch);
	RegisterSetting(&kMaleVoicePitch);
	RegisterSetting(&kNarratorVoicePitch);

	if (CreateINI)
		Save();
}

std::string MakeSillyName()
{
	std::string Out("Fuz Ro ");
	for (int i = 0; i < 16; i++)
		Out += "Bork";
#ifdef VR_BUILD
	Out += " (for Skyrim VR)";
#endif
	return Out;
}

bool CanShowDialogSubtitles()
{
	return GetINISetting("bDialogueSubtitles:Interface")->data.u8 != 0;
}

bool CanShowGeneralSubtitles()
{
	return GetINISetting("bGeneralSubtitles:Interface")->data.u8 != 0;
}


SubtitleHasher::HashT SubtitleHasher::CalculateHash(const char* String)
{
	SME_ASSERT(String);

	// Uses the djb2 string hashing algorithm
	// http://www.cse.yorku.ca/~oz/hash.html

	HashT Hash = 0;
	int i;

	while (i = *String++)
		Hash = ((Hash << 5) + Hash) + i; // Hash * 33 + i

	return Hash;
}

void SubtitleHasher::Add(const char* Subtitle)
{
	IScopedCriticalSection Guard(&Lock);
	if (Subtitle && strlen(Subtitle) > 1 && HasMatch(Subtitle) == false)
		Store.insert(CalculateHash(Subtitle));
}

bool SubtitleHasher::HasMatch(const char* Subtitle)
{
	IScopedCriticalSection Guard(&Lock);
	HashT Current = CalculateHash(Subtitle);
	return Store.find(Current) != Store.end();
}

void SubtitleHasher::Purge(void)
{
	IScopedCriticalSection Guard(&Lock);
	Store.clear();
}

void SubtitleHasher::Tick(void)
{
	IScopedCriticalSection Guard(&Lock);

	TickCounter.Update();
	TickReminder -= TickCounter.GetTimePassed();

	if (TickReminder <= 0.0f)
	{
		TickReminder = kPurgeInterval;

#ifndef NDEBUG
		_MESSAGE("SubtitleHasher::Tick - Tock!");
#endif
		// we need to periodically purge the hash store as we can't differentiate b'ween topic responses with the same dialog text but different voice assets
		// for instance, there may be two responses with the text "Hello there!" but only one with a valid voice file
		Purge();
	}
}

BSIStream* BSIStream::CreateInstance(const char* FilePath, void* ParentLocation)
{
	auto Instance = (BSIStream*)Heap_Allocate(0x20);		// standard bucket
	return CALL_MEMBER_FN(Instance, Ctor)(FilePath, ParentLocation);
}

override::MenuTopicManager* override::MenuTopicManager::GetSingleton(void)
{
	return (override::MenuTopicManager*)::MenuTopicManager::GetSingleton();
}




// TTS Additions

namespace FuzRoBorkNamespace {

	PluginHandle g_pluginHandle;
	
	string storedBookSpeech = "";
	string storedPagesSpeech = "";

	//SubtitleHasher			SubtitleHasher::Instance;
	//const double			SubtitleHasher::kPurgeInterval = 1000.0 * 60.0f;

	LPCWSTR speechPaths[] = {
		L"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Speech Server\\v11.0\\Voices",
		L"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Speech Server\\v10.0\\Voices",
		L"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Speech\\Voices",
		L"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Speech_OneCore\\Voices",
		L"HKEY_LOCAL_MACHINE\\SOFTWARE\\Wow6432Node\\Microsoft\\Speech\\Voices"
	};

	vector<NPCObj> NPCList;

	NPCObj::NPCObj(string _name, string _race, string _lang, float _rate, float _vol, int _pitch) {
		name = _name;
		race = _race;
		lang = _lang;
		rate = _rate;
		vol = _vol;
		pitch = _pitch;
	}

	boolean actionSpeaking = false;

	ISpVoice* pVoice = NULL;

	SpeakObj::SpeakObj(std::string _speech, std::string _lang, float _rate, float _vol, int _pitch) {
		speech = _speech;
		lang = _lang;
		rate = _rate;
		vol = _vol;
		pitch = _pitch;
	}

	map< int, string > hotkeyTexts;
	map<string, vector<string>> actionList;
	map< string, string > fixes;
	vector< string > voices;

	const char* lastTopic = "";
	clock_t lastTime = clock();
	int rLast = -1;
	bool sendingXVAS = false;

	void ReloadXML(StaticFunctionTag* base) {
		LoadXML();
	}

	void LoadXML()
	{
		tinyxml2::XMLDocument doc;

		if (doc.LoadFile("Data\\skse\\plugins\\FuzRoBork.xml") != tinyxml2::XMLError::XML_SUCCESS) {
			OutputDebugString("XML file not found; exiting\n");
			return;
		}

		XMLElement* root = doc.FirstChildElement("config");

		if (!root) {
			OutputDebugString("root tag not found in XML; exiting\n");
			return;
		}

		_MESSAGE("Loading XML file");

		// hotkeys

		XMLElement* xKeys = root->FirstChildElement("hotkeys");

		if (xKeys) {
			hotkeyTexts.clear();
			XMLElement* xKey = xKeys->FirstChildElement("hotkey");
			UInt32 idx = 1;
			while (xKey) {
				string text = string(xKey->GetText());
				text = findReplace(text, "[", "<");
				text = findReplace(text, "]", ">");
				hotkeyTexts[idx] = text;
				xKey = xKey->NextSiblingElement("hotkey");
				idx++;
			}
			_MESSAGE("Loaded hotkeys");
		}


		// fixes

		XMLElement* xFixes = root->FirstChildElement("fixes");

		if (xFixes) {
			fixes.clear();

			XMLElement* xFix = xFixes->FirstChildElement("fix");
			while (xFix) {

				XMLElement* xFind = xFix->FirstChildElement("find");
				XMLElement* xReplace = xFix->FirstChildElement("replace");

				if (!xFind || !xFix)
					continue;

				string find = string(xFind->GetText());
				string replace = string(xReplace->GetText());
				replace = findReplace(replace, "[", "<");
				replace = findReplace(replace, "]", ">");
				fixes[find] = replace;
				xFix = xFix->NextSiblingElement("fix");
			}
			_MESSAGE("Loaded fixes");
		}

		// actions

		OutputDebugString("Rebuilding action list from xml\n");

		XMLElement* pcs = root->FirstChildElement("pcs");

		if (pcs) {


			XMLElement* xPC = pcs->FirstChildElement("pc");

			if (xPC) 
			{

				XMLElement* xTopic = xPC->FirstChildElement("topic");

				while (xTopic) {

					vector<string> optionList;

					XMLElement* xOption = xTopic->FirstChildElement("option");

					string aName = (const char*)xTopic->Attribute("name");

					OutputDebugString(aName.c_str());
					OutputDebugString("\n");

					while (xOption) {
						if (xOption->GetText()) {
							OutputDebugString(string(xOption->GetText()).c_str());
							OutputDebugString("\n");
							optionList.push_back(string(xOption->GetText()));
						}
						else
							optionList.push_back(" "); // add empties to make less common

						xOption = xOption->NextSiblingElement("option");
					}

					if (optionList.size() != 0) {
						actionList[aName] = optionList;
					}
					xTopic = xTopic->NextSiblingElement("topic");
				}

				_MESSAGE("Loaded actions");
			}
		}

	}

	bool findSpeechToken(ISpObjectToken** pelt, wstring name, LPCWSTR key) {

		CComPtr<ISpObjectTokenCategory>   cpVoiceCat;
		CComPtr<IEnumSpObjectTokens> cpVoiceEnum;

		HRESULT hr = SpGetCategoryFromId(key, &cpVoiceCat);

		if (SUCCEEDED(hr))
			hr = cpVoiceCat->EnumTokens(NULL, NULL, &cpVoiceEnum);

		if (SUCCEEDED(hr)) {

			UInt32 size = 0;
			cpVoiceEnum->GetCount(&size);

			for (UInt32 idx = 0; idx < size; idx++)
			{

				ISpObjectToken* pCurVoiceToken;
				CComPtr<ISpDataKey> attributes;
				LPWSTR nameValue;

				if (S_OK != cpVoiceEnum->Next(1, &pCurVoiceToken, NULL))
					break;

				if (S_OK != pCurVoiceToken->OpenKey(L"Attributes", &attributes))
					continue;

				if (S_OK != attributes->GetStringValue(L"Name", &nameValue))
					continue;

				OutputDebugStringW(nameValue);
				OutputDebugString("\n");

				if (name == wstring(nameValue))
				{
					OutputDebugString("found voice\n");
					*pelt = pCurVoiceToken;
					return true;
				}
			}
		}
		return false;
	}

	string findReplace(string str, const std::string oldStr, const string newStr) {
		std::size_t pos = 0;
		while ((pos = str.find(oldStr, pos)) != std::string::npos) {
			str.replace(pos, oldStr.length(), newStr);
			pos += newStr.length();
		}
		return str;
	}


	void replaceUnspeakables(string& str) {


		map<string, string>::iterator it;

		for (it = fixes.begin(); it != fixes.end(); it++)
		{
			str = findReplace(str, it->first, it->second);
		}


		// remove parenthesis and brackets if option ticked

		if (kSpeakParentheses.GetData().i == 0) {
			int idx = 0;
			boolean par = false;
			string newStr = "";
			while (idx < str.length()) {
				if (!par && str[idx] == '(')
					par = true;
				else if (par && str[idx] == ')')
					par = false;
				else if (!par)
					newStr += str[idx];
				idx++;
			}
			str = newStr;
			idx = 0;
			par = false;
			newStr = "";
			while (idx < str.length()) {
				if (!par && str[idx] == '[')
					par = true;
				else if (par && str[idx] == ']')
					par = false;
				else if (!par)
					newStr += str[idx];
				idx++;
			}
			str = newStr;
		}
	}


	void speakTask(SpeakObj nSpeech)
	{
		//_MESSAGE("speakTask: '%s'", nSpeech.speech);

		if (nSpeech.lang == "xVASynth") {
			_MESSAGE("Sending text to xVASynth");
			sendToxVASynth(nSpeech);
			return;
		}


		if (FAILED(::CoInitialize(NULL))) {
			_MESSAGE("failed to coinitialize");
			return;
		}

		// Declare local identifiers:
		HRESULT hr = S_OK;
		ISpObjectToken* cpToken;

		bool found = false;

		std::wstring wideLang(nSpeech.lang.size() + 1, L' ');
		std::size_t newLength = std::mbstowcs(&wideLang[0], nSpeech.lang.c_str(), wideLang.size());
		wideLang.resize(newLength);


		// Create the SAPI voice.
		hr = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (void**)&pVoice);

		if (SUCCEEDED(hr))
		{
			//_MESSAGE("searching for voices in registry");
			
			for (int i = 0; i < size(speechPaths); i++) {
				found = findSpeechToken(&cpToken, wideLang, speechPaths[i]);
				if (found)
					break;
			}
			if (!found) {
				for (int i = 0; i < size(speechPaths); i++) {
					hr = SpFindBestToken(speechPaths[i], NULL, NULL, &cpToken);
					if (SUCCEEDED(hr))
						break;
				}
			}
		}
		if (SUCCEEDED(hr))
		{

			hr = pVoice->SetVoice(cpToken);
			//_MESSAGE("set voice");

		}
		if (SUCCEEDED(hr))
		{
			hr = pVoice->SetRate(nSpeech.rate);
			//_MESSAGE("set rate");

		}
		if (SUCCEEDED(hr))
		{

			hr = pVoice->SetVolume(nSpeech.vol);
			//_MESSAGE("set volume");
		}
		if (SUCCEEDED(hr))
		{
			replaceUnspeakables(nSpeech.speech);

			if (nSpeech.speech.length() < 1) {
				//_MESSAGE("No speech string");
				return;
			}

			// pitch

			string speech = "<pitch absmiddle=\"" + to_string(nSpeech.pitch) + "\">" + nSpeech.speech + "</pitch>";

			const char* nChar = speech.c_str();

			//_MESSAGE("Speaking '%s'", nChar);

			wchar_t* lSpeech = new wchar_t[4096];
			MultiByteToWideChar(CP_ACP, 0, nChar, -1, lSpeech, 4096);

			if (pVoice == NULL) {
				_MESSAGE("No pVoice");
				return;
			}
			else {
				SPVOICESTATUS   pStatus;
				pVoice->GetStatus(&pStatus, NULL);
				if (pStatus.dwRunningState == SPRS_IS_SPEAKING) {
					_MESSAGE("Still speaking");
					return;
				}

			}
			hr = pVoice->Speak(lSpeech, SPF_PURGEBEFORESPEAK | SPF_ASYNC, NULL);
			actionSpeaking = false;
			//_MESSAGE("Spoke");
		}


		::CoUninitialize();
	}


	void startNarratorSpeech(string text) { // unchecked

		_MESSAGE("Starting narrator speech %s", text.c_str());

		string lang = kNarratorLanguage.GetData().s;

		float rate = kNarratorVoiceRate.GetData().f;

		float volume = kNarratorVoiceVolume.GetData().f;

		float pitch = kNarratorVoicePitch.GetData().i;

		SpeakObj pSpeech(text, lang, rate, volume, pitch);

		thread t1(speakTask, pSpeech);
		t1.detach();
	}

	void startStoredBookSpeech(StaticFunctionTag* base) {
		if (storedBookSpeech.size() == 0)
			return;
		stopSpeaking();
		OutputDebugString("Starting stored book speech\n");
		startNarratorSpeech(storedBookSpeech);
	}
	void startStoredPagesSpeech(StaticFunctionTag* base) {
		if (storedPagesSpeech.size() == 0)
			return;
		stopSpeaking();
		OutputDebugString("Starting stored book speech\n");
		startNarratorSpeech(storedPagesSpeech);
	}
	void startBookSpeech(string text) {
		stopSpeaking();
		storeBookSpeech(text);
		OutputDebugString("Starting book speech\n");
		startNarratorSpeech(text);
	}
	
	void storeBookSpeech(string text) {
		OutputDebugString("Storing book speech\n");
		storedBookSpeech = text;
		if (kPlayBooks.GetData().i == 1) {
			stopSpeaking();
			startNarratorSpeech(text);
		}
	}

	void storeFirstPagesSpeech(string text) {
		OutputDebugString("Storing first pages speech\n");
		storedPagesSpeech = text;
		if (kPlayBookPages.GetData().i == 1 && kPlayBooks.GetData().i == 0) {
			stopSpeaking();
			startNarratorSpeech(text);
		}
	}
	
	void storePagesSpeech(string text) {
		OutputDebugString("Storing pages speech\n");
		storedPagesSpeech = text;
		if (kPlayBookPages.GetData().i == 1) {
			stopSpeaking();
			startNarratorSpeech(text);
		}
	}
	void sendToxVASynth(SpeakObj obj) {
		if (sendingXVAS || voices.size() == 0)
			return;
		sendingXVAS = true;
		_MESSAGE("Writing speech to file for xVASynth");

		UInt32 idx = kxVASynthVoice.GetData().u;
		if (idx >= voices.size())
			idx = 0;

		regex tags("<[^>]*>");
		string s = obj.speech;
		s = regex_replace(s, tags, "");
		json j;
		j["voiceId"] = voices[idx];
		j["gameId"] = "skyrim";
		j["pitch"] = obj.pitch;
		j["rate"] = obj.rate;
		j["vol"] = obj.vol;
		j["text"] = s;


		ofstream o("Data\\SKSE\\Plugins\\xVASynthText.json");
		o << setw(4) << j << endl;

		sendingXVAS = false;
	}

	void speakLoadingScreen(string text) {
		startNarratorSpeech(text);
	}

	void startNPCSpeech(const char * text) {

		if (kPlayNPCDialogue.GetData().i == 0) {
			_MESSAGE("No dialogue");
			return;
		}

		_MESSAGE("NPC Speaking '%s'", text);


		TESObjectREFR* refr = MenuTopicManager::GetSingleton()->GetDialogueTarget();

		// standard values

		UInt32 sex = 1; // female

		if (!refr || !refr->baseForm) {
			_MESSAGE("dialogue target not found!");
			return;
		}

		TESNPC* npc = DYNAMIC_CAST(refr->baseForm, TESForm, TESNPC);


		sex = npc ? CALL_MEMBER_FN(npc, GetSex)() : 1;

		string lang = sex == 0 ? kMaleLanguage.GetData().s : kFemaleLanguage.GetData().s;

		float rate = sex == 0 ? kMaleVoiceRate.GetData().f : kFemaleVoiceRate.GetData().f;

		float volume = sex == 0 ? kMaleVoiceVolume.GetData().f : kFemaleVoiceVolume.GetData().f;

		float pitch = sex == 0 ? kMaleVoicePitch.GetData().i : kFemaleVoicePitch.GetData().i;


		// specific NPC override

		if (npc) {
			const char* nName(npc->fullName.GetName());
			const char* nRace(npc->race.race->fullName.GetName());

			//Console_Print(nName.c_str());
			//Console_Print(nRace.c_str());

			for (vector<NPCObj>::size_type i = 0; i < NPCList.size(); i++) {

				//_MESSAGE(NPCList[i].name.c_str());

				regex npcxname(NPCList[i].name);
				regex npcxrace(NPCList[i].race);
				if (NPCList[i].name.compare(nName) == 0 || (NPCList[i].name.length() > 0 && regex_match(nName, npcxname)) || NPCList[i].race.compare(nRace) == 0 || (NPCList[i].race.length() > 0 && regex_match(nRace, npcxrace))) {

					if (NPCList[i].lang.length() > 0)
						lang = NPCList[i].lang;
					if (NPCList[i].rate > -11 && NPCList[i].rate < 11)
						rate = NPCList[i].rate;
					if (NPCList[i].vol > -1 && NPCList[i].vol < 101)
						volume = NPCList[i].vol;
					if (NPCList[i].pitch > -11 && NPCList[i].pitch < 11)
						pitch = NPCList[i].pitch;
				}
			}
		}

		SpeakObj pSpeech(text, lang, rate, volume, pitch);

		thread t1(speakTask, pSpeech);
		t1.detach();
		//_MESSAGE("NPC Speaking initiated successfully");
	}

	void startPlayerSpeech(const char* _title) {
		_MESSAGE("Starting Player Speech: %s", _title);

		TESNPC* pc = DYNAMIC_CAST((*g_thePlayer)->baseForm, TESForm, TESNPC);

		string lang = kPlayerLanguage.GetData().s;

		float rate = kPlayerVoiceRate.GetData().f;

		float volume = kPlayerVoiceVolume.GetData().f;

		float pitch = kPlayerVoicePitch.GetData().i;

		/*
		OutputDebugString(lang.c_str());
		OutputDebugString(" lang\n");
		OutputDebugString(to_string(rate).c_str());
		OutputDebugString(" rate\n");
		OutputDebugString(to_string(volume).c_str());
		OutputDebugString(" vol\n");
		*/

		SpeakObj pSpeech(_title, lang, rate, volume, pitch);

		thread t3(speakTask, pSpeech);
		t3.detach();
	}

	string randSpeech[] = { "I <emph>used</emph> to be an adventurer like you. Then I took an <emph>arrow</emph> in the knee.","What is better - to be <emph>born</emph> good, or to <emph>overcome</emph> your evil nature through <emph>great</emph> effort?","Let me <emph>guess</emph>, Someone stole your <emph>sweetroll</emph>!","My cousin is out <emph> fighting dragons.</emph> And what do <emph>I</emph> get? <emph>Guard</emph> duty.","Oh, there <emph>once</emph> was a hero named <emph>Ragnar</emph> the Red, who came <emph>riding</emph> to Whiterun from old <emph>Rorikstead</emph>. ","And the <emph>braggart</emph> did swagger and <emph>brandish</emph> his blade as he <emph>told</emph> of bold battles and <emph>gold</emph> he had made. ","But <emph>then</emph> he went quiet, did <emph>Ragnar</emph> the Red when he <emph>met</emph> the shield-maiden <emph>Matilda</emph>, who said; ","Oh, you <emph>talk</emph> and you <emph>lie</emph> and you <emph>drink</emph> all our mead; now I <emph>think</emph> it's high time that you <emph>lie</emph> down and <emph>bleed</emph>!. ","And so <emph>then</emph> came <emph>clashing</emph> and <emph>slashing</emph> of steel, as the <emph>brave</emph> lass Matilda <emph>charged</emph> in, full of zeal. ","And the <emph>braggart</emph> named <emph>Ragnar</emph> was boastful no more... when his <emph>ugly</emph> red <emph>head</emph> rolled <emph>around</emph> on the <emph>floor</emph>!","There are <emph>formalities</emph> that must be observed, at the first meeting of two of the <emph>dov</emph>. ","My <emph>favorite</emph> drinking buddy! Let's get some <emph>mead</emph>.","Perhaps we should find a random stranger to murder. Practice <emph>does</emph> make perfect.","You stink of <emph>death</emph> my friend. I <emph>salute</emph> you.","You <emph>never</emph> should have <emph>come</emph> here.","Babette, my girl - <emph>pack</emph> your things. We're <emph>moving</emph>!","<emph>Enough</emph>! I will <emph>not</emph> stand idly by while a <emph>dragon</emph> burns my hold and <emph>slaughters</emph> my <emph>people</emph>!","I'm told they call me <emph>Dirge</emph> because I'm the <emph>last</emph> thing you hear before they put you in the ground.","Been so long since I killed a Stormcloak, my sword arm's getting <emph>flabby</emph>.","If these ruins frighten you, take comfort from the knowledge that <emph>I</emph> am here.","Look at <emph>that</emph>. Am I <emph>drunk</emph>? I must be <emph>drunk</emph>.","I used to be the <emph>top</emph> soldier for the Stormcloaks, then i took a <emph>sword</emph> to the chest","And who are <emph>you</emph> to challenge <emph>me</emph>? I've conquered <emph>mortality</emph> itself. ","I've <emph>spat</emph> in the eyes of the <emph>Daedric Lords</emph>! ","This is <emph>my</emph> realm now, I've <emph>sacrificed</emph> too much to let you <emph>take</emph> it from me!","I'll see you <emph>burn</emph>!","You do not even know our <emph>tongue</emph>, do you? ","Such <emph>arrogance</emph>, to <emph>dare</emph> take for yourself the name of <emph>Dovah</emph>!","Well well... Another <emph>maggot</emph> to <emph>squash</emph> beneath my boot!","Either <emph>I'm</emph> drunk, or <emph>you're</emph> naked. Possibly both.","I'll see you in pieces!","I've been hunting and fishing in these parts for <emph>years</emph>.","I've got my <emph>eyes</emph> on you.","We’re one of the <emph>same</emph> kind, you and I. I’m <emph>happy</emph> to have met you.","Looking to <emph>protect</emph> yourself? Or <emph>deal</emph> some damage?","I got to <emph>thinking</emph>, maybe <emph>I’m</emph> the Dragonborn and I <emph>just</emph> don’t <emph>know</emph> it yet.","It's a <emph>fine</emph> day with <emph>you</emph> around!","My father said I should go to <emph>college</emph>, but he didnt say <emph>which</emph> one.","I'd be a lot <emph>happier</emph> and a lot <emph>warmer</emph> with a <emph>belly</emph> full of <emph>mead</emph>.","I heard about <emph>you</emph> and your <emph>honeyed</emph> words.","I guess you don't have potatoes in your ears <emph>after</emph> all." };

	void testSpeech(StaticFunctionTag* base, BSFixedString _which) {

		string which(_which.data);

		//Console_Print("testing speech");

		int r = static_cast<double>(rand()) / RAND_MAX * size(randSpeech);
		string speech = randSpeech[r];

		_MESSAGE("Speaking random %s text: %s", which.c_str());

		if (which.compare("P") == 0 || which.compare("p") == 0) {
			startPlayerSpeech(speech.c_str());
		}
		else if (which.compare("F") == 0 || which.compare("f") == 0) {
			SpeakObj pSpeech(speech, kFemaleLanguage.GetData().s, kFemaleVoiceRate.GetData().f, kFemaleVoiceVolume.GetData().f, kFemaleVoicePitch.GetData().i);
			thread t3(speakTask, pSpeech);
			t3.detach();
		}
		else if (which.compare("M") == 0 || which.compare("m") == 0) {
			SpeakObj pSpeech(speech, kMaleLanguage.GetData().s, kMaleVoiceRate.GetData().f, kMaleVoiceVolume.GetData().f, kMaleVoicePitch.GetData().i);
			thread t3(speakTask, pSpeech);
			t3.detach();
		}
		else if (which.compare("N") == 0 || which.compare("n") == 0) {
			SpeakObj pSpeech(speech, kNarratorLanguage.GetData().s, kNarratorVoiceRate.GetData().f, kNarratorVoiceVolume.GetData().f, kNarratorVoicePitch.GetData().i);
			thread t3(speakTask, pSpeech);
			t3.detach();
		}
		else if (which.compare("S") == 0 || which.compare("s") == 0) {
			SpeakObj pSpeech("Bork bork bork!", "eSpeak-en", 0, 100.0, 0);
			thread t3(speakTask, pSpeech);
			t3.detach();
		}
		else if (which.compare("X") == 0 || which.compare("x") == 0) {
			if (sendingXVAS)
				return;
			SpeakObj pSpeech(speech, "xVASynth", kPlayerVoiceRate.GetData().f, kPlayerVoiceVolume.GetData().f, kPlayerVoicePitch.GetData().i);
			thread t1(speakTask, pSpeech);
			t1.detach();
		}

	}

	boolean isSpeaking() {
		if (pVoice == NULL)
			return FALSE;
		SPVOICESTATUS   pStatus;
		pVoice->GetStatus(&pStatus, NULL);
		if (pStatus.dwRunningState == SPRS_IS_SPEAKING)
			return TRUE;

		return FALSE;
	}

	void stopSpeaking() {
		if (pVoice == NULL)
			return;
		pVoice->Speak(NULL, SPF_PURGEBEFORESPEAK | SPF_ASYNC, 0);
	}


	void pStopSpeech(StaticFunctionTag* base) {
		stopSpeaking();
	}

	vector<string> explode(const string& delimiter, const string& str)
	{
		vector<string> arr;

		int strleng = str.length();
		int lenD = delimiter.length();
		if (lenD == 0)
			return arr;//no change

		int i = 0;
		int k = 0;
		while (i < strleng)
		{
			int j = 0;
			while (i + j < strleng && j < lenD && str[i + j] == delimiter[j])
				j++;
			if (j == lenD)//found delimiter
			{
				arr.push_back(str.substr(k, i - k));
				i += lenD;
				k = i;
			}
			else
			{
				i++;
			}
		}
		arr.push_back(str.substr(k, i - k));
		return arr;
	}

	void setMCMConfig(StaticFunctionTag* base, BSFixedString _strings, BSFixedString _booleans, BSFixedString _floats)
	{
		OutputDebugString("Beginning to parse config settings\n");

		string strings(_strings.data);
		string booleans(_booleans.data);
		string floats(_floats.data);

		OutputDebugString((strings + "\n").c_str());
		OutputDebugString((booleans + "\n").c_str());
		OutputDebugString((floats + "\n").c_str());

		vector<string> stringV = explode("^", strings);
		vector<string> boolV = explode("^", booleans);
		vector<string> floatV = explode("^", floats);

		if (stringV.size() == 5) {
			OutputDebugString("Strings correct size\n");
			kPlayerLanguage.SetDataAsString(stringV[0].c_str());
			kFemaleLanguage.SetDataAsString(stringV[1].c_str());
			kMaleLanguage.SetDataAsString(stringV[2].c_str());
			kNarratorLanguage.SetDataAsString(stringV[3].c_str());
			kxVASynthVoice.SetUInt(stoi(stringV[4].c_str()));
		}

		if (boolV.size() == 9) {
			OutputDebugString("Bools correct size\n");
			kSkipEmptyResponses.SetInt(boolV[0].compare("TRUE") == 0 ? 1 : 0);
			kPlayPlayerDialogue.SetInt(boolV[1].compare("TRUE") == 0 ? 1 : 0);
			kPlayNPCDialogue.SetInt(boolV[2].compare("TRUE") == 0 ? 1 : 0);
			kPlayBookPages.SetInt(boolV[3].compare("TRUE") == 0 ? 1 : 0);
			kPlayBooks.SetInt(boolV[4].compare("TRUE") == 0 ? 1 : 0);
			kPlayLoadingScreenText.SetInt(boolV[5].compare("TRUE") == 0 ? 1 : 0);
			kVoicePlayerActions.SetInt(boolV[6].compare("TRUE") == 0 ? 1 : 0);
			kSpeakParentheses.SetInt(boolV[7].compare("TRUE") == 0 ? 1 : 0);
			kEnableHotKeys.SetInt(boolV[8].compare("TRUE") == 0 ? 1 : 0);
		}

		if (floatV.size() == 13) {
			OutputDebugString("Floats correct size\n");

			kMsPerWordSilence.SetFloat(stof(floatV[0]));

			kPlayerVoiceVolume.SetFloat(stof(floatV[1]));
			kFemaleVoiceVolume.SetFloat(stof(floatV[2]));
			kMaleVoiceVolume.SetFloat(stof(floatV[3]));
			kNarratorVoiceVolume.SetFloat(stof(floatV[4]));

			kPlayerVoiceRate.SetFloat(stof(floatV[5]));
			kFemaleVoiceRate.SetFloat(stof(floatV[6]));
			kMaleVoiceRate.SetFloat(stof(floatV[7]));
			kNarratorVoiceRate.SetFloat(stof(floatV[8]));

			kPlayerVoicePitch.SetInt(stoi(floatV[9]));
			kFemaleVoicePitch.SetInt(stoi(floatV[10]));
			kMaleVoicePitch.SetInt(stoi(floatV[11]));
			kNarratorVoicePitch.SetInt(stoi(floatV[12]));
		}

	}

	bool checkIfInArray(vector<LPWSTR> sA, LPWSTR s) {
		for (int i = 0; i < sA.size(); i++) {

			if (wstring(sA[i]) == wstring(s))
				return true;
		}
		return false;
	}

	void addCatToVector(vector<LPWSTR>* nameA, LPCWSTR key) {

		HRESULT hr = S_OK;

		// Find the voice token that best matches the specified attributes.
		CComPtr<ISpObjectTokenCategory>   cpVoiceCat;
		CComPtr<IEnumSpObjectTokens> cpVoiceEnum;

		hr = SpGetCategoryFromId(key, &cpVoiceCat);

		if (SUCCEEDED(hr))
			hr = cpVoiceCat->EnumTokens(NULL, NULL, &cpVoiceEnum);

		if (!SUCCEEDED(hr))
			return;

		OutputDebugString("voices available:\n");

		ISpObjectToken* pCurVoiceToken;
		bool fFound = false;

		for (UInt32 idx = 0; S_OK == cpVoiceEnum->Next(1, &pCurVoiceToken, NULL); idx++)
		{
			if (nameA->size() >= 100)
				break;

			CComPtr<ISpDataKey> attributes;
			if (S_OK != pCurVoiceToken->OpenKey(L"Attributes", &attributes))
				return;

			LPWSTR nameValue;

			fFound = SUCCEEDED(attributes->GetStringValue(L"Name", &nameValue));

			// Note, ISpObjectToken inherits from ISpDataKey.
			if (SUCCEEDED(hr) && fFound)
			{

				if (checkIfInArray(*nameA, nameValue))
					continue;

				OutputDebugStringW(nameValue);
				OutputDebugString("\n");

				nameA->push_back(nameValue);

			}
		}
	}


	void hotSpeech(StaticFunctionTag* base, UInt32 _which) {
		OutputDebugString("starting hot key speech\n");

		if (kEnableHotKeys.GetData().i == 0)
			return;

		if (hotkeyTexts.count(_which) == 0) {
			_MESSAGE("Hotkey %d not found", _which);
			return;
		}

		stopSpeaking();

		string text = hotkeyTexts.at(_which);

		text = findReplace(text, "[", "<");
		text = findReplace(text, "]", ">");
		startPlayerSpeech(text.c_str());
	}




	void actionSpeech(StaticFunctionTag* base, BSFixedString _which) {

		if (kVoicePlayerActions.GetData().i == 0)
			return;

		_MESSAGE("Player action: %s", _which.data);

		// preventing duplicates

		clock_t nowTime = clock();
		double interval = double(nowTime - lastTime);

		//Console_Print(to_string(interval).c_str());

		const char* _lastTopic = lastTopic;
		lastTopic = _which.data;

		if (interval < 3 && strcmp(_lastTopic, _which.data) == 0)
			return;

		lastTime = nowTime;

		string which = string(_which.data);

		if (actionList.find(which) != actionList.end()) {

			int r = 0;
			if (actionList[which].size() > 1) {
				r = static_cast<double>(rand()) / RAND_MAX * actionList[which].size();

				while (strcmp(_lastTopic, _which.data) == 0 && r == rLast) // prevent duplicates
					r = static_cast<double>(rand()) / RAND_MAX * actionList[which].size();
			}

			actionSpeaking = true;
			stopSpeaking();
			string text = actionList[which][r];
			text = findReplace(text, "[", "<");
			text = findReplace(text, "]", ">");
			startPlayerSpeech(text.c_str());
			return;

		}
	}
	void sendLanguages(StaticFunctionTag* t, VMArray<BSFixedString> names, VMArray<BSFixedString> xVANames) {

		vector<LPWSTR> nameA;

		addCatToVector(&nameA, SPCAT_VOICES);
		addCatToVector(&nameA, L"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Speech Server\\v10.0\\Voices");
		addCatToVector(&nameA, L"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Speech_OneCore\\Voices");
		addCatToVector(&nameA, L"HKEY_LOCAL_MACHINE\\SOFTWARE\\Wow6432Node\\Microsoft\\Speech\\Voices");
		addCatToVector(&nameA, L"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Speech\\Voices");

		_MESSAGE("Sending languages");
		for (UInt32 i = 0; i < nameA.size(); i++) {


			char nameBuffer[500];
			// First arg is the pointer to destination char, second arg is
			// the pointer to source wchar_t, last arg is the size of char buffer
			wcstombs(nameBuffer, nameA[i], 500);

			BSFixedString bst;
			bst.data = nameBuffer;
			_MESSAGE("adding language %s", nameBuffer);

			names.Set(&bst, i);
		}


		// xVASynth

		string path = "Data\\SKSE\\Plugins\\xVASynthVoices.json";

		_MESSAGE("Path %s", path.c_str());

		std::ifstream i(path);

		if (i.good()) {
			_MESSAGE("File exists %s", path.c_str());

			json j;
			i >> j;
			if (j.is_object()) {

				_MESSAGE("adding xVASynth voices from %s", path.c_str());

				json jg = j["games"];
				if (jg.is_object()) {
					_MESSAGE("got games");
					json jgs = jg["skyrim"];
					if (jgs.is_array()) {
						_MESSAGE("got skyrim %d", jgs.size());
						for (UINT32 i = 0; i < jgs.size(); i++) {

							voices.push_back(jgs[i]["id"]);

							string ostr = jgs[i]["name"];
							const char * orig = ostr.c_str();

							//_MESSAGE("got string %s", orig);

							size_t newsize = strlen(orig) + 1;
							wchar_t* wcstring = new wchar_t[newsize];

							// Convert char* string to a wchar_t* string.
							size_t convertedChars = 0;
							mbstowcs_s(&convertedChars, wcstring, newsize, orig, _TRUNCATE);

							char nameBuffer[500];
							// First arg is the pointer to destination char, second arg is
							// the pointer to source wchar_t, last arg is the size of char buffer
							wcstombs(nameBuffer, wcstring, 500);

							BSFixedString bst;
							bst.data = nameBuffer;
							_MESSAGE("adding xVASynth voice %s", nameBuffer);

							xVANames.Set(&bst, i);
						}

						BSFixedString vs;
						vs.data = "xVASynth";
						names.Set(&vs, nameA.size());
					}
				}
			}
		}
	}

	bool RegisterFuncs(VMClassRegistry* registry)
	{
		// register the function with the game
		// note you need to have the actual function name for the 3rd argument, but use  
		// "MyNameSpace_MyFunction" for the name it will be used as in papyrus scripts to avoid conflicts
		// since you can't use the namespace in your scripts like we can in the plugin
		registry->RegisterFunction(new NativeFunction1 <StaticFunctionTag, void, BSFixedString>("FuzRoBork_testSpeech", "BorkMCM", testSpeech, registry));
		registry->RegisterFunction(new NativeFunction1 <StaticFunctionTag, void, UInt32>("FuzRoBork_hotSpeech", "BorkMCM", hotSpeech, registry));
		registry->RegisterFunction(new NativeFunction1 <StaticFunctionTag, void, BSFixedString>("FuzRoBork_actionSpeech", "BorkPC", actionSpeech, registry));
		registry->RegisterFunction(new NativeFunction0 <StaticFunctionTag, void>("FuzRoBork_storedBookSpeech", "BorkMCM", startStoredBookSpeech, registry));
		registry->RegisterFunction(new NativeFunction0 <StaticFunctionTag, void>("FuzRoBork_storedPagesSpeech", "BorkMCM", startStoredPagesSpeech, registry));
		registry->RegisterFunction(new NativeFunction3 <StaticFunctionTag, void, BSFixedString, BSFixedString, BSFixedString>("FuzRoBork_setMCMConfig", "BorkMCM", setMCMConfig, registry));
		registry->RegisterFunction(new NativeFunction0 <StaticFunctionTag, void>("FuzRoBork_stopSpeech", "BorkMCM", pStopSpeech, registry));
		registry->RegisterFunction(new NativeFunction2 <StaticFunctionTag, void, VMArray<BSFixedString>, VMArray<BSFixedString>>("FuzRoBork_getLanguages", "BorkMCM", sendLanguages, registry));
		registry->RegisterFunction(new NativeFunction0 <StaticFunctionTag, void>("FuzRoBork_reloadXML", "BorkMCM", ReloadXML, registry));

		return true;
	}
}