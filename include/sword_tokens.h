#ifndef GUARD_SWORD_TOKEN_H
#define GUARD_SWORD_TOKEN_H

#define MAX_SWORD_TOKENS 999999
#define MAX_SWORD_TOKENS_DIGITS 6

u32 GetSwordTokens(void);
void SetSwordTokens(u32 amount);
bool8 AddSwordTokens(u32 toAdd);
bool8 RemoveSwordTokens(u32 toSub);
u32 GetSwordTokensHelper(void);

#endif
