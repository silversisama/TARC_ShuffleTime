#include "global.h"
#include "battle_anim.h"
#include "bg.h"
#include "card.h"
#include "card_effect.h"
#include "decompress.h"
#include "gpu_regs.h"
#include "main.h"
#include "malloc.h"
#include "menu.h"
#include "menu_helpers.h"
#include "overworld.h"
#include "palette.h"
#include "random.h"
#include "scanline_effect.h"
#include "shuffle_time.h"
#include "sound.h"
#include "sprite.h"
#include "string_util.h"
#include "task.h"
#include "window.h"

#include "item_menu.h"

#include "constants/rgb.h"
#include "constants/songs.h"

enum TagPalIds
{
    TAG_PAL_TITLE = 1,
    TAG_PAL_CURSOR,
    TAG_PAL_SELECTED,
    TAG_PAL_CARD,
    TAG_PAL_COUNT = TAG_PAL_CARD,
};

enum TagGfxIds
{
    TAG_GFX_TITLE_S = 1,
    TAG_GFX_TITLE_H,
    TAG_GFX_TITLE_U,
    TAG_GFX_TITLE_F,
    TAG_GFX_TITLE_L,
    TAG_GFX_TITLE_E,
    TAG_GFX_TITLE_T,
    TAG_GFX_TITLE_I,
    TAG_GFX_TITLE_M,
    TAG_GFX_CURSOR,
    TAG_GFX_SELECTED,
    TAG_GFX_CARD,
    TAG_GFX_COUNT   = TAG_GFX_CARD,
};

#define CARD_GFX_TAG(i) (TAG_GFX_COUNT + 1 + (i))
#define CARD_PAL_TAG(i) (TAG_PAL_COUNT + 1 + (i))

#define CARD_POS_X(i) (24 + 48 * (i))

#define SPRITE_TITLE_SHUFFLE_COUNT 7
#define SPRITE_TITLE_TIME_COUNT    4
#define SPRITE_TITLE_COUNT         (SPRITE_TITLE_SHUFFLE_COUNT + SPRITE_TITLE_TIME_COUNT)

#define SPRITE_CURSOR_IDX_START SPRITE_TITLE_COUNT
#define SPRITE_CURSOR_COUNT     2

#define SPRITE_CARD_COUNT     5
#define SPRITE_CARD_IDX_START (SPRITE_CURSOR_IDX_START + SPRITE_CURSOR_COUNT)
#define SPRITE_COUNT          (SPRITE_TITLE_COUNT + SPRITE_CURSOR_COUNT +  SPRITE_CARD_COUNT)

#define MAX_DRAWS SPRITE_CARD_COUNT

// Title's animation constants
#define TITLE_SLIDE_SPEED 2
#define TITLE_STEPS       40
#define TITLE_DELTA       (TITLE_SLIDE_SPEED * TITLE_STEPS)
// Title's "SHUFFLE" animation constants
#define SHUFFLE_TARGET_POS_Y  16
#define SHUFFLE_INITIAL_POS_X 32
#define SHUFFLE_INITIAL_POS_Y (SHUFFLE_TARGET_POS_Y - TITLE_DELTA)
// Title's "TIME" animation constants
#define TIME_TARGET_POS_Y  48
#define TIME_INITIAL_POS_X 128
#define TIME_INITIAL_POS_Y (TIME_TARGET_POS_Y - TITLE_DELTA)
// Cursor initial position
#define CURSOR_INITIAL_POS_X 120
#define CURSOR_INITIAL_POS_Y 96
// Card's animation constants
#define CARD_INITIAL_POS_X -56
#define CARD_INITIAL_POS_Y 96
#define CARD_SLIDE_SPEED   4
// Card's flip animation constants
#define CARD_FLIP_FRAMES   12
#define CARD_FLIP_DURATION 4
#define CARD_FLIP_HALF     6
// Selected's animation constants
#define SELECTED_FRAMES   12
#define SELECTED_DURATION 6

enum WindowIds
{
    WINDOW_TITLE,
    WINDOW_TEXT,
    WINDOW_COUNT,
};

enum {
    INTRO_STATE_TITLE_SLIDE,
    INTRO_STATE_CARDS_SLIDE,
    INTRO_STATE_CARDS_FLIP,
    INTRO_STATE_CURSOR_SHOW,
    INTRO_STATE_DONE,
};

enum {
    SELECT_STATE_CURSOR_HIDE,
    SELECT_STATE_SELECTED_SHOW,
    SELECT_STATE_CARD_SHINE,
    SELECT_STATE_SELECTED_HIDE,
    SELECT_STATE_APPLY_EFFECT,
    SELECT_STATE_MORPH_FLIP,
    SELECT_STATE_MORPH_DATA,
    SELECT_STATE_WAIT_INPUT,
    SELECT_STATE_CARD_OUT,
    SELECT_STATE_DONE,
};

struct ShuffleTimeGUI
{
    MainCallback savedCallback;
    u8 state;
    u32 spriteIds[SPRITE_COUNT];
    u8 handIds[SPRITE_CARD_COUNT];
    u8 cursor;
    bool8 inputLocked;
    bool8 isDrawed[SPRITE_CARD_COUNT];
    u8 drawCount;
    u8 maxDraws;
    u8 morphCardId;
};

// RAM
EWRAM_DATA static struct ShuffleTimeGUI *sShuffleTimeGuiDataPtr = NULL;
EWRAM_DATA static u8* sBg1TilemapBuffer = NULL;

// Function Declarations
static void Task_ShuffleTimeWaitFadeIn(u8 taskId);
static void Task_ShuffleTimeMain(u8 taskId);
static void ShuffleTimeShuffleHand(void);
static void ShuffleTimeUpdateHandOam(void);
static void ShuffleTimeReloadCard(u8 cardId, u8 gfxTag, u8 palTag);
static void ShuffleTimeReloadCards(void);
// Animations task functions
static void Task_ShuffleTimeIntroAnimation(u8 taskId);
static void Task_ShuffleTimeCardSelected(u8 taskId);

