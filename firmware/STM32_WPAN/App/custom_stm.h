/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    App/custom_stm.h
  * @author  MCD Application Team
  * @brief   Header for custom_stm.c module.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef CUSTOM_STM_H
#define CUSTOM_STM_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "ble_types.h"

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
typedef enum
{
  /* musicService */
  CUSTOM_STM_MUSIC_PLAYSONG_CHAR,
  CUSTOM_STM_MUSIC_ISPLAYING_CHAR,
  /* visionService */
  CUSTOM_STM_VISION_DATA_CHAR,
  /* mainService */
  CUSTOM_STM_MAIN_SETTASK_CHAR,
  CUSTOM_STM_MAIN_CURRENTTASK_CHAR,
  CUSTOM_STM_MAIN_APPREADY_CHAR,
} Custom_STM_Char_Opcode_t;

typedef enum
{
  /* music_playSong_writeChar */
  CUSTOM_STM_MUSIC_PLAYSONG_CHAR_WRITE_EVT,
  /* music_isPlaying_notifyChar */
  CUSTOM_STM_MUSIC_ISPLAYING_CHAR_NOTIFY_ENABLED_EVT,
  CUSTOM_STM_MUSIC_ISPLAYING_CHAR_NOTIFY_DISABLED_EVT,
  /* vision_data_notifyChar */
  CUSTOM_STM_VISION_DATA_CHAR_NOTIFY_ENABLED_EVT,
  CUSTOM_STM_VISION_DATA_CHAR_NOTIFY_DISABLED_EVT,
  /* main_setTask_writeChar */
  CUSTOM_STM_MAIN_SETTASK_CHAR_WRITE_EVT,
  /* main_currentTask_notifyChar */
  CUSTOM_STM_MAIN_CURRENTTASK_CHAR_NOTIFY_ENABLED_EVT,
  CUSTOM_STM_MAIN_CURRENTTASK_CHAR_NOTIFY_DISABLED_EVT,
  /* main_appReady_writeChar */
  CUSTOM_STM_MAIN_APPREADY_CHAR_WRITE_EVT,
  CUSTOM_STM_NOTIFICATION_COMPLETE_EVT,

  CUSTOM_STM_BOOT_REQUEST_EVT
} Custom_STM_Opcode_evt_t;

typedef struct
{
  uint8_t * pPayload;
  uint8_t   Length;
} Custom_STM_Data_t;

typedef struct
{
  Custom_STM_Opcode_evt_t       Custom_Evt_Opcode;
  Custom_STM_Data_t             DataTransfered;
  uint16_t                      ConnectionHandle;
  uint8_t                       ServiceInstance;
  uint16_t                      AttrHandle;
} Custom_STM_App_Notification_evt_t;

/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
extern uint8_t SizeMusic_Playsong_Char;
extern uint8_t SizeMusic_Isplaying_Char;
extern uint8_t SizeVision_Data_Char;
extern uint8_t SizeMain_Settask_Char;
extern uint8_t SizeMain_Currenttask_Char;
extern uint8_t SizeMain_Appready_Char;

/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* External variables --------------------------------------------------------*/
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/* Exported macros -----------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions ------------------------------------------------------- */
void SVCCTL_InitCustomSvc(void);
void Custom_STM_App_Notification(Custom_STM_App_Notification_evt_t *pNotification);
tBleStatus Custom_STM_App_Update_Char(Custom_STM_Char_Opcode_t CharOpcode,  uint8_t *pPayload);
/* USER CODE BEGIN EF */

/* USER CODE END EF */

#ifdef __cplusplus
}
#endif

#endif /*CUSTOM_STM_H */
