TARCMAP_MapScripts::
	map_script MAP_SCRIPT_ON_LOAD, TARCSTART
	.byte 0

@HasMoveTutor
TARCSTART::
	setflag FLAG_BADGE01_GET
	setflag FLAG_BADGE02_GET
	setflag FLAG_BADGE03_GET
	setflag FLAG_BADGE04_GET
	setflag FLAG_BADGE05_GET
	setflag FLAG_BADGE06_GET
	setflag FLAG_BADGE07_GET
	setflag FLAG_BADGE08_GET
	setflag FLAG_UNUSED_0x020
	return



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
	.string "sings of blessing and of curse. Ancient\p"
	.string "cards, etched with arcane symbols,\n"
	.string "whisper truths no mortal dares to seek.\p"
	.string "Each draw, a glimpse into destiny. Each\n"
	.string "spread, a thread tugged from the\n"
	.string "tapestry of fate.$"

TARCINTRO_Text_1:
	.string "Some say the cards choose their bearer.\n"
	.string "Others say they warn. But all agree:\n"
	.string "when the Major Arcana stirs, the world\n"
	.string "shifts. Emperors have fallen to the\p"
	.string "Tower. Lovers have burned in the Sun.\n"
	.string "And from the ashes, a Fool has risen,\n"
	.string "again and again, to walk the path\n"
	.string "between chaos and salvation.$"

TARCINTRO_Text_2:
	.string "This time, the Fool bears your face.\n"
	.string "Your journey begins not as a warrior,\n"
	.string "nor a king, but as a question. A\p"
	.string "question the cards are eager to\n"
	.string "answer. Will you become the Magician,\n"
	.string "mastering fate itself? Or the Hanged\p"
	.string "Man, lost to sacrifice and sorrow?$"

TARCINTRO_Text_3:
	.string "History will not remember your name.\n"
	.string "Myth will. Draw your card. Begin your\n"
	.string "legend.$"

BirchCasePick2::
   goto_if_eq VAR_TARCBALL_STATE, 1, BirchCaseNoMore
   callnative StartNewPokeballCaseUI
   waitstate
   callnative StartNewPokeballCaseUI
   waitstate
   setvar VAR_TARCBALL_STATE, 1
   releaseall
   end
BirchCasePick1_1::
   goto_if_eq VAR_TARCBALL2_STATE, 1, BirchCaseNoMore
   callnative StartNewPokeballCaseUI
   waitstate
   setvar VAR_TARCBALL2_STATE, 1
   releaseall
   end
BirchCasePick1_2::
   goto_if_eq VAR_TARCBALL3_STATE, 1, BirchCaseNoMore
   callnative StartNewPokeballCaseUI
   waitstate
   setvar VAR_TARCBALL3_STATE, 1
   releaseall
   end
BirchCasePick1_3::
   goto_if_eq VAR_TARCBALL4_STATE, 1, BirchCaseNoMore
   callnative StartNewPokeballCaseUI
   waitstate
   setvar VAR_TARCBALL4_STATE, 1
   releaseall
   end
BirchCasePick1_4::
   goto_if_eq VAR_TARCBALL5_STATE, 1, BirchCaseNoMore
   callnative StartNewPokeballCaseUI
   waitstate
   setvar VAR_TARCBALL5_STATE, 1
   releaseall
   end
BirchCaseNoMore:
	msgbox TARCText_NoMore, MSGBOX_AUTOCLOSE
    releaseall
	end

TARCText_NoEntry1::
	msgbox TARCText_NoEntry, MSGBOX_AUTOCLOSE
    releaseall
	end

TARCText_NoEntry:
	.string "There are slimy vines!/l"
	.string "You don't want to enter!'!$"

TARCText_NoMore:
	.string "No more picks right now!$"

TARCEventScript_Roxannematch::
	trainerbattle_single TRAINER_ROXANNE_1, TARCText_RoxannePrematch, TARCText_RoxanneMatchDefeat, RustboroCity_Gym_Text_RoxanneRematchNeedTwoMons
	msgbox TARCText_RoxannePostmatch, MSGBOX_AUTOCLOSE
	end