// Const Data
// gui image data
static const u32 sShuffleTimeGuiTiles[]   = INCBIN_U32("graphics/shuffle_time/gui_tiles.4bpp.lz");
static const u32 sShuffleTimeGuiTilemap[] = INCBIN_U32("graphics/shuffle_time/gui_tilemap.bin.lz");
static const u16 sShuffleTimeGuiPal[]     = INCBIN_U16("graphics/shuffle_time/gui.gbapal");
// Title and cards image data
static const u32 sShuffleTimeTitleSGfx[] = INCBIN_U32("graphics/shuffle_time/title/title_S.4bpp.lz");
static const u32 sShuffleTimeTitleHGfx[] = INCBIN_U32("graphics/shuffle_time/title/title_H.4bpp.lz");
static const u32 sShuffleTimeTitleUGfx[] = INCBIN_U32("graphics/shuffle_time/title/title_U.4bpp.lz");
static const u32 sShuffleTimeTitleFGfx[] = INCBIN_U32("graphics/shuffle_time/title/title_F.4bpp.lz");
static const u32 sShuffleTimeTitleLGfx[] = INCBIN_U32("graphics/shuffle_time/title/title_L.4bpp.lz");
static const u32 sShuffleTimeTitleEGfx[] = INCBIN_U32("graphics/shuffle_time/title/title_E.4bpp.lz");
static const u32 sShuffleTimeTitleTGfx[] = INCBIN_U32("graphics/shuffle_time/title/title_T.4bpp.lz");
static const u32 sShuffleTimeTitleIGfx[] = INCBIN_U32("graphics/shuffle_time/title/title_I.4bpp.lz");
static const u32 sShuffleTimeTitleMGfx[] = INCBIN_U32("graphics/shuffle_time/title/title_M.4bpp.lz");
static const u16 sShuffleTimeTitlePal[]  = INCBIN_U16("graphics/shuffle_time/title/title.gbapal");
static const u32 sShuffleTimeCardGfx[]   = INCBIN_U32("graphics/shuffle_time/back.4bpp.lz");
static const u16 sShuffleTimeCardPal[]   = INCBIN_U16("graphics/shuffle_time/back.gbapal");
static const u32 sShuffleTimeCursorGfx[] = INCBIN_U32("graphics/shuffle_time/cursor.4bpp.lz");
static const u16 sShuffleTimeCursorPal[] = INCBIN_U16("graphics/shuffle_time/cursor.gbapal");
// Card selected animation image data
static const u32 sShuffleTimeSelected00Gfx[] = INCBIN_U32("graphics/shuffle_time/selected/selected_00.4bpp.lz");
static const u32 sShuffleTimeSelected01Gfx[] = INCBIN_U32("graphics/shuffle_time/selected/selected_01.4bpp.lz");
static const u32 sShuffleTimeSelected02Gfx[] = INCBIN_U32("graphics/shuffle_time/selected/selected_02.4bpp.lz");
static const u32 sShuffleTimeSelected03Gfx[] = INCBIN_U32("graphics/shuffle_time/selected/selected_03.4bpp.lz");
static const u32 sShuffleTimeSelected04Gfx[] = INCBIN_U32("graphics/shuffle_time/selected/selected_04.4bpp.lz");
static const u32 sShuffleTimeSelected05Gfx[] = INCBIN_U32("graphics/shuffle_time/selected/selected_05.4bpp.lz");
static const u32 sShuffleTimeSelected06Gfx[] = INCBIN_U32("graphics/shuffle_time/selected/selected_06.4bpp.lz");
static const u32 sShuffleTimeSelected07Gfx[] = INCBIN_U32("graphics/shuffle_time/selected/selected_07.4bpp.lz");
static const u32 sShuffleTimeSelected08Gfx[] = INCBIN_U32("graphics/shuffle_time/selected/selected_08.4bpp.lz");
static const u32 sShuffleTimeSelected09Gfx[] = INCBIN_U32("graphics/shuffle_time/selected/selected_09.4bpp.lz");
static const u32 sShuffleTimeSelected10Gfx[] = INCBIN_U32("graphics/shuffle_time/selected/selected_10.4bpp.lz");
static const u32 sShuffleTimeSelected11Gfx[] = INCBIN_U32("graphics/shuffle_time/selected/selected_11.4bpp.lz");
static const u16 sShuffleTimeSelectedPal[]   = INCBIN_U16("graphics/shuffle_time/selected/selected.gbapal");

static const struct WindowTemplate sShuffleTimeWindowTemplates[] =
{
    [WINDOW_TEXT] =
    {
        .bg          = 0,
        .tilemapLeft = 1,
        .tilemapTop  = 16,
        .width       = 28,
        .height      = 3,
        .paletteNum  = 15,
        .baseBlock   = 1,
    },
    DUMMY_WIN_TEMPLATE,
};

static const u8 sText_SelectCard[] = _("Select a card");
static const u8 sFontColor[3] =
{
    TEXT_COLOR_WHITE, TEXT_COLOR_DARK_GRAY, TEXT_COLOR_LIGHT_GRAY
};

#include "data/text/shuffle_time_card_descriptions.h"

static const struct BgTemplate sShuffleTimeBgTemplates[] =
{
    {
        .bg            = 0,
        .charBaseIndex = 0,
        .mapBaseIndex  = 15,
        .priority      = 0,
    },
    {
        .bg            = 1,
        .charBaseIndex = 3,
        .mapBaseIndex  = 23,
        .priority      = 2,
    },
};

