Scriptname BorkPC extends Actor

Function FuzRoBork_actionSpeech(string which) global native

Event OnDeath(Actor akKiller)
	FuzRoBork_actionSpeech("dead")
EndEvent

Event OnSit(ObjectReference akFurniture)
	FuzRoBork_actionSpeech("sit")
endEvent

Event OnGetUp(ObjectReference akFurniture)
	FuzRoBork_actionSpeech("stand")
endEvent


