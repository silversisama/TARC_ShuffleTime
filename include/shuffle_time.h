#ifndef GUARD_SHUFFLE_TIME_H
#define GUARD_SHUFFLE_TIME_H

#include "main.h"

#define SHUFFLE_TIME_ENABLED    TRUE
#define SHUFFLE_TIME_ODDS       100


void Task_OpenShuffleTime(u8 taskId);
void ShuffleTimeExtraDraws(u8 count);
void ShuffleTimeDisplayMessage(const u8 *src);
void ShuffleTimeMorphCard(u8 targetCardId);

void ShuffleTimeInputLock(void);
void ShuffleTimeInputUnlock(void);
bool8 IsShuffleTimeInputLocked(void);

#endif