static const struct CompressedSpriteSheet sShuffleTimeSpriteSheets[] =
{
    { .data = sShuffleTimeTitleSGfx,     .size = 0x200, .tag = TAG_GFX_TITLE_S  },
    { .data = sShuffleTimeTitleHGfx,     .size = 0x200, .tag = TAG_GFX_TITLE_H  },
    { .data = sShuffleTimeTitleUGfx,     .size = 0x200, .tag = TAG_GFX_TITLE_U  },
    { .data = sShuffleTimeTitleFGfx,     .size = 0x200, .tag = TAG_GFX_TITLE_F  },
    { .data = sShuffleTimeTitleLGfx,     .size = 0x200, .tag = TAG_GFX_TITLE_L  },
    { .data = sShuffleTimeTitleEGfx,     .size = 0x200, .tag = TAG_GFX_TITLE_E  },
    { .data = sShuffleTimeTitleTGfx,     .size = 0x200, .tag = TAG_GFX_TITLE_T  },
    { .data = sShuffleTimeTitleIGfx,     .size = 0x200, .tag = TAG_GFX_TITLE_I  },
    { .data = sShuffleTimeTitleMGfx,     .size = 0x200, .tag = TAG_GFX_TITLE_M  },
    { .data = sShuffleTimeCursorGfx,     .size = 0x800, .tag = TAG_GFX_CURSOR   },
    { .data = sShuffleTimeSelected00Gfx, .size = 0x800, .tag = TAG_GFX_SELECTED },
    { .data = sShuffleTimeCardGfx,       .size = 0x800, .tag = TAG_GFX_CARD     },
    {},
};

static const struct SpritePalette sShuffleTimeSpritePalettes[] =
{
    { .data = sShuffleTimeTitlePal,    .tag = TAG_PAL_TITLE    },
    { .data = sShuffleTimeCursorPal,   .tag = TAG_PAL_CURSOR   },
    { .data = sShuffleTimeSelectedPal, .tag = TAG_PAL_SELECTED },
    { .data = sShuffleTimeCardPal,     .tag = TAG_PAL_CARD     },
    {},
};

static const struct SpriteTemplate sShuffleTimeTitleSpriteTemplates[] =
{
    {
        .tileTag     = TAG_GFX_TITLE_S,
        .paletteTag  = TAG_PAL_TITLE,
        .oam         = &gOamData_AffineOff_ObjNormal_32x32,
        .anims       = gDummySpriteAnimTable,
        .images      = NULL,
        .affineAnims = NULL,
        .callback    = SpriteCallbackDummy,
    },
    {
        .tileTag     = TAG_GFX_TITLE_H,
        .paletteTag  = TAG_PAL_TITLE,
        .oam         = &gOamData_AffineOff_ObjNormal_32x32,
        .anims       = gDummySpriteAnimTable,
        .images      = NULL,
        .affineAnims = NULL,
        .callback    = SpriteCallbackDummy,
    },
    {
        .tileTag     = TAG_GFX_TITLE_U,
        .paletteTag  = TAG_PAL_TITLE,
        .oam         = &gOamData_AffineOff_ObjNormal_32x32,
        .anims       = gDummySpriteAnimTable,
        .images      = NULL,
        .affineAnims = NULL,
        .callback    = SpriteCallbackDummy,
    },
    {
        .tileTag     = TAG_GFX_TITLE_F,
        .paletteTag  = TAG_PAL_TITLE,
        .oam         = &gOamData_AffineOff_ObjNormal_32x32,
        .anims       = gDummySpriteAnimTable,
        .images      = NULL,
        .affineAnims = NULL,
        .callback    = SpriteCallbackDummy,
    },
    {
        .tileTag     = TAG_GFX_TITLE_F,
        .paletteTag  = TAG_PAL_TITLE,
        .oam         = &gOamData_AffineOff_ObjNormal_32x32,
        .anims       = gDummySpriteAnimTable,
        .images      = NULL,
        .affineAnims = NULL,
        .callback    = SpriteCallbackDummy,
    },
    {
        .tileTag     = TAG_GFX_TITLE_L,
        .paletteTag  = TAG_PAL_TITLE,
        .oam         = &gOamData_AffineOff_ObjNormal_32x32,
        .anims       = gDummySpriteAnimTable,
        .images      = NULL,
        .affineAnims = NULL,
        .callback    = SpriteCallbackDummy,
    },
    {
        .tileTag     = TAG_GFX_TITLE_E,
        .paletteTag  = TAG_PAL_TITLE,
        .oam         = &gOamData_AffineOff_ObjNormal_32x32,
        .anims       = gDummySpriteAnimTable,
        .images      = NULL,
        .affineAnims = NULL,
        .callback    = SpriteCallbackDummy,
    },
    {
        .tileTag     = TAG_GFX_TITLE_T,
        .paletteTag  = TAG_PAL_TITLE,
        .oam         = &gOamData_AffineOff_ObjNormal_32x32,
        .anims       = gDummySpriteAnimTable,
        .images      = NULL,
        .affineAnims = NULL,
        .callback    = SpriteCallbackDummy,
    },
    {
        .tileTag     = TAG_GFX_TITLE_I,
        .paletteTag  = TAG_PAL_TITLE,
        .oam         = &gOamData_AffineOff_ObjNormal_32x32,
        .anims       = gDummySpriteAnimTable,
        .images      = NULL,
        .affineAnims = NULL,
        .callback    = SpriteCallbackDummy,
    },
    {
        .tileTag     = TAG_GFX_TITLE_M,
        .paletteTag  = TAG_PAL_TITLE,
        .oam         = &gOamData_AffineOff_ObjNormal_32x32,
        .anims       = gDummySpriteAnimTable,
        .images      = NULL,
        .affineAnims = NULL,
        .callback    = SpriteCallbackDummy,
    },
    {
        .tileTag     = TAG_GFX_TITLE_E,
        .paletteTag  = TAG_PAL_TITLE,
        .oam         = &gOamData_AffineOff_ObjNormal_32x32,
        .anims       = gDummySpriteAnimTable,
        .images      = NULL,
        .affineAnims = NULL,
        .callback    = SpriteCallbackDummy,
    },
    {
        .tileTag     = TAG_GFX_CURSOR,
        .paletteTag  = TAG_PAL_CURSOR,
        .oam         = &gOamData_AffineOff_ObjNormal_64x64,
        .anims       = gDummySpriteAnimTable,
        .images      = NULL,
        .affineAnims = NULL,
        .callback    = SpriteCallbackDummy,
    },
    {
        .tileTag     = TAG_GFX_SELECTED,
        .paletteTag  = TAG_PAL_SELECTED,
        .oam         = &gOamData_AffineOff_ObjNormal_64x64,
        .anims       = gDummySpriteAnimTable,
        .images      = NULL,
        .affineAnims = NULL,
        .callback    = SpriteCallbackDummy,
    },
    {
        .tileTag     = TAG_GFX_CARD, // Updated dinamically
        .paletteTag  = TAG_PAL_CARD, // Updated dinamically
        .oam         = &gOamData_AffineNormal_ObjNormal_64x64,
        .anims       = gDummySpriteAnimTable,
        .images      = NULL,
        .affineAnims = gDummySpriteAffineAnimTable,
        .callback    = SpriteCallbackDummy,
    },
    {
        .tileTag     = TAG_GFX_CARD, // Updated dinamically
        .paletteTag  = TAG_PAL_CARD, // Updated dinamically
        .oam         = &gOamData_AffineNormal_ObjNormal_64x64,
        .anims       = gDummySpriteAnimTable,
        .images      = NULL,
        .affineAnims = gDummySpriteAffineAnimTable,
        .callback    = SpriteCallbackDummy,
    },
    {
        .tileTag     = TAG_GFX_CARD, // Updated dinamically
        .paletteTag  = TAG_PAL_CARD, // Updated dinamically
        .oam         = &gOamData_AffineNormal_ObjNormal_64x64,
        .anims       = gDummySpriteAnimTable,
        .images      = NULL,
        .affineAnims = gDummySpriteAffineAnimTable,
        .callback    = SpriteCallbackDummy,
    },
    {
        .tileTag     = TAG_GFX_CARD, // Updated dinamically
        .paletteTag  = TAG_PAL_CARD, // Updated dinamically
        .oam         = &gOamData_AffineNormal_ObjNormal_64x64,
        .anims       = gDummySpriteAnimTable,
        .images      = NULL,
        .affineAnims = gDummySpriteAffineAnimTable,
        .callback    = SpriteCallbackDummy,
    },
    {
        .tileTag     = TAG_GFX_CARD, // Updated dinamically
        .paletteTag  = TAG_PAL_CARD, // Updated dinamically
        .oam         = &gOamData_AffineNormal_ObjNormal_64x64,
        .anims       = gDummySpriteAnimTable,
        .images      = NULL,
        .affineAnims = gDummySpriteAffineAnimTable,
        .callback    = SpriteCallbackDummy,
    },
};

