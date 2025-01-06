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
SME::INI::INISetting	kCustomTempPath("CustomTempPath",
	"General",
	"BLEH",
	(char*)"");

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
SME::INI::INISetting	kxVASynthGame("xVASynthGame",
	"General",
	"BLEH",
	(char*)"");

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
	RegisterSetting(&kCustomTempPath);

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

NPCObj::NPCObj(const char* _name, const char* _race, const char* _lang, int _sex, float _rate, float _vol, int _pitch) {
	name = _name;
	race = _race;
	sex = _sex;
	lang = _lang;
	rate = _rate;
	vol = _vol;
	pitch = _pitch;
}
NPCObj::NPCObj() {
}

SpeakObj::SpeakObj(string _speech, const char*  _lang, float _rate, float _vol, int _pitch) {
	speech = _speech;
	lang = _lang;
	rate = _rate;
	vol = _vol;
	pitch = _pitch;
}
SpeakObj::SpeakObj() {
}


QueueObj::QueueObj(TESNPC* _npc, string _speech) {
	speech = string(_speech);
	npc = _npc;
}

namespace FuzRoBorkNamespace {

	PluginHandle g_pluginHandle;
	
	vector<QueueObj> speakingQueue;

	const char* storedBookSpeech = "";
	const char* storedPagesSpeech = "";

	//SubtitleHasher			SubtitleHasher::Instance;
	//const double			SubtitleHasher::kPurgeInterval = 1000.0 * 60.0f;

	vector<wstring> GetSpeechPaths() {

		wstring key0 = wstring(SPCAT_VOICES);
		wstring root(SPREG_LOCAL_MACHINE_ROOT);
		wstring key1(SPREG_LOCAL_MACHINE_ROOT);
		key1 += wstring(L" Server\\v11.0\\Voices");
		wstring key2(SPREG_LOCAL_MACHINE_ROOT);
		key2 += wstring(L" Server\\v10.0\\Voices");
		wstring key3(SPREG_LOCAL_MACHINE_ROOT);
		key3 += wstring(L"_OneCore\\Voices");
		wstring key4(SPCAT_VOICES); 
		key4.replace(19, 9, L"SOFTWARE\\Wow6432Node\\");

		vector<wstring> s = { key0, key1, key2, key3, key4};
		return s;
	}

	vector<NPCObj> NPCList;


	ISpVoice* pVoice = NULL;


	map< int, string> hotkeyTexts;
	vector< string > randomTexts;
	vector< wstring > speechPaths = GetSpeechPaths();
	map<string, vector<string>> actionList;
	map< string, string> fixes;
	map< string, string> transMap;
	map<string, json > gameVoices;

	wstring XVAFolder = L"";

	int checkWavCount = 0;
	const char* lastTopic = "";
	clock_t lastTime = clock();
	int rLast = -1;
	bool sendingXVAS = false;
	bool playingXVAS = false;
	bool stopTimer = false;

	void AddSpeechToQueue(TESNPC* npc, const char* speech) {
		_MESSAGE("Adding speech to queue position %i: %s ", speakingQueue.size(), speech);

		QueueObj obj(npc, string(speech));
		speakingQueue.push_back(obj);
	}
	bool GetSpeechFromQueue(SpeakObj& obj) {
		if (speakingQueue.size() == 0) {
			return false;
		}
		_MESSAGE("got queued speech (queue size: %i): %s", speakingQueue.size(), speakingQueue[0].speech.c_str());
		obj = GetNPCSpeech(speakingQueue[0].npc, speakingQueue[0].speech);
		return true;
	}
	void EraseFromQueue() {
		if (speakingQueue.size() == 0)
			return;
		_MESSAGE("Erasing first queued speech (queue size: %i): %s", speakingQueue.size(), speakingQueue[0].speech.c_str());
		speakingQueue.erase(speakingQueue.begin());
		_MESSAGE("Remaning queue size: %i", speakingQueue.size());
	}
	
	void StringToLower(string& s) {
		for (int i = 0; i < s.size(); i++)
		{
			s[i] = tolower(s[i]);
		}
	}

	bool GetNPC(const char* nName, const char* nRace, NPCObj& npc) {
		for (vector<NPCObj>::size_type i = 0; i < NPCList.size(); i++) {
			_MESSAGE("checking list entry %s %d %s", nName, i, NPCList[i].name);

			if (
				(
					(nName != "null" && strlen(nName) > 0 && (strcmp(NPCList[i].name, nName) == 0 || IsRegexMatch(nName, NPCList[i].name)))
					|| (nRace != "null" && strlen(nRace) > 0 && (strcmp(NPCList[i].race, nRace) == 0 || IsRegexMatch(nRace, NPCList[i].race) && (npc.sex == -1 || npc.sex == NPCList[i].sex)))
				)
			)
			{
				_MESSAGE("Found match for %s %s", NPCList[i].name, NPCList[i].lang);
				npc = NPCList[i];
				return true;
			}
		}
		_MESSAGE("checked list, no match");
		return false;
	}

