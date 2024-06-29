/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    App/custom_app.c
  * @author  MCD Application Team
  * @brief   Custom Example Application (Server)
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "app_common.h"
#include "dbg_trace.h"
#include "ble.h"
#include "custom_app.h"
#include "custom_stm.h"
#include "stm32_seq.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "Basic/robot.h"
#include <stdbool.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
typedef struct
{
  /* musicService */
  uint8_t               Music_isplaying_char_Notification_Status;
  /* visionService */
  uint8_t               Vision_data_char_Notification_Status;
  /* mainService */
  uint8_t               Main_currenttask_char_Notification_Status;
  uint8_t               Main_errorcode_char_Notification_Status;
  /* driveService */
  uint8_t               Drive_data_char_Notification_Status;
  /* USER CODE BEGIN CUSTOM_APP_Context_t */

  /* USER CODE END CUSTOM_APP_Context_t */

  uint16_t              ConnectionHandle;
} Custom_App_Context_t;

/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private defines ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macros -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/**
 * START of Section BLE_APP_CONTEXT
 */

static Custom_App_Context_t Custom_App_Context;

/**
 * END of Section BLE_APP_CONTEXT
 */

uint8_t UpdateCharData[247];
uint8_t NotifyCharData[247];

/* USER CODE BEGIN PV */