static const u32 *sShuffleTimeSelectedGfxs[SELECTED_FRAMES] =
{
    sShuffleTimeSelected00Gfx,
    sShuffleTimeSelected01Gfx,
    sShuffleTimeSelected02Gfx,
    sShuffleTimeSelected03Gfx,
    sShuffleTimeSelected04Gfx,
    sShuffleTimeSelected05Gfx,
    sShuffleTimeSelected06Gfx,
    sShuffleTimeSelected07Gfx,
    sShuffleTimeSelected08Gfx,
    sShuffleTimeSelected09Gfx,
    sShuffleTimeSelected10Gfx,
    sShuffleTimeSelected11Gfx,
};

static const s16 sFlipAnimTable[CARD_FLIP_FRAMES] =
{
    0x100, 0x0C0, 0x080, 0x040, 0x010, 0x005,
    0x005, 0x010, 0x040, 0x080, 0x0C0, 0x100,
};

// All sprites inital positions
static const s16 sSpriteInitialPos[SPRITE_COUNT][2] =
{
    // Title "shuffle"
    { SHUFFLE_INITIAL_POS_X + 0 * 32, SHUFFLE_INITIAL_POS_Y }, // s
    { SHUFFLE_INITIAL_POS_X + 1 * 32, SHUFFLE_INITIAL_POS_Y }, // h
    { SHUFFLE_INITIAL_POS_X + 2 * 32, SHUFFLE_INITIAL_POS_Y }, // u
    { SHUFFLE_INITIAL_POS_X + 3 * 32, SHUFFLE_INITIAL_POS_Y }, // f
    { SHUFFLE_INITIAL_POS_X + 4 * 32, SHUFFLE_INITIAL_POS_Y }, // f
    { SHUFFLE_INITIAL_POS_X + 5 * 32, SHUFFLE_INITIAL_POS_Y }, // l
    { SHUFFLE_INITIAL_POS_X + 6 * 32, SHUFFLE_INITIAL_POS_Y }, // e
    // Title "time"
    { TIME_INITIAL_POS_X + 0 * 32, TIME_INITIAL_POS_Y }, // t
    { TIME_INITIAL_POS_X + 1 * 32, TIME_INITIAL_POS_Y }, // i
    { TIME_INITIAL_POS_X + 2 * 32, TIME_INITIAL_POS_Y }, // m
    { TIME_INITIAL_POS_X + 3 * 32, TIME_INITIAL_POS_Y }, // e
    // Cursor
    { CURSOR_INITIAL_POS_X, CURSOR_INITIAL_POS_Y },
    { CURSOR_INITIAL_POS_X, CURSOR_INITIAL_POS_Y },
    // Cards
    { CARD_INITIAL_POS_X, CARD_INITIAL_POS_Y }, // c1
    { CARD_INITIAL_POS_X, CARD_INITIAL_POS_Y }, // c2
    { CARD_INITIAL_POS_X, CARD_INITIAL_POS_Y }, // c3
    { CARD_INITIAL_POS_X, CARD_INITIAL_POS_Y }, // c4
    { CARD_INITIAL_POS_X, CARD_INITIAL_POS_Y }, // c5
};

// Card's target position on screen (px)
static const s16 sCardTargetX[SPRITE_CARD_COUNT] =
{
    CARD_POS_X(0), CARD_POS_X(1), CARD_POS_X(2), CARD_POS_X(3), CARD_POS_X(4),
};

// Functions
static void ShuffleTime_VBlankCB(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
}

static void ShuffleTime_MainCB(void)
{
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    DoScheduledBgTilemapCopiesToVram();
    UpdatePaletteFade();
}

