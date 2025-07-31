#include "global.h"
#include "sword_tokens.h"

u32 GetSwordTokens(void)
{
    return gSaveBlock1Ptr->swordTokens ^ gSaveBlock2Ptr->encryptionKey;
}

void SetSwordTokens(u32 amount)
{
    gSaveBlock1Ptr->swordTokens = amount ^ gSaveBlock2Ptr->encryptionKey;
}

bool8 AddSwordTokens(u32 toAdd)
{
    u32 curAmount = GetSwordTokens();
    if (curAmount >= MAX_SWORD_TOKENS)
        return FALSE;

    u32 newAmount = curAmount + toAdd;
    if (newAmount > MAX_SWORD_TOKENS || newAmount < curAmount)
        newAmount = MAX_SWORD_TOKENS;

    SetSwordTokens(newAmount);
    return TRUE;
}

bool8 RemoveSwordTokens(u32 toSub)
{
    u32 curAmount = GetSwordTokens();

    if (curAmount < toSub)
        return FALSE;

    SetSwordTokens(curAmount - toSub);
    return TRUE;
}
