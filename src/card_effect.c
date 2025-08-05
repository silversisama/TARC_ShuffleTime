#include "global.h"
#include "card.h"
#include "event_data.h"
#include "item.h"
#include "menu.h"
#include "money.h"
#include "pokemon.h"
#include "random.h"
#include "shuffle_time.h"
#include "string_util.h"
#include "strings.h"
#include "sword_tokens.h"
#include "task.h"
#include "text.h"
#include "window.h"
#include "constants/items.h"

#define ABILITY_ITEM_COUNT 2

enum
{
    SUIT_COINS,
    SUIT_CUPS,
    SUIT_SWORDS,
    SUIT_WANDS,
    SUIT_COUNT,
};

typedef void (*CardEffectFunc)(u8 rank);

struct CardEffect
{
    CardEffectFunc func;
    u8 rank;
};

//// Functions
void ApplyCardEffect(u8 cardId);
void ApplyCardEffects_OnWildEncounter(struct Pokemon *mon);
void ApplyCardEffects_OnMoneyReward(u32 *moneyReward);

//// Utility functions
static bool8 TryAddItem(u16 itemId, u16 count);

//// Menu functions
// Choose ability item menu functions
static void Task_ChooseAbilityItem(u8 taskId);
static void Task_InputAbilityItemMenu(u8 taskId);
static void Task_HandleAbilityItemMenu(u8 taskId);
// Choose suit menu functions
static void Task_ChooseSuit(u8 taskId);
static void Task_InputSuitMenu(u8 taskId);
static void Task_HandleSuitMenu(u8 taskId);

//// Card effect flags interface
// Death card effect flag interface
void CardEffectDeathEnable(void);
void CardEffectDeathDisable(void);
bool8 IsCardEffectDeathEnabled(void);
// Fortune card effect flag interface
void CardEffectFortuneEnable(void);
void CardEffectFortuneDisable(void);
bool8 IsCardEffectFortuneEnabled(void);
// Justice card effect flag interface
void CardEffectJusticeEnable(void);
void CardEffectJusticeDisable(void);
bool8 IsCardEffectJusticeEnabled(void);

//// Generic Card Effect
static void sCardEffectGiveItem(u16 itemId, u16 count);
static void sCardEffectExtraDraws(u8 count);

//// Card effects
static void sCardEffectChariot(u8 rank);
static void sCardEffectCoins(u8 rank);
static void sCardEffectCups(u8 rank);
static void sCardEffectDeath(u8 rank);
static void sCardEffectDevil(u8 rank);
static void sCardEffectEmperor(u8 rank);
static void sCardEffectEmpress(u8 rank);
static void sCardEffectFool(u8 rank);
static void sCardEffectFortune(u8 rank);
static void sCardEffectHangedman(u8 rank);
static void sCardEffectHermit(u8 rank);
static void sCardEffectHierophant(u8 rank);
static void sCardEffectJudge(u8 rank);
static void sCardEffectJustice(u8 rank);
static void sCardEffectLovers(u8 rank);
static void sCardEffectMagician(u8 rank);
static void sCardEffectMoon(u8 rank);
static void sCardEffectPriestess(u8 rank);
static void sCardEffectStar(u8 rank);
static void sCardEffectStrength(u8 rank);
static void sCardEffectSun(u8 rank);
static void sCardEffectSwords(u8 rank);
static void sCardEffectTemperance(u8 rank);
static void sCardEffectTower(u8 rank);
static void sCardEffectWands(u8 rank);
static void sCardEffectWorld(u8 rank);