static bool8 ShuffleTime_InitBgs(void)
{
    ResetVramOamAndBgCntRegs();
    ResetAllBgsCoordinates();
    sBg1TilemapBuffer = AllocZeroed(BG_SCREEN_SIZE);
    if (sBg1TilemapBuffer == NULL)
        return FALSE;

    ResetBgsAndClearDma3BusyFlags(0);
    InitBgsFromTemplates(0, sShuffleTimeBgTemplates, NELEMS(sShuffleTimeBgTemplates));
    SetBgTilemapBuffer(1, sBg1TilemapBuffer);
    ScheduleBgCopyTilemapToVram(1);
    SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_OBJ_1D_MAP | DISPCNT_OBJ_ON);
    SetGpuReg(REG_OFFSET_BLDCNT, 0);
    ShowBg(0);
    ShowBg(1);

    return TRUE;
}

static bool8 ShuffleTime_LoadGraphics(void)
{
    switch (sShuffleTimeGuiDataPtr->state)
    {
    case 0:
        ResetTempTileDataBuffers();
        DecompressAndCopyTileDataToVram(1, sShuffleTimeGuiTiles, 0, 0, 0);
        sShuffleTimeGuiDataPtr->state++;
        break;
    case 1:
        if (FreeTempTileDataBuffersIfPossible() != TRUE)
        {
            LZDecompressWram(sShuffleTimeGuiTilemap, sBg1TilemapBuffer);
            sShuffleTimeGuiDataPtr->state++;
        }
        break;
    case 2:
        LoadPalette(sShuffleTimeGuiPal, PLTT_ID(0), PLTT_SIZEOF(16));
        sShuffleTimeGuiDataPtr->state++;
        break;
    case 3:
        for(u8 i = 0; i < TAG_GFX_COUNT; i++)
            LoadCompressedSpriteSheet(&sShuffleTimeSpriteSheets[i]);
        sShuffleTimeGuiDataPtr->state++;
        break;
    case 4:
        LoadSpritePalettes(sShuffleTimeSpritePalettes);
        sShuffleTimeGuiDataPtr->state++;
        break;
    default:
        sShuffleTimeGuiDataPtr->state = 0;
        return TRUE;
    }

    return FALSE;
}

static void ShuffleTime_InitWindows(void)
{
    InitWindows(sShuffleTimeWindowTemplates);
    DeactivateAllTextPrinters();
    LoadMessageBoxAndBorderGfx();
}

static void ShuffleTimeGuiFreeResources(void)
{
    Free(sShuffleTimeGuiDataPtr);
    Free(sBg1TilemapBuffer);

    FreeAllWindowBuffers();
}

static void Task_ShuffleTimeFadeAndExit(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        // Keep the palette faded to prevent color flash
        BlendPalettes(PALETTES_ALL, 16, RGB_BLACK);
        SetMainCallback2(sShuffleTimeGuiDataPtr->savedCallback);
        ShuffleTimeGuiFreeResources();
        DestroyTask(taskId);
    }
}

static void ShuffleTimePrintMessage(const u8 *src)
{
    FillWindowPixelBuffer(WINDOW_TEXT, PIXEL_FILL(TEXT_COLOR_WHITE));

    AddTextPrinterParameterized4(WINDOW_TEXT, FONT_SMALL, 0, 0, 0, 0, sFontColor, TEXT_SKIP_DRAW, src);

    PutWindowTilemap(WINDOW_TEXT);
    CopyWindowToVram(WINDOW_TEXT, COPYWIN_FULL);
}

static void ShuffleTimePrintSelectCard(void)
{
    ShuffleTimePrintMessage(sText_SelectCard);
}

static void ShuffleTimePrintDescription(void)
{
    const u8 cursor = sShuffleTimeGuiDataPtr->cursor;
    const u8 cardId = sShuffleTimeGuiDataPtr->handIds[cursor];
    if (!sShuffleTimeGuiDataPtr->isDrawed[cursor])
        ShuffleTimePrintMessage(sCardDescriptions[cardId]);
    else
        ShuffleTimePrintSelectCard();
}

static void ShuffleTimeFadeAndExit(void)
{
    BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_BLACK);
    CreateTask(Task_ShuffleTimeFadeAndExit, 0);
    SetVBlankCallback(ShuffleTime_VBlankCB);
    SetMainCallback2(ShuffleTime_MainCB);
}

static bool8 ShuffleTime_InitSprites(void)
{
    u32 *spriteIds = sShuffleTimeGuiDataPtr->spriteIds;
    for (u8 i = 0; i < SPRITE_COUNT; i++)
    {
        const s16 x = sSpriteInitialPos[i][0];
        const s16 y = sSpriteInitialPos[i][1];
        const struct SpriteTemplate *template = &sShuffleTimeTitleSpriteTemplates[i];
        spriteIds[i] = CreateSprite(template, x, y, 0);
    }
    gSprites[spriteIds[SPRITE_CURSOR_IDX_START]].invisible = TRUE;
    gSprites[spriteIds[SPRITE_CURSOR_IDX_START + 1]].invisible = TRUE;
    return TRUE;
}

