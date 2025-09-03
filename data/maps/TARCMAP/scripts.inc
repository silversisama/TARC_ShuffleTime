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
	setflag FLAG_RECEIVED_RUNNING_SHOES
	setflag FLAG_SYS_B_DASH	
	setrespawn HEAL_LOCATION_TARCMAP	
	return

TARCMAPFINISHTWOSTEP::
	lock
	faceplayer
	goto_if_eq VAR_TARCSHOP_STATE, 1, TARCFINISH
	applymovement LOCALID_SHOPGUY, TARC_TwoStep
	goto TARCFINISH

TARCMAPFINISHONESTEP::
	lock
	faceplayer
	goto_if_eq VAR_TARCSHOP_STATE, 1, TARCFINISH
	applymovement LOCALID_SHOPGUY, TARC_OneStep
	goto TARCFINISH
	return

TARCFINISH::
    setvar VAR_TARCSHOP_STATE, 1
	msgbox TARCFINISHWAIT, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, TARCMAPSHOP 
	msgbox Okay_Then
	releaseall
	return
	end

TARCMAPSHOP::
	pokemart TARCSHOP
	releaseall
	return
	end

TARCSHOP:
	.2byte ITEM_EXP_CANDY_XL
	pokemartlistend

TARCFINISHWAIT:
	.string "Before you go, do you want to \n"
	.string "Sell your extra items?$"

Okay_Then:
	.string "Okay then, hope you get a high\n"
	.string "score!$"

TARCINTRO::
	lockall
	msgbox TARCINTRO_Text_0
	msgbox TARCINTRO_Text_1
	msgbox TARCINTRO_Text_2
	msgbox TARCINTRO_Text_3
	releaseall
	return
WhereGlace::
	lockall
	msgbox WhereGlaceText
	releaseall
	return

WhereGlaceText:
	.string "Glacie is on vacation. No Ice here!$"

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
	.string "There are slimy vines!\n"
	.string "You don't want to enter!$"

TARCText_NoMore:
	.string "No more picks right now!$"

TARCEventScript_Roxannematch::
	trainerbattle_single TRAINER_ROXANNE_1, TARCText_RoxannePrematch, TARCText_RoxanneMatchDefeat, RustboroCity_Gym_Text_RoxanneRematchNeedTwoMons
	msgbox TARCText_RoxannePostmatch, MSGBOX_AUTOCLOSE
	end
TARCText_RoxannePrematch:
	.string "I'm about to rock your world!$"
TARCText_RoxanneMatchDefeat:
	.string "I bit the dust - and the gravel.$"

TARCText_RoxannePostmatch:
	.string "Guess I take things for granite.$"

TARCEventScript_Brawlymatch::
	trainerbattle_single TRAINER_BRAWLY_1, TARCText_BrawlyPrematch, TARCText_BrawlyMatchDefeat, RustboroCity_Gym_Text_RoxanneRematchNeedTwoMons
	msgbox TARCText_BrawlyPostmatch, MSGBOX_AUTOCLOSE
	end
TARCText_BrawlyPrematch:
	.string "My fists are legendary -- want an \n"
	.string "autograph, or a bruise?$"
TARCText_BrawlyMatchDefeat:
	.string "Wow. You KO'd my confidence.$"
TARCText_BrawlyPostmatch:
	.string "Guess that was a low blow to my pride.$"

TARCEventScript_Wattsonmatch::
	trainerbattle_single TRAINER_WATTSON_1, TARCText_WattsonPrematch, TARCText_WattsonMatchDefeat, RustboroCity_Gym_Text_RoxanneRematchNeedTwoMons
	msgbox TARCText_WattsonPostmatch, MSGBOX_AUTOCLOSE
	end
TARCText_WattsonPrematch:
	.string "My strategy? High energy, low mercy.$"
TARCText_WattsonMatchDefeat:
	.string "Too bad your team was fully grounded...$"
TARCText_WattsonPostmatch:
	.string "Guess I was all static and no surge.$"


TARCEventScript_Flannerymatch::
	trainerbattle_single TRAINER_FLANNERY_1, TARCText_FlanneryPrematch, TARCText_FlanneryMatchDefeat, RustboroCity_Gym_Text_RoxanneRematchNeedTwoMons
	msgbox TARCText_FlanneryPostmatch, MSGBOX_AUTOCLOSE
	end
TARCText_FlanneryPrematch:
	.string "Your win streak? About to be cooked.$"
TARCText_FlanneryMatchDefeat:
	.string "Well, that went up in smoke.$"
