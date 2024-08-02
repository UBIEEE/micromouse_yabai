#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void RobotControl_RunTask(uint8_t task);
void RobotControl_ResetMaze(void);

#ifdef __cplusplus
}
#endif