static bool8 ShuffleTime_DoGfxSetup(void)
{
    switch (gMain.state)
    {
    case 0:
        SetVBlankHBlankCallbacksToNull();
        ClearScheduledBgCopiesToVram();
        gMain.state++;
        break;
    case 1:
        ScanlineEffect_Stop();
        gMain.state++;
        break;
    case 2:
        FreeAllSpritePalettes();
        gMain.state++;
        break;
    case 3:
        ResetPaletteFade();
        ResetSpriteData();
        ResetTasks();
        gMain.state++;
        break;
    case 4:
        if (ShuffleTime_InitBgs())
        {
            sShuffleTimeGuiDataPtr->state = 0;
            gMain.state++;
        }
        else
        {
            ShuffleTimeFadeAndExit();
            return TRUE;
        }
        break;
    case 5:
        if (ShuffleTime_LoadGraphics() == TRUE)
            gMain.state++;
        break;
    case 6:
        ShuffleTime_InitWindows();
        gMain.state++;
        break;
    case 7:
        ShuffleTime_InitSprites();
        gMain.state++;
        break;
    case 8:
        CreateTask(Task_ShuffleTimeWaitFadeIn, 0);
        gMain.state++;
        break;
    case 9:
        CreateTask(Task_ShuffleTimeIntroAnimation, 0);
        gMain.state++;
        break;
    case 10:
        BlendPalettes(PALETTES_ALL, 16, RGB_BLACK);
        gMain.state++;
        break;
    case 11:
        BeginNormalPaletteFade(PALETTES_ALL, 0, 16, 0, RGB_BLACK);
        gMain.state++;
        break;
    case 12:
        DrawStdWindowFrame(WINDOW_TEXT, TRUE);
        ShuffleTimePrintSelectCard();
        gMain.state++;
        break;
    default:
        SetVBlankCallback(ShuffleTime_VBlankCB);
        SetMainCallback2(ShuffleTime_MainCB);
        return TRUE;
    }

    return FALSE;
}

static void ShuffleTime_RunSetup(void)
{
    while(!ShuffleTime_DoGfxSetup()) {}
}

static void ShuffleTimeGuiInit(MainCallback callback)
{
    if ((sShuffleTimeGuiDataPtr = AllocZeroed(sizeof(struct ShuffleTimeGUI))) == NULL)
    {
        SetMainCallback2(callback);
        return;
    }
    sShuffleTimeGuiDataPtr->savedCallback = callback;
    sShuffleTimeGuiDataPtr->cursor = 2;
    sShuffleTimeGuiDataPtr->inputLocked = TRUE;
    sShuffleTimeGuiDataPtr->maxDraws = 1;
    ShuffleTimeShuffleHand();

    SetMainCallback2(ShuffleTime_RunSetup);
}

void Task_OpenShuffleTime(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        CleanupOverworldWindowsAndTilemaps();
        ShuffleTimeGuiInit(CB2_ReturnToField);
        DestroyTask(taskId);
    }
}

static void Task_ShuffleTimeWaitFadeIn(u8 taskId)
{
    if (!gPaletteFade.active)
        gTasks[taskId].func = Task_ShuffleTimeMain;
}

// -1 = move to the left, +1 = move to the right
static void ShuffleTimeHandleCursor(s8 delta)
{
    u8 *cursor = &sShuffleTimeGuiDataPtr->cursor;
    const u32 spriteId = sShuffleTimeGuiDataPtr->spriteIds[SPRITE_CURSOR_IDX_START];
    *cursor = (*cursor + SPRITE_CARD_COUNT + delta) % SPRITE_CARD_COUNT;
    gSprites[spriteId].x = CARD_POS_X(*cursor);
}

static void Task_ShuffleTimeMain(u8 taskId)
{
    if (gPaletteFade.active || sShuffleTimeGuiDataPtr->inputLocked)
      return;

    if (DEBUG_SHUFFLE_TIME && JOY_NEW(B_BUTTON))
    {
        PlaySE(SE_SELECT);
        BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_BLACK);
        gTasks[taskId].func = Task_ShuffleTimeFadeAndExit;
    }
    else if (DEBUG_SHUFFLE_TIME && JOY_NEW(R_BUTTON))
    {
        PlaySE(SE_SELECT);

        ShuffleTimeShuffleHand();
        ShuffleTimeReloadCards();
        ShuffleTimeUpdateHandOam();
        ShuffleTimePrintDescription();
    }
    else if (JOY_NEW(A_BUTTON))
    {
        const u8 cursor = sShuffleTimeGuiDataPtr->cursor;
        if(sShuffleTimeGuiDataPtr->isDrawed[cursor])
            PlaySE(SE_BOO);
        else
        {
            PlaySE(SE_SELECT);
            sShuffleTimeGuiDataPtr->drawCount++;
            sShuffleTimeGuiDataPtr->isDrawed[cursor] = TRUE;
            // Set initial state
            gTasks[taskId].data[0] = SELECT_STATE_CURSOR_HIDE;
            gTasks[taskId].func = Task_ShuffleTimeCardSelected;
        }
    }
    else if (JOY_NEW(DPAD_RIGHT))
    {
        PlaySE(SE_SELECT);
        ShuffleTimeHandleCursor(+1);
        ShuffleTimePrintDescription();
    }
    else if (JOY_NEW(DPAD_LEFT))
    {
        PlaySE(SE_SELECT);
        ShuffleTimeHandleCursor(-1);
        ShuffleTimePrintDescription();
    }
}

////// Animation functions

//// Intro animation funcitions

static bool8 TitleSlideStep(u8 step)
{
    if (step >= TITLE_STEPS)
        return TRUE;

    for (u8 i = 0; i < SPRITE_TITLE_COUNT; i++)
    {
        const u32 spriteId = sShuffleTimeGuiDataPtr->spriteIds[i];
        gSprites[spriteId].y += TITLE_SLIDE_SPEED;
    }

    return FALSE;
}

static bool8 CardsSlideStep(void)
{
    bool8 allDone = TRUE;

    for (u8 i = 0; i < SPRITE_CARD_COUNT; i++)
    {
        const u32 *spriteIds  = sShuffleTimeGuiDataPtr->spriteIds;
        const u32 spriteId    = spriteIds[i + SPRITE_CARD_IDX_START];
        struct Sprite *sprite = &gSprites[spriteId];
        const s16 target      = sCardTargetX[i];
        if (sprite->x < target)
        {
            sprite->x += CARD_SLIDE_SPEED;
            if (sprite->x > target)
                sprite->x = target;
            allDone = FALSE;
        }
    }

    return allDone;
}