static const struct CardEffect sCardEffects[CARD_COUNT] =
{
    [CARD_BACK]       = {},
    [CARD_CHARIOT]    = { .func = sCardEffectChariot },
    [CARD_COINS1]     = { .func = sCardEffectCoins, .rank = 1 },
    [CARD_COINS2]     = { .func = sCardEffectCoins, .rank = 2 },
    [CARD_COINS3]     = { .func = sCardEffectCoins, .rank = 3 },
    [CARD_COINS4]     = { .func = sCardEffectCoins, .rank = 4 },
    [CARD_COINS5]     = { .func = sCardEffectCoins, .rank = 5 },
    [CARD_COINS6]     = { .func = sCardEffectCoins, .rank = 6 },
    [CARD_COINS7]     = { .func = sCardEffectCoins, .rank = 7 },
    [CARD_COINS8]     = { .func = sCardEffectCoins, .rank = 8 },
    [CARD_CUPS1]      = { .func = sCardEffectCups, .rank = 1 },
    [CARD_CUPS2]      = { .func = sCardEffectCups, .rank = 2 },
    [CARD_CUPS3]      = { .func = sCardEffectCups, .rank = 3 },
    [CARD_CUPS4]      = { .func = sCardEffectCups, .rank = 4 },
    [CARD_CUPS5]      = { .func = sCardEffectCups, .rank = 5 },
    [CARD_CUPS6]      = { .func = sCardEffectCups, .rank = 6 },
    [CARD_CUPS7]      = { .func = sCardEffectCups, .rank = 7 },
    [CARD_CUPS8]      = { .func = sCardEffectCups, .rank = 8 },
    [CARD_DEATH]      = { .func = sCardEffectDeath },
    [CARD_DEVIL]      = { .func = sCardEffectDevil },
    [CARD_EMPEROR]    = { .func = sCardEffectEmperor },
    [CARD_EMPRESS]    = { .func = sCardEffectEmpress },
    [CARD_FOOL]       = { .func = sCardEffectFool },
    [CARD_FORTUNE]    = { .func = sCardEffectFortune },
    [CARD_HANGEDMAN]  = { .func = sCardEffectHangedman },
    [CARD_HERMIT]     = { .func = sCardEffectHermit },
    [CARD_HIEROPHANT] = { .func = sCardEffectHierophant },
    [CARD_JUDGE]      = { .func = sCardEffectJudge },
    [CARD_JUSTICE]    = { .func = sCardEffectJustice },
    [CARD_LOVERS]     = { .func = sCardEffectLovers },
    [CARD_MAGICIAN]   = { .func = sCardEffectMagician },
    [CARD_MOON]       = { .func = sCardEffectMoon },
    [CARD_PRIESTESS]  = { .func = sCardEffectPriestess },
    [CARD_STAR]       = { .func = sCardEffectStar },
    [CARD_STRENGTH]   = { .func = sCardEffectStrength },
    [CARD_SUN]        = { .func = sCardEffectSun },
    [CARD_SWORDS1]    = { .func = sCardEffectSwords, .rank = 1 },
    [CARD_SWORDS2]    = { .func = sCardEffectSwords, .rank = 2 },
    [CARD_SWORDS3]    = { .func = sCardEffectSwords, .rank = 3 },
    [CARD_SWORDS4]    = { .func = sCardEffectSwords, .rank = 4 },
    [CARD_SWORDS5]    = { .func = sCardEffectSwords, .rank = 5 },
    [CARD_SWORDS6]    = { .func = sCardEffectSwords, .rank = 6 },
    [CARD_SWORDS7]    = { .func = sCardEffectSwords, .rank = 7 },
    [CARD_SWORDS8]    = { .func = sCardEffectSwords, .rank = 8 },
    [CARD_TEMPERANCE] = { .func = sCardEffectTemperance },
    [CARD_TOWER]      = { .func = sCardEffectTower },
    [CARD_WANDS1]     = { .func = sCardEffectWands, .rank = 1 },
    [CARD_WANDS2]     = { .func = sCardEffectWands, .rank = 2 },
    [CARD_WANDS3]     = { .func = sCardEffectWands, .rank = 3 },
    [CARD_WANDS4]     = { .func = sCardEffectWands, .rank = 4 },
    [CARD_WANDS5]     = { .func = sCardEffectWands, .rank = 5 },
    [CARD_WANDS6]     = { .func = sCardEffectWands, .rank = 6 },
    [CARD_WANDS7]     = { .func = sCardEffectWands, .rank = 7 },
    [CARD_WANDS8]     = { .func = sCardEffectWands, .rank = 8 },
    [CARD_WORLD]      = { .func = sCardEffectWorld },
};

