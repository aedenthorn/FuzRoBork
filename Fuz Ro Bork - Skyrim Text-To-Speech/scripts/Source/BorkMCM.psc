Scriptname BorkMCM extends SKI_ConfigBase  

bool skipVal = true
bool playPlayerVal = true
bool playNPCVal = true
bool playBookVal = false
bool playPagesVal = false
bool playLoadVal = false
bool PlayActionsVal = false
bool overrideAllVal = false
bool playParaVal = false
bool enableKeysVal = false

int pLangVal = 0
int fLangVal = 0
int mLangVal = 0
int nLangVal = 0

int nSexVal = 0

int bookReadKeyVal = 157
int entrySpeakKeyVal = 42
int stopSpeakKeyVal = 45
int HotKey1Val = 2
int HotKey2Val = 3
int HotKey3Val = 4
int HotKey4Val = 5
int HotKey5Val = 6
int HotKey6Val = 7
int HotKey7Val = 8
int HotKey8Val = 9
int HotKey9Val = 10

float msVal = 500.0

float pRateVal = 0.0
float fRateVal = 0.0
float mRateVal = 0.0
float nRateVal = 0.0

float pVolVal = 30.0
float fVolVal = 30.0
float mVolVal = 30.0
float nVolVal = 30.0

float pPitchVal = 0.0
float fPitchVal = 0.0
float mPitchVal = 0.0
float nPitchVal = 0.0

string[] LangList
string[] LCIDList
string[] SexList

int function GetVersion()
    return 18
endFunction

event OnVersionUpdate(int a_version)
    if (a_version > 1)
        OnConfigInit()
    endIf
endEvent

Function FuzRoBork_setMCMConfig(string strings, string booleans, string floats) global native
Function FuzRoBork_storedPagesSpeech() global native
Function FuzRoBork_storedBookSpeech() global native
Function FuzRoBork_testSpeech(string which) global native
Function FuzRoBork_hotSpeech(int which) global native
Function FuzRoBork_stopSpeech() global native
Function FuzRoBork_getLanguages(string[] langs) global native
Function FuzRoBork_reloadXML() global native

event OnGameReload()
    parent.OnGameReload() ; Don't forget to call the parent!
    doSendVals()

    If(enableKeysVal)
		RegisterForKey(entrySpeakKeyVal)
		RegisterForKey(bookReadKeyVal)
		RegisterForKey(stopSpeakKeyVal)
        RegisterForKey(HotKey1Val)
        RegisterForKey(HotKey2Val)
        RegisterForKey(HotKey3Val)
        RegisterForKey(HotKey4Val)
        RegisterForKey(HotKey5Val)
        RegisterForKey(HotKey6Val)
        RegisterForKey(HotKey7Val)
        RegisterForKey(HotKey8Val)
        RegisterForKey(HotKey9Val)
    EndIf

endEvent

Event OnConfigInit()
    Pages = new string[2]
    Pages[0] = "Main"
    Pages[1] = "Voices"

    SexList  = new string[2]
    SexList[0] = "Male"
    SexList[1] = "Female"

EndEvent

Event OnConfigClose()
    doSendVals()
EndEvent

event OnPageReset(string page)
    {Called when a new page is selected, including the initial empty page}
    if(page == "")
        LoadCustomContent("FuzRoBork/bork_logo.dds", 0,0)
        return
    else
        UnloadCustomContent()
    endIf

    if(page == "Main")
    
        SetCursorFillMode(TOP_TO_BOTTOM)

        AddHeaderOption("Components")
        AddToggleOptionST("playPlayerST","Speak Player Text", playPlayerVal)
        AddToggleOptionST("playNPCST","Speak NPC Text", playNPCVal)
        AddToggleOptionST("playBookST","Auto Book Speak", playBookVal)
        AddToggleOptionST("playPagesST","Auto Page Turn Speak", playPagesVal)
        AddToggleOptionST("playLoadST","Speak Loading Screens", playLoadVal)
        AddToggleOptionST("playActionsST","Voice Player Actions", PlayActionsVal)

        AddHeaderOption("Options")
        AddSliderOptionST("msST","Text duration", msVal, "{0} ms / word")
        AddToggleOptionST("playParaST","Speak Parentheticals", playParaVal)
        AddToggleOptionST("enableKeysST","Enable Keys", enableKeysVal)
        AddTextOptionST("reloadXMLST", "", "Reload XML")

        SetCursorPosition(1)

        int flag = OPTION_FLAG_DISABLED
        
        If(enableKeysVal)
            flag = OPTION_FLAG_NONE
        EndIf
        
        AddHeaderOption("Keys")
        AddKeyMapOptionST("bookReadKeyST", "Book Read Key", bookReadKeyVal,flag)
        AddKeyMapOptionST("entrySpeakKeyST", "Dialog Speak Key", entrySpeakKeyVal,flag)
        AddKeyMapOptionST("stopSpeakKeyST", "Stop Speaking Key", stopSpeakKeyVal,flag)
    
        AddKeyMapOptionST("HotKey1ST", "HotKey 1", HotKey1Val,flag)
        AddKeyMapOptionST("HotKey2ST", "HotKey 2", HotKey2Val,flag)
        AddKeyMapOptionST("HotKey3ST", "HotKey 3", HotKey3Val,flag)
        AddKeyMapOptionST("HotKey4ST", "HotKey 4", HotKey4Val,flag)
        AddKeyMapOptionST("HotKey5ST", "HotKey 5", HotKey5Val,flag)
        AddKeyMapOptionST("HotKey6ST", "HotKey 6", HotKey6Val,flag)
        AddKeyMapOptionST("HotKey7ST", "HotKey 7", HotKey7Val,flag)
        AddKeyMapOptionST("HotKey8ST", "HotKey 8", HotKey8Val,flag)
        AddKeyMapOptionST("HotKey9ST", "HotKey 9", HotKey9Val,flag)

    elseIf(page == "Voices")    

        LangList = new string[100]
        
        FuzRoBork_getLanguages(LangList)

        SetCursorFillMode(TOP_TO_BOTTOM)

        SetCursorPosition(0) ; Move cursor to top right position

        AddHeaderOption("Player Character")
        AddMenuOptionST("pLangST","Player Language", pLangVal)
        AddSliderOptionST("pRateST","Player Speech Rate", pRateVal, "{1}x")
        AddSliderOptionST("pVolST","Player Volume", pVolVal, "{0}%")
        AddSliderOptionST("pPitchST","Player Pitch", pPitchVal)
        AddTextOptionST("pLangTST","", "[Test]")

        AddHeaderOption("Female NPCs")

        AddMenuOptionST("fLangST","Female NPC Language", fLangVal)
        AddSliderOptionST("fRateST","Female Speech Rate", fRateVal, "{1}x")
        AddSliderOptionST("fVolST","Female NPC Volume", fVolVal, "{0}%")
        AddSliderOptionST("fPitchST","Female NPC Pitch", fPitchVal)
        AddTextOptionST("fLangTST","", "[Test]")

        SetCursorPosition(1)
        
        AddHeaderOption("Male NPCs")

        AddMenuOptionST("mLangST","Male NPC Language", mLangVal)
        AddSliderOptionST("mRateST","Male Speech Rate", mRateVal, "{1}x")
        AddSliderOptionST("mVolST","Male NPC Volume", mVolVal, "{0}%")
        AddSliderOptionST("mPitchST","Male NPC Pitch", mPitchVal)
        AddTextOptionST("mLangTST", "", "[Test]")

        AddHeaderOption("Book Narrator")

        AddMenuOptionST("nLangST","Narrator Language", nLangVal)
        AddSliderOptionST("nRateST","Narrator Speech Rate", nRateVal, "{1}x")
        AddSliderOptionST("nVolST","Narrator Volume", nVolVal, "{0}%")
        AddSliderOptionST("nPitchST","Narrator Pitch", nPitchVal)
        AddTextOptionST("nLangTST", "", "[Test]")
    endIf