static bool8 CardsFlipStep(u8 step)
{
    if (step >= CARD_FLIP_FRAMES * CARD_FLIP_DURATION)
        return TRUE;

    if (step == 0)
        ShuffleTimeReloadCards();

    // Only update OAM matrix on new frames
    const u32 *spriteIds = sShuffleTimeGuiDataPtr->spriteIds;
    if (step % CARD_FLIP_DURATION == 0)
    {
        for(int i = 0; i < SPRITE_CARD_COUNT; i++)
        {
            const u8 frame        = step / CARD_FLIP_DURATION;
            const u32 spriteId    = spriteIds[SPRITE_CARD_IDX_START + i];
            struct Sprite *sprite = &gSprites[spriteId];
            SetOamMatrixRotationScaling(sprite->oam.matrixNum, sFlipAnimTable[frame], 0x0100, 0x0000);
        }
    }

    // Update OAM data (palette and sprite sheet) mid-animation
    if (step == CARD_FLIP_HALF * CARD_FLIP_DURATION)
        ShuffleTimeUpdateHandOam();

    return FALSE;
}

#define tState data[0]
#define tFrame data[1]

static void Task_ShuffleTimeIntroAnimation(u8 taskId)
{
    if (gPaletteFade.active)
        return;

    switch(gTasks[taskId].tState)
    {
    case INTRO_STATE_TITLE_SLIDE:
        if (TitleSlideStep(gTasks[taskId].tFrame++))
        {
            gTasks[taskId].tFrame = 0;
            gTasks[taskId].tState++;
        }
        break;
    case INTRO_STATE_CARDS_SLIDE:
        if (CardsSlideStep())
            gTasks[taskId].tState++;
        break;
    case INTRO_STATE_CARDS_FLIP:
        if (CardsFlipStep(gTasks[taskId].tFrame++))
            gTasks[taskId].tState++;
        break;
    case INTRO_STATE_CURSOR_SHOW:
        const u32 *spriteIds = sShuffleTimeGuiDataPtr->spriteIds;
        const u32 cursorId = spriteIds[SPRITE_CURSOR_IDX_START];
        gSprites[cursorId].invisible = FALSE;
        ShuffleTimePrintDescription();
        gTasks[taskId].tState++;
        break;
    case INTRO_STATE_DONE:
        sShuffleTimeGuiDataPtr->inputLocked = FALSE;
        DestroyTask(taskId);
        break;
    }
}

//// Card selection functions

static bool8 CardShineStep(u8 step)
{
    if (step >= SELECTED_FRAMES * SELECTED_DURATION)
        return TRUE;

    if (step == 0)
        PlaySE(SE_SHINY);

    // Only update sprite sheet on new frames
    if (step % SELECTED_DURATION == 0)
    {
        const u8 frame = step / SELECTED_DURATION;
        struct CompressedSpriteSheet spriteSheet =
        {
            .data = sShuffleTimeSelectedGfxs[frame],
            .size = 0x800,
            .tag  = TAG_GFX_SELECTED,
        };
        FreeSpriteTilesByTag(TAG_GFX_SELECTED);
        LoadCompressedSpriteSheet(&spriteSheet);
    }

    return FALSE;
}

static bool8 CardOutStep(void)
{
    const s16 target = 300;

    const u8 cursor = sShuffleTimeGuiDataPtr->cursor;
    const u32 *spriteIds = sShuffleTimeGuiDataPtr->spriteIds;
    const u32 spriteId = spriteIds[cursor + SPRITE_CARD_IDX_START];
    struct Sprite *sprite = &gSprites[spriteId];

    if (sprite->x >= target)
        return TRUE;

    sprite->x += CARD_SLIDE_SPEED;
    if (sprite->x > target)
        sprite->x = target;

    return FALSE;
}

static bool8 CardMorphStep(u8 step)
{
    if (step >= 2 * CARD_FLIP_FRAMES * CARD_FLIP_DURATION)
        return TRUE;

    const u32 *spriteIds = sShuffleTimeGuiDataPtr->spriteIds;
    const u8 cursor       = sShuffleTimeGuiDataPtr->cursor;
    const u32 spriteId    = spriteIds[cursor + SPRITE_CARD_IDX_START];
    struct Sprite *sprite = &gSprites[spriteId];

    // Double flip animation
    if (step % CARD_FLIP_DURATION == 0)
    {
        const u8 frame        = (step / CARD_FLIP_DURATION) % CARD_FLIP_FRAMES;
        SetOamMatrixRotationScaling(sprite->oam.matrixNum, sFlipAnimTable[frame], 0x0100, 0x0000);
    }

    // Update OAM data mid-animation in the first flip
    if (step == CARD_FLIP_HALF * CARD_FLIP_DURATION)
    {
        sprite->oam.tileNum    = GetSpriteTileStartByTag(CARD_GFX_TAG(-1));
        sprite->oam.paletteNum = IndexOfSpritePaletteTag(CARD_PAL_TAG(-1));
    }

    // Update and Reload SpriteSheet and SpritePalete
    if (step == CARD_FLIP_FRAMES * CARD_FLIP_DURATION)
    {
        const u8 cardId = sShuffleTimeGuiDataPtr->morphCardId;
        ShuffleTimeReloadCard(cardId, CARD_GFX_TAG(cursor), CARD_PAL_TAG(cursor));
    }

    // Update Oam data mid-animation in the second flip
    if (step == CARD_FLIP_HALF * CARD_FLIP_DURATION + CARD_FLIP_FRAMES * CARD_FLIP_DURATION)
    {
        sprite->oam.tileNum    = GetSpriteTileStartByTag(CARD_GFX_TAG(cursor));
        sprite->oam.paletteNum = IndexOfSpritePaletteTag(CARD_PAL_TAG(cursor));
    }

    return FALSE;
}