TARCText_RoxannePrematch:
	.string "Test.$"
TARCText_RoxanneMatchDefeat:
	.string "Test.$"

TARCText_RoxannePostmatch:
	.string "Test.$"

TARCEventScript_Brawlymatch::
	trainerbattle_single TRAINER_BRAWLY_1, TARCText_BrawlyPrematch, TARCText_BrawlyMatchDefeat, RustboroCity_Gym_Text_RoxanneRematchNeedTwoMons
	msgbox TARCText_BrawlyPostmatch, MSGBOX_AUTOCLOSE
	end
TARCText_BrawlyPrematch:
	.string "Test.$"
TARCText_BrawlyMatchDefeat:
	.string "Test.$"
TARCText_BrawlyPostmatch:
	.string "Test.$"

TARCEventScript_Wattsonmatch::
	trainerbattle_single TRAINER_WATTSON_1, TARCText_WattsonPrematch, TARCText_WattsonMatchDefeat, RustboroCity_Gym_Text_RoxanneRematchNeedTwoMons
	msgbox TARCText_WattsonPostmatch, MSGBOX_AUTOCLOSE
	end
TARCText_WattsonPrematch:
	.string "Test.$"
TARCText_WattsonMatchDefeat:
	.string "Test.$"
TARCText_WattsonPostmatch:
	.string "Test.$"


TARCEventScript_Flannerymatch::
	trainerbattle_single TRAINER_FLANNERY_1, TARCText_FlanneryPrematch, TARCText_FlanneryMatchDefeat, RustboroCity_Gym_Text_RoxanneRematchNeedTwoMons
	msgbox TARCText_FlanneryPostmatch, MSGBOX_AUTOCLOSE
	end
TARCText_FlanneryPrematch:
	.string "Test.$"
TARCText_FlanneryMatchDefeat:
	.string "Test.$"
TARCText_FlanneryPostmatch:
	.string "Test.$"

TARCEventScript_Normanmatch::
	trainerbattle_single TRAINER_NORMAN_1, TARCText_NormanPrematch, TARCText_NormanMatchDefeat, RustboroCity_Gym_Text_RoxanneRematchNeedTwoMons
	msgbox TARCText_NormanPostmatch, MSGBOX_AUTOCLOSE
	end
TARCText_NormanPrematch:
	.string "Test.$"
TARCText_NormanMatchDefeat:
	.string "Test.$"
TARCText_NormanPostmatch:
	.string "Test.$"

TARCEventScript_Tatematch::
	trainerbattle_double TRAINER_TATE_AND_LIZA_1, TARCText_TatePrematch, TARCText_TateMatchDefeat, RustboroCity_Gym_Text_RoxanneRematchNeedTwoMons
	msgbox TARCText_TatePostmatch, MSGBOX_AUTOCLOSE
	end
TARCText_TatePrematch:
	.string "Test.$"
TARCText_TateMatchDefeat:
	.string "Test.$"
TARCText_TatePostmatch:
	.string "Test.$"


TARCEventScript_Winonamatch::
	trainerbattle_single TRAINER_WINONA_1, TARCText_WinonaPrematch, TARCText_WinonaMatchDefeat, RustboroCity_Gym_Text_RoxanneRematchNeedTwoMons
	msgbox TARCText_WinonaPostmatch, MSGBOX_AUTOCLOSE
	end
TARCText_WinonaPrematch:
	.string "Test.$"
TARCText_WinonaMatchDefeat:
	.string "Test.$"
TARCText_WinonaPostmatch:
	.string "Test.$"

TARCEventScript_Sidneymatch::
	trainerbattle_single TRAINER_JUAN_1, TARCText_SidneyPrematch, TARCText_SidneyMatchDefeat, RustboroCity_Gym_Text_RoxanneRematchNeedTwoMons
	msgbox TARCText_SidneyPostmatch, MSGBOX_AUTOCLOSE
	end
TARCText_SidneyPrematch:
	.string "Test.$"