static const u16 sMajorArcanas[] =
{
    CARD_CHARIOT,
    CARD_DEATH,
    CARD_DEVIL,
    CARD_EMPEROR,
    CARD_EMPRESS,
    CARD_FOOL,
    CARD_FORTUNE,
    CARD_HANGEDMAN,
    CARD_HERMIT,
    CARD_HIEROPHANT,
    CARD_JUDGE,
    CARD_JUSTICE,
    CARD_LOVERS,
    CARD_MAGICIAN,
    CARD_MOON,
    CARD_PRIESTESS,
    CARD_STAR,
    CARD_STRENGTH,
    CARD_SUN,
    CARD_TEMPERANCE,
    CARD_TOWER,
    CARD_WORLD,
};

static const u16 sHealingItems[] =
{
    ITEM_ETHER,
    ITEM_FULL_HEAL,
    ITEM_POTION,
    ITEM_SUPER_POTION,
    ITEM_HYPER_POTION,
    ITEM_MAX_POTION,
    ITEM_FULL_RESTORE,
    ITEM_REVIVE,
    ITEM_MAX_REVIVE,
};

static const u16 sPokeballItems[][2] =
{
    { ITEM_POKE_BALL,   5 },
    { ITEM_GREAT_BALL,  4 },
    { ITEM_ULTRA_BALL,  3 },
    { ITEM_MASTER_BALL, 1 },
};

static const u16 sVitaminItems[] =
{
    ITEM_HP_UP,
    ITEM_PROTEIN,
    ITEM_IRON,
    ITEM_CALCIUM,
    ITEM_ZINC,
    ITEM_CARBOS,
    ITEM_PP_UP,
    ITEM_PP_MAX,
};

static const u16 sAbilityItems[] =
{
    ITEM_ABILITY_PATCH,
    ITEM_ABILITY_CAPSULE,
};

static u16 ExpCandyItems[][2] =
{
    { ITEM_EXP_CANDY_XS, 2 },
    { ITEM_EXP_CANDY_XS, 3 },
    { ITEM_EXP_CANDY_XS, 4 },
    { ITEM_EXP_CANDY_S,  1 },
    { ITEM_EXP_CANDY_S,  2 },
    { ITEM_EXP_CANDY_S,  3 },
    { ITEM_EXP_CANDY_S,  4 },
    { ITEM_EXP_CANDY_M,  2 },
};

static u8 sSuitCardIndices[SUIT_COUNT][2] =
{
    [SUIT_COINS]  = { CARD_COINS1,  CARD_COINS8},
    [SUIT_CUPS]   = { CARD_CUPS1,   CARD_CUPS8},
    [SUIT_SWORDS] = { CARD_SWORDS1, CARD_SWORDS8},
    [SUIT_WANDS]  = { CARD_WANDS1,  CARD_WANDS8},
};

static const u8 sText_Receive[]          = _("You received {STR_VAR_1}!");
static const u8 sText_ReceiveFail[]      = _("You couldn't receive items");
static const u8 sText_RestoreHP[]        = _("Your party restore {STR_VAR_1}% HP!");
static const u8 sText_RestoreFullHP[]    = _("Your party is fully healed!");
static const u8 sText_Lose[]             = _("You lose {STR_VAR_1}");
static const u8 sText_EffectDeath[]      = _("The next wild POKéMON will run away");
static const u8 sText_EffectFortune[]    = _("The next wild POKéMON will be shiny");
static const u8 sText_EffectHangedman[]  = _("Repel effects removed");
static const u8 sText_EffectJustice[]    = _("The next trainer will give 30% more money");
static const u8 sText_EffectTower[]      = _("{STR_VAR_1} loses {STR_VAR_2} HP");
static const u8 sText_EffectHierophant[] = _("Choose 2 more cards");
static const u8 sText_EffectMoon[]       = _("Choose an ability item");
static const u8 sText_EffectMagician[]   = _("Choose one of suits to gain");
static const u8 sText_SuitCoins[]        = _("S.Coins");
static const u8 sText_SuitCups[]         = _("S.Cups");
static const u8 sText_SuitSwords[]       = _("S.Swords");
static const u8 sText_SuitWands[]        = _("S.Wands");
static const u8 sText_SwordTokens[]      = _("Sword Tokens");

