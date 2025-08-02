TARCMAP_MapScripts::
	.byte 0


TARCINTRO::
	lockall
	msgbox TARCINTRO_Text_0
	msgbox TARCINTRO_Text_1
	msgbox TARCINTRO_Text_2
	msgbox TARCINTRO_Text_3
	releaseall
	return


TARCINTRO_Text_0:
	.string "In the weave of time, beyond sight and\n"
	.string "reason, the divination of the Tarot\n"
	.string "sings of blessing and of curse. Ancient\l"
	.string "cards, etched with arcane symbols,\l"
	.string "whisper truths no mortal dares to seek.\l"
	.string "Each draw, a glimpse into destiny. Each\l"
	.string "spread, a thread tugged from the\l"
	.string "tapestry of fate.$"

TARCINTRO_Text_1:
	.string "Some say the cards choose their bearer.\n"
	.string "Others say they warn. But all agree:\n"
	.string "when the Major Arcana stirs, the world\l"
	.string "shifts. Emperors have fallen to the\l"
	.string "Tower. Lovers have burned in the Sun.\l"
	.string "And from the ashes, a Fool has risen,\l"
	.string "again and again, to walk the path\l"
	.string "between chaos and salvation.$"

TARCINTRO_Text_2:
	.string "This time, the Fool bears your face.\n"
	.string "Your journey begins not as a warrior,\n"
	.string "nor a king, but as a question. A\l"
	.string "question the cards are eager to\l"
	.string "answer. Will you become the Magician,\l"
	.string "mastering fate itself? Or the Hanged\l"
	.string "Man, lost to sacrifice and sorrow?$"

TARCINTRO_Text_3:
	.string "History will not remember your name.\n"
	.string "Myth will. Draw your card. Begin your\n"
	.string "legend.$"

BirchCasePick2::
   .... the beginning of your script
   callnative StartNewPokeballCaseUI
   waitstate
   .... the rest of your script

TARCEventScript_Roxannematch::
	trainerbattle_single TRAINER_ROXANNE_1, TARCText_RoxannePrematch, TARCText_RoxanneMatchDefeat, RustboroCity_Gym_Text_RoxanneRematchNeedTwoMons
	msgbox TARCText_RoxannePostmatch, MSGBOX_AUTOCLOSE
	end

TARCEventScript_Brawlymatch::
	trainerbattle_single TRAINER_BRAWLY_1, TARCText_BrawlyPrematch, TARCText_BrawlyMatchDefeat, RustboroCity_Gym_Text_RoxanneRematchNeedTwoMons
	msgbox TARCText_BrawlyPostmatch, MSGBOX_AUTOCLOSE
	end

TARCEventScript_Wattsonmatch::
	trainerbattle_single TRAINER_WATTSON_1, TARCText_WattsonPrematch, TARCText_WattsonMatchDefeat, RustboroCity_Gym_Text_RoxanneRematchNeedTwoMons
	msgbox TARCText_WattsonPostmatch, MSGBOX_AUTOCLOSE
	end

TARCEventScript_Flannerymatch::
	trainerbattle_single TRAINER_FLANNERY_1, TARCText_FlanneryPrematch, TARCText_FlanneryMatchDefeat, RustboroCity_Gym_Text_RoxanneRematchNeedTwoMons
	msgbox TARCText_FlanneryPostmatch, MSGBOX_AUTOCLOSE
	end

TARCEventScript_Normanmatch::
	trainerbattle_single TRAINER_NORMAN_1, TARCText_NormanPrematch, TARCText_NormanMatchDefeat, RustboroCity_Gym_Text_RoxanneRematchNeedTwoMons
	msgbox TARCText_NormanPostmatch, MSGBOX_AUTOCLOSE
	end

TARCEventScript_Tatematch::
	trainerbattle_single TRAINER_TATE_1, TARCText_TatePrematch, TARCText_TateMatchDefeat, RustboroCity_Gym_Text_RoxanneRematchNeedTwoMons
	msgbox TARCText_TatePostmatch, MSGBOX_AUTOCLOSE
	end


TARCEventScript_Winonamatch::
	trainerbattle_single TRAINER_WINONA_1, TARCText_WinonaPrematch, TARCText_WinonaMatchDefeat, RustboroCity_Gym_Text_RoxanneRematchNeedTwoMons
	msgbox TARCText_WinonaPostmatch, MSGBOX_AUTOCLOSE
	end

TARCEventScript_Juanmatch::
	trainerbattle_single TRAINER_WINONA_1, TARCText_JuanPrematch, TARCText_JuanMatchDefeat, RustboroCity_Gym_Text_RoxanneRematchNeedTwoMons
	msgbox TARCText_JuanPostmatch, MSGBOX_AUTOCLOSE
	end

TARC_HEALER::
	lock
	faceplayer
	msgbox Route111_OldLadysRestStop_Text_RestUpHere, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, Route111_OldLadysRestStop_EventScript_Rest
	goto_if_eq VAR_RESULT, NO, Route111_OldLadysRestStop_EventScript_DeclineRest
	end

Route111_OldLadysRestStop_EventScript_Rest::
	msgbox Route111_OldLadysRestStop_Text_TakeYourTimeRestUp, MSGBOX_DEFAULT
	closemessage
	call Common_EventScript_OutOfCenterPartyHeal
	msgbox Route111_OldLadysRestStop_Text_StillTiredTakeAnotherRest, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, Route111_OldLadysRestStop_EventScript_Rest
	goto_if_eq VAR_RESULT, NO, Route111_OldLadysRestStop_EventScript_DeclineRest
	end

Route111_OldLadysRestStop_EventScript_DeclineRest::
	msgbox Route111_OldLadysRestStop_Text_DontNeedToBeShy, MSGBOX_DEFAULT
	release
	end

Route111_OldLadysRestStop_Text_RestUpHere:
	.string "Oh, dear, dear.\n"
	.string "Aren't your POKÈMON exhausted?\p"
	.string "If you'd like, rest up here.\n"
	.string "That's a fine idea! You should do that.$"

Route111_OldLadysRestStop_Text_TakeYourTimeRestUp:
	.string "That's right.\n"
	.string "Take your time and rest up!$"

Route111_OldLadysRestStop_Text_StillTiredTakeAnotherRest:
	.string "Oh, dear, dear.\n"
	.string "Are your POKÈMON still tired?\p"
	.string "You should take another rest here.\n"
	.string "That's a fine idea. You should do that.$"

Route111_OldLadysRestStop_Text_DontNeedToBeShy:
	.string "Is that so?\n"
	.string "You don't need to be shy about it.$"


Aio_Intro::
	lockall
	msgbox Aio_Intro_Text_0
	releaseall
	return


Aio_Intro_Text_0:
	.string "Hiya, and welcome to my Romhack! I'm Aio,\n"
	.string "the creator of this game. Are you ready\n"
	.string "to go down in history? This room is your\l"
	.string "chance to play in a mini game for fame\l"
	.string "and glory! The premise is simple. During\l"
	.string "the course of your trials, a mini game\l"
	.string "will appear after battle. Your goal is to\l"
	.string "earn as much money as possible using a\l"
	.string "randomised set of cards presented to\l"
	.string "you. After 12 battles, the romhack will\l"
	.string "be over, and you will be presented with\l"
	.string "your total cash accumulated for\l"
	.string "bragging rights among the community!\l"
	.string "This mini game will be in my full length\l"
	.string "Persona inspired Romhack, Pokemon\l"
	.string "Rosebud! Without further ado, have fun\l"
	.string "and good luck!$"