TARCText_SidneyMatchDefeat:
	.string "Test.$"
TARCText_SidneyPostmatch:
	.string "Test.$"

TARCEventScript_Drakematch::
	trainerbattle_single TRAINER_JUAN_1, TARCText_DrakePrematch, TARCText_DrakeMatchDefeat, RustboroCity_Gym_Text_RoxanneRematchNeedTwoMons
	msgbox TARCText_DrakePostmatch, MSGBOX_AUTOCLOSE
	end
TARCText_DrakePrematch:
	.string "Test.$"
TARCText_DrakeMatchDefeat:
	.string "Test.$"
TARCText_DrakePostmatch:
	.string "Test.$"

TARCEventScript_Phoebematch::
	trainerbattle_single TRAINER_JUAN_1, TARCText_PhoebePrematch, TARCText_PhoebeMatchDefeat, RustboroCity_Gym_Text_RoxanneRematchNeedTwoMons
	msgbox TARCText_PhoebePostmatch, MSGBOX_AUTOCLOSE
	end
TARCText_PhoebePrematch:
	.string "Test.$"

TARCText_PhoebeMatchDefeat:
	.string "Test.$"

TARCText_PhoebePostmatch:
	.string "Test.$"

TARCEventScript_Stevenmatch::
	trainerbattle_single TRAINER_JUAN_1, TARCText_StevenPrematch, TARCText_StevenMatchDefeat, RustboroCity_Gym_Text_RoxanneRematchNeedTwoMons
	msgbox TARCText_StevenPostmatch, MSGBOX_AUTOCLOSE
	end
TARCText_StevenPrematch:
	.string "Test.$"

TARCText_StevenMatchDefeat:
	.string "Test.$"

TARCText_StevenPostmatch:
	.string "Test.$"

TARCEventScript_Juanmatch::
	trainerbattle_single TRAINER_JUAN_1, TARCText_JuanPrematch, TARCText_JuanMatchDefeat, RustboroCity_Gym_Text_RoxanneRematchNeedTwoMons
	msgbox TARCText_JuanPostmatch, MSGBOX_AUTOCLOSE
	end
TARCText_JuanPrematch:
	.string "Test.$"

TARCText_JuanMatchDefeat:
	.string "Test.$"

TARCText_JuanPostmatch:
	.string "Test.$"


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
	.string "Aren't your POKEMON exhausted?\p"
	.string "If you'd like, rest up here.\n"
	.string "That's a fine idea! You should do that.$"

Route111_OldLadysRestStop_Text_TakeYourTimeRestUp:
	.string "That's right.\n"
	.string "Take your time and rest up!$"

Route111_OldLadysRestStop_Text_StillTiredTakeAnotherRest:
	.string "Oh, dear, dear.\n"
	.string "Are your POKEMON still tired?\p"
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


BP_Mart_MoveTutor::
	lock
	faceplayer
	message ST_Mart_Greetings
	waitmessage
	bpmart BP_Mart_Moves, TRUE
	compare VAR_RESULT, FALSE
	goto_if_eq BP_Mart_MoveTutor_2
	msgbox IllTakeYourSTNow

BP_Mart_MoveTutor_1:
	release
	return

BP_Mart_MoveTutor_2:
	msgbox PleaseSaveSomeST
	goto BP_Mart_MoveTutor_1


	.align 2

ST_Mart_Greetings:
	.string "You can spend Sword Tokens here!$"

IllTakeYourSTNow:
	.string "I'll take any sword tokens. Thanks!$"

PleaseSaveSomeST:
	.string "Please save some sword tokens. Thanks!$"