static const struct WindowTemplate sAbilityItemWindowTemplate =
{
    .bg          = 0,
    .tilemapLeft = 1,
    .tilemapTop  = 10,
    .width       = 11,
    .height      = 4,
    .paletteNum  = 15,
    .baseBlock   = 0x60,
};

static const struct MenuAction sAbilityItemMenuAction[ABILITY_ITEM_COUNT] =
{
    { gItemsInfo[sAbilityItems[0]].name, { .void_u8 = Task_HandleAbilityItemMenu } },
    { gItemsInfo[sAbilityItems[1]].name, { .void_u8 = Task_HandleAbilityItemMenu } },
};

static const struct WindowTemplate sSuitWindowTemplate =
{
    .bg          = 0,
    .tilemapLeft = 1,
    .tilemapTop  = 6,
    .width       = 11,
    .height      = 8,
    .paletteNum  = 15,
    .baseBlock   = 0x60,
};

static const struct MenuAction sSuitMenuAction[SUIT_COUNT] =
{
    { sText_SuitCoins,  { .void_u8 = Task_HandleSuitMenu } },
    { sText_SuitCups,   { .void_u8 = Task_HandleSuitMenu } },
    { sText_SuitSwords, { .void_u8 = Task_HandleSuitMenu } },
    { sText_SuitWands,  { .void_u8 = Task_HandleSuitMenu } },
};

//// Functions

void ApplyCardEffect(u8 cardId)
{
    if (cardId >= CARD_COUNT)
        return;

    const struct CardEffect *cardEffect = &sCardEffects[cardId];
    if (cardEffect->func == NULL)
        return;

    cardEffect->func(cardEffect->rank);
}

void ApplyCardEffects_OnWildEncounter(struct Pokemon *mon)
{
    if(IsCardEffectFortuneEnabled())
    {
        CardEffectFortuneDisable();
        bool8 isShiny = TRUE;
        SetMonData(mon, MON_DATA_IS_SHINY, &isShiny);
    }
}

void ApplyCardEffects_OnMoneyReward(u32 *moneyReward)
{
    if(IsCardEffectJusticeEnabled())
    {
        CardEffectJusticeDisable();
        const u32 percentage = 130;
        (*moneyReward) = ((*moneyReward) * percentage) / 100;
    }
}

//// Generic card effect

static void sCardEffectGiveItem(u16 itemId, u16 count)
{
    // Try to add item to bag or pc
    if (TryAddItem(itemId, count))
    {
        if (count == 1)
            StringCopy(gStringVar1, GetItemName(itemId));
        else
        {
            ConvertIntToDecimalStringN(gStringVar1, count, STR_CONV_MODE_LEFT_ALIGN, 2);
            StringAppend(gStringVar1, gText_Space);
            StringAppend(gStringVar1, GetItemName(itemId));
        }
        StringExpandPlaceholders(gStringVar4, sText_Receive);
        ShuffleTimeDisplayMessage(gStringVar4);
    }
    else
        ShuffleTimeDisplayMessage(sText_ReceiveFail);
}

void sCardEffectExtraDraws(u8 count)
{
    ShuffleTimeExtraDraws(count);
}

//// Card effect functions

static void sCardEffectChariot(u8 rank)
{
    // Give speed vitamin
    sCardEffectGiveItem(ITEM_CARBOS, 1);
}

static void sCardEffectCoins(u8 rank)
{
    // Give 300 poke base and 100 extra for every rank (above 1)
    const u16 money = 300 + 100 * (rank - 1);
    // AddMoney already check for overflow
    AddMoney(&gSaveBlock1Ptr->money, money);

    ConvertIntToDecimalStringN(gStringVar1, money, STR_CONV_MODE_LEFT_ALIGN, (rank == 8) ? 4 : 3);
    StringExpandPlaceholders(gStringVar2, gText_PokedollarVar1);
    StringCopy(gStringVar1, gStringVar2);
    StringExpandPlaceholders(gStringVar4, sText_Receive);
    ShuffleTimeDisplayMessage(gStringVar4);
}