endEvent

state playPlayerST; TOGGLE
    event OnSelectST()
        playPlayerVal = !playPlayerVal
        SetToggleOptionValueST(playPlayerVal)
    endEvent

    event OnDefaultST()
        playPlayerVal= true
        SetToggleOptionValueST( playPlayerVal)
    endEvent

    event OnHighlightST()
        SetInfoText("Play speech for PC dialogue choices")
    endEvent
endState

state playNPCST; TOGGLE
    event OnSelectST()
        playNPCVal= !playNPCVal
        SetToggleOptionValueST(playNPCVal)
    endEvent

    event OnDefaultST()
        playNPCVal= true
        SetToggleOptionValueST( playNPCVal)
    endEvent

    event OnHighlightST()
        SetInfoText("Play speech for unspoken NPC dialogue")
    endEvent
endState

state playBookST; TOGGLE
    event OnSelectST()
        playBookVal= !playBookVal
        SetToggleOptionValueST(playBookVal)
    endEvent

    event OnDefaultST()
        playBookVal= false
        SetToggleOptionValueST( playBookVal)
    endEvent

    event OnHighlightST()
        SetInfoText("Automatically read books aloud when opened")
    endEvent
endState

state playPagesST; TOGGLE
    event OnSelectST()
        playPagesVal= !playPagesVal
        SetToggleOptionValueST(playPagesVal)
    endEvent

    event OnDefaultST()
        playPagesVal= false
        SetToggleOptionValueST( playPagesVal)
    endEvent

    event OnHighlightST()
        SetInfoText("Automatically read current pages aloud when pages are turned")
    endEvent
endState

state playLoadST; TOGGLE
    event OnSelectST()
        playLoadVal= !playLoadVal
        SetToggleOptionValueST(playLoadVal)
    endEvent

    event OnDefaultST()
        playLoadVal= false
        SetToggleOptionValueST( playLoadVal)
    endEvent

    event OnHighlightST()
        SetInfoText("Read loading screen text aloud.")
    endEvent
endState

state playActionsST; TOGGLE
    event OnSelectST()
        playActionsVal = !playActionsVal
        SetToggleOptionValueST(playActionsVal)
    endEvent

    event OnDefaultST()
        playActionsVal= true
        SetToggleOptionValueST( playActionsVal)
    endEvent

    event OnHighlightST()
        SetInfoText("Play speech for PC actions set in Data\\skse\\Plugins\\FuzRoBork.xml")
    endEvent
endState

state playParaST; TOGGLE
    event OnSelectST()
        playParaVal= !playParaVal
        SetToggleOptionValueST(playParaVal)
    endEvent

    event OnDefaultST()
        playParaVal= false
        SetToggleOptionValueST( playParaVal)
    endEvent

    event OnHighlightST()
        SetInfoText("Play parenthetical speech, e.g. (Persuade) or (10 Gold)")
    endEvent
endState

state reloadXMLST; TEXT
    event OnSelectST()
		FuzRoBork_reloadXML()
    endEvent

    event OnHighlightST()
        SetInfoText("Reload XML from file")
    endEvent
endState