bool g_robot_tasks_registered = false;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* musicService */
static void Custom_Music_isplaying_char_Update_Char(void);
static void Custom_Music_isplaying_char_Send_Notification(void);
/* visionService */
static void Custom_Vision_data_char_Update_Char(void);
static void Custom_Vision_data_char_Send_Notification(void);
/* mainService */
static void Custom_Main_currenttask_char_Update_Char(void);
static void Custom_Main_currenttask_char_Send_Notification(void);
static void Custom_Main_errorcode_char_Update_Char(void);
static void Custom_Main_errorcode_char_Send_Notification(void);
/* driveService */
static void Custom_Drive_data_char_Update_Char(void);
static void Custom_Drive_data_char_Send_Notification(void);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Functions Definition ------------------------------------------------------*/
void Custom_STM_App_Notification(Custom_STM_App_Notification_evt_t *pNotification)
{
  /* USER CODE BEGIN CUSTOM_STM_App_Notification_1 */

  /* USER CODE END CUSTOM_STM_App_Notification_1 */
  switch (pNotification->Custom_Evt_Opcode)
  {
    /* USER CODE BEGIN CUSTOM_STM_App_Notification_Custom_Evt_Opcode */

    /* USER CODE END CUSTOM_STM_App_Notification_Custom_Evt_Opcode */

    /* musicService */
    case CUSTOM_STM_MUSIC_PLAYSONG_CHAR_WRITE_EVT:
      /* USER CODE BEGIN CUSTOM_STM_MUSIC_PLAYSONG_CHAR_WRITE_EVT */

      /* USER CODE END CUSTOM_STM_MUSIC_PLAYSONG_CHAR_WRITE_EVT */
      break;

    case CUSTOM_STM_MUSIC_ISPLAYING_CHAR_NOTIFY_ENABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_MUSIC_ISPLAYING_CHAR_NOTIFY_ENABLED_EVT */

      /* USER CODE END CUSTOM_STM_MUSIC_ISPLAYING_CHAR_NOTIFY_ENABLED_EVT */
      break;

    case CUSTOM_STM_MUSIC_ISPLAYING_CHAR_NOTIFY_DISABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_MUSIC_ISPLAYING_CHAR_NOTIFY_DISABLED_EVT */

      /* USER CODE END CUSTOM_STM_MUSIC_ISPLAYING_CHAR_NOTIFY_DISABLED_EVT */
      break;

    /* visionService */
    case CUSTOM_STM_VISION_DATA_CHAR_NOTIFY_ENABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_VISION_DATA_CHAR_NOTIFY_ENABLED_EVT */

      /* USER CODE END CUSTOM_STM_VISION_DATA_CHAR_NOTIFY_ENABLED_EVT */
      break;

    case CUSTOM_STM_VISION_DATA_CHAR_NOTIFY_DISABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_VISION_DATA_CHAR_NOTIFY_DISABLED_EVT */

      /* USER CODE END CUSTOM_STM_VISION_DATA_CHAR_NOTIFY_DISABLED_EVT */
      break;

    /* mainService */
    case CUSTOM_STM_MAIN_SETTASK_CHAR_WRITE_EVT:
      /* USER CODE BEGIN CUSTOM_STM_MAIN_SETTASK_CHAR_WRITE_EVT */

      /* USER CODE END CUSTOM_STM_MAIN_SETTASK_CHAR_WRITE_EVT */
      break;

    case CUSTOM_STM_MAIN_CURRENTTASK_CHAR_NOTIFY_ENABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_MAIN_CURRENTTASK_CHAR_NOTIFY_ENABLED_EVT */

      /* USER CODE END CUSTOM_STM_MAIN_CURRENTTASK_CHAR_NOTIFY_ENABLED_EVT */
      break;

    case CUSTOM_STM_MAIN_CURRENTTASK_CHAR_NOTIFY_DISABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_MAIN_CURRENTTASK_CHAR_NOTIFY_DISABLED_EVT */

      /* USER CODE END CUSTOM_STM_MAIN_CURRENTTASK_CHAR_NOTIFY_DISABLED_EVT */
      break;

    case CUSTOM_STM_MAIN_APPREADY_CHAR_WRITE_EVT:
      /* USER CODE BEGIN CUSTOM_STM_MAIN_APPREADY_CHAR_WRITE_EVT */

      /* USER CODE END CUSTOM_STM_MAIN_APPREADY_CHAR_WRITE_EVT */
      break;

    case CUSTOM_STM_MAIN_ERRORCODE_CHAR_NOTIFY_ENABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_MAIN_ERRORCODE_CHAR_NOTIFY_ENABLED_EVT */

      /* USER CODE END CUSTOM_STM_MAIN_ERRORCODE_CHAR_NOTIFY_ENABLED_EVT */
      break;

    case CUSTOM_STM_MAIN_ERRORCODE_CHAR_NOTIFY_DISABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_MAIN_ERRORCODE_CHAR_NOTIFY_DISABLED_EVT */

      /* USER CODE END CUSTOM_STM_MAIN_ERRORCODE_CHAR_NOTIFY_DISABLED_EVT */
      break;

    /* driveService */
    case CUSTOM_STM_DRIVE_DATA_CHAR_NOTIFY_ENABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_DRIVE_DATA_CHAR_NOTIFY_ENABLED_EVT */

      /* USER CODE END CUSTOM_STM_DRIVE_DATA_CHAR_NOTIFY_ENABLED_EVT */
      break;

    case CUSTOM_STM_DRIVE_DATA_CHAR_NOTIFY_DISABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_DRIVE_DATA_CHAR_NOTIFY_DISABLED_EVT */

      /* USER CODE END CUSTOM_STM_DRIVE_DATA_CHAR_NOTIFY_DISABLED_EVT */
      break;

    case CUSTOM_STM_NOTIFICATION_COMPLETE_EVT:
      /* USER CODE BEGIN CUSTOM_STM_NOTIFICATION_COMPLETE_EVT */

      /* USER CODE END CUSTOM_STM_NOTIFICATION_COMPLETE_EVT */
      break;

    default:
      /* USER CODE BEGIN CUSTOM_STM_App_Notification_default */

      /* USER CODE END CUSTOM_STM_App_Notification_default */
      break;
  }
  /* USER CODE BEGIN CUSTOM_STM_App_Notification_2 */

  /* USER CODE END CUSTOM_STM_App_Notification_2 */
  return;
}