static void sCardEffectCups(u8 rank)
{
    // Restore the party hp. Starts at 10% and 8% extra for every rank (above 1)
    const u16 percentage = 10 + 8 * (rank - 1);
    for (u8 i = 0; i < PARTY_SIZE; i++)
    {
        struct Pokemon *mon = &gPlayerParty[i];

        if (!GetMonData(mon, MON_DATA_SPECIES) || GetMonData(mon, MON_DATA_IS_EGG))
            continue;

        const u16 maxHP = GetMonData(mon, MON_DATA_MAX_HP);
        const u16 curHP = GetMonData(mon, MON_DATA_HP);

        if (curHP < maxHP)
        {
            const u16 amount = (maxHP * percentage) / 100;
            u16 newHP = curHP + amount;
            if (newHP > maxHP)
                newHP = maxHP;
            SetMonData(mon, MON_DATA_HP, &newHP);
        }
    }
    ConvertIntToDecimalStringN(gStringVar1, percentage, STR_CONV_MODE_LEFT_ALIGN, 2);
    StringExpandPlaceholders(gStringVar4, sText_RestoreHP);
    ShuffleTimeDisplayMessage(gStringVar4);
}

static void sCardEffectDeath(u8 rank)
{
    // Next wild encounter runs
    CardEffectDeathEnable();
    ShuffleTimeDisplayMessage(sText_EffectDeath);
}

static void sCardEffectDevil(u8 rank)
{
    // Lose 500 Poke, draw random swords
    const u16 money = 500;
    RemoveMoney(&gSaveBlock1Ptr->money, money);

    ConvertIntToDecimalStringN(gStringVar1, money, STR_CONV_MODE_LEFT_ALIGN, 3);
    StringExpandPlaceholders(gStringVar2, gText_PokedollarVar1);
    StringCopy(gStringVar1, gStringVar2);
    StringExpandPlaceholders(gStringVar4, sText_Lose);
    ShuffleTimeDisplayMessage(gStringVar4);

    const u16 cardId = RandomUniform(RNG_NONE, CARD_SWORDS1, CARD_SWORDS8);
    ShuffleTimeMorphCard(cardId);
}

static void sCardEffectEmperor(u8 rank)
{
    // Rerolls into any major arcana and choose one more
    const u16 cardId = RandomElement(RNG_NONE, sMajorArcanas);
    ShuffleTimeMorphCard(cardId);
    sCardEffectExtraDraws(1);
}

static void sCardEffectEmpress(u8 rank)
{
    // Give healing item
    const u16 itemId = RandomElement(RNG_NONE, sHealingItems);
    sCardEffectGiveItem(itemId, 1);
}

static void sCardEffectFool(u8 rank)
{
    // Rerolls into any card and choose one more
    const u16 cardId = RandomUniform(RNG_NONE, 1, CARD_COUNT - 1);
    ShuffleTimeMorphCard(cardId);
    sCardEffectExtraDraws(1);
}

static void sCardEffectFortune(u8 rank)
{
    // Next wild POKéMON will be shiny
    CardEffectFortuneEnable();
    ShuffleTimeDisplayMessage(sText_EffectFortune);
}

static void sCardEffectHangedman(u8 rank)
{
    // Remove repel effects
    if(REPEL_STEP_COUNT)
        VarSet(VAR_REPEL_STEP_COUNT, 0);
    ShuffleTimeDisplayMessage(sText_EffectHangedman);
}

static void sCardEffectHermit(u8 rank)
{
    // Give max repel
    sCardEffectGiveItem(ITEM_MAX_REPEL, 1);
}

static void sCardEffectHierophant(u8 rank)
{
    // Choose 2 more cards
    sCardEffectExtraDraws(2);
    ShuffleTimeDisplayMessage(sText_EffectHierophant);
}

static void sCardEffectJudge(u8 rank)
{
    // Give lure (1-5)
    const u16 count = RandomUniform(RNG_NONE, 1, 5);
    sCardEffectGiveItem(ITEM_LURE, count);
}

static void sCardEffectJustice(u8 rank)
{
    // Trainers give 30% more money in next battle
    CardEffectJusticeEnable();
    ShuffleTimeDisplayMessage(sText_EffectJustice);
}

