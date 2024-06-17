#pragma once

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

void Robot_Init(void);
void Robot_Update(void);
void Robot_SendFeedback(void);

#ifdef __cplusplus
}
#endif