state enableKeysST; TOGGLE
    event OnSelectST()
        enableKeysVal= !enableKeysVal
        SetToggleOptionValueST(enableKeysVal)
        If(enableKeysVal)
            SetOptionFlagsST(OPTION_FLAG_NONE, false, "entrySpeakKeyST")
            SetOptionFlagsST(OPTION_FLAG_NONE, false, "stopSpeakKeyST")
            SetOptionFlagsST(OPTION_FLAG_NONE, false, "HotKey1ST")
            SetOptionFlagsST(OPTION_FLAG_NONE, false, "HotKey2ST")
            SetOptionFlagsST(OPTION_FLAG_NONE, false, "HotKey3ST")
            SetOptionFlagsST(OPTION_FLAG_NONE, false, "HotKey4ST")
            SetOptionFlagsST(OPTION_FLAG_NONE, false, "HotKey5ST")
            SetOptionFlagsST(OPTION_FLAG_NONE, false, "HotKey6ST")
            SetOptionFlagsST(OPTION_FLAG_NONE, false, "HotKey7ST")
            SetOptionFlagsST(OPTION_FLAG_NONE, false, "HotKey8ST")
            SetOptionFlagsST(OPTION_FLAG_NONE, false, "HotKey9ST")

            RegisterForKey(HotKey1Val)
            RegisterForKey(HotKey2Val)
            RegisterForKey(HotKey3Val)
            RegisterForKey(HotKey4Val)
            RegisterForKey(HotKey5Val)
            RegisterForKey(HotKey6Val)
            RegisterForKey(HotKey7Val)
            RegisterForKey(HotKey8Val)
            RegisterForKey(HotKey9Val)
        Else
            SetOptionFlagsST(OPTION_FLAG_DISABLED, false, "entrySpeakKeyST")
            SetOptionFlagsST(OPTION_FLAG_DISABLED, false, "stopSpeakKeyST")
            SetOptionFlagsST(OPTION_FLAG_DISABLED, false, "HotKey1ST")
            SetOptionFlagsST(OPTION_FLAG_DISABLED, false, "HotKey2ST")
            SetOptionFlagsST(OPTION_FLAG_DISABLED, false, "HotKey3ST")
            SetOptionFlagsST(OPTION_FLAG_DISABLED, false, "HotKey4ST")
            SetOptionFlagsST(OPTION_FLAG_DISABLED, false, "HotKey5ST")
            SetOptionFlagsST(OPTION_FLAG_DISABLED, false, "HotKey6ST")
            SetOptionFlagsST(OPTION_FLAG_DISABLED, false, "HotKey7ST")
            SetOptionFlagsST(OPTION_FLAG_DISABLED, false, "HotKey8ST")
            SetOptionFlagsST(OPTION_FLAG_DISABLED, false, "HotKey9ST")

            UnregisterForKey(HotKey1Val)
            UnregisterForKey(HotKey2Val)
            UnregisterForKey(HotKey3Val)
            UnregisterForKey(HotKey4Val)
            UnregisterForKey(HotKey5Val)
            UnregisterForKey(HotKey6Val)
            UnregisterForKey(HotKey7Val)
            UnregisterForKey(HotKey8Val)
            UnregisterForKey(HotKey9Val)
        EndIf
    endEvent

    event OnDefaultST()
        enableKeysVal= false
        SetToggleOptionValueST( enableKeysVal)
        SetOptionFlagsST(OPTION_FLAG_DISABLED, false, "entrySpeakKeyST")
        SetOptionFlagsST(OPTION_FLAG_DISABLED, false, "stopSpeakKeyST")
        SetOptionFlagsST(OPTION_FLAG_DISABLED, false, "HotKey1ST")
        SetOptionFlagsST(OPTION_FLAG_DISABLED, false, "HotKey2ST")
        SetOptionFlagsST(OPTION_FLAG_DISABLED, false, "HotKey3ST")
        SetOptionFlagsST(OPTION_FLAG_DISABLED, false, "HotKey4ST")
        SetOptionFlagsST(OPTION_FLAG_DISABLED, false, "HotKey5ST")
        SetOptionFlagsST(OPTION_FLAG_DISABLED, false, "HotKey6ST")
        SetOptionFlagsST(OPTION_FLAG_DISABLED, false, "HotKey7ST")
        SetOptionFlagsST(OPTION_FLAG_DISABLED, false, "HotKey8ST")
        SetOptionFlagsST(OPTION_FLAG_DISABLED, false, "HotKey9ST")
    endEvent

    event OnHighlightST()
        SetInfoText("Play parenthetical speech, e.g. (Persuade) or (10 Gold)")
    endEvent
endState


state pLangST; Menu
    event OnMenuOpenST()
        If(pLangVal < 0)
            pLangVal = 0
        EndIf
        SetMenuDialogStartIndex(pLangVal)
        SetMenuDialogDefaultIndex(0)
        SetMenuDialogOptions(LangList)
    endEvent

    event OnMenuAcceptST(int a_index)
        If(a_index < 0)
            a_index = 0
        EndIf
        pLangVal = a_index
        SetMenuOptionValueST(pLangVal)
    endEvent

    event OnDefaultST()
        pLangVal = 0
        SetMenuOptionValueST(pLangVal)
    endEvent

    event OnHighlightST()
        SetInfoText("Language for player character voice: " + LangList[pLangVal])
    endEvent
endState

state fLangST; Menu
    event OnMenuOpenST()
        If(fLangVal < 0)
            fLangVal = 0
        EndIf
        SetMenuDialogStartIndex(fLangVal)
        SetMenuDialogDefaultIndex(0)
        SetMenuDialogOptions(LangList)
    endEvent

    event OnMenuAcceptST(int a_index)
        If(a_index < 0)
            a_index = 0
        EndIf
        fLangVal = a_index
        SetMenuOptionValueST(fLangVal)
    endEvent

    event OnDefaultST()
        fLangVal = 0
        SetMenuOptionValueST(fLangVal)
    endEvent

    event OnHighlightST()
        SetInfoText("Language for female NPC voice: " + LangList[fLangVal])
    endEvent