TARCText_FlanneryPostmatch:
	.string "Guess I burned myself out.$"

TARCEventScript_Normanmatch::
	trainerbattle_single TRAINER_NORMAN_1, TARCText_NormanPrematch, TARCText_NormanMatchDefeat, RustboroCity_Gym_Text_RoxanneRematchNeedTwoMons
	msgbox TARCText_NormanPostmatch, MSGBOX_AUTOCLOSE
	end
TARCText_NormanPrematch:
	.string "Simple strategies. Superior results.$"
TARCText_NormanMatchDefeat:
	.string "You turned my easy win into an easy loss.$"
TARCText_NormanPostmatch:
	.string "Guess I'm just your stepping stone \n"
	.string "to greatness.$"

TARCEventScript_Tatematch::
	trainerbattle_double TRAINER_TATE_AND_LIZA_1, TARCText_TatePrematch, TARCText_TateMatchDefeat, RustboroCity_Gym_Text_RoxanneRematchNeedTwoMons
	msgbox TARCText_TatePostmatch, MSGBOX_AUTOCLOSE
	end
TARCText_TatePrematch:
	.string "We're sensing… an L in your future.$"
TARCText_TateMatchDefeat:
	.string "Looks like our future sight was... \n"
	.string "slightly off.$"
TARCText_TatePostmatch:
	.string "Guess we weren't clairvoyant enough.$"


TARCEventScript_Winonamatch::
	trainerbattle_single TRAINER_WINONA_1, TARCText_WinonaPrematch, TARCText_WinonaMatchDefeat, RustboroCity_Gym_Text_RoxanneRematchNeedTwoMons
	msgbox TARCText_WinonaPostmatch, MSGBOX_AUTOCLOSE
	end
TARCText_WinonaPrematch:
	.string "This battle is about to take off.$"
TARCText_WinonaMatchDefeat:
	.string "Well, that was a crash landing.$"
TARCText_WinonaPostmatch:
	.string "Guess my strategy never took off.$"

TARCEventScript_Sidneymatch::
	trainerbattle_single TRAINER_SIDNEY, TARCText_SidneyPrematch, TARCText_SidneyMatchDefeat, RustboroCity_Gym_Text_RoxanneRematchNeedTwoMons
	msgbox TARCText_SidneyPostmatch, MSGBOX_AUTOCLOSE
	end
TARCText_SidneyPrematch:
	.string "Ready to face your darkest fears?$"
TARCText_SidneyMatchDefeat:
	.string "Even the deepest shadows can't hide\n"
	.string "defeat.$"
TARCText_SidneyPostmatch:
	.string "Guess I'll slink back into the\n"
	.string "shadows.$"

TARCEventScript_Drakematch::
	trainerbattle_single TRAINER_DRAKE, TARCText_DrakePrematch, TARCText_DrakeMatchDefeat, RustboroCity_Gym_Text_RoxanneRematchNeedTwoMons
	msgbox TARCText_DrakePostmatch, MSGBOX_AUTOCLOSE
	end
TARCText_DrakePrematch:
	.string "Let's see if you can handle my scale of\n"
	.string "power.$"
TARCText_DrakeMatchDefeat:
	.string "My scales couldn't shield me from that \n"
	.string "hit.$"
TARCText_DrakePostmatch:
	.string "Guess my scales weren't as tough as \n"
	.string "I thought..$"

TARCEventScript_Phoebematch::
	trainerbattle_single TRAINER_PHOEBE, TARCText_PhoebePrematch, TARCText_PhoebeMatchDefeat, RustboroCity_Gym_Text_RoxanneRematchNeedTwoMons
	msgbox TARCText_PhoebePostmatch, MSGBOX_AUTOCLOSE
	end
TARCText_PhoebePrematch:
	.string "I'm dying to see how you handle this.$"

TARCText_PhoebeMatchDefeat:
	.string "Well... that's a grave mistake on my\n"
	.string "part.$"

TARCText_PhoebePostmatch:
	.string "Guess my plan was dead on arrival.$"

TARCEventScript_Stevenmatch::
	trainerbattle_single TRAINER_STEVEN, TARCText_StevenPrematch, TARCText_StevenMatchDefeat, RustboroCity_Gym_Text_RoxanneRematchNeedTwoMons
	msgbox TARCText_StevenPostmatch, MSGBOX_AUTOCLOSE
	end
TARCText_StevenPrematch:
	.string "You can't cut it against my steel.$"

TARCText_StevenMatchDefeat:
	.string "You really hammered me there.$"