void Custom_APP_Notification(Custom_App_ConnHandle_Not_evt_t *pNotification)
{
  /* USER CODE BEGIN CUSTOM_APP_Notification_1 */

  /* USER CODE END CUSTOM_APP_Notification_1 */

  switch (pNotification->Custom_Evt_Opcode)
  {
    /* USER CODE BEGIN CUSTOM_APP_Notification_Custom_Evt_Opcode */

    /* USER CODE END P2PS_CUSTOM_Notification_Custom_Evt_Opcode */
    case CUSTOM_CONN_HANDLE_EVT :
      /* USER CODE BEGIN CUSTOM_CONN_HANDLE_EVT */
      
      Robot_OnConnect();
      
      /* USER CODE END CUSTOM_CONN_HANDLE_EVT */
      break;

    case CUSTOM_DISCON_HANDLE_EVT :
      /* USER CODE BEGIN CUSTOM_DISCON_HANDLE_EVT */

      Robot_OnDisconnect();

      /* USER CODE END CUSTOM_DISCON_HANDLE_EVT */
      break;

    default:
      /* USER CODE BEGIN CUSTOM_APP_Notification_default */

      /* USER CODE END CUSTOM_APP_Notification_default */
      break;
  }

  /* USER CODE BEGIN CUSTOM_APP_Notification_2 */

  /* USER CODE END CUSTOM_APP_Notification_2 */

  return;
}

void Custom_APP_Init(void)
{
  /* USER CODE BEGIN CUSTOM_APP_Init */

  //
  // Initialize robot stuff.
  //
  Robot_Init();

  //
  // Register the robot update task with the sequencer.
  // The task will be executed every 20ms (see SysTick_Handler in stm32wbxx_it.c).
  //
  UTIL_SEQ_RegTask(1 << CFG_TASK_ROBOT_UPDATE_ID, 0, Robot_Update);

  //
  // Register the robot send feedback task with the sequencer.
  // The task will be executed every 200ms (see SysTick_Handler in stm32wbxx_it.c).
  //
  UTIL_SEQ_RegTask(1 << CFG_TASK_ROBOT_SEND_FEEDBACK_ID, 0, Robot_SendFeedback);

  //
  // The tasks have been registered with the sequencer, so signal that the task
  // is ready to be run.
  //
  g_robot_tasks_registered = true;

  /* USER CODE END CUSTOM_APP_Init */
  return;
}

/* USER CODE BEGIN FD */

/* USER CODE END FD */

/*************************************************************
 *
 * LOCAL FUNCTIONS
 *
 *************************************************************/

/* musicService */
void Custom_Music_isplaying_char_Update_Char(void) /* Property Read */
{
  uint8_t updateflag = 0;

  /* USER CODE BEGIN Music_isplaying_char_UC_1*/

  /* USER CODE END Music_isplaying_char_UC_1*/

  if (updateflag != 0)
  {
    Custom_STM_App_Update_Char(CUSTOM_STM_MUSIC_ISPLAYING_CHAR, (uint8_t *)UpdateCharData);
  }

  /* USER CODE BEGIN Music_isplaying_char_UC_Last*/

  /* USER CODE END Music_isplaying_char_UC_Last*/
  return;
}

void Custom_Music_isplaying_char_Send_Notification(void) /* Property Notification */
{
  uint8_t updateflag = 0;

  /* USER CODE BEGIN Music_isplaying_char_NS_1*/

  /* USER CODE END Music_isplaying_char_NS_1*/

  if (updateflag != 0)
  {
    Custom_STM_App_Update_Char(CUSTOM_STM_MUSIC_ISPLAYING_CHAR, (uint8_t *)NotifyCharData);
  }

  /* USER CODE BEGIN Music_isplaying_char_NS_Last*/

  /* USER CODE END Music_isplaying_char_NS_Last*/

  return;
}

/* visionService */
void Custom_Vision_data_char_Update_Char(void) /* Property Read */
{
  uint8_t updateflag = 0;

  /* USER CODE BEGIN Vision_data_char_UC_1*/

  /* USER CODE END Vision_data_char_UC_1*/

  if (updateflag != 0)
  {
    Custom_STM_App_Update_Char(CUSTOM_STM_VISION_DATA_CHAR, (uint8_t *)UpdateCharData);
  }

  /* USER CODE BEGIN Vision_data_char_UC_Last*/

  /* USER CODE END Vision_data_char_UC_Last*/
  return;
}