endState

state mLangST; Menu
    event OnMenuOpenST()
        If(mLangVal < 0)
            mLangVal = 0
        EndIf
        SetMenuDialogStartIndex(mLangVal)
        SetMenuDialogDefaultIndex(0)
        SetMenuDialogOptions(LangList)
    endEvent

    event OnMenuAcceptST(int a_index)
        If(a_index < 0)
            a_index = 0
        EndIf
        mLangVal = a_index
        SetMenuOptionValueST(mLangVal)
    endEvent

    event OnDefaultST()
        mLangVal = 0
        SetMenuOptionValueST(mLangVal)
    endEvent

    event OnHighlightST()
        SetInfoText("Language for male NPC voice: " + LangList[mLangVal])
    endEvent
endState


state nLangST; Menu
    event OnMenuOpenST()
        If(nLangVal < 0)
            nLangVal = 0
        EndIf
        SetMenuDialogStartIndex(nLangVal)
        SetMenuDialogDefaultIndex(0)
        SetMenuDialogOptions(LangList)
    endEvent

    event OnMenuAcceptST(int a_index)
        If(a_index < 0)
            a_index = 0
        EndIf
        nLangVal = a_index
        SetMenuOptionValueST(nLangVal)
    endEvent

    event OnDefaultST()
        nLangVal = 0
        SetMenuOptionValueST(nLangVal)
    endEvent

    event OnHighlightST()
        SetInfoText("Language for book narrator voice: " + LangList[nLangVal])
    endEvent
endState


state nGenderST; TEXT
    event OnSelectST()
        nSexVal += 1
        if(nSexVal > 1)
            nSexVal = 0
        endif
        SetTextOptionValueST(SexList[nSexVal])
    endEvent

    event OnDefaultST()
    nSexVal = 1
        SetTextOptionValueST(SexList[1])
    endEvent

    event OnHighlightST()
        SetInfoText("Gender for book narrator voice") 
    endEvent
endState


state pLangTST; TEXT
    event OnSelectST()
        doSendVals()
        FuzRoBork_testSpeech("p")
    endEvent
    event OnHighlightST()
        SetInfoText("Test player character voice")
    endEvent
endState

state fLangTST; TEXT
    event OnSelectST()
        doSendVals()
        FuzRoBork_testSpeech("f")
    endEvent
    event OnHighlightST()
        SetInfoText("Test female NPC voice")
    endEvent
endState

state mLangTST; TEXT
    event OnSelectST()
        doSendVals()
        FuzRoBork_testSpeech("m")
    endEvent
    event OnHighlightST()
        SetInfoText("Test male NPC voice")
    endEvent
endState

state nLangTST; TEXT
    event OnSelectST()
        doSendVals()
        FuzRoBork_testSpeech("n")
    endEvent
    event OnHighlightST()
        SetInfoText("Test book narrator voice")
    endEvent
endState

state msST ; SLIDER
    event OnSliderOpenST()
        SetSliderDialogStartValue(msVal)
        SetSliderDialogDefaultValue(500)
        SetSliderDialogRange(100, 2000)
        SetSliderDialogInterval(1)
    endEvent

    event OnSliderAcceptST(float value)
        msVal = value
        SetSliderOptionValueST(value, "{0} ms / word")
    endEvent

    event OnDefaultST()
        msVal = 500
        SetSliderOptionValueST(msVal, "{0} ms / word")
    endEvent

    event OnHighlightST()
        SetInfoText("Number of milliseconds it takes to say a word of NPC text (doesn't affect audio or PC text)") 
    endEvent
endState

state pVolST ; SLIDER
    event OnSliderOpenST()
        SetSliderDialogStartValue(pVolVal)
        SetSliderDialogDefaultValue(30)
        SetSliderDialogRange(1,100)
        SetSliderDialogInterval(1)
    endEvent

    event OnSliderAcceptST(float value)
        pVolVal= value as int
        SetSliderOptionValueST(pVolVal, "{0}%" )
    endEvent

    event OnDefaultST()
        pVolVal= 30
        SetSliderOptionValueST(pVolVal, "{0}%" )
    endEvent

    event OnHighlightST()
        SetInfoText("Player speech volume %") 
    endEvent
endState

state fVolST ; SLIDER
    event OnSliderOpenST()
        SetSliderDialogStartValue(fVolVal)
        SetSliderDialogDefaultValue(30)
        SetSliderDialogRange(1,100)
        SetSliderDialogInterval(1)
    endEvent

    event OnSliderAcceptST(float value)
        fVolVal= value as int
        SetSliderOptionValueST(fVolVal, "{0}%" )
    endEvent

    event OnDefaultST()
        fVolVal= 30
        SetSliderOptionValueST(fVolVal, "{0}%" )
    endEvent

    event OnHighlightST()
        SetInfoText("Female NPC speech volume %") 
    endEvent
endState

state mVolST ; SLIDER
    event OnSliderOpenST()
        SetSliderDialogStartValue(mVolVal)
        SetSliderDialogDefaultValue(30)
        SetSliderDialogRange(1,100)
        SetSliderDialogInterval(1)
    endEvent

    event OnSliderAcceptST(float value)
        mVolVal= value as int
        SetSliderOptionValueST(mVolVal, "{0}%" )
    endEvent

    event OnDefaultST()
        mVolVal= 30
        SetSliderOptionValueST(mVolVal, "{0}%" )
    endEvent

    event OnHighlightST()
        SetInfoText("Male NPC speech volume %") 
    endEvent
endState