static void sCardEffectLovers(u8 rank)
{
    // Random coins card and choose one more
    const u16 cardId = RandomUniform(RNG_NONE, CARD_COINS1, CARD_COINS8);
    ShuffleTimeMorphCard(cardId);
    sCardEffectExtraDraws(1);
}

static void sCardEffectMagician(u8 rank)
{
    // Choose one of suits to gain
    CreateTask(Task_ChooseSuit, 0);
    ShuffleTimeDisplayMessage(sText_EffectMagician);
}


static void sCardEffectMoon(u8 rank)
{
    // Choose an ability item
    CreateTask(Task_ChooseAbilityItem, 0);
    ShuffleTimeDisplayMessage(sText_EffectMoon);
}

static void sCardEffectPriestess(u8 rank)
{
    // Give a rare candy
    sCardEffectGiveItem(ITEM_RARE_CANDY, 1);
}

static void sCardEffectStar(u8 rank)
{
    // Random cups card and choose two more
    const u16 cardId = RandomUniform(RNG_NONE, CARD_CUPS1, CARD_CUPS8);
    ShuffleTimeMorphCard(cardId);
    sCardEffectExtraDraws(2);
}

static void sCardEffectStrength(u8 rank)
{
    // Give random vitamin
    const u16 itemId = RandomElement(RNG_NONE, sVitaminItems);
    sCardEffectGiveItem(itemId, 1);
}

static void sCardEffectSun(u8 rank)
{
    // Full heal team, choose one more
    for (u8 i = 0; i < PARTY_SIZE; i++)
    {
        struct Pokemon *mon = &gPlayerParty[i];

        if (!GetMonData(mon, MON_DATA_SPECIES) || GetMonData(mon, MON_DATA_IS_EGG))
            continue;

        const u16 maxHP = GetMonData(mon, MON_DATA_MAX_HP);
        const u16 curHP = GetMonData(mon, MON_DATA_HP);
        if (curHP < maxHP)
            SetMonData(mon, MON_DATA_HP, &maxHP);
    }
    ShuffleTimeDisplayMessage(sText_RestoreFullHP);
    sCardEffectExtraDraws(1);
}

static void sCardEffectSwords(u8 rank)
{
    // Grants points for TMs.
    // The amount of tokens to add is 2^rank
    const u16 amount = 1 << rank;
    AddSwordTokens(amount);

    ConvertIntToDecimalStringN(gStringVar1, amount, STR_CONV_MODE_LEFT_ALIGN, 3);
    StringAppend(gStringVar1, gText_Space);
    StringAppend(gStringVar1, sText_SwordTokens);
    StringExpandPlaceholders(gStringVar4, sText_Receive);
    ShuffleTimeDisplayMessage(gStringVar4);
}

static void sCardEffectTemperance(u8 rank)
{
    // Gives random PokeBall (1-5 poke, or 1-4 great, or 1-3 ultra, or 1 master)
    const u16 choose   = Random() % ARRAY_COUNT(sPokeballItems);
    const u16 itemId   = sPokeballItems[choose][0];
    const u16 maxCount = sPokeballItems[choose][1];
    const u16 count    = RandomUniform(RNG_NONE, 1, maxCount);
    sCardEffectGiveItem(itemId, count);
}

static void sCardEffectTower(u8 rank)
{
    // Halve lead POKéMON hp, draw random wands
    for (u8 i = 0; i < PARTY_SIZE; i++)
    {
        struct Pokemon *mon = &gPlayerParty[i];

        if (!GetMonData(mon, MON_DATA_SPECIES) || GetMonData(mon, MON_DATA_IS_EGG))
            continue;

        const u16 curHP = GetMonData(mon, MON_DATA_HP);
        const u16 newHP = curHP / 2;
        if (curHP > 0)
        {
            SetMonData(mon, MON_DATA_HP, &newHP);

            u8 nickname[max(32, POKEMON_NAME_BUFFER_SIZE)];
            GetMonData(mon, MON_DATA_NICKNAME, nickname);
            StringCopy(gStringVar1, nickname);

            u16 amount = curHP - newHP;
            ConvertIntToDecimalStringN(gStringVar2, amount, STR_CONV_MODE_LEFT_ALIGN, 3);
            StringExpandPlaceholders(gStringVar4, sText_EffectTower);
            ShuffleTimeDisplayMessage(gStringVar4);

            break; // Only affect the lead POKéMON
        }
    }
    const u16 cardId = RandomUniform(RNG_NONE, CARD_WANDS1, CARD_WANDS8);
    ShuffleTimeMorphCard(cardId);
}