	bool IsRegexMatch(const char* str, const char* rx) {
		return false;
		_MESSAGE("checking regex %s %s", str, rx);
		try {
			return (strlen(rx) > 0 && regex_match(str, regex(rx)));
		}
		catch(...)
		{
			_MESSAGE("regex error");
			return false;
		}
		return false;
	}

	void wav_find_timer_start(std::function<bool(void)> func, unsigned int interval)
	{
		std::thread([func, interval]()
		{
			while (true)
			{
				auto x = std::chrono::steady_clock::now() + std::chrono::milliseconds(interval);
				if (func() || stopTimer || checkWavCount > 600) {
					stopTimer = false;
					_MESSAGE("finished playing xVASynth wav %d", checkWavCount);
					playingXVAS = false;
					return;
				}
				std::this_thread::sleep_until(x);

			}
		}).detach();
	}

	wstring GetXVAFolder() {
		if (XVAFolder == L"") {
			wchar_t* folder = NULL;

			if (strlen(kCustomTempPath.GetData().s) == 0) {
				::SHGetKnownFolderPath(FOLDERID_RoamingAppData, KF_FLAG_CREATE, NULL, &folder);

				XVAFolder = folder;
				XVAFolder += L"\\xVASynth\\realTimeTTS\\";
			}
			else {
				const char* c = kCustomTempPath.GetData().s;
				wstring str(c, c + strlen(c));
				XVAFolder = str;
				_MESSAGE("Custom xVASynth path: %s", string(str.begin(), str.end()).c_str());
			}
			_MESSAGE("Folder set to %s", string(XVAFolder.begin(), XVAFolder.end()).c_str());
		}
		return XVAFolder;
	}

	bool CheckForWav() {
		wstring path = GetXVAFolder();

		if (path == L"") {
			return false;
		}

		path += L"output.wav";
		wstring_convert<codecvt_utf8_utf16<wchar_t>> conv;
		string str = conv.to_bytes(path);
		if(PlaySound(str.c_str(), NULL, SND_FILENAME | SND_NODEFAULT)) {
			playingXVAS = false;
			std::remove(str.c_str());
			return true;
		}
		checkWavCount++;
		return false;
	}

	void ImportTranslationFiles()
	{
		char	appdataPath[MAX_PATH];
		ASSERT(SUCCEEDED(SHGetFolderPath(NULL, CSIDL_LOCAL_APPDATA, NULL, SHGFP_TYPE_CURRENT, appdataPath)));

		std::string	modlistPath = appdataPath;
		modlistPath += "\\Skyrim Special Edition\\plugins.txt";

		// Parse mod list file to acquire translation filenames
		IFileStream modlistFile;
		if (modlistFile.Open(modlistPath.c_str()))
		{
			while (!modlistFile.HitEOF())
			{
				char buf[512];
				modlistFile.ReadString(buf, 512, '\n', '\r');

				// skip comments
				if (buf[0] == '#')
					continue;

				// Determine extension type
				std::string line = buf;

				// SE: added this
				if (line.length() > 0)
				{
					if (line.front() != '*')
						continue; // Skip not enabled files

					line = line.substr(1); // Remove the * from name
				}

				std::string::size_type lastDelim = line.rfind('.');
				if (lastDelim != std::string::npos)
				{
					std::string ext = line.substr(lastDelim);

					if (_stricmp(ext.c_str(), ".ESM") == 0 || _stricmp(ext.c_str(), ".ESP") == 0 || _stricmp(ext.c_str(), ".ESL") == 0)
					{
						std::string name = line.substr(0, lastDelim);
						ParseTranslation(name);
					}
				}
			}
		}

		modlistFile.Close();
	}