state nVolST ; SLIDER
    event OnSliderOpenST()
        SetSliderDialogStartValue(nVolVal)
        SetSliderDialogDefaultValue(30)
        SetSliderDialogRange(1,100)
        SetSliderDialogInterval(1)
    endEvent

    event OnSliderAcceptST(float value)
        nVolVal= value as int
        SetSliderOptionValueST(nVolVal, "{0}%" )
    endEvent

    event OnDefaultST()
        nVolVal= 30
        SetSliderOptionValueST(nVolVal, "{0}%" )
    endEvent

    event OnHighlightST()
        SetInfoText("Narrator speech volume %") 
    endEvent
endState


state pRateST ; SLIDER
    event OnSliderOpenST()
        SetSliderDialogStartValue(pRateVal)
        SetSliderDialogDefaultValue(0)
        SetSliderDialogRange(-10,10)
        SetSliderDialogInterval(0.1)
    endEvent

    event OnSliderAcceptST(float value)
        pRateVal = value
        SetSliderOptionValueST(pRateVal, "{1}x" )
    endEvent

    event OnDefaultST()
        pRateVal = 1
        SetSliderOptionValueST(pRateVal, "{1}x" )
    endEvent

    event OnHighlightST()
        SetInfoText("Rate of speech for PC") 
    endEvent
endState

state fRateST ; SLIDER
    event OnSliderOpenST()
        SetSliderDialogStartValue(fRateVal)
        SetSliderDialogDefaultValue(0)
        SetSliderDialogRange(-10,10)
        SetSliderDialogInterval(0.1)
    endEvent

    event OnSliderAcceptST(float value)
        fRateVal = value
        SetSliderOptionValueST(fRateVal, "{1}x" )
    endEvent

    event OnDefaultST()
        fRateVal = 50
        SetSliderOptionValueST(fRateVal, "{1}x" )
    endEvent

    event OnHighlightST()
        SetInfoText("Rate of speech for female NPC") 
    endEvent
endState

state mRateST ; SLIDER
    event OnSliderOpenST()
        SetSliderDialogStartValue(mRateVal)
        SetSliderDialogDefaultValue(0)
        SetSliderDialogRange(-10,10)
        SetSliderDialogInterval(0.1)
    endEvent

    event OnSliderAcceptST(float value)
        mRateVal = value
        SetSliderOptionValueST(mRateVal, "{1}x")
    endEvent

    event OnDefaultST()
        mRateVal = 1
        SetSliderOptionValueST(mRateVal, "{1}x")
    endEvent

    event OnHighlightST()
        SetInfoText("Rate of speech for male NPC") 
    endEvent
endState

state nRateST ; SLIDER
    event OnSliderOpenST()
        SetSliderDialogStartValue(nRateVal)
        SetSliderDialogDefaultValue(0)
        SetSliderDialogRange(-10,10)
        SetSliderDialogInterval(0.1)
    endEvent

    event OnSliderAcceptST(float value)
        nRateVal= value
        SetSliderOptionValueST(nRateVal, "{1}x")
    endEvent

    event OnDefaultST()
        nRateVal= 1
        SetSliderOptionValueST(nRateVal, "{1}x")
    endEvent

    event OnHighlightST()
        SetInfoText("Rate of speech for narrator") 
    endEvent
endState

state pPitchST ; SLIDER
    event OnSliderOpenST()
        SetSliderDialogStartValue(pPitchVal)
        SetSliderDialogDefaultValue(0)
        SetSliderDialogRange(-10,10)
        SetSliderDialogInterval(1)
    endEvent

    event OnSliderAcceptST(float value)
        pPitchVal = value
        SetSliderOptionValueST(pPitchVal)
    endEvent

    event OnDefaultST()
        pPitchVal= 1
        SetSliderOptionValueST(pPitchVal)
    endEvent

    event OnHighlightST()
        SetInfoText("Pitch of player voice") 
    endEvent
endState

state fPitchST ; SLIDER
    event OnSliderOpenST()
        SetSliderDialogStartValue(fPitchVal)
        SetSliderDialogDefaultValue(0)
        SetSliderDialogRange(-10,10)
        SetSliderDialogInterval(1)
    endEvent

    event OnSliderAcceptST(float value)
        fPitchVal = value
        SetSliderOptionValueST(fPitchVal)
    endEvent

    event OnDefaultST()
        fPitchVal= 1
        SetSliderOptionValueST(fPitchVal)
    endEvent

    event OnHighlightST()
        SetInfoText("Pitch of female NPC voice") 
    endEvent
endState

state mPitchST ; SLIDER
    event OnSliderOpenST()
        SetSliderDialogStartValue(mPitchVal)
        SetSliderDialogDefaultValue(0)
        SetSliderDialogRange(-10,10)
        SetSliderDialogInterval(1)
    endEvent

    event OnSliderAcceptST(float value)
        mPitchVal = value
        SetSliderOptionValueST(mPitchVal)
    endEvent

    event OnDefaultST()
        mPitchVal= 1
        SetSliderOptionValueST(mPitchVal)
    endEvent

    event OnHighlightST()
        SetInfoText("Pitch of male NPC voice") 
    endEvent
endState

state nPitchST ; SLIDER
    event OnSliderOpenST()
        SetSliderDialogStartValue(nPitchVal)
        SetSliderDialogDefaultValue(0)
        SetSliderDialogRange(-10,10)
        SetSliderDialogInterval(1)
    endEvent

    event OnSliderAcceptST(float value)
        nPitchVal = value
        SetSliderOptionValueST(nPitchVal)
    endEvent

    event OnDefaultST()
        nPitchVal= 1
        SetSliderOptionValueST(nPitchVal)
    endEvent

    event OnHighlightST()
        SetInfoText("Pitch of narrator voice") 
    endEvent
endState