TARCText_StevenPostmatch:
	.string "Guess I wasn't as unbreakable as I\n"
	.string "thought.$"

TARCEventScript_Juanmatch::
	trainerbattle_single TRAINER_JUAN_1, TARCText_JuanPrematch, TARCText_JuanMatchDefeat, RustboroCity_Gym_Text_RoxanneRematchNeedTwoMons
	msgbox TARCText_JuanPostmatch, MSGBOX_AUTOCLOSE
	end
TARCText_JuanPrematch:
	.string "Time to soak up another win.$"

TARCText_JuanMatchDefeat:
	.string "You swam circles around me.$"

TARCText_JuanPostmatch:
	.string "Guess I am sleeping with the Magikarp.$"

ARC_HEALER::
	lock
	faceplayer
	msgbox WantCandy, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, GiveCandy
	goto_if_eq VAR_RESULT, NO, NoCandy
	end

WantCandy:
	.string "Hey, kid, want some candy?$"
	
GiveCandy::
	msgbox HereUGo
	addvar VAR_PACIFIDLOG_TOWN_STATE, 1
	giveitem ITEM_EXP_CANDY_XL 10
	releaseall
	end
HereUGo:
	.string "Enjoy the hackaroo!$"
NoCandy::
	msgbox BeThatWay
	releaseall
	end
BeThatWay:
	.string "Fine, be that way. Ugh.$"

TARC_HEALER::
	lock
	faceplayer
	msgbox Route111_OldLadysRestStop_Text_RestUpHere, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, Route111_OldLadysRestStop_EventScript_Rest
	goto_if_eq VAR_RESULT, NO, Route111_OldLadysRestStop_EventScript_DeclineRest
	end

Route111_OldLadysRestStop_EventScript_Rest::
	msgbox Route111_OldLadysRestStop_Text_TakeYourTimeRestUp, MSGBOX_DEFAULT
	addvar VAR_VERDANTURF_TOWN_STATE, 1
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
	setvar VAR_TEMP_7, 1
	msgbox Aio_Intro_Text_0
	releaseall
	return

TARC_OneStep:
	walk_right
	step_end

TARC_TwoStep:
	walk_right
	walk_right
	step_end


Aio_Intro_Text_0:
	.string "Hiya, and welcome to my Romhack! I'm Aio,\n"
	.string "the creator of this game. Are you ready\p"
	.string "to go down in history? This room is your\n"
	.string "chance to play in a mini game for fame\p"
	.string "and glory! The premise is simple. During\n"
	.string "the course of your trials, a mini game\p"
	.string "will appear after battle. Your goal is to\n"
	.string "earn as much money as possible using a\p"
	.string "randomised set of cards presented to\n"
	.string "you. After 12 battles, the romhack will\p"
	.string "be over, and you will be presented with\n"
	.string "your total cash accumulated for\p"
	.string "bragging rights among the community!\n"
	.string "If you get items, you can sell them, too!\p"
	.string "This mini game will be in my full length\n"
	.string "Persona inspired Romhack, Pokemon\p"
	.string "Rosebud! Without further ado, have fun\n"
	.string "and good luck! Now, choose your Pokemon!$"

Aio_Movement:
	walk_up
	walk_up
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	step_end

TriggerFinish::
	lockall
	applymovement OBJ_EVENT_ID_PLAYER, Aio_Movement
	waitmovement(0)
	msgbox YouWin
	special GetMoneyHelper
	msgbox MoneyGet
	special GetSwordTokensHelper
	msgbox SwordsGet
	buffernumberstring STR_VAR_3, VAR_VERDANTURF_TOWN_STATE 
	msgbox JoyRest
	buffernumberstring STR_VAR_1, VAR_PACIFIDLOG_TOWN_STATE 
	msgbox CandyCount
	msgbox FinishGame
	special GameClear
	waitstate
	releaseall
	end

YouWin:
	.string "Thank you for playing my tech demo!\n"
	.string "Press A button to see your stats!$"

MoneyGet: 
	.string "Total money earned is {STR_VAR_1}!$"

SwordsGet: 
	.string "Total Sword Tokens earned is {STR_VAR_1}!$"

JoyRest:
	.string "You rested with Joy {STR_VAR_3} times!$"

CandyCount:
	.string "You received candies {STR_VAR_1} times!$"

FinishGame:
	.string "Aio and LC hope you have a great day!\n"
	.string "Look forward to Pokemon Rosebud, ETA \p"
	.string "Fall 2026!$"

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
	.string "You can spend sword tokens here!$"

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