static void Task_ShuffleTimeCardSelected(u8 taskId)
{
    if (gPaletteFade.active)
        return;

    const u32 *spriteIds = sShuffleTimeGuiDataPtr->spriteIds;
    const u32 cursorId   = spriteIds[SPRITE_CURSOR_IDX_START];
    const u32 selectedId = spriteIds[SPRITE_CURSOR_IDX_START + 1];
    const u8 cursor      = sShuffleTimeGuiDataPtr->cursor;

    switch(gTasks[taskId].tState)
    {
    case SELECT_STATE_CURSOR_HIDE:
        gSprites[cursorId].invisible = TRUE;
        gTasks[taskId].tState++;
        break;
    case SELECT_STATE_SELECTED_SHOW:
        gSprites[selectedId].x         = CARD_POS_X(sShuffleTimeGuiDataPtr->cursor);
        gSprites[selectedId].invisible = FALSE;
        gTasks[taskId].tState++;
        break;
    case SELECT_STATE_CARD_SHINE:
        if (CardShineStep(gTasks[taskId].tFrame++))
        {
            gTasks[taskId].tFrame = 0;
            gTasks[taskId].tState++;
        }
        break;
    case SELECT_STATE_SELECTED_HIDE:
        gSprites[selectedId].invisible = TRUE;
        gTasks[taskId].tState++;
        break;
    case SELECT_STATE_APPLY_EFFECT:
        const u8 cardId = sShuffleTimeGuiDataPtr->handIds[cursor];
        ApplyCardEffect(cardId);
        if (sShuffleTimeGuiDataPtr->morphCardId == CARD_BACK)
            gTasks[taskId].tState = SELECT_STATE_WAIT_INPUT;
        else
            gTasks[taskId].tState = SELECT_STATE_MORPH_FLIP;
        break;
    case SELECT_STATE_MORPH_FLIP:
        if (CardMorphStep(gTasks[taskId].tFrame++))
        {
            gTasks[taskId].tFrame = 0;
            gTasks[taskId].tState++;
        }
        break;
    case SELECT_STATE_MORPH_DATA:
        sShuffleTimeGuiDataPtr->handIds[cursor] = sShuffleTimeGuiDataPtr->morphCardId;
        sShuffleTimeGuiDataPtr->morphCardId = CARD_BACK;
        gTasks[taskId].tState = SELECT_STATE_SELECTED_SHOW;
        break;
    case SELECT_STATE_WAIT_INPUT:
        if (!IsShuffleTimeInputLocked() && JOY_NEW(A_BUTTON | B_BUTTON))
            gTasks[taskId].tState++;
        break;
    case SELECT_STATE_CARD_OUT:
        if (CardOutStep())
            gTasks[taskId].tState++;
        break;
    case SELECT_STATE_DONE:
        if (sShuffleTimeGuiDataPtr->drawCount < sShuffleTimeGuiDataPtr->maxDraws)
        {
            ShuffleTimePrintSelectCard();
            gSprites[cursorId].invisible = FALSE;
        }
        else
            ShuffleTimeFadeAndExit();
        gTasks[taskId].func = Task_ShuffleTimeMain;
        break;
    }
}

#undef tState
#undef tFrame

//// Helper Functions

static void ShuffleTimeShuffleHand(void)
{
    // CARD_BACK is excluded from the deck shuffle
    const u8 deck_start = CARD_CHARIOT;
    const u8 deck_count = CARD_COUNT - deck_start;

    u8 deck[deck_count];
    for(u8 i = 0; i < deck_count; i++)
        deck[i] = deck_start + i;

    Shuffle8(deck, deck_count);

    for(u8 i = 0; i < SPRITE_CARD_COUNT; i++)
        sShuffleTimeGuiDataPtr->handIds[i] = deck[i];
}

static void ShuffleTimeReloadCard(u8 cardId, u8 gfxTag, u8 palTag)
{
    struct CompressedSpriteSheet cardSpriteSheet =
    {
        .data = gCardGfxs[cardId],
        .size = 0x800,
        .tag  = gfxTag,
    };
    FreeSpriteTilesByTag(cardSpriteSheet.tag);
    LoadCompressedSpriteSheet(&cardSpriteSheet);

    struct SpritePalette cardSpritePalette =
    {
        .data = gCardPals[cardId],
        .tag  = palTag,
    };
    FreeSpritePaletteByTag(cardSpritePalette.tag);
    LoadSpritePalette(&cardSpritePalette);
}

static void ShuffleTimeReloadCards()
{
    for(u8 i = 0; i < SPRITE_CARD_COUNT; i++)
    {
        const u8 cardId = sShuffleTimeGuiDataPtr->handIds[i];
        ShuffleTimeReloadCard(cardId, CARD_GFX_TAG(i), CARD_PAL_TAG(i));
    }
}

static void ShuffleTimeUpdateHandOam(void)
{
    for (u8 i = 0; i < SPRITE_CARD_COUNT; i++)
    {
        const u32 spriteId     = sShuffleTimeGuiDataPtr->spriteIds[SPRITE_CARD_IDX_START + i];
        struct Sprite *sprite  = &gSprites[spriteId];
        sprite->oam.tileNum    = GetSpriteTileStartByTag(CARD_GFX_TAG(i));
        sprite->oam.paletteNum = IndexOfSpritePaletteTag(CARD_PAL_TAG(i));
    }
}

//// Interface functions for card effects

void ShuffleTimeExtraDraws(u8 count)
{
    u8 *maxDraws = &sShuffleTimeGuiDataPtr->maxDraws;
    *maxDraws += count;
    if (*maxDraws > MAX_DRAWS)
        *maxDraws = MAX_DRAWS;
}

void ShuffleTimeDisplayMessage(const u8 *src)
{
    ShuffleTimePrintMessage(src);
}

void ShuffleTimeMorphCard(u8 targetCardId)
{
    sShuffleTimeGuiDataPtr->morphCardId = targetCardId;
    // Force morph if is called after the apply effect function
    const u8 taskId = FindTaskIdByFunc(Task_ShuffleTimeCardSelected);
    gTasks[taskId].data[0] = SELECT_STATE_MORPH_FLIP;
}

void ShuffleTimeInputLock(void)
{
    sShuffleTimeGuiDataPtr->inputLocked = TRUE;
}

void ShuffleTimeInputUnlock(void)
{
    sShuffleTimeGuiDataPtr->inputLocked = FALSE;
}

bool8 IsShuffleTimeInputLocked(void)
{
    return sShuffleTimeGuiDataPtr->inputLocked;
}