state bookReadKeyST; KEYMAP
    event OnKeyMapChangeST(int newKeyCode, string conflictControl, string conflictName)
        bool continue = true
        if (conflictControl != "")
            string msg
            if (conflictName != "")
                msg = "This key is already mapped to:\n\"" + conflictControl + "\"\n(" + conflictName + ")\n\nAre you sure you want to continue?"
            else
                msg = "This key is already mapped to:\n\"" + conflictControl + "\"\n\nAre you sure you want to continue?"
            endIf

            continue = ShowMessage(msg, true, "$Yes", "$No")
        endIf
        if (continue)
            bookReadKeyVal= newKeyCode    
            SetKeyMapOptionValueST(bookReadKeyVal)
        endIf
    endEvent
    event OnDefaultST()
        bookReadKeyVal = 157
        SetKeyMapOptionValueST(bookReadKeyVal)
    endEvent
    event OnHighlightST()
        SetInfoText("Pressing this key with a book open will read the entire book to you.")
    endEvent
endState

state entrySpeakKeyST; KEYMAP
    event OnKeyMapChangeST(int newKeyCode, string conflictControl, string conflictName)
        bool continue = true
        if (conflictControl != "")
            string msg
            if (conflictName != "")
                msg = "This key is already mapped to:\n\"" + conflictControl + "\"\n(" + conflictName + ")\n\nAre you sure you want to continue?"
            else
                msg = "This key is already mapped to:\n\"" + conflictControl + "\"\n\nAre you sure you want to continue?"
            endIf

            continue = ShowMessage(msg, true, "$Yes", "$No")
        endIf
        if (continue)
            entrySpeakKeyVal= newKeyCode    
            SetKeyMapOptionValueST(entrySpeakKeyVal)
        endIf
    endEvent
    event OnDefaultST()
        entrySpeakKeyVal = 42
        SetKeyMapOptionValueST(entrySpeakKeyVal)
    endEvent
    event OnHighlightST()
        SetInfoText("Pressing this key in will speak the current highlighted dialogue item or current book pages.")
    endEvent
endState

state stopSpeakKeyST; KEYMAP
    event OnKeyMapChangeST(int newKeyCode, string conflictControl, string conflictName)
        bool continue = true
        if (conflictControl != "")
            string msg
            if (conflictName != "")
                msg = "This key is already mapped to:\n\"" + conflictControl + "\"\n(" + conflictName + ")\n\nAre you sure you want to continue?"
            else
                msg = "This key is already mapped to:\n\"" + conflictControl + "\"\n\nAre you sure you want to continue?"
            endIf

            continue = ShowMessage(msg, true, "$Yes", "$No")
        endIf
        if (continue)
            UnregisterForKey(stopSpeakKeyVal)
            stopSpeakKeyVal= newKeyCode 
            RegisterForKey(stopSpeakKeyVal)
            SetKeyMapOptionValueST(stopSpeakKeyVal)
        endIf
    endEvent
    event OnDefaultST()
        UnregisterForKey(stopSpeakKeyVal)
        stopSpeakKeyVal = 45
        RegisterForKey(stopSpeakKeyVal)
        SetKeyMapOptionValueST(stopSpeakKeyVal)
    endEvent
    event OnHighlightST()
        SetInfoText("Pressing this key will interrupt current speech.")
    endEvent
endState

state HotKey1ST; KEYMAP
    event OnKeyMapChangeST(int newKeyCode, string conflictControl, string conflictName)
        bool continue = true
        if (conflictControl != "")
            string msg
            if (conflictName != "")
                msg = "This key is already mapped to:\n\"" + conflictControl + "\"\n(" + conflictName + ")\n\nAre you sure you want to continue?"
            else
                msg = "This key is already mapped to:\n\"" + conflictControl + "\"\n\nAre you sure you want to continue?"
            endIf

            continue = ShowMessage(msg, true, "$Yes", "$No")
        endIf
        if (continue)
            UnregisterForKey(HotKey1Val)
            HotKey1Val= newKeyCode  
            RegisterForKey(HotKey1Val)
            SetKeyMapOptionValueST(HotKey1Val)
        endIf
    endEvent
    event OnDefaultST()
        UnregisterForKey(HotKey1Val)
        HotKey1Val = 2
        RegisterForKey(HotKey1Val)
        SetKeyMapOptionValueST(HotKey1Val)
    endEvent
    event OnHighlightST()
        SetInfoText("Play custom speech defined in Data\\skse\\Plugins\\FuzRoBork.xml")
    endEvent
endState

state HotKey2ST; KEYMAP
    event OnKeyMapChangeST(int newKeyCode, string conflictControl, string conflictName)
        bool continue = true
        if (conflictControl != "")
            string msg
            if (conflictName != "")
                msg = "This key is already mapped to:\n\"" + conflictControl + "\"\n(" + conflictName + ")\n\nAre you sure you want to continue?"
            else
                msg = "This key is already mapped to:\n\"" + conflictControl + "\"\n\nAre you sure you want to continue?"
            endIf

            continue = ShowMessage(msg, true, "$Yes", "$No")
        endIf
        if (continue)
            UnregisterForKey(HotKey2Val)
            HotKey2Val= newKeyCode  
            RegisterForKey(HotKey2Val)
            SetKeyMapOptionValueST(HotKey2Val)
        endIf
    endEvent
    event OnDefaultST()
        UnregisterForKey(HotKey2Val)
        HotKey2Val = 3
        RegisterForKey(HotKey2Val)
        SetKeyMapOptionValueST(HotKey2Val)
    endEvent
    event OnHighlightST()
        SetInfoText("Play custom speech defined in Data\\skse\\Plugins\\FuzRoBork.xml")
    endEvent
endState