static void sCardEffectWands(u8 rank)
{
    // Give exp candy
    const u8 index  = rank - 1;
    const u8 itemId = ExpCandyItems[index][0];
    const u8 count  = ExpCandyItems[index][1] * gPlayerPartyCount;
    sCardEffectGiveItem(itemId, count);
}

static void sCardEffectWorld(u8 rank)
{
    // Give 3 full restore and 2 elixir
    bool8 gotFullRestore = TryAddItem(ITEM_FULL_RESTORE, 3);
    bool8 gotElixir      = TryAddItem(ITEM_ELIXIR, 2);

    if (!(gotFullRestore || gotElixir))
    {
        ShuffleTimeDisplayMessage(sText_ReceiveFail);
        return;
    }

    if (gotFullRestore)
    {
        ConvertIntToDecimalStringN(gStringVar1, 3, STR_CONV_MODE_LEFT_ALIGN, 1);
        StringAppend(gStringVar1, gText_Space);
        StringAppend(gStringVar1, GetItemName(ITEM_FULL_RESTORE));
    }

    if (gotElixir)
    {
        ConvertIntToDecimalStringN(gStringVar2, 2, STR_CONV_MODE_LEFT_ALIGN, 1);
        StringAppend(gStringVar2, gText_Space);
        StringAppend(gStringVar2, GetItemName(ITEM_ELIXIR));
    }

    if (gotFullRestore && gotElixir)
    {
        StringAppend(gStringVar1, gText_SpaceAndSpace);
        StringAppend(gStringVar1, gStringVar2);
    }

    if (!gotFullRestore && gotElixir)
        StringCopy(gStringVar1, gStringVar2);

    StringExpandPlaceholders(gStringVar4, sText_Receive);
    ShuffleTimeDisplayMessage(gStringVar4);
}

// Util functions

static bool8 TryAddItem(u16 itemId, u16 count)
{
    // Try to add item to bag or pc
    return AddBagItem(itemId, count) || AddPCItem(itemId, count);
}

//// AbilityItem selection functions

#define tState    data[0]
#define tWindowId data[1]

static void Task_ChooseAbilityItem(u8 taskId)
{
    switch (gTasks[taskId].tState)
    {
    case 0:
        gTasks[taskId].tWindowId = AddWindow(&sAbilityItemWindowTemplate);
        gTasks[taskId].tState++;
        break;
    case 1:
        DrawStdWindowFrame(gTasks[taskId].tWindowId, TRUE);
        gTasks[taskId].tState++;
        break;
    case 2:
        SetStandardWindowBorderStyle(gTasks[taskId].tWindowId, FALSE);
        PrintMenuTable(gTasks[taskId].tWindowId, ABILITY_ITEM_COUNT, sAbilityItemMenuAction);
        InitMenuInUpperLeftCornerNormal(gTasks[taskId].tWindowId, ABILITY_ITEM_COUNT, 0);
        PutWindowTilemap(gTasks[taskId].tWindowId);
        CopyWindowToVram(gTasks[taskId].tWindowId, COPYWIN_FULL);
        gTasks[taskId].tState++;
        break;
    case 3:
        ShuffleTimeInputLock();
        gTasks[taskId].func = Task_InputAbilityItemMenu;
        break;
    case 4:
        ClearStdWindowAndFrameToTransparent(gTasks[taskId].tWindowId, TRUE);
        RemoveWindow(gTasks[taskId].tWindowId);
        gTasks[taskId].tState++;
        break;
    case 5:
        ShuffleTimeInputUnlock();
        gTasks[taskId].tState++;
        break;
    default:
        DestroyTask(taskId);
        break;
    }
}

