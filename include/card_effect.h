#ifndef GUARD_CARD_EFFECT_H
#define GUARD_CARD_EFFECT_H

#include "pokemon.h"

void ApplyCardEffect(u8 cardId);
void ApplyCardEffects_OnWildEncounter(struct Pokemon *mon);
void ApplyCardEffects_OnMoneyReward(u32 *money);

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

#endif