state HotKey3ST; KEYMAP
    event OnKeyMapChangeST(int newKeyCode, string conflictControl, string conflictName)
        bool continue = true
        if (conflictControl != "")
            string msg
            if (conflictName != "")
                msg = "This key is already mapped to:\n\"" + conflictControl + "\"\n(" + conflictName + ")\n\nAre you sure you want to continue?"
            else
                msg = "This key is already mapped to:\n\"" + conflictControl + "\"\n\nAre you sure you want to continue?"
            endIf

            continue = ShowMessage(msg, true, "$Yes", "$No")
        endIf
        if (continue)
            UnregisterForKey(HotKey3Val)
            HotKey3Val= newKeyCode  
            RegisterForKey(HotKey3Val)
            SetKeyMapOptionValueST(HotKey3Val)
        endIf
    endEvent
    event OnDefaultST()
        UnregisterForKey(HotKey3Val)
        HotKey3Val = 4
        RegisterForKey(HotKey3Val)
        SetKeyMapOptionValueST(HotKey3Val)
    endEvent
    event OnHighlightST()
        SetInfoText("Play custom speech defined in Data\\skse\\Plugins\\FuzRoBork.xml")
    endEvent
endState

state HotKey4ST; KEYMAP
    event OnKeyMapChangeST(int newKeyCode, string conflictControl, string conflictName)
        bool continue = true
        if (conflictControl != "")
            string msg
            if (conflictName != "")
                msg = "This key is already mapped to:\n\"" + conflictControl + "\"\n(" + conflictName + ")\n\nAre you sure you want to continue?"
            else
                msg = "This key is already mapped to:\n\"" + conflictControl + "\"\n\nAre you sure you want to continue?"
            endIf

            continue = ShowMessage(msg, true, "$Yes", "$No")
        endIf
        if (continue)
            UnregisterForKey(HotKey4Val)
            HotKey4Val= newKeyCode  
            RegisterForKey(HotKey4Val)
            SetKeyMapOptionValueST(HotKey4Val)
        endIf
    endEvent
    event OnDefaultST()
        UnregisterForKey(HotKey4Val)
        HotKey4Val = 5
        RegisterForKey(HotKey4Val)
        SetKeyMapOptionValueST(HotKey4Val)
    endEvent
    event OnHighlightST()
        SetInfoText("Play custom speech defined in Data\\skse\\Plugins\\FuzRoBork.xml")
    endEvent
endState

state HotKey5ST; KEYMAP
    event OnKeyMapChangeST(int newKeyCode, string conflictControl, string conflictName)
        bool continue = true
        if (conflictControl != "")
            string msg
            if (conflictName != "")
                msg = "This key is already mapped to:\n\"" + conflictControl + "\"\n(" + conflictName + ")\n\nAre you sure you want to continue?"
            else
                msg = "This key is already mapped to:\n\"" + conflictControl + "\"\n\nAre you sure you want to continue?"
            endIf

            continue = ShowMessage(msg, true, "$Yes", "$No")
        endIf
        if (continue)
            UnregisterForKey(HotKey5Val)
            HotKey5Val= newKeyCode  
            RegisterForKey(HotKey5Val)
            SetKeyMapOptionValueST(HotKey5Val)
        endIf
    endEvent
    event OnDefaultST()
        UnregisterForKey(HotKey5Val)
        HotKey5Val = 6
        RegisterForKey(HotKey5Val)
        SetKeyMapOptionValueST(HotKey5Val)
    endEvent
    event OnHighlightST()
        SetInfoText("Play custom speech defined in Data\\skse\\Plugins\\FuzRoBork.xml")
    endEvent
endState

state HotKey6ST; KEYMAP
    event OnKeyMapChangeST(int newKeyCode, string conflictControl, string conflictName)
        bool continue = true
        if (conflictControl != "")
            string msg
            if (conflictName != "")
                msg = "This key is already mapped to:\n\"" + conflictControl + "\"\n(" + conflictName + ")\n\nAre you sure you want to continue?"
            else
                msg = "This key is already mapped to:\n\"" + conflictControl + "\"\n\nAre you sure you want to continue?"
            endIf

            continue = ShowMessage(msg, true, "$Yes", "$No")
        endIf
        if (continue)
            UnregisterForKey(HotKey6Val)
            HotKey6Val= newKeyCode  
            RegisterForKey(HotKey6Val)
            SetKeyMapOptionValueST(HotKey6Val)
        endIf
    endEvent
    event OnDefaultST()
        UnregisterForKey(HotKey6Val)
        HotKey6Val = 7
        RegisterForKey(HotKey6Val)
        SetKeyMapOptionValueST(HotKey6Val)
    endEvent
    event OnHighlightST()
        SetInfoText("Play custom speech defined in Data\\skse\\Plugins\\FuzRoBork.xml")
    endEvent
endState

state HotKey7ST; KEYMAP
    event OnKeyMapChangeST(int newKeyCode, string conflictControl, string conflictName)
        bool continue = true
        if (conflictControl != "")
            string msg
            if (conflictName != "")
                msg = "This key is already mapped to:\n\"" + conflictControl + "\"\n(" + conflictName + ")\n\nAre you sure you want to continue?"
            else
                msg = "This key is already mapped to:\n\"" + conflictControl + "\"\n\nAre you sure you want to continue?"
            endIf

            continue = ShowMessage(msg, true, "$Yes", "$No")
        endIf
        if (continue)
            UnregisterForKey(HotKey7Val)
            HotKey7Val= newKeyCode  
            RegisterForKey(HotKey7Val)
            SetKeyMapOptionValueST(HotKey7Val)
        endIf
    endEvent
    event OnDefaultST()
        UnregisterForKey(HotKey7Val)
        HotKey7Val = 8
        RegisterForKey(HotKey7Val)
        SetKeyMapOptionValueST(HotKey7Val)
    endEvent
    event OnHighlightST()
        SetInfoText("Play custom speech defined in Data\\skse\\Plugins\\FuzRoBork.xml")
    endEvent