void Custom_Vision_data_char_Send_Notification(void) /* Property Notification */
{
  uint8_t updateflag = 0;

  /* USER CODE BEGIN Vision_data_char_NS_1*/

  /* USER CODE END Vision_data_char_NS_1*/

  if (updateflag != 0)
  {
    Custom_STM_App_Update_Char(CUSTOM_STM_VISION_DATA_CHAR, (uint8_t *)NotifyCharData);
  }

  /* USER CODE BEGIN Vision_data_char_NS_Last*/

  /* USER CODE END Vision_data_char_NS_Last*/

  return;
}

/* mainService */
void Custom_Main_currenttask_char_Update_Char(void) /* Property Read */
{
  uint8_t updateflag = 0;

  /* USER CODE BEGIN Main_currenttask_char_UC_1*/

  /* USER CODE END Main_currenttask_char_UC_1*/

  if (updateflag != 0)
  {
    Custom_STM_App_Update_Char(CUSTOM_STM_MAIN_CURRENTTASK_CHAR, (uint8_t *)UpdateCharData);
  }

  /* USER CODE BEGIN Main_currenttask_char_UC_Last*/

  /* USER CODE END Main_currenttask_char_UC_Last*/
  return;
}

void Custom_Main_currenttask_char_Send_Notification(void) /* Property Notification */
{
  uint8_t updateflag = 0;

  /* USER CODE BEGIN Main_currenttask_char_NS_1*/

  /* USER CODE END Main_currenttask_char_NS_1*/

  if (updateflag != 0)
  {
    Custom_STM_App_Update_Char(CUSTOM_STM_MAIN_CURRENTTASK_CHAR, (uint8_t *)NotifyCharData);
  }

  /* USER CODE BEGIN Main_currenttask_char_NS_Last*/

  /* USER CODE END Main_currenttask_char_NS_Last*/

  return;
}

void Custom_Main_errorcode_char_Update_Char(void) /* Property Read */
{
  uint8_t updateflag = 0;

  /* USER CODE BEGIN Main_errorcode_char_UC_1*/

  /* USER CODE END Main_errorcode_char_UC_1*/

  if (updateflag != 0)
  {
    Custom_STM_App_Update_Char(CUSTOM_STM_MAIN_ERRORCODE_CHAR, (uint8_t *)UpdateCharData);
  }

  /* USER CODE BEGIN Main_errorcode_char_UC_Last*/

  /* USER CODE END Main_errorcode_char_UC_Last*/
  return;
}

void Custom_Main_errorcode_char_Send_Notification(void) /* Property Notification */
{
  uint8_t updateflag = 0;

  /* USER CODE BEGIN Main_errorcode_char_NS_1*/

  /* USER CODE END Main_errorcode_char_NS_1*/

  if (updateflag != 0)
  {
    Custom_STM_App_Update_Char(CUSTOM_STM_MAIN_ERRORCODE_CHAR, (uint8_t *)NotifyCharData);
  }

  /* USER CODE BEGIN Main_errorcode_char_NS_Last*/

  /* USER CODE END Main_errorcode_char_NS_Last*/

  return;
}

/* driveService */
void Custom_Drive_data_char_Update_Char(void) /* Property Read */
{
  uint8_t updateflag = 0;

  /* USER CODE BEGIN Drive_data_char_UC_1*/

  /* USER CODE END Drive_data_char_UC_1*/

  if (updateflag != 0)
  {
    Custom_STM_App_Update_Char(CUSTOM_STM_DRIVE_DATA_CHAR, (uint8_t *)UpdateCharData);
  }

  /* USER CODE BEGIN Drive_data_char_UC_Last*/

  /* USER CODE END Drive_data_char_UC_Last*/
  return;
}

void Custom_Drive_data_char_Send_Notification(void) /* Property Notification */
{
  uint8_t updateflag = 0;

  /* USER CODE BEGIN Drive_data_char_NS_1*/

  /* USER CODE END Drive_data_char_NS_1*/

  if (updateflag != 0)
  {
    Custom_STM_App_Update_Char(CUSTOM_STM_DRIVE_DATA_CHAR, (uint8_t *)NotifyCharData);
  }

  /* USER CODE BEGIN Drive_data_char_NS_Last*/

  /* USER CODE END Drive_data_char_NS_Last*/

  return;
}

/* USER CODE BEGIN FD_LOCAL_FUNCTIONS*/

/* USER CODE END FD_LOCAL_FUNCTIONS*/