	void ParseTranslation(string name)
	{
		Setting* setting = GetINISetting("sLanguage:General");
		std::string path = "Interface\\Translations\\";

		// Construct translation filename
		path += name;
		path += "_";
		path += (setting && setting->GetType() == Setting::kType_String) ? setting->data.s : "ENGLISH";
		path += ".txt";

		BSResourceNiBinaryStream fileStream(path.c_str());
		if (!fileStream.IsValid())
			return;

		// Check if file is empty, if not check if the BOM is UTF-16
		UInt16	bom = 0;
		UInt32	ret = fileStream.Read(&bom, sizeof(UInt16));
		if (ret == 0) {
			return;
		}
		if (bom != 0xFEFF) {
			_MESSAGE("BOM Error, file must be encoded in UCS-2 LE: %s", path.c_str());
			return;
		}
		wstring_convert<codecvt_utf8_utf16<wchar_t>> converter;

		while (true)
		{
			wchar_t buf[512];

			UInt32	len = fileStream.ReadLine_w(buf, sizeof(buf) / sizeof(buf[0]), '\n');
			if (len == 0) // End of file
				return;

			// at least $ + wchar_t + \t + wchar_t
			if (len < 4 || buf[0] != '$')
				continue;

			wchar_t last = buf[len - 1];
			if (last == '\r')
				len--;

			// null terminate
			buf[len] = 0;

			UInt32 delimIdx = 0;
			for (UInt32 i = 0; i < len; i++)
				if (buf[i] == '\t')
					delimIdx = i;

			// at least $ + wchar_t
			if (delimIdx < 2)
				continue;

			// replace \t by \0
			buf[delimIdx] = 0;

			wchar_t* key = NULL;
			wchar_t* translation = NULL;
			BSScaleformTranslator::GetCachedString(&key, buf, 0);
			BSScaleformTranslator::GetCachedString(&translation, &buf[delimIdx + 1], 0);
			wstring  wkey(key);
			wstring  wt(translation);
			
			//_MESSAGE("Adding translation %s %s", skey.c_str(), st.c_str());
			transMap[converter.to_bytes(wkey)] = converter.to_bytes(wt);
		}
	}


	void ReloadXML(StaticFunctionTag* base) 
	{
		LoadXML();
	}

	void LoadXML()
	{

		wstring_convert<codecvt_utf8_utf16<wchar_t>> converter;

		randomTexts = { "I <emph>used</emph> to be an adventurer like you. Then I took an <emph>arrow</emph> in the knee.", "What is better - to be <emph>born</emph> good, or to <emph>overcome</emph> your evil nature through <emph>great</emph> effort?", "Let me <emph>guess</emph>, Someone stole your <emph>sweetroll</emph>!", "My cousin is out <emph> fighting dragons.</emph> And what do <emph>I</emph> get? <emph>Guard</emph> duty.", "Oh, there <emph>once</emph> was a hero named <emph>Ragnar</emph> the Red, who came <emph>riding</emph> to Whiterun from old <emph>Rorikstead</emph>. ", "And the <emph>braggart</emph> did swagger and <emph>brandish</emph> his blade as he <emph>told</emph> of bold battles and <emph>gold</emph> he had made. ", "But <emph>then</emph> he went quiet, did <emph>Ragnar</emph> the Red when he <emph>met</emph> the shield-maiden <emph>Matilda</emph>, who said; ", "Oh, you <emph>talk</emph> and you <emph>lie</emph> and you <emph>drink</emph> all our mead; now I <emph>think</emph> it's high time that you <emph>lie</emph> down and <emph>bleed</emph>!. ", "And so <emph>then</emph> came <emph>clashing</emph> and <emph>slashing</emph> of steel, as the <emph>brave</emph> lass Matilda <emph>charged</emph> in, full of zeal. ", "And the <emph>braggart</emph> named <emph>Ragnar</emph> was boastful no more... when his <emph>ugly</emph> red <emph>head</emph> rolled <emph>around</emph> on the <emph>floor</emph>!", "There are <emph>formalities</emph> that must be observed, at the first meeting of two of the <emph>dov</emph>. ", "My <emph>favorite</emph> drinking buddy! Let's get some <emph>mead</emph>.", "Perhaps we should find a random stranger to murder. Practice <emph>does</emph> make perfect.", "You stink of <emph>death</emph> my friend. I <emph>salute</emph> you.", "You <emph>never</emph> should have <emph>come</emph> here.", "Babette, my girl - <emph>pack</emph> your things. We're <emph>moving</emph>!", "<emph>Enough</emph>! I will <emph>not</emph> stand idly by while a <emph>dragon</emph> burns my hold and <emph>slaughters</emph> my <emph>people</emph>!", "I'm told they call me <emph>Dirge</emph> because I'm the <emph>last</emph> thing you hear before they put you in the ground.", "Been so long since I killed a Stormcloak, my sword arm's getting <emph>flabby</emph>.", "If these ruins frighten you, take comfort from the knowledge that <emph>I</emph> am here.", "Look at <emph>that</emph>. Am I <emph>drunk</emph>? I must be <emph>drunk</emph>.", "I used to be the <emph>top</emph> soldier for the Stormcloaks, then i took a <emph>sword</emph> to the chest", "And who are <emph>you</emph> to challenge <emph>me</emph>? I've conquered <emph>mortality</emph> itself. ", "I've <emph>spat</emph> in the eyes of the <emph>Daedric Lords</emph>! ", "This is <emph>my</emph> realm now, I've <emph>sacrificed</emph> too much to let you <emph>take</emph> it from me!", "I'll see you <emph>burn</emph>!", "You do not even know our <emph>tongue</emph>, do you? ", "Such <emph>arrogance</emph>, to <emph>dare</emph> take for yourself the name of <emph>Dovah</emph>!", "Well well... Another <emph>maggot</emph> to <emph>squash</emph> beneath my boot!", "Either <emph>I'm</emph> drunk, or <emph>you're</emph> naked. Possibly both.", "I'll see you in pieces!", "I've been hunting and fishing in these parts for <emph>years</emph>.", "I've got my <emph>eyes</emph> on you.", "We're one of the <emph>same</emph> kind, you and I. I'm <emph>happy</emph> to have met you.", "Looking to <emph>protect</emph> yourself? Or <emph>deal</emph> some damage?", "I got to <emph>thinking</emph>, maybe <emph>I'm</emph> the Dragonborn and I <emph>just</emph> don't <emph>know</emph> it yet.", "It's a <emph>fine</emph> day with <emph>you</emph> around!", "My father said I should go to <emph>college</emph>, but he didnt say <emph>which</emph> one.", "I'd be a lot <emph>happier</emph> and a lot <emph>warmer</emph> with a <emph>belly</emph> full of <emph>mead</emph>.", "I heard about <emph>you</emph> and your <emph>honeyed</emph> words.", "I guess you don't have potatoes in your ears <emph>after</emph> all." };

		tinyxml2::XMLDocument doc;

		if (doc.LoadFile("Data\\skse\\plugins\\FuzRoBork.xml") != tinyxml2::XMLError::XML_SUCCESS) {
			_MESSAGE("XML file not found; exiting");
			return;
		}

		XMLElement* root = doc.FirstChildElement("config");

		if (!root) {
			_MESSAGE("root tag not found in XML; exiting");
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
				string text(xKey->GetText());
				findReplace(text, "[", "<");
				findReplace(text, "]", ">");
				hotkeyTexts[idx] = text;
				xKey = xKey->NextSiblingElement("hotkey");
				idx++;
			}
			_MESSAGE("Loaded hotkeys");
		}

		
		// random texts