BP_Mart_Moves:
	.2byte MOVE_MEGA_PUNCH
	.2byte MOVE_FIRE_PUNCH
	.2byte MOVE_ICE_PUNCH
	.2byte MOVE_THUNDER_PUNCH
	.2byte MOVE_SWORDS_DANCE
	.2byte MOVE_BIND
	.2byte MOVE_MEGA_KICK
	.2byte MOVE_BODY_SLAM
	.2byte MOVE_LOW_KICK
	.2byte MOVE_COUNTER
	.2byte MOVE_SEISMIC_TOSS
	.2byte MOVE_THUNDER_WAVE
	.2byte MOVE_DEFENSE_CURL
	.2byte MOVE_SWIFT
	.2byte MOVE_SOFT_BOILED
	.2byte MOVE_DREAM_EATER
	.2byte MOVE_SKY_ATTACK
	.2byte MOVE_ROCK_SLIDE
	.2byte MOVE_SUPER_FANG
	.2byte MOVE_SNORE
	.2byte MOVE_SPITE
	.2byte MOVE_MUD_SLAP
	.2byte MOVE_ICY_WIND
	.2byte MOVE_OUTRAGE
	.2byte MOVE_GIGA_DRAIN
	.2byte MOVE_ENDURE
	.2byte MOVE_ROLLOUT
	.2byte MOVE_FURY_CUTTER
	.2byte MOVE_HEAL_BELL
	.2byte MOVE_PAIN_SPLIT
	.2byte MOVE_IRON_TAIL
	.2byte MOVE_SYNTHESIS
	.2byte MOVE_TWISTER
	.2byte MOVE_PSYCH_UP
	.2byte MOVE_ANCIENT_POWER
	.2byte MOVE_UPROAR
	.2byte MOVE_HEAT_WAVE
	.2byte MOVE_FOCUS_PUNCH
	.2byte MOVE_HELPING_HAND
	.2byte MOVE_TRICK
	.2byte MOVE_ROLE_PLAY
	.2byte MOVE_SUPERPOWER
	.2byte MOVE_MAGIC_COAT
	.2byte MOVE_RECYCLE
	.2byte MOVE_KNOCK_OFF
	.2byte MOVE_ENDEAVOR
	.2byte MOVE_SKILL_SWAP
	.2byte MOVE_SNATCH
	.2byte MOVE_DIVE
	.2byte MOVE_HYPER_VOICE
	.2byte MOVE_AIR_CUTTER
	.2byte MOVE_SIGNAL_BEAM
	.2byte MOVE_IRON_DEFENSE
	.2byte MOVE_BLOCK
	.2byte MOVE_BOUNCE
	.2byte MOVE_COVET
	.2byte MOVE_SHOCK_WAVE
	.2byte MOVE_WATER_PULSE
	.2byte MOVE_GRAVITY
	.2byte MOVE_TAILWIND
	.2byte MOVE_GASTRO_ACID
	.2byte MOVE_LAST_RESORT
	.2byte MOVE_WORRY_SEED
	.2byte MOVE_SUCKER_PUNCH
	.2byte MOVE_MAGNET_RISE
	.2byte MOVE_AQUA_TAIL
	.2byte MOVE_SEED_BOMB
	.2byte MOVE_DRAGON_PULSE
	.2byte MOVE_DRAIN_PUNCH
	.2byte MOVE_VACUUM_WAVE
	.2byte MOVE_EARTH_POWER
	.2byte MOVE_ZEN_HEADBUTT
	.2byte MOVE_DEFOG
	.2byte MOVE_GUNK_SHOT
	.2byte MOVE_IRON_HEAD
	.2byte MOVE_STEALTH_ROCK
	.2byte MOVE_BUG_BITE
	.2byte MOVE_OMINOUS_WIND
	.2byte MOVE_WONDER_ROOM
	.2byte MOVE_TELEKINESIS
	.2byte MOVE_MAGIC_ROOM
	.2byte MOVE_FOUL_PLAY
	.2byte MOVE_AFTER_YOU
	.2byte MOVE_ALLY_SWITCH
	.2byte MOVE_ELECTROWEB
	.2byte MOVE_DRILL_RUN
	.2byte MOVE_DUAL_CHOP
	.2byte MOVE_LASER_FOCUS
	.2byte MOVE_THROAT_CHOP
	.2byte MOVE_STOMPING_TANTRUM
	.2byte MOVE_LIQUIDATION
	.2byte MOVE_NONE
	.2byte ITEM_NONE