static void Task_InputAbilityItemMenu(u8 taskId)
{
    s8 inputCode = Menu_ProcessInputNoWrap();
    switch (inputCode)
    {
    case MENU_NOTHING_CHOSEN:
        break;
    case MENU_B_PRESSED:
        break;
    default:
        sAbilityItemMenuAction[inputCode].func.void_u8(inputCode);
        gTasks[taskId].func = Task_ChooseAbilityItem;
        gTasks[taskId].tState++;
        break;
    }
}

static void Task_HandleAbilityItemMenu(u8 inputCode)
{
    const u16 itemId = sAbilityItems[inputCode];
    sCardEffectGiveItem(itemId, 1);
}

//// Suit selection functions

static void Task_ChooseSuit(u8 taskId)
{
    switch (gTasks[taskId].tState)
    {
    case 0:
        gTasks[taskId].tWindowId = AddWindow(&sSuitWindowTemplate);
        gTasks[taskId].tState++;
        break;
    case 1:
        DrawStdWindowFrame(gTasks[taskId].tWindowId, TRUE);
        gTasks[taskId].tState++;
        break;
    case 2:
        SetStandardWindowBorderStyle(gTasks[taskId].tWindowId, FALSE);
        PrintMenuTable(gTasks[taskId].tWindowId, SUIT_COUNT, sSuitMenuAction);
        InitMenuInUpperLeftCornerNormal(gTasks[taskId].tWindowId, SUIT_COUNT, 0);
        PutWindowTilemap(gTasks[taskId].tWindowId);
        CopyWindowToVram(gTasks[taskId].tWindowId, COPYWIN_FULL);
        gTasks[taskId].tState++;
        break;
    case 3:
        ShuffleTimeInputLock();
        gTasks[taskId].func = Task_InputSuitMenu;
        break;
    case 4:
        ClearStdWindowAndFrameToTransparent(gTasks[taskId].tWindowId, TRUE);
        RemoveWindow(gTasks[taskId].tWindowId);
        gTasks[taskId].tState++;
        break;
    case 5:
        ShuffleTimeInputUnlock();
        gTasks[taskId].tState++;
        break;
    default:
        DestroyTask(taskId);
        break;
    }
}

static void Task_InputSuitMenu(u8 taskId)
{
    s8 inputCode = Menu_ProcessInputNoWrap();
    switch (inputCode)
    {
    case MENU_NOTHING_CHOSEN:
        break;
    case MENU_B_PRESSED:
        break;
    default:
        sSuitMenuAction[inputCode].func.void_u8(inputCode);
        gTasks[taskId].func = Task_ChooseSuit;
        gTasks[taskId].tState++;
        break;
    }
}

static void Task_HandleSuitMenu(u8 inputCode)
{
    u8 start  = sSuitCardIndices[inputCode][0];
    u8 end    = sSuitCardIndices[inputCode][1];
    u8 cardId = RandomUniform(RNG_NONE, start, end);
    ShuffleTimeMorphCard(cardId);
}

#undef tState
#undef tWindowId

////// Card effect flags interface

//// Death card effect flag interface

void CardEffectDeathEnable(void)
{
    FlagSet(FLAG_CARD_EFFECT_DEATH);
}

void CardEffectDeathDisable(void)
{
    FlagClear(FLAG_CARD_EFFECT_DEATH);
}

bool8 IsCardEffectDeathEnabled(void)
{
    return FlagGet(FLAG_CARD_EFFECT_DEATH);
}

//// Fortune card effect flag interface

void CardEffectFortuneEnable(void)
{
    FlagSet(FLAG_CARD_EFFECT_FORTUNE);
}

void CardEffectFortuneDisable(void)
{
    FlagClear(FLAG_CARD_EFFECT_FORTUNE);
}

bool8 IsCardEffectFortuneEnabled(void)
{
    return FlagGet(FLAG_CARD_EFFECT_FORTUNE);
}

//// Justice card effect flag interface

void CardEffectJusticeEnable(void)
{
    FlagSet(FLAG_CARD_EFFECT_JUSTICE);
}

void CardEffectJusticeDisable(void)
{
    FlagClear(FLAG_CARD_EFFECT_JUSTICE);
}

bool8 IsCardEffectJusticeEnabled(void)
{
    return FlagGet(FLAG_CARD_EFFECT_JUSTICE);
}