		XMLElement* xRandoms = root->FirstChildElement("random");

		if (xRandoms) {
			randomTexts.clear();
			XMLElement* xRand = xRandoms->FirstChildElement("text");
			while (xRand) {
				string chars = xRand->GetText();
				findReplace(chars, "[", "<");
				findReplace(chars, "]", ">");
				string str(chars);
				randomTexts.push_back(str.c_str());
				xRand = xRand->NextSiblingElement("text");
			}
			_MESSAGE("Loaded %d random texts", randomTexts.size());
		}
		
		// specific npcs

		XMLElement* xNPCs = root->FirstChildElement("npcs");

		if (xNPCs) {
			NPCList.clear();
			XMLElement* xNPC = xNPCs->FirstChildElement("npc");
			while (xNPC) {
				const char* name = "";
				const char* race = "";
				if (xNPC->FindAttribute("name") != 0)
					name = xNPC->FindAttribute("name")->Value();
				if (xNPC->FindAttribute("race") != 0)
					race = xNPC->FindAttribute("race")->Value();


				if (name == "" && race == "")
					continue;

				XMLElement* xLang = xNPC->FirstChildElement("language");
				string lang = "";
				if(xLang)
					lang = string(xLang->GetText());

				XMLElement* xSex = xNPC->FirstChildElement("sex");
				int sex = -1;
				if (xSex) {
					string ssex = xSex->GetText();
					try {
						sex = stoi(ssex);
					}
					catch (invalid_argument ex) {

					}
				}

				XMLElement* xVol = xNPC->FirstChildElement("volume");
				float vol = 30;
				if (xVol) {
					string svol = xVol->GetText();
					try {
						vol = stof(svol);
					}
					catch (invalid_argument ex) {

					}
				}

				XMLElement* xPitch = xNPC->FirstChildElement("pitch");
				int pitch = 0;
				if (xPitch) {
					string spitch = xPitch->GetText();
					try {
						pitch = stoi(spitch);
					}
					catch (invalid_argument ex) {

					}
				}

				XMLElement* xRate = xNPC->FirstChildElement("rate");
				float rate = 0;
				if (xRate) {
					string srate = xRate->GetText();
					try {
						rate = stof(srate);
					}
					catch (invalid_argument ex) {

					}
				}

				NPCObj obj(name, race, lang.c_str(), sex, rate, vol, pitch);
				NPCList.push_back(obj);
				xNPC = xNPC->NextSiblingElement("npc");
			}
			_MESSAGE("Loaded %d npcs", NPCList.size());
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
				string find(xFind->GetText());
				string replace(xReplace->GetText());
				findReplace(replace, "[", "<");
				findReplace(replace, "]", ">");
				fixes[find.c_str()] = replace.c_str();
				xFix = xFix->NextSiblingElement("fix");
			}
			_MESSAGE("Loaded fixes");
		}

		// actions

		XMLElement* pcs = root->FirstChildElement("pcs");

		if (pcs) {


			XMLElement* xPC = pcs->FirstChildElement("pc");

			if (xPC) 
			{

				XMLElement* xTopic = xPC->FirstChildElement("topic");

				while (xTopic) {

					vector<string> optionList;

					XMLElement* xOption = xTopic->FirstChildElement("option");

					const char* aName = (const char*)xTopic->Attribute("name");

					OutputDebugString(aName);
					OutputDebugString("\n");

					while (xOption) {
						if (xOption->GetText()) {
							OutputDebugString(xOption->GetText());
							OutputDebugString("\n");
							optionList.push_back(string(xOption->GetText()));
						}
						else
							optionList.push_back(" "); // add empties to make less common

						xOption = xOption->NextSiblingElement("option");
					}

					if (optionList.size() != 0) {
						actionList[string(aName)] = optionList;
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

	void findReplace(string& str, string oldStr, string newStr) {
		std::size_t pos = 0;
		while ((pos = str.find(oldStr, pos)) != string::npos) {
			str.replace(pos, oldStr.size(), newStr);
			pos += newStr.size();
		}
	}

	const wchar_t* GetWC(const char* c)
	{
		const size_t cSize = strlen(c) + 1;
		wchar_t* wc = new wchar_t[cSize];
		mbstowcs(wc, c, cSize);

		return wc;
	}

	void replaceUnspeakables(string& str) {
		
		for (auto i : fixes)
		{
			findReplace(str, i.first, i.second);
		}

		for (auto i : transMap)
		{
			findReplace(str, i.first.c_str(), i.second.c_str());
		}

		// remove \t \r, etc

		int idx = 0;
		boolean par = false;

		string newStr = "";
		while (idx < str.size()) {
			if (str[idx] != '\r' && str[idx] != '\n' && str[idx] != '\t')
				newStr += str[idx];
			idx++;
		}
		str = newStr.c_str();

		// remove parenthesis and brackets if option ticked

		if (kSpeakParentheses.GetData().i == 0) {
			idx = 0;
			par = false;
			newStr = "";
			while (idx < str.size()) {
				if (!par && str[idx] == '(')
					par = true;
				else if (par && str[idx] == ')')
					par = false;
				else if (!par)
					newStr += str[idx];
				idx++;
			}
			idx = 0;
			par = false;
			str = newStr;
			newStr = "";
			while (idx < str.size()) {
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

		_MESSAGE("speakTask: '%s'", nSpeech.speech.c_str());
		string speech = nSpeech.speech;
		replaceUnspeakables(speech);

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
		wstring_convert<codecvt_utf8_utf16<wchar_t>> converter;

		wstring wlang = converter.from_bytes(nSpeech.lang);
		wstring wspeech = converter.from_bytes(speech);

		std::wstring wideLang(wlang.size() + 1, L' ');
		std::size_t newLength = std::mbstowcs(&wideLang[0], nSpeech.lang, wideLang.size());
		wideLang.resize(newLength);

		_MESSAGE("Creating SAPI voice");

		// Create the SAPI voice.
		hr = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (void**)&pVoice);

		if (SUCCEEDED(hr))
		{
			_MESSAGE("searching for voices in registry");
			for (int i = 0; i < size(speechPaths); i++) {
				found = findSpeechToken(&cpToken, wideLang, speechPaths[i].c_str());
				if (found)
					break;
			}
			if (!found) {
				for (int i = 0; i < size(speechPaths); i++) {
					hr = SpFindBestToken(speechPaths[i].c_str(), NULL, NULL, &cpToken);
					if (SUCCEEDED(hr))
						break;
				}
			}
		}
		if (SUCCEEDED(hr))
		{

			hr = pVoice->SetVoice(cpToken);
			_MESSAGE("set voice");

		}
		if (SUCCEEDED(hr))
		{
			hr = pVoice->SetRate(nSpeech.rate);
			_MESSAGE("set rate");

		}
		if (SUCCEEDED(hr))
		{

			hr = pVoice->SetVolume(nSpeech.vol);
			_MESSAGE("set volume");
		}
		if (SUCCEEDED(hr))
		{
			if (speech.size() < 1) {
				_MESSAGE("No speech string");
				return;
			}

			// pitch

			wspeech = L"<pitch absmiddle=\"" + to_wstring(nSpeech.pitch) + L"\">" + wspeech + L"</pitch>";

			//_MESSAGE("Speaking '%s'", lSpeech);

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
			hr = pVoice->Speak(wspeech.c_str(), SPF_PURGEBEFORESPEAK | SPF_ASYNC, NULL);
			actionSpeaking = false;
			_MESSAGE("Spoke");
		}


		::CoUninitialize();
	}


	void startNarratorSpeech(const char* text) { // unchecked

		//_MESSAGE("Starting narrator speech %s", text.c_str());

		const char* lang = kNarratorLanguage.GetData().s;

		float rate = kNarratorVoiceRate.GetData().f;

		float volume = kNarratorVoiceVolume.GetData().f;

		float pitch = kNarratorVoicePitch.GetData().i;

		SpeakObj pSpeech(text, lang, rate, volume, pitch);

		thread t1(speakTask, pSpeech);
		t1.detach();
	}

	void startStoredBookSpeech(StaticFunctionTag* base) {
		if (strlen(storedBookSpeech) == 0)
			return;
		stopSpeaking();
		OutputDebugString("Starting stored book speech\n");
		startNarratorSpeech(storedBookSpeech);
	}
	void startStoredPagesSpeech(StaticFunctionTag* base) {
		if (strlen(storedPagesSpeech) == 0)
			return;
		stopSpeaking();
		OutputDebugString("Starting stored book speech\n");
		startNarratorSpeech(storedPagesSpeech);
	}
	void startBookSpeech(const char* text) {
		stopSpeaking();
		storeBookSpeech(text);
		OutputDebugString("Starting book speech\n");
		startNarratorSpeech(text);
	}
	
	void storeBookSpeech(const char* text) {
		OutputDebugString("Storing book speech\n");
		storedBookSpeech = text;
		if (kPlayBooks.GetData().i == 1) {
			stopSpeaking();
			startNarratorSpeech(text);
		}
	}

	void storeFirstPagesSpeech(const char* text) {
		OutputDebugString("Storing first pages speech\n");
		storedPagesSpeech = text;
		if (kPlayBookPages.GetData().i == 1 && kPlayBooks.GetData().i == 0) {
			stopSpeaking();
			startNarratorSpeech(text);
		}
	}
	
	void storePagesSpeech(const char* text) {
		OutputDebugString("Storing pages speech\n");
		storedPagesSpeech = text;
		if (kPlayBookPages.GetData().i == 1) {
			stopSpeaking();
			startNarratorSpeech(text);
		}
	}
	void sendToxVASynth(SpeakObj obj) {
		if (sendingXVAS || playingXVAS || gameVoices.size() == 0)
			return;
		sendingXVAS = true;
		_MESSAGE("Writing speech to file for xVASynth");

		string game(kxVASynthGame.GetData().s);
		string voice = "";

		if (obj.speech.size() > 0) {
			StringToLower(game);

			if (game.size() == 0 && obj.lang != "xVASynth") {
				_MESSAGE("Empty game string sent");
				sendingXVAS = false;
				return;
			}

			if (gameVoices.count(game) == 0) {
				_MESSAGE("Game %s not found", game);
				sendingXVAS = false;
				return;
			}
			if (gameVoices[game].size() == 0) {
				_MESSAGE("Game %s has no voices", game);
				sendingXVAS = false;
				return;
			}

			UInt32 idx = kxVASynthVoice.GetData().u;

			if (idx >= gameVoices[game].size())
				idx = 0;

			_MESSAGE("Voice id is %d", idx);

			if (idx >= gameVoices[game].size()) {
				_MESSAGE("Index %d out of bounds for game %s", idx, game);
				return;
			}
			
			voice = gameVoices[game][idx]["id"];

			_MESSAGE("Voice name is %s", string(gameVoices[game][idx]["name"]).c_str());
		}
		else {
			_MESSAGE("Sending initialization data only");
		}

		regex tags("<[^>]*>");
		obj.speech = regex_replace(obj.speech, tags, "").c_str();

		string speech(obj.speech);

		json j;
		j["voiceId"] = voice;
		j["gameId"] = game;
		//j["xVA_pitch"] = 1 + obj.pitch / 10 * 3.33;
		j["ffmpeg_pitchMult"] = 1 + obj.pitch / 10;
		//j["xVA_pace"] = rate;
		j["vol"] = obj.vol / 100 * 3.33;
		j["text"] = speech;
		j["done"] = false;
		j["use_ffmpeg"] = true;
		j["pad_start"] = 0;
		j["pad_end"] = 200;
		j["ffmpeg_tempo"] = 1 + obj.rate / 10;

		wstring fpath = GetXVAFolder();

		if (fpath == L"") {
			_MESSAGE("xVAFolder not set");
			return;
		}

		wstring wpath = fpath + L"output.wav";
		wstring_convert<codecvt_utf8_utf16<wchar_t>> conv;
		string str = conv.to_bytes(wpath);
		remove(str.c_str());

		wpath = fpath + L"xVASynthText.json";
		string path = conv.to_bytes(wpath);
		_MESSAGE("Writing to %s", path.c_str());
		ofstream o(wpath);
		o << setw(4) << j << endl;

		_MESSAGE("Wrote speech to file for xVASynth");

		sendingXVAS = false;
		checkWavCount = 0;
		if (obj.speech.size() > 0) {
			stopTimer = false;
			playingXVAS = true;
			_MESSAGE("Waiting for wav file creation");

			wav_find_timer_start(CheckForWav, 100);
		}
	}

	void speakLoadingScreen(const char* text) {
		startNarratorSpeech(text);
	}

	SpeakObj GetNPCSpeech(TESNPC* npc, string text) {
		
		
		_MESSAGE("NPC Speaking '%s'", text.c_str());


		// standard values

		UInt32 sex = 1; // female

		sex = npc ? CALL_MEMBER_FN(npc, GetSex)() : 1;

		const char* lang = sex == 0 ? kMaleLanguage.GetData().s : kFemaleLanguage.GetData().s;

		float rate = sex == 0 ? kMaleVoiceRate.GetData().f : kFemaleVoiceRate.GetData().f;

		float volume = sex == 0 ? kMaleVoiceVolume.GetData().f : kFemaleVoiceVolume.GetData().f;

		float pitch = sex == 0 ? kMaleVoicePitch.GetData().i : kFemaleVoicePitch.GetData().i;

		if (npc) {

			// specific NPC override

			const char* nName = npc->fullName.GetName();
			const char* nRace = npc->race.race->fullName.GetName();

			_MESSAGE("Looking for custom npc: %s %s (%s)", nRace, nName, sex == 0 ? "male" : "female");

			NPCObj npc;

			if (GetNPC(nName, nRace, npc)) {

				if (strlen(npc.lang) > 0)
					lang = npc.lang;
				if (npc.rate > -11 && npc.rate < 11)
					rate = npc.rate;
				if (npc.vol > -1 && npc.vol < 101)
					volume = npc.vol;
				if (npc.pitch > -11 && npc.pitch < 11)
					pitch = npc.pitch;

				_MESSAGE("Got language %s", lang);
			}
		}

		SpeakObj pSpeech(text, lang, rate, volume, pitch);

		return pSpeech;

		//_MESSAGE("NPC Speaking initiated successfully");
	}

	void startPlayerSpeech(const char* text) {
		//_MESSAGE("Starting Player Speech: %s", _title.c_str());

		TESNPC* pc = DYNAMIC_CAST((*g_thePlayer)->baseForm, TESForm, TESNPC);

		const char* lang = kPlayerLanguage.GetData().s;

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

		SpeakObj pSpeech(text, lang, rate, volume, pitch);

		thread t3(speakTask, pSpeech);
		t3.detach();
	}

	void testSpeech(StaticFunctionTag* base, BSFixedString _which) {

		if (isSpeaking() || isXVASpeaking())
			return;

		string which(_which.data);

		if (which.compare("XX") == 0 || which.compare("xx") == 0) {
			if (sendingXVAS || playingXVAS)
				return;
			SpeakObj pSpeech("", "xVASynth", kPlayerVoiceRate.GetData().f, kPlayerVoiceVolume.GetData().f, kPlayerVoicePitch.GetData().i);
			thread t1(speakTask, pSpeech);
			t1.detach();
			return;
		}

		srand(time(0));
		int r = static_cast<double>(rand() % randomTexts.size());

		const char* speech = randomTexts[r].c_str();

		_MESSAGE("Speaking random %s text", which.c_str());

		if (which.compare("P") == 0 || which.compare("p") == 0) {
			startPlayerSpeech(speech);
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
			if (sendingXVAS || playingXVAS)
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
	
	boolean isXVASpeaking() {
		return playingXVAS;
	}

	void stopSpeaking() {
		_MESSAGE("Stopping speech");
		PlaySound(NULL, NULL, SND_SYNC);
		playingXVAS = false;
		stopTimer = true;
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
		_MESSAGE("Beginning to parse config settings");

		string strings(_strings.data);
		string booleans(_booleans.data);
		string floats(_floats.data);

		vector<string> stringV = explode("^", strings);
		vector<string> boolV = explode("^", booleans);
		vector<string> floatV = explode("^", floats);

		kPlayerLanguage.SetDataAsString(stringV[0].c_str());
		kFemaleLanguage.SetDataAsString(stringV[1].c_str());
		kMaleLanguage.SetDataAsString(stringV[2].c_str());
		kNarratorLanguage.SetDataAsString(stringV[3].c_str());
		kxVASynthGame.SetDataAsString(stringV[4].c_str());
		try {
			kxVASynthVoice.SetUInt(stringV[5].size() > 0 ? stoi(stringV[5].c_str()) : 0);
		}
		catch (invalid_argument ex) {
			_MESSAGE("Error converting voice index to int");
		}

		if (boolV.size() == 9) {
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
		_MESSAGE("Config settings completed");

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
		wstring ks = wstring(key);
		_MESSAGE("voices available in key %s:", string(ks.begin(), ks.end()).c_str());

		gLog.Indent();
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
				wstring nameS = wstring(nameValue);
				_MESSAGE(string(nameS.begin(), nameS.end()).c_str());

				nameA->push_back(nameValue);

			}
		}
		gLog.Outdent();

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

		findReplace(text, "[", "<");
		findReplace(text, "]", ">");
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

		
		if (actionList.find(_which.data) != actionList.end()) {

			int r = 0;
			if (actionList[_which.data].size() > 1) {
				r = static_cast<double>(rand()) / RAND_MAX * actionList[_which.data].size();

				while (strcmp(_lastTopic, _which.data) == 0 && r == rLast) // prevent duplicates
					r = static_cast<double>(rand()) / RAND_MAX * actionList[_which.data].size();
			}

			actionSpeaking = true;
			stopSpeaking();
			string text = actionList[_which.data][r];
			findReplace(text, "[", "<");
			findReplace(text, "]", ">");
			startPlayerSpeech(text.c_str());
			return;

		}
	}
	void sendLanguages(StaticFunctionTag* t, VMArray<BSFixedString> names) {

		_MESSAGE("Sending languages");
		vector<LPWSTR> nameA;

		addCatToVector(&nameA, speechPaths[0].c_str());
		addCatToVector(&nameA, speechPaths[1].c_str());
		addCatToVector(&nameA, speechPaths[2].c_str());
		addCatToVector(&nameA, speechPaths[3].c_str());
		addCatToVector(&nameA, speechPaths[4].c_str());

		int langcount = 0;
		for (UInt32 i = 0; i < nameA.size(); i++) {


			char nameBuffer[500]; 
			// First arg is the pointer to destination char, second arg is
			// the pointer to source wchar_t, last arg is the size of char buffer
			wcstombs(nameBuffer, nameA[i], 500);

			BSFixedString bst;
			bst.data = nameBuffer;
			_MESSAGE("adding language %s", nameBuffer);

			names.Set(&bst, i);
			langcount++;
		}
		BSFixedString vs;
		vs.data = "xVASynth";
		names.Set(&vs, nameA.size());
		_MESSAGE("Sent %d languages to MCM", langcount);
	}
	void sendXGames(StaticFunctionTag* t, VMArray<BSFixedString> xGames) {

		_MESSAGE("Sending games for xVASynth");

		
		wstring path = GetXVAFolder();

		if (path == L"") {
			_MESSAGE("xVAFolder not set");
			return;
		}
		path += L"xVASynthVoices.json";

		std::ifstream i(path);

		if (i.good()) {
			//_MESSAGE("File exists %s", path.c_str());

			json j;
			i >> j;
			if (j.is_object()) {

				_MESSAGE("adding xVASynth voices");

				json jg = j["games"];
				if (jg.is_object()) {
					_MESSAGE("got games");
					UInt32 gameIndex = 0;
					for (json::iterator it = jg.begin(); it != jg.end(); ++it) {
						string key = it.key();
						_MESSAGE("got %d voices for %s", it.value().size(), key);

						BSFixedString vs;
						vs.data = key.c_str();
						xGames.Set(&vs, gameIndex);

						gameIndex++;

						StringToLower(key);

						gameVoices[key] = it.value();
					}
					_MESSAGE("Sent voices");

				}
			}
		}
		else {
			_MESSAGE("xVAFolder not found");
		}
	}

	void sendXGameVoices(StaticFunctionTag* t, BSFixedString game, VMArray<BSFixedString> xVoices) {

		string gameData(game.data);
		StringToLower(gameData);
		if (gameVoices.count(gameData) == 0) {
			_MESSAGE("No voices for %s", game.data);
			return;
		}
		_MESSAGE("Sending %d voices for game %s", gameVoices.count(gameData), game.data);

		json vlist = gameVoices[gameData];

		for (UINT32 i = 0; i < vlist.size(); i++) {

			string voice = vlist[i]["name"];

			BSFixedString bst;
			bst.data = voice.c_str();
			_MESSAGE("adding xVASynth voice %s", voice.c_str());

			xVoices.Set(&bst, i);
		}
	}

	bool RegisterFuncs(VMClassRegistry* registry)
	{
		_MESSAGE("Registering Papyrus functions");

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
		registry->RegisterFunction(new NativeFunction1 <StaticFunctionTag, void, VMArray<BSFixedString>>("FuzRoBork_getLanguages", "BorkMCM", sendLanguages, registry));
		registry->RegisterFunction(new NativeFunction1 <StaticFunctionTag, void, VMArray<BSFixedString>>("FuzRoBork_getXGames", "BorkMCM", sendXGames, registry));
		registry->RegisterFunction(new NativeFunction2 <StaticFunctionTag, void, BSFixedString, VMArray<BSFixedString>>("FuzRoBork_getXVoices", "BorkMCM", sendXGameVoices, registry));
		registry->RegisterFunction(new NativeFunction0 <StaticFunctionTag, void>("FuzRoBork_reloadXML", "BorkMCM", ReloadXML, registry));

		_MESSAGE("Registered Papyrus functions");

		return true;
	}

}