endState

state HotKey8ST; KEYMAP
    event OnKeyMapChangeST(int newKeyCode, string conflictControl, string conflictName)
        bool continue = true
        if (conflictControl != "")
            string msg
            if (conflictName != "")
                msg = "This key is already mapped to:\n\"" + conflictControl + "\"\n(" + conflictName + ")\n\nAre you sure you want to continue?"
            else
                msg = "This key is already mapped to:\n\"" + conflictControl + "\"\n\nAre you sure you want to continue?"
            endIf

            continue = ShowMessage(msg, true, "$Yes", "$No")
        endIf
        if (continue)
            UnregisterForKey(HotKey8Val)
            HotKey8Val= newKeyCode  
            RegisterForKey(HotKey8Val)
            SetKeyMapOptionValueST(HotKey8Val)
        endIf
    endEvent
    event OnDefaultST()
        UnregisterForKey(HotKey8Val)
        HotKey8Val = 9
        RegisterForKey(HotKey8Val)
        SetKeyMapOptionValueST(HotKey8Val)
    endEvent
    event OnHighlightST()
        SetInfoText("Play custom speech defined in Data\\skse\\Plugins\\FuzRoBork.xml")
    endEvent
endState


state HotKey9ST; KEYMAP
    event OnKeyMapChangeST(int newKeyCode, string conflictControl, string conflictName)
        bool continue = true
        if (conflictControl != "")
            string msg
            if (conflictName != "")
                msg = "This key is already mapped to:\n\"" + conflictControl + "\"\n(" + conflictName + ")\n\nAre you sure you want to continue?"
            else
                msg = "This key is already mapped to:\n\"" + conflictControl + "\"\n\nAre you sure you want to continue?"
            endIf

            continue = ShowMessage(msg, true, "$Yes", "$No")
        endIf
        if (continue)
            UnregisterForKey(HotKey9Val)
            HotKey9Val= newKeyCode  
            RegisterForKey(HotKey9Val)
            SetKeyMapOptionValueST(HotKey9Val)
        endIf
    endEvent
    event OnDefaultST()
        UnregisterForKey(HotKey9Val)
        HotKey9Val = 9
        RegisterForKey(HotKey9Val)
        SetKeyMapOptionValueST(HotKey9Val)
    endEvent
    event OnHighlightST()
        SetInfoText("Play custom speech defined in Data\\skse\\Plugins\\FuzRoBork.xml")
    endEvent
endState

Function doSendVals()

    string strings = ""

	If(pLangVal < 0)
		pLangVal = 0
    EndIf
	If(fLangVal < 0)
		fLangVal = 0
    EndIf
	If(mLangVal < 0)
		mLangVal = 0
    EndIf
	If(nLangVal < 0)
		nLangVal = 0
    EndIf

    strings += LangList[pLangVal]
    strings += "^"+LangList[fLangVal]
    strings += "^"+LangList[mLangVal]
    strings += "^"+LangList[nLangVal]
    
    string booleans = ""

    booleans += skipVal
    booleans += "^"+playPlayerVal
    booleans += "^"+playNPCVal
    booleans += "^"+playPagesVal
    booleans += "^"+playBookVal
    booleans += "^"+playLoadVal
    booleans += "^"+playActionsVal
    booleans += "^"+playParaVal
    booleans += "^"+enableKeysVal

    string floats = ""

    floats += msVal
    floats += "^"+pVolVal
    floats += "^"+fVolVal
    floats += "^"+mVolVal
    floats += "^"+nVolVal
    
    floats += "^"+pRateVal
    floats += "^"+fRateVal
    floats += "^"+mRateVal
    floats += "^"+nRateVal
    
    floats += "^" + pPitchVal
    floats += "^" + fPitchVal
    floats += "^" + mPitchVal
    floats += "^" + nPitchVal
    
    FuzRoBork_setMCMConfig(strings, booleans, floats)
EndFunction

Event OnKeyDown(Int KeyCode)
    if(KeyCode == entrySpeakKeyVal)
        if(UI.IsMenuOpen("Dialogue Menu"))
            UI.Invoke("Dialogue Menu", "_root.DialogueMenu_mc.TTS_doSendTextForSpeech")
        elseif(UI.IsMenuOpen("Book Menu"))
            FuzRoBork_storedPagesSpeech()
        endIf
    elseIf(KeyCode == bookReadKeyVal)
        if(UI.IsMenuOpen("Book Menu"))
			FuzRoBork_storedBookSpeech()
        endIf
    elseIf(KeyCode == stopSpeakKeyVal)
        FuzRoBork_stopSpeech()
    elseIf(KeyCode == HotKey1Val)
        FuzRoBork_hotSpeech(1)
    elseIf(KeyCode == HotKey2Val)
        FuzRoBork_hotSpeech(2)
    elseIf(KeyCode == HotKey3Val)
        FuzRoBork_hotSpeech(3)
    elseIf(KeyCode == HotKey4Val)
        FuzRoBork_hotSpeech(4)
    elseIf(KeyCode == HotKey5Val)
        FuzRoBork_hotSpeech(5)
    elseIf(KeyCode == HotKey6Val)
        FuzRoBork_hotSpeech(6)
    elseIf(KeyCode == HotKey7Val)
        FuzRoBork_hotSpeech(7)
    elseIf(KeyCode == HotKey8Val)
        FuzRoBork_hotSpeech(8)
    elseIf(KeyCode == HotKey9Val)
        FuzRoBork_hotSpeech(9)
    endIf
endEvent

