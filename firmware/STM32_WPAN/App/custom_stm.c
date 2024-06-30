/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    App/custom_stm.c
  * @author  MCD Application Team
  * @brief   Custom Example Service.
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
#include "common_blesvc.h"
#include "custom_stm.h"

/* USER CODE BEGIN Includes */

#include "Basic/robot.h"
#include "Buzzer/buzzer.h"
#include "Control/robot_control.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
typedef struct{
  uint16_t  CustomMusicserviceHdle;                    /**< musicService handle */
  uint16_t  CustomMusic_Playsong_CharHdle;                  /**< music_playSong_writeChar handle */
  uint16_t  CustomMusic_Isplaying_CharHdle;                  /**< music_isPlaying_notifyChar handle */
  uint16_t  CustomVisionserviceHdle;                    /**< visionService handle */
  uint16_t  CustomVision_Data_CharHdle;                  /**< vision_data_notifyChar handle */
  uint16_t  CustomMainserviceHdle;                    /**< mainService handle */
  uint16_t  CustomMain_Settask_CharHdle;                  /**< main_setTask_writeChar handle */
  uint16_t  CustomMain_Currenttask_CharHdle;                  /**< main_currentTask_notifyChar handle */
  uint16_t  CustomMain_Appready_CharHdle;                  /**< main_appReady_writeChar handle */
  uint16_t  CustomMain_Errorcode_CharHdle;                  /**< main_errorCode_notifyChar handle */
  uint16_t  CustomDriveserviceHdle;                    /**< driveService handle */
  uint16_t  CustomDrive_Data_CharHdle;                  /**< drive_data_notifyChar handle */
  uint16_t  CustomDrive_Gyrodata_CharHdle;                  /**< drive_gyroData_notifyChar handle */
  uint16_t  CustomDrive_Acceldata_CharHdle;                  /**< drive_accelData_notifyChar handle */
/* USER CODE BEGIN Context */
  /* Place holder for Characteristic Descriptors Handle*/

/* USER CODE END Context */
}CustomContext_t;

/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private defines -----------------------------------------------------------*/
#define UUID_128_SUPPORTED  1

#if (UUID_128_SUPPORTED == 1)
#define BM_UUID_LENGTH  UUID_TYPE_128
#else
#define BM_UUID_LENGTH  UUID_TYPE_16
#endif

#define BM_REQ_CHAR_SIZE    (3)

/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macros ------------------------------------------------------------*/
#define CHARACTERISTIC_DESCRIPTOR_ATTRIBUTE_OFFSET         2
#define CHARACTERISTIC_VALUE_ATTRIBUTE_OFFSET              1
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
uint8_t SizeMusic_Playsong_Char = 1;
uint8_t SizeMusic_Isplaying_Char = 1;
uint8_t SizeVision_Data_Char = 4;
uint8_t SizeMain_Settask_Char = 1;
uint8_t SizeMain_Currenttask_Char = 1;
uint8_t SizeMain_Appready_Char = 1;
uint8_t SizeMain_Errorcode_Char = 1;
uint8_t SizeDrive_Data_Char = 16;
uint8_t SizeDrive_Gyrodata_Char = 4*6;
uint8_t SizeDrive_Acceldata_Char = 4*3;

/**
 * START of Section BLE_DRIVER_CONTEXT
 */
static CustomContext_t CustomContext;

/**
 * END of Section BLE_DRIVER_CONTEXT
 */

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
static SVCCTL_EvtAckStatus_t Custom_STM_Event_Handler(void *pckt);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Functions Definition ------------------------------------------------------*/
/* USER CODE BEGIN PFD */

/* USER CODE END PFD */

/* Private functions ----------------------------------------------------------*/

#define COPY_UUID_128(uuid_struct, uuid_15, uuid_14, uuid_13, uuid_12, uuid_11, uuid_10, uuid_9, uuid_8, uuid_7, uuid_6, uuid_5, uuid_4, uuid_3, uuid_2, uuid_1, uuid_0) \
do {\
    uuid_struct[0] = uuid_0; uuid_struct[1] = uuid_1; uuid_struct[2] = uuid_2; uuid_struct[3] = uuid_3; \
    uuid_struct[4] = uuid_4; uuid_struct[5] = uuid_5; uuid_struct[6] = uuid_6; uuid_struct[7] = uuid_7; \
    uuid_struct[8] = uuid_8; uuid_struct[9] = uuid_9; uuid_struct[10] = uuid_10; uuid_struct[11] = uuid_11; \
    uuid_struct[12] = uuid_12; uuid_struct[13] = uuid_13; uuid_struct[14] = uuid_14; uuid_struct[15] = uuid_15; \
}while(0)
#define COPY_UUID_128(uuid_struct, uuid_15, uuid_14, uuid_13, uuid_12, uuid_11, uuid_10, uuid_9, uuid_8, uuid_7, uuid_6, uuid_5, uuid_4, uuid_3, uuid_2, uuid_1, uuid_0) \
do {\
    uuid_struct[0] = uuid_0; uuid_struct[1] = uuid_1; uuid_struct[2] = uuid_2; uuid_struct[3] = uuid_3; \
    uuid_struct[4] = uuid_4; uuid_struct[5] = uuid_5; uuid_struct[6] = uuid_6; uuid_struct[7] = uuid_7; \
    uuid_struct[8] = uuid_8; uuid_struct[9] = uuid_9; uuid_struct[10] = uuid_10; uuid_struct[11] = uuid_11; \
    uuid_struct[12] = uuid_12; uuid_struct[13] = uuid_13; uuid_struct[14] = uuid_14; uuid_struct[15] = uuid_15; \
}while(0)
#define COPY_UUID_128(uuid_struct, uuid_15, uuid_14, uuid_13, uuid_12, uuid_11, uuid_10, uuid_9, uuid_8, uuid_7, uuid_6, uuid_5, uuid_4, uuid_3, uuid_2, uuid_1, uuid_0) \
do {\
    uuid_struct[0] = uuid_0; uuid_struct[1] = uuid_1; uuid_struct[2] = uuid_2; uuid_struct[3] = uuid_3; \
    uuid_struct[4] = uuid_4; uuid_struct[5] = uuid_5; uuid_struct[6] = uuid_6; uuid_struct[7] = uuid_7; \
    uuid_struct[8] = uuid_8; uuid_struct[9] = uuid_9; uuid_struct[10] = uuid_10; uuid_struct[11] = uuid_11; \
    uuid_struct[12] = uuid_12; uuid_struct[13] = uuid_13; uuid_struct[14] = uuid_14; uuid_struct[15] = uuid_15; \
}while(0)
#define COPY_UUID_128(uuid_struct, uuid_15, uuid_14, uuid_13, uuid_12, uuid_11, uuid_10, uuid_9, uuid_8, uuid_7, uuid_6, uuid_5, uuid_4, uuid_3, uuid_2, uuid_1, uuid_0) \
do {\
    uuid_struct[0] = uuid_0; uuid_struct[1] = uuid_1; uuid_struct[2] = uuid_2; uuid_struct[3] = uuid_3; \
    uuid_struct[4] = uuid_4; uuid_struct[5] = uuid_5; uuid_struct[6] = uuid_6; uuid_struct[7] = uuid_7; \
    uuid_struct[8] = uuid_8; uuid_struct[9] = uuid_9; uuid_struct[10] = uuid_10; uuid_struct[11] = uuid_11; \
    uuid_struct[12] = uuid_12; uuid_struct[13] = uuid_13; uuid_struct[14] = uuid_14; uuid_struct[15] = uuid_15; \
}while(0)

#define COPY_MUSICSERVICE_UUID(uuid_struct)          COPY_UUID_128(uuid_struct,0x00,0x00,0x00,0x00,0xcc,0x7a,0x48,0x2a,0x98,0x4a,0x7f,0x2e,0xd5,0xb3,0xe5,0x8f)
#define COPY_MUSIC_PLAYSONG_WRITECHAR_UUID(uuid_struct)    COPY_UUID_128(uuid_struct,0x00,0x00,0x00,0x00,0x8e,0x22,0x45,0x41,0x9d,0x4c,0x21,0xed,0xae,0x82,0xed,0x19)
#define COPY_MUSIC_ISPLAYING_NOTIFYCHAR_UUID(uuid_struct)    COPY_UUID_128(uuid_struct,0x00,0x00,0x00,0x01,0x8e,0x22,0x45,0x41,0x9d,0x4c,0x21,0xed,0xae,0x82,0xed,0x19)
#define COPY_VISIONSERVICE_UUID(uuid_struct)          COPY_UUID_128(uuid_struct,0x00,0x00,0x00,0x01,0xcc,0x7a,0x48,0x2a,0x98,0x4a,0x7f,0x2e,0xd5,0xb3,0xe5,0x8f)
#define COPY_VISION_DATA_NOTIFYCHAR_UUID(uuid_struct)    COPY_UUID_128(uuid_struct,0x00,0x00,0x00,0x02,0x8e,0x22,0x45,0x41,0x9d,0x4c,0x21,0xed,0xae,0x82,0xed,0x19)
#define COPY_MAINSERVICE_UUID(uuid_struct)          COPY_UUID_128(uuid_struct,0x00,0x00,0x00,0x02,0xcc,0x7a,0x48,0x2a,0x98,0x4a,0x7f,0x2e,0xd5,0xb3,0xe5,0x8f)
#define COPY_MAIN_SETTASK_WRITECHAR_UUID(uuid_struct)    COPY_UUID_128(uuid_struct,0x00,0x00,0x00,0x03,0x8e,0x22,0x45,0x41,0x9d,0x4c,0x21,0xed,0xae,0x82,0xed,0x19)
#define COPY_MAIN_CURRENTTASK_NOTIFYCHAR_UUID(uuid_struct)    COPY_UUID_128(uuid_struct,0x00,0x00,0x00,0x04,0x8e,0x22,0x45,0x41,0x9d,0x4c,0x21,0xed,0xae,0x82,0xed,0x19)
#define COPY_MAIN_APPREADY_WRITECHAR_UUID(uuid_struct)    COPY_UUID_128(uuid_struct,0x00,0x00,0x00,0x05,0x8e,0x22,0x45,0x41,0x9d,0x4c,0x21,0xed,0xae,0x82,0xed,0x19)
#define COPY_MAIN_ERRORCODE_NOTIFYCHAR_UUID(uuid_struct)    COPY_UUID_128(uuid_struct,0x00,0x00,0x00,0x07,0x8e,0x22,0x45,0x41,0x9d,0x4c,0x21,0xed,0xae,0x82,0xed,0x19)
#define COPY_DRIVESERVICE_UUID(uuid_struct)          COPY_UUID_128(uuid_struct,0x00,0x00,0x00,0x03,0xcc,0x7a,0x48,0x2a,0x98,0x4a,0x7f,0x2e,0xd5,0xb3,0xe5,0x8f)
#define COPY_DRIVE_DATA_NOTIFYCHAR_UUID(uuid_struct)    COPY_UUID_128(uuid_struct,0x00,0x00,0x00,0x06,0x8e,0x22,0x45,0x41,0x9d,0x4c,0x21,0xed,0xae,0x82,0xed,0x19)
#define COPY_DRIVE_GYRODATA_NOTIFYCHAR_UUID(uuid_struct)    COPY_UUID_128(uuid_struct,0x00,0x00,0x00,0x08,0x8e,0x22,0x45,0x41,0x9d,0x4c,0x21,0xed,0xae,0x82,0xed,0x19)
#define COPY_DRIVE_ACCELDATA_NOTIFYCHAR_UUID(uuid_struct)    COPY_UUID_128(uuid_struct,0x00,0x00,0x00,0x09,0x8e,0x22,0x45,0x41,0x9d,0x4c,0x21,0xed,0xae,0x82,0xed,0x19)

/* USER CODE BEGIN PF */

/* USER CODE END PF */

/**
 * @brief  Event handler
 * @param  Event: Address of the buffer holding the Event
 * @retval Ack: Return whether the Event has been managed or not
 */
static SVCCTL_EvtAckStatus_t Custom_STM_Event_Handler(void *Event)
{
  SVCCTL_EvtAckStatus_t return_value;
  hci_event_pckt *event_pckt;
  evt_blecore_aci *blecore_evt;
  aci_gatt_attribute_modified_event_rp0 *attribute_modified;
  aci_gatt_notification_complete_event_rp0    *notification_complete;
  Custom_STM_App_Notification_evt_t     Notification;
  /* USER CODE BEGIN Custom_STM_Event_Handler_1 */

  /* USER CODE END Custom_STM_Event_Handler_1 */

  return_value = SVCCTL_EvtNotAck;
  event_pckt = (hci_event_pckt *)(((hci_uart_pckt*)Event)->data);

  switch (event_pckt->evt)
  {
    case HCI_VENDOR_SPECIFIC_DEBUG_EVT_CODE:
      blecore_evt = (evt_blecore_aci*)event_pckt->data;
      switch (blecore_evt->ecode)
      {
        case ACI_GATT_ATTRIBUTE_MODIFIED_VSEVT_CODE:
          /* USER CODE BEGIN EVT_BLUE_GATT_ATTRIBUTE_MODIFIED_BEGIN */

          /* USER CODE END EVT_BLUE_GATT_ATTRIBUTE_MODIFIED_BEGIN */
          attribute_modified = (aci_gatt_attribute_modified_event_rp0*)blecore_evt->data;
          if (attribute_modified->Attr_Handle == (CustomContext.CustomMusic_Isplaying_CharHdle + CHARACTERISTIC_DESCRIPTOR_ATTRIBUTE_OFFSET))
          {
            return_value = SVCCTL_EvtAckFlowEnable;
            /* USER CODE BEGIN CUSTOM_STM_Service_1_Char_2 */

            /* USER CODE END CUSTOM_STM_Service_1_Char_2 */
            switch (attribute_modified->Attr_Data[0])
            {
              /* USER CODE BEGIN CUSTOM_STM_Service_1_Char_2_attribute_modified */

              /* USER CODE END CUSTOM_STM_Service_1_Char_2_attribute_modified */

              /* Disabled Notification management */
              case (!(COMSVC_Notification)):
                /* USER CODE BEGIN CUSTOM_STM_Service_1_Char_2_Disabled_BEGIN */

                /* USER CODE END CUSTOM_STM_Service_1_Char_2_Disabled_BEGIN */
                Notification.Custom_Evt_Opcode = CUSTOM_STM_MUSIC_ISPLAYING_CHAR_NOTIFY_DISABLED_EVT;
                Custom_STM_App_Notification(&Notification);
                /* USER CODE BEGIN CUSTOM_STM_Service_1_Char_2_Disabled_END */

                /* USER CODE END CUSTOM_STM_Service_1_Char_2_Disabled_END */
                break;

              /* Enabled Notification management */
              case COMSVC_Notification:
                /* USER CODE BEGIN CUSTOM_STM_Service_1_Char_2_COMSVC_Notification_BEGIN */

                /* USER CODE END CUSTOM_STM_Service_1_Char_2_COMSVC_Notification_BEGIN */
                Notification.Custom_Evt_Opcode = CUSTOM_STM_MUSIC_ISPLAYING_CHAR_NOTIFY_ENABLED_EVT;
                Custom_STM_App_Notification(&Notification);
                /* USER CODE BEGIN CUSTOM_STM_Service_1_Char_2_COMSVC_Notification_END */

                /* USER CODE END CUSTOM_STM_Service_1_Char_2_COMSVC_Notification_END */
                break;

              default:
                /* USER CODE BEGIN CUSTOM_STM_Service_1_Char_2_default */

                /* USER CODE END CUSTOM_STM_Service_1_Char_2_default */
              break;
            }
          }  /* if (attribute_modified->Attr_Handle == (CustomContext.CustomMusic_Isplaying_CharHdle + CHARACTERISTIC_DESCRIPTOR_ATTRIBUTE_OFFSET))*/

          else if (attribute_modified->Attr_Handle == (CustomContext.CustomVision_Data_CharHdle + CHARACTERISTIC_DESCRIPTOR_ATTRIBUTE_OFFSET))
          {
            return_value = SVCCTL_EvtAckFlowEnable;
            /* USER CODE BEGIN CUSTOM_STM_Service_2_Char_1 */

            /* USER CODE END CUSTOM_STM_Service_2_Char_1 */
            switch (attribute_modified->Attr_Data[0])
            {
              /* USER CODE BEGIN CUSTOM_STM_Service_2_Char_1_attribute_modified */

              /* USER CODE END CUSTOM_STM_Service_2_Char_1_attribute_modified */

              /* Disabled Notification management */
              case (!(COMSVC_Notification)):
                /* USER CODE BEGIN CUSTOM_STM_Service_2_Char_1_Disabled_BEGIN */

                /* USER CODE END CUSTOM_STM_Service_2_Char_1_Disabled_BEGIN */
                Notification.Custom_Evt_Opcode = CUSTOM_STM_VISION_DATA_CHAR_NOTIFY_DISABLED_EVT;
                Custom_STM_App_Notification(&Notification);
                /* USER CODE BEGIN CUSTOM_STM_Service_2_Char_1_Disabled_END */

                /* USER CODE END CUSTOM_STM_Service_2_Char_1_Disabled_END */
                break;

              /* Enabled Notification management */
              case COMSVC_Notification:
                /* USER CODE BEGIN CUSTOM_STM_Service_2_Char_1_COMSVC_Notification_BEGIN */

                /* USER CODE END CUSTOM_STM_Service_2_Char_1_COMSVC_Notification_BEGIN */
                Notification.Custom_Evt_Opcode = CUSTOM_STM_VISION_DATA_CHAR_NOTIFY_ENABLED_EVT;
                Custom_STM_App_Notification(&Notification);
                /* USER CODE BEGIN CUSTOM_STM_Service_2_Char_1_COMSVC_Notification_END */

                /* USER CODE END CUSTOM_STM_Service_2_Char_1_COMSVC_Notification_END */
                break;

              default:
                /* USER CODE BEGIN CUSTOM_STM_Service_2_Char_1_default */

                /* USER CODE END CUSTOM_STM_Service_2_Char_1_default */
              break;
            }
          }  /* if (attribute_modified->Attr_Handle == (CustomContext.CustomVision_Data_CharHdle + CHARACTERISTIC_DESCRIPTOR_ATTRIBUTE_OFFSET))*/

          else if (attribute_modified->Attr_Handle == (CustomContext.CustomMain_Currenttask_CharHdle + CHARACTERISTIC_DESCRIPTOR_ATTRIBUTE_OFFSET))
          {
            return_value = SVCCTL_EvtAckFlowEnable;
            /* USER CODE BEGIN CUSTOM_STM_Service_3_Char_2 */

            /* USER CODE END CUSTOM_STM_Service_3_Char_2 */
            switch (attribute_modified->Attr_Data[0])
            {
              /* USER CODE BEGIN CUSTOM_STM_Service_3_Char_2_attribute_modified */

              /* USER CODE END CUSTOM_STM_Service_3_Char_2_attribute_modified */

              /* Disabled Notification management */
              case (!(COMSVC_Notification)):
                /* USER CODE BEGIN CUSTOM_STM_Service_3_Char_2_Disabled_BEGIN */

                /* USER CODE END CUSTOM_STM_Service_3_Char_2_Disabled_BEGIN */
                Notification.Custom_Evt_Opcode = CUSTOM_STM_MAIN_CURRENTTASK_CHAR_NOTIFY_DISABLED_EVT;
                Custom_STM_App_Notification(&Notification);
                /* USER CODE BEGIN CUSTOM_STM_Service_3_Char_2_Disabled_END */

                /* USER CODE END CUSTOM_STM_Service_3_Char_2_Disabled_END */
                break;

              /* Enabled Notification management */
              case COMSVC_Notification:
                /* USER CODE BEGIN CUSTOM_STM_Service_3_Char_2_COMSVC_Notification_BEGIN */

                /* USER CODE END CUSTOM_STM_Service_3_Char_2_COMSVC_Notification_BEGIN */
                Notification.Custom_Evt_Opcode = CUSTOM_STM_MAIN_CURRENTTASK_CHAR_NOTIFY_ENABLED_EVT;
                Custom_STM_App_Notification(&Notification);
                /* USER CODE BEGIN CUSTOM_STM_Service_3_Char_2_COMSVC_Notification_END */

                /* USER CODE END CUSTOM_STM_Service_3_Char_2_COMSVC_Notification_END */
                break;

              default:
                /* USER CODE BEGIN CUSTOM_STM_Service_3_Char_2_default */

                /* USER CODE END CUSTOM_STM_Service_3_Char_2_default */
              break;
            }
          }  /* if (attribute_modified->Attr_Handle == (CustomContext.CustomMain_Currenttask_CharHdle + CHARACTERISTIC_DESCRIPTOR_ATTRIBUTE_OFFSET))*/

          else if (attribute_modified->Attr_Handle == (CustomContext.CustomMain_Errorcode_CharHdle + CHARACTERISTIC_DESCRIPTOR_ATTRIBUTE_OFFSET))
          {
            return_value = SVCCTL_EvtAckFlowEnable;
            /* USER CODE BEGIN CUSTOM_STM_Service_3_Char_4 */

            /* USER CODE END CUSTOM_STM_Service_3_Char_4 */
            switch (attribute_modified->Attr_Data[0])
            {
              /* USER CODE BEGIN CUSTOM_STM_Service_3_Char_4_attribute_modified */

              /* USER CODE END CUSTOM_STM_Service_3_Char_4_attribute_modified */

              /* Disabled Notification management */
              case (!(COMSVC_Notification)):
                /* USER CODE BEGIN CUSTOM_STM_Service_3_Char_4_Disabled_BEGIN */

                /* USER CODE END CUSTOM_STM_Service_3_Char_4_Disabled_BEGIN */
                Notification.Custom_Evt_Opcode = CUSTOM_STM_MAIN_ERRORCODE_CHAR_NOTIFY_DISABLED_EVT;
                Custom_STM_App_Notification(&Notification);
                /* USER CODE BEGIN CUSTOM_STM_Service_3_Char_4_Disabled_END */

                /* USER CODE END CUSTOM_STM_Service_3_Char_4_Disabled_END */
                break;

              /* Enabled Notification management */
              case COMSVC_Notification:
                /* USER CODE BEGIN CUSTOM_STM_Service_3_Char_4_COMSVC_Notification_BEGIN */

                /* USER CODE END CUSTOM_STM_Service_3_Char_4_COMSVC_Notification_BEGIN */
                Notification.Custom_Evt_Opcode = CUSTOM_STM_MAIN_ERRORCODE_CHAR_NOTIFY_ENABLED_EVT;
                Custom_STM_App_Notification(&Notification);
                /* USER CODE BEGIN CUSTOM_STM_Service_3_Char_4_COMSVC_Notification_END */

                /* USER CODE END CUSTOM_STM_Service_3_Char_4_COMSVC_Notification_END */
                break;

              default:
                /* USER CODE BEGIN CUSTOM_STM_Service_3_Char_4_default */

                /* USER CODE END CUSTOM_STM_Service_3_Char_4_default */
              break;
            }
          }  /* if (attribute_modified->Attr_Handle == (CustomContext.CustomMain_Errorcode_CharHdle + CHARACTERISTIC_DESCRIPTOR_ATTRIBUTE_OFFSET))*/

          else if (attribute_modified->Attr_Handle == (CustomContext.CustomDrive_Data_CharHdle + CHARACTERISTIC_DESCRIPTOR_ATTRIBUTE_OFFSET))
          {
            return_value = SVCCTL_EvtAckFlowEnable;
            /* USER CODE BEGIN CUSTOM_STM_Service_4_Char_1 */

            /* USER CODE END CUSTOM_STM_Service_4_Char_1 */
            switch (attribute_modified->Attr_Data[0])
            {
              /* USER CODE BEGIN CUSTOM_STM_Service_4_Char_1_attribute_modified */

              /* USER CODE END CUSTOM_STM_Service_4_Char_1_attribute_modified */

              /* Disabled Notification management */
              case (!(COMSVC_Notification)):
                /* USER CODE BEGIN CUSTOM_STM_Service_4_Char_1_Disabled_BEGIN */

                /* USER CODE END CUSTOM_STM_Service_4_Char_1_Disabled_BEGIN */
                Notification.Custom_Evt_Opcode = CUSTOM_STM_DRIVE_DATA_CHAR_NOTIFY_DISABLED_EVT;
                Custom_STM_App_Notification(&Notification);
                /* USER CODE BEGIN CUSTOM_STM_Service_4_Char_1_Disabled_END */

                /* USER CODE END CUSTOM_STM_Service_4_Char_1_Disabled_END */
                break;

              /* Enabled Notification management */
              case COMSVC_Notification:
                /* USER CODE BEGIN CUSTOM_STM_Service_4_Char_1_COMSVC_Notification_BEGIN */

                /* USER CODE END CUSTOM_STM_Service_4_Char_1_COMSVC_Notification_BEGIN */
                Notification.Custom_Evt_Opcode = CUSTOM_STM_DRIVE_DATA_CHAR_NOTIFY_ENABLED_EVT;
                Custom_STM_App_Notification(&Notification);
                /* USER CODE BEGIN CUSTOM_STM_Service_4_Char_1_COMSVC_Notification_END */

                /* USER CODE END CUSTOM_STM_Service_4_Char_1_COMSVC_Notification_END */
                break;

              default:
                /* USER CODE BEGIN CUSTOM_STM_Service_4_Char_1_default */

                /* USER CODE END CUSTOM_STM_Service_4_Char_1_default */
              break;
            }
          }  /* if (attribute_modified->Attr_Handle == (CustomContext.CustomDrive_Data_CharHdle + CHARACTERISTIC_DESCRIPTOR_ATTRIBUTE_OFFSET))*/

          else if (attribute_modified->Attr_Handle == (CustomContext.CustomDrive_Gyrodata_CharHdle + CHARACTERISTIC_DESCRIPTOR_ATTRIBUTE_OFFSET))
          {
            return_value = SVCCTL_EvtAckFlowEnable;
            /* USER CODE BEGIN CUSTOM_STM_Service_4_Char_2 */

            /* USER CODE END CUSTOM_STM_Service_4_Char_2 */
            switch (attribute_modified->Attr_Data[0])
            {
              /* USER CODE BEGIN CUSTOM_STM_Service_4_Char_2_attribute_modified */

              /* USER CODE END CUSTOM_STM_Service_4_Char_2_attribute_modified */

              /* Disabled Notification management */
              case (!(COMSVC_Notification)):
                /* USER CODE BEGIN CUSTOM_STM_Service_4_Char_2_Disabled_BEGIN */

                /* USER CODE END CUSTOM_STM_Service_4_Char_2_Disabled_BEGIN */
                Notification.Custom_Evt_Opcode = CUSTOM_STM_DRIVE_GYRODATA_CHAR_NOTIFY_DISABLED_EVT;
                Custom_STM_App_Notification(&Notification);
                /* USER CODE BEGIN CUSTOM_STM_Service_4_Char_2_Disabled_END */

                /* USER CODE END CUSTOM_STM_Service_4_Char_2_Disabled_END */
                break;

              /* Enabled Notification management */
              case COMSVC_Notification:
                /* USER CODE BEGIN CUSTOM_STM_Service_4_Char_2_COMSVC_Notification_BEGIN */

                /* USER CODE END CUSTOM_STM_Service_4_Char_2_COMSVC_Notification_BEGIN */
                Notification.Custom_Evt_Opcode = CUSTOM_STM_DRIVE_GYRODATA_CHAR_NOTIFY_ENABLED_EVT;
                Custom_STM_App_Notification(&Notification);
                /* USER CODE BEGIN CUSTOM_STM_Service_4_Char_2_COMSVC_Notification_END */

                /* USER CODE END CUSTOM_STM_Service_4_Char_2_COMSVC_Notification_END */
                break;

              default:
                /* USER CODE BEGIN CUSTOM_STM_Service_4_Char_2_default */

                /* USER CODE END CUSTOM_STM_Service_4_Char_2_default */
              break;
            }
          }  /* if (attribute_modified->Attr_Handle == (CustomContext.CustomDrive_Gyrodata_CharHdle + CHARACTERISTIC_DESCRIPTOR_ATTRIBUTE_OFFSET))*/

          else if (attribute_modified->Attr_Handle == (CustomContext.CustomDrive_Acceldata_CharHdle + CHARACTERISTIC_DESCRIPTOR_ATTRIBUTE_OFFSET))
          {
            return_value = SVCCTL_EvtAckFlowEnable;
            /* USER CODE BEGIN CUSTOM_STM_Service_4_Char_3 */

            /* USER CODE END CUSTOM_STM_Service_4_Char_3 */
            switch (attribute_modified->Attr_Data[0])
            {
              /* USER CODE BEGIN CUSTOM_STM_Service_4_Char_3_attribute_modified */

              /* USER CODE END CUSTOM_STM_Service_4_Char_3_attribute_modified */

              /* Disabled Notification management */
              case (!(COMSVC_Notification)):
                /* USER CODE BEGIN CUSTOM_STM_Service_4_Char_3_Disabled_BEGIN */

                /* USER CODE END CUSTOM_STM_Service_4_Char_3_Disabled_BEGIN */
                Notification.Custom_Evt_Opcode = CUSTOM_STM_DRIVE_ACCELDATA_CHAR_NOTIFY_DISABLED_EVT;
                Custom_STM_App_Notification(&Notification);
                /* USER CODE BEGIN CUSTOM_STM_Service_4_Char_3_Disabled_END */

                /* USER CODE END CUSTOM_STM_Service_4_Char_3_Disabled_END */
                break;

              /* Enabled Notification management */
              case COMSVC_Notification:
                /* USER CODE BEGIN CUSTOM_STM_Service_4_Char_3_COMSVC_Notification_BEGIN */

                /* USER CODE END CUSTOM_STM_Service_4_Char_3_COMSVC_Notification_BEGIN */
                Notification.Custom_Evt_Opcode = CUSTOM_STM_DRIVE_ACCELDATA_CHAR_NOTIFY_ENABLED_EVT;
                Custom_STM_App_Notification(&Notification);
                /* USER CODE BEGIN CUSTOM_STM_Service_4_Char_3_COMSVC_Notification_END */

                /* USER CODE END CUSTOM_STM_Service_4_Char_3_COMSVC_Notification_END */
                break;

              default:
                /* USER CODE BEGIN CUSTOM_STM_Service_4_Char_3_default */

                /* USER CODE END CUSTOM_STM_Service_4_Char_3_default */
              break;
            }
          }  /* if (attribute_modified->Attr_Handle == (CustomContext.CustomDrive_Acceldata_CharHdle + CHARACTERISTIC_DESCRIPTOR_ATTRIBUTE_OFFSET))*/

          else if (attribute_modified->Attr_Handle == (CustomContext.CustomMusic_Playsong_CharHdle + CHARACTERISTIC_VALUE_ATTRIBUTE_OFFSET))
          {
            return_value = SVCCTL_EvtAckFlowEnable;
            /* USER CODE BEGIN CUSTOM_STM_Service_1_Char_1_ACI_GATT_ATTRIBUTE_MODIFIED_VSEVT_CODE */

            if (attribute_modified->Attr_Data_Length == 1) {
              Buzzer_PlaySong(attribute_modified->Attr_Data[0]);
            }

            /* USER CODE END CUSTOM_STM_Service_1_Char_1_ACI_GATT_ATTRIBUTE_MODIFIED_VSEVT_CODE */
          } /* if (attribute_modified->Attr_Handle == (CustomContext.CustomMusic_Playsong_CharHdle + CHARACTERISTIC_VALUE_ATTRIBUTE_OFFSET))*/
          else if (attribute_modified->Attr_Handle == (CustomContext.CustomMain_Settask_CharHdle + CHARACTERISTIC_VALUE_ATTRIBUTE_OFFSET))
          {
            return_value = SVCCTL_EvtAckFlowEnable;
            /* USER CODE BEGIN CUSTOM_STM_Service_3_Char_1_ACI_GATT_ATTRIBUTE_MODIFIED_VSEVT_CODE */

            if (attribute_modified->Attr_Data_Length == 1) {
              RobotControl_RunTask(attribute_modified->Attr_Data[0]);
            }

            /* USER CODE END CUSTOM_STM_Service_3_Char_1_ACI_GATT_ATTRIBUTE_MODIFIED_VSEVT_CODE */
          } /* if (attribute_modified->Attr_Handle == (CustomContext.CustomMain_Settask_CharHdle + CHARACTERISTIC_VALUE_ATTRIBUTE_OFFSET))*/
          else if (attribute_modified->Attr_Handle == (CustomContext.CustomMain_Appready_CharHdle + CHARACTERISTIC_VALUE_ATTRIBUTE_OFFSET))
          {
            return_value = SVCCTL_EvtAckFlowEnable;
            /* USER CODE BEGIN CUSTOM_STM_Service_3_Char_3_ACI_GATT_ATTRIBUTE_MODIFIED_VSEVT_CODE */

            Robot_DashboardAppReady();

            /* USER CODE END CUSTOM_STM_Service_3_Char_3_ACI_GATT_ATTRIBUTE_MODIFIED_VSEVT_CODE */
          } /* if (attribute_modified->Attr_Handle == (CustomContext.CustomMain_Appready_CharHdle + CHARACTERISTIC_VALUE_ATTRIBUTE_OFFSET))*/
          /* USER CODE BEGIN EVT_BLUE_GATT_ATTRIBUTE_MODIFIED_END */

          /* USER CODE END EVT_BLUE_GATT_ATTRIBUTE_MODIFIED_END */
          break;

        case ACI_GATT_READ_PERMIT_REQ_VSEVT_CODE :
          /* USER CODE BEGIN EVT_BLUE_GATT_READ_PERMIT_REQ_BEGIN */

          /* USER CODE END EVT_BLUE_GATT_READ_PERMIT_REQ_BEGIN */
          /* USER CODE BEGIN EVT_BLUE_GATT_READ_PERMIT_REQ_END */

          /* USER CODE END EVT_BLUE_GATT_READ_PERMIT_REQ_END */
          break;

        case ACI_GATT_WRITE_PERMIT_REQ_VSEVT_CODE:
          /* USER CODE BEGIN EVT_BLUE_GATT_WRITE_PERMIT_REQ_BEGIN */

          /* USER CODE END EVT_BLUE_GATT_WRITE_PERMIT_REQ_BEGIN */
          /* USER CODE BEGIN EVT_BLUE_GATT_WRITE_PERMIT_REQ_END */

          /* USER CODE END EVT_BLUE_GATT_WRITE_PERMIT_REQ_END */
          break;

		case ACI_GATT_NOTIFICATION_COMPLETE_VSEVT_CODE:
        {
          /* USER CODE BEGIN EVT_BLUE_GATT_NOTIFICATION_COMPLETE_BEGIN */

          /* USER CODE END EVT_BLUE_GATT_NOTIFICATION_COMPLETE_BEGIN */
          notification_complete = (aci_gatt_notification_complete_event_rp0*)blecore_evt->data;
          Notification.Custom_Evt_Opcode = CUSTOM_STM_NOTIFICATION_COMPLETE_EVT;
          Notification.AttrHandle = notification_complete->Attr_Handle;
          Custom_STM_App_Notification(&Notification);
          /* USER CODE BEGIN EVT_BLUE_GATT_NOTIFICATION_COMPLETE_END */

          /* USER CODE END EVT_BLUE_GATT_NOTIFICATION_COMPLETE_END */
          break;
        }

        /* USER CODE BEGIN BLECORE_EVT */

        /* USER CODE END BLECORE_EVT */
        default:
          /* USER CODE BEGIN EVT_DEFAULT */

          /* USER CODE END EVT_DEFAULT */
          break;
      }
      /* USER CODE BEGIN EVT_VENDOR*/

      /* USER CODE END EVT_VENDOR*/
      break; /* HCI_VENDOR_SPECIFIC_DEBUG_EVT_CODE */

      /* USER CODE BEGIN EVENT_PCKT_CASES*/

      /* USER CODE END EVENT_PCKT_CASES*/

    default:
      /* USER CODE BEGIN EVENT_PCKT*/

      /* USER CODE END EVENT_PCKT*/
      break;
  }

  /* USER CODE BEGIN Custom_STM_Event_Handler_2 */

  /* USER CODE END Custom_STM_Event_Handler_2 */

  return(return_value);
}/* end Custom_STM_Event_Handler */

/* Public functions ----------------------------------------------------------*/

/**
 * @brief  Service initialization
 * @param  None
 * @retval None
 */
void SVCCTL_InitCustomSvc(void)
{

  Char_UUID_t  uuid;
  tBleStatus ret = BLE_STATUS_INVALID_PARAMS;
  uint8_t max_attr_record;

  /* USER CODE BEGIN SVCCTL_InitCustomSvc_1 */

  /* USER CODE END SVCCTL_InitCustomSvc_1 */

  /**
   *  Register the event handler to the BLE controller
   */
  SVCCTL_RegisterSvcHandler(Custom_STM_Event_Handler);

  /**
   *          musicService
   *
   * Max_Attribute_Records = 1 + 2*2 + 1*no_of_char_with_notify_or_indicate_property + 1*no_of_char_with_broadcast_property
   * service_max_attribute_record = 1 for musicService +
   *                                2 for music_playSong_writeChar +
   *                                2 for music_isPlaying_notifyChar +
   *                                1 for music_isPlaying_notifyChar configuration descriptor +
   *                              = 6
   *
   * This value doesn't take into account number of descriptors manually added
   * In case of descriptors added, please update the max_attr_record value accordingly in the next SVCCTL_InitService User Section
   */
  max_attr_record = 6;

  /* USER CODE BEGIN SVCCTL_InitService */
  /* max_attr_record to be updated if descriptors have been added */

  /* USER CODE END SVCCTL_InitService */

  COPY_MUSICSERVICE_UUID(uuid.Char_UUID_128);
  ret = aci_gatt_add_service(UUID_TYPE_128,
                             (Service_UUID_t *) &uuid,
                             PRIMARY_SERVICE,
                             max_attr_record,
                             &(CustomContext.CustomMusicserviceHdle));
  if (ret != BLE_STATUS_SUCCESS)
  {
    APP_DBG_MSG("  Fail   : aci_gatt_add_service command: musicService, error code: 0x%x \n\r", ret);
  }
  else
  {
    APP_DBG_MSG("  Success: aci_gatt_add_service command: musicService \n\r");
  }

  /**
   *  music_playSong_writeChar
   */
  COPY_MUSIC_PLAYSONG_WRITECHAR_UUID(uuid.Char_UUID_128);
  ret = aci_gatt_add_char(CustomContext.CustomMusicserviceHdle,
                          UUID_TYPE_128, &uuid,
                          SizeMusic_Playsong_Char,
                          CHAR_PROP_WRITE,
                          ATTR_PERMISSION_NONE,
                          GATT_NOTIFY_ATTRIBUTE_WRITE,
                          0x10,
                          CHAR_VALUE_LEN_CONSTANT,
                          &(CustomContext.CustomMusic_Playsong_CharHdle));
  if (ret != BLE_STATUS_SUCCESS)
  {
    APP_DBG_MSG("  Fail   : aci_gatt_add_char command   : MUSIC_PLAYSONG_CHAR, error code: 0x%x \n\r", ret);
  }
  else
  {
    APP_DBG_MSG("  Success: aci_gatt_add_char command   : MUSIC_PLAYSONG_CHAR \n\r");
  }

  /* USER CODE BEGIN SVCCTL_Init_Service1_Char1/ */
  /* Place holder for Characteristic Descriptors */

  /* USER CODE END SVCCTL_Init_Service1_Char1 */
  /**
   *  music_isPlaying_notifyChar
   */
  COPY_MUSIC_ISPLAYING_NOTIFYCHAR_UUID(uuid.Char_UUID_128);
  ret = aci_gatt_add_char(CustomContext.CustomMusicserviceHdle,
                          UUID_TYPE_128, &uuid,
                          SizeMusic_Isplaying_Char,
                          CHAR_PROP_NOTIFY,
                          ATTR_PERMISSION_NONE,
                          GATT_DONT_NOTIFY_EVENTS,
                          0x10,
                          CHAR_VALUE_LEN_CONSTANT,
                          &(CustomContext.CustomMusic_Isplaying_CharHdle));
  if (ret != BLE_STATUS_SUCCESS)
  {
    APP_DBG_MSG("  Fail   : aci_gatt_add_char command   : MUSIC_ISPLAYING_CHAR, error code: 0x%x \n\r", ret);
  }
  else
  {
    APP_DBG_MSG("  Success: aci_gatt_add_char command   : MUSIC_ISPLAYING_CHAR \n\r");
  }

  /* USER CODE BEGIN SVCCTL_Init_Service1_Char2/ */
  /* Place holder for Characteristic Descriptors */

  /* USER CODE END SVCCTL_Init_Service1_Char2 */

  /**
   *          visionService
   *
   * Max_Attribute_Records = 1 + 2*1 + 1*no_of_char_with_notify_or_indicate_property + 1*no_of_char_with_broadcast_property
   * service_max_attribute_record = 1 for visionService +
   *                                2 for vision_data_notifyChar +
   *                                1 for vision_data_notifyChar configuration descriptor +
   *                              = 4
   *
   * This value doesn't take into account number of descriptors manually added
   * In case of descriptors added, please update the max_attr_record value accordingly in the next SVCCTL_InitService User Section
   */
  max_attr_record = 4;

  /* USER CODE BEGIN SVCCTL_InitService */
  /* max_attr_record to be updated if descriptors have been added */

  /* USER CODE END SVCCTL_InitService */

  COPY_VISIONSERVICE_UUID(uuid.Char_UUID_128);
  ret = aci_gatt_add_service(UUID_TYPE_128,
                             (Service_UUID_t *) &uuid,
                             PRIMARY_SERVICE,
                             max_attr_record,
                             &(CustomContext.CustomVisionserviceHdle));
  if (ret != BLE_STATUS_SUCCESS)
  {
    APP_DBG_MSG("  Fail   : aci_gatt_add_service command: visionService, error code: 0x%x \n\r", ret);
  }
  else
  {
    APP_DBG_MSG("  Success: aci_gatt_add_service command: visionService \n\r");
  }

  /**
   *  vision_data_notifyChar
   */
  COPY_VISION_DATA_NOTIFYCHAR_UUID(uuid.Char_UUID_128);
  ret = aci_gatt_add_char(CustomContext.CustomVisionserviceHdle,
                          UUID_TYPE_128, &uuid,
                          SizeVision_Data_Char,
                          CHAR_PROP_NOTIFY,
                          ATTR_PERMISSION_NONE,
                          GATT_DONT_NOTIFY_EVENTS,
                          0x10,
                          CHAR_VALUE_LEN_CONSTANT,
                          &(CustomContext.CustomVision_Data_CharHdle));
  if (ret != BLE_STATUS_SUCCESS)
  {
    APP_DBG_MSG("  Fail   : aci_gatt_add_char command   : VISION_DATA_CHAR, error code: 0x%x \n\r", ret);
  }
  else
  {
    APP_DBG_MSG("  Success: aci_gatt_add_char command   : VISION_DATA_CHAR \n\r");
  }

  /* USER CODE BEGIN SVCCTL_Init_Service2_Char1/ */
  /* Place holder for Characteristic Descriptors */

  /* USER CODE END SVCCTL_Init_Service2_Char1 */

  /**
   *          mainService
   *
   * Max_Attribute_Records = 1 + 2*4 + 1*no_of_char_with_notify_or_indicate_property + 1*no_of_char_with_broadcast_property
   * service_max_attribute_record = 1 for mainService +
   *                                2 for main_setTask_writeChar +
   *                                2 for main_currentTask_notifyChar +
   *                                2 for main_appReady_writeChar +
   *                                2 for main_errorCode_notifyChar +
   *                                1 for main_currentTask_notifyChar configuration descriptor +
   *                                1 for main_errorCode_notifyChar configuration descriptor +
   *                              = 11
   *
   * This value doesn't take into account number of descriptors manually added
   * In case of descriptors added, please update the max_attr_record value accordingly in the next SVCCTL_InitService User Section
   */
  max_attr_record = 11;

  /* USER CODE BEGIN SVCCTL_InitService */
  /* max_attr_record to be updated if descriptors have been added */

  /* USER CODE END SVCCTL_InitService */

  COPY_MAINSERVICE_UUID(uuid.Char_UUID_128);
  ret = aci_gatt_add_service(UUID_TYPE_128,
                             (Service_UUID_t *) &uuid,
                             PRIMARY_SERVICE,
                             max_attr_record,
                             &(CustomContext.CustomMainserviceHdle));
  if (ret != BLE_STATUS_SUCCESS)
  {
    APP_DBG_MSG("  Fail   : aci_gatt_add_service command: mainService, error code: 0x%x \n\r", ret);
  }
  else
  {
    APP_DBG_MSG("  Success: aci_gatt_add_service command: mainService \n\r");
  }

  /**
   *  main_setTask_writeChar
   */
  COPY_MAIN_SETTASK_WRITECHAR_UUID(uuid.Char_UUID_128);
  ret = aci_gatt_add_char(CustomContext.CustomMainserviceHdle,
                          UUID_TYPE_128, &uuid,
                          SizeMain_Settask_Char,
                          CHAR_PROP_WRITE,
                          ATTR_PERMISSION_NONE,
                          GATT_NOTIFY_ATTRIBUTE_WRITE,
                          0x10,
                          CHAR_VALUE_LEN_CONSTANT,
                          &(CustomContext.CustomMain_Settask_CharHdle));
  if (ret != BLE_STATUS_SUCCESS)
  {
    APP_DBG_MSG("  Fail   : aci_gatt_add_char command   : MAIN_SETTASK_CHAR, error code: 0x%x \n\r", ret);
  }
  else
  {
    APP_DBG_MSG("  Success: aci_gatt_add_char command   : MAIN_SETTASK_CHAR \n\r");
  }

  /* USER CODE BEGIN SVCCTL_Init_Service3_Char1/ */
  /* Place holder for Characteristic Descriptors */

  /* USER CODE END SVCCTL_Init_Service3_Char1 */
  /**
   *  main_currentTask_notifyChar
   */
  COPY_MAIN_CURRENTTASK_NOTIFYCHAR_UUID(uuid.Char_UUID_128);
  ret = aci_gatt_add_char(CustomContext.CustomMainserviceHdle,
                          UUID_TYPE_128, &uuid,
                          SizeMain_Currenttask_Char,
                          CHAR_PROP_NOTIFY,
                          ATTR_PERMISSION_NONE,
                          GATT_DONT_NOTIFY_EVENTS,
                          0x10,
                          CHAR_VALUE_LEN_CONSTANT,
                          &(CustomContext.CustomMain_Currenttask_CharHdle));
  if (ret != BLE_STATUS_SUCCESS)
  {
    APP_DBG_MSG("  Fail   : aci_gatt_add_char command   : MAIN_CURRENTTASK_CHAR, error code: 0x%x \n\r", ret);
  }
  else
  {
    APP_DBG_MSG("  Success: aci_gatt_add_char command   : MAIN_CURRENTTASK_CHAR \n\r");
  }

  /* USER CODE BEGIN SVCCTL_Init_Service3_Char2/ */
  /* Place holder for Characteristic Descriptors */

  /* USER CODE END SVCCTL_Init_Service3_Char2 */
  /**
   *  main_appReady_writeChar
   */
  COPY_MAIN_APPREADY_WRITECHAR_UUID(uuid.Char_UUID_128);
  ret = aci_gatt_add_char(CustomContext.CustomMainserviceHdle,
                          UUID_TYPE_128, &uuid,
                          SizeMain_Appready_Char,
                          CHAR_PROP_WRITE,
                          ATTR_PERMISSION_NONE,
                          GATT_NOTIFY_ATTRIBUTE_WRITE,
                          0x10,
                          CHAR_VALUE_LEN_CONSTANT,
                          &(CustomContext.CustomMain_Appready_CharHdle));
  if (ret != BLE_STATUS_SUCCESS)
  {
    APP_DBG_MSG("  Fail   : aci_gatt_add_char command   : MAIN_APPREADY_CHAR, error code: 0x%x \n\r", ret);
  }
  else
  {
    APP_DBG_MSG("  Success: aci_gatt_add_char command   : MAIN_APPREADY_CHAR \n\r");
  }

  /* USER CODE BEGIN SVCCTL_Init_Service3_Char3/ */
  /* Place holder for Characteristic Descriptors */

  /* USER CODE END SVCCTL_Init_Service3_Char3 */
  /**
   *  main_errorCode_notifyChar
   */
  COPY_MAIN_ERRORCODE_NOTIFYCHAR_UUID(uuid.Char_UUID_128);
  ret = aci_gatt_add_char(CustomContext.CustomMainserviceHdle,
                          UUID_TYPE_128, &uuid,
                          SizeMain_Errorcode_Char,
                          CHAR_PROP_NOTIFY,
                          ATTR_PERMISSION_NONE,
                          GATT_DONT_NOTIFY_EVENTS,
                          0x10,
                          CHAR_VALUE_LEN_CONSTANT,
                          &(CustomContext.CustomMain_Errorcode_CharHdle));
  if (ret != BLE_STATUS_SUCCESS)
  {
    APP_DBG_MSG("  Fail   : aci_gatt_add_char command   : MAIN_ERRORCODE_CHAR, error code: 0x%x \n\r", ret);
  }
  else
  {
    APP_DBG_MSG("  Success: aci_gatt_add_char command   : MAIN_ERRORCODE_CHAR \n\r");
  }

  /* USER CODE BEGIN SVCCTL_Init_Service3_Char4/ */
  /* Place holder for Characteristic Descriptors */

  /* USER CODE END SVCCTL_Init_Service3_Char4 */

  /**
   *          driveService
   *
   * Max_Attribute_Records = 1 + 2*3 + 1*no_of_char_with_notify_or_indicate_property + 1*no_of_char_with_broadcast_property
   * service_max_attribute_record = 1 for driveService +
   *                                2 for drive_data_notifyChar +
   *                                2 for drive_gyroData_notifyChar +
   *                                2 for drive_accelData_notifyChar +
   *                                1 for drive_data_notifyChar configuration descriptor +
   *                                1 for drive_gyroData_notifyChar configuration descriptor +
   *                                1 for drive_accelData_notifyChar configuration descriptor +
   *                              = 10
   *
   * This value doesn't take into account number of descriptors manually added
   * In case of descriptors added, please update the max_attr_record value accordingly in the next SVCCTL_InitService User Section
   */
  max_attr_record = 10;

  /* USER CODE BEGIN SVCCTL_InitService */
  /* max_attr_record to be updated if descriptors have been added */

  /* USER CODE END SVCCTL_InitService */

  COPY_DRIVESERVICE_UUID(uuid.Char_UUID_128);
  ret = aci_gatt_add_service(UUID_TYPE_128,
                             (Service_UUID_t *) &uuid,
                             PRIMARY_SERVICE,
                             max_attr_record,
                             &(CustomContext.CustomDriveserviceHdle));
  if (ret != BLE_STATUS_SUCCESS)
  {
    APP_DBG_MSG("  Fail   : aci_gatt_add_service command: driveService, error code: 0x%x \n\r", ret);
  }
  else
  {
    APP_DBG_MSG("  Success: aci_gatt_add_service command: driveService \n\r");
  }

  /**
   *  drive_data_notifyChar
   */
  COPY_DRIVE_DATA_NOTIFYCHAR_UUID(uuid.Char_UUID_128);
  ret = aci_gatt_add_char(CustomContext.CustomDriveserviceHdle,
                          UUID_TYPE_128, &uuid,
                          SizeDrive_Data_Char,
                          CHAR_PROP_NOTIFY,
                          ATTR_PERMISSION_NONE,
                          GATT_DONT_NOTIFY_EVENTS,
                          0x10,
                          CHAR_VALUE_LEN_CONSTANT,
                          &(CustomContext.CustomDrive_Data_CharHdle));
  if (ret != BLE_STATUS_SUCCESS)
  {
    APP_DBG_MSG("  Fail   : aci_gatt_add_char command   : DRIVE_DATA_CHAR, error code: 0x%x \n\r", ret);
  }
  else
  {
    APP_DBG_MSG("  Success: aci_gatt_add_char command   : DRIVE_DATA_CHAR \n\r");
  }

  /* USER CODE BEGIN SVCCTL_Init_Service4_Char1/ */
  /* Place holder for Characteristic Descriptors */

  /* USER CODE END SVCCTL_Init_Service4_Char1 */
  /**
   *  drive_gyroData_notifyChar
   */
  COPY_DRIVE_GYRODATA_NOTIFYCHAR_UUID(uuid.Char_UUID_128);
  ret = aci_gatt_add_char(CustomContext.CustomDriveserviceHdle,
                          UUID_TYPE_128, &uuid,
                          SizeDrive_Gyrodata_Char,
                          CHAR_PROP_NOTIFY,
                          ATTR_PERMISSION_NONE,
                          GATT_DONT_NOTIFY_EVENTS,
                          0x10,
                          CHAR_VALUE_LEN_CONSTANT,
                          &(CustomContext.CustomDrive_Gyrodata_CharHdle));
  if (ret != BLE_STATUS_SUCCESS)
  {
    APP_DBG_MSG("  Fail   : aci_gatt_add_char command   : DRIVE_GYRODATA_CHAR, error code: 0x%x \n\r", ret);
  }
  else
  {
    APP_DBG_MSG("  Success: aci_gatt_add_char command   : DRIVE_GYRODATA_CHAR \n\r");
  }

  /* USER CODE BEGIN SVCCTL_Init_Service4_Char2/ */
  /* Place holder for Characteristic Descriptors */

  /* USER CODE END SVCCTL_Init_Service4_Char2 */
  /**
   *  drive_accelData_notifyChar
   */
  COPY_DRIVE_ACCELDATA_NOTIFYCHAR_UUID(uuid.Char_UUID_128);
  ret = aci_gatt_add_char(CustomContext.CustomDriveserviceHdle,
                          UUID_TYPE_128, &uuid,
                          SizeDrive_Acceldata_Char,
                          CHAR_PROP_NOTIFY,
                          ATTR_PERMISSION_NONE,
                          GATT_DONT_NOTIFY_EVENTS,
                          0x10,
                          CHAR_VALUE_LEN_CONSTANT,
                          &(CustomContext.CustomDrive_Acceldata_CharHdle));
  if (ret != BLE_STATUS_SUCCESS)
  {
    APP_DBG_MSG("  Fail   : aci_gatt_add_char command   : DRIVE_ACCELDATA_CHAR, error code: 0x%x \n\r", ret);
  }
  else
  {
    APP_DBG_MSG("  Success: aci_gatt_add_char command   : DRIVE_ACCELDATA_CHAR \n\r");
  }

  /* USER CODE BEGIN SVCCTL_Init_Service4_Char3/ */
  /* Place holder for Characteristic Descriptors */

  /* USER CODE END SVCCTL_Init_Service4_Char3 */

  /* USER CODE BEGIN SVCCTL_InitCustomSvc_2 */

  /* USER CODE END SVCCTL_InitCustomSvc_2 */

  return;
}

/**
 * @brief  Characteristic update
 * @param  CharOpcode: Characteristic identifier
 * @param  Service_Instance: Instance of the service to which the characteristic belongs
 *
 */
tBleStatus Custom_STM_App_Update_Char(Custom_STM_Char_Opcode_t CharOpcode, uint8_t *pPayload)
{
  tBleStatus ret = BLE_STATUS_INVALID_PARAMS;
  /* USER CODE BEGIN Custom_STM_App_Update_Char_1 */

  /* USER CODE END Custom_STM_App_Update_Char_1 */

  switch (CharOpcode)
  {

    case CUSTOM_STM_MUSIC_PLAYSONG_CHAR:
      ret = aci_gatt_update_char_value(CustomContext.CustomMusicserviceHdle,
                                       CustomContext.CustomMusic_Playsong_CharHdle,
                                       0, /* charValOffset */
                                       SizeMusic_Playsong_Char, /* charValueLen */
                                       (uint8_t *)  pPayload);
      if (ret != BLE_STATUS_SUCCESS)
      {
        APP_DBG_MSG("  Fail   : aci_gatt_update_char_value MUSIC_PLAYSONG_CHAR command, result : 0x%x \n\r", ret);
      }
      else
      {
        APP_DBG_MSG("  Success: aci_gatt_update_char_value MUSIC_PLAYSONG_CHAR command\n\r");
      }
      /* USER CODE BEGIN CUSTOM_STM_App_Update_Service_1_Char_1*/

      /* USER CODE END CUSTOM_STM_App_Update_Service_1_Char_1*/
      break;

    case CUSTOM_STM_MUSIC_ISPLAYING_CHAR:
      ret = aci_gatt_update_char_value(CustomContext.CustomMusicserviceHdle,
                                       CustomContext.CustomMusic_Isplaying_CharHdle,
                                       0, /* charValOffset */
                                       SizeMusic_Isplaying_Char, /* charValueLen */
                                       (uint8_t *)  pPayload);
      if (ret != BLE_STATUS_SUCCESS)
      {
        APP_DBG_MSG("  Fail   : aci_gatt_update_char_value MUSIC_ISPLAYING_CHAR command, result : 0x%x \n\r", ret);
      }
      else
      {
        APP_DBG_MSG("  Success: aci_gatt_update_char_value MUSIC_ISPLAYING_CHAR command\n\r");
      }
      /* USER CODE BEGIN CUSTOM_STM_App_Update_Service_1_Char_2*/

      /* USER CODE END CUSTOM_STM_App_Update_Service_1_Char_2*/
      break;

    case CUSTOM_STM_VISION_DATA_CHAR:
      ret = aci_gatt_update_char_value(CustomContext.CustomVisionserviceHdle,
                                       CustomContext.CustomVision_Data_CharHdle,
                                       0, /* charValOffset */
                                       SizeVision_Data_Char, /* charValueLen */
                                       (uint8_t *)  pPayload);
      if (ret != BLE_STATUS_SUCCESS)
      {
        APP_DBG_MSG("  Fail   : aci_gatt_update_char_value VISION_DATA_CHAR command, result : 0x%x \n\r", ret);
      }
      else
      {
        APP_DBG_MSG("  Success: aci_gatt_update_char_value VISION_DATA_CHAR command\n\r");
      }
      /* USER CODE BEGIN CUSTOM_STM_App_Update_Service_2_Char_1*/

      /* USER CODE END CUSTOM_STM_App_Update_Service_2_Char_1*/
      break;

    case CUSTOM_STM_MAIN_SETTASK_CHAR:
      ret = aci_gatt_update_char_value(CustomContext.CustomMainserviceHdle,
                                       CustomContext.CustomMain_Settask_CharHdle,
                                       0, /* charValOffset */
                                       SizeMain_Settask_Char, /* charValueLen */
                                       (uint8_t *)  pPayload);
      if (ret != BLE_STATUS_SUCCESS)
      {
        APP_DBG_MSG("  Fail   : aci_gatt_update_char_value MAIN_SETTASK_CHAR command, result : 0x%x \n\r", ret);
      }
      else
      {
        APP_DBG_MSG("  Success: aci_gatt_update_char_value MAIN_SETTASK_CHAR command\n\r");
      }
      /* USER CODE BEGIN CUSTOM_STM_App_Update_Service_3_Char_1*/

      /* USER CODE END CUSTOM_STM_App_Update_Service_3_Char_1*/
      break;

    case CUSTOM_STM_MAIN_CURRENTTASK_CHAR:
      ret = aci_gatt_update_char_value(CustomContext.CustomMainserviceHdle,
                                       CustomContext.CustomMain_Currenttask_CharHdle,
                                       0, /* charValOffset */
                                       SizeMain_Currenttask_Char, /* charValueLen */
                                       (uint8_t *)  pPayload);
      if (ret != BLE_STATUS_SUCCESS)
      {
        APP_DBG_MSG("  Fail   : aci_gatt_update_char_value MAIN_CURRENTTASK_CHAR command, result : 0x%x \n\r", ret);
      }
      else
      {
        APP_DBG_MSG("  Success: aci_gatt_update_char_value MAIN_CURRENTTASK_CHAR command\n\r");
      }
      /* USER CODE BEGIN CUSTOM_STM_App_Update_Service_3_Char_2*/

      /* USER CODE END CUSTOM_STM_App_Update_Service_3_Char_2*/
      break;

    case CUSTOM_STM_MAIN_APPREADY_CHAR:
      ret = aci_gatt_update_char_value(CustomContext.CustomMainserviceHdle,
                                       CustomContext.CustomMain_Appready_CharHdle,
                                       0, /* charValOffset */
                                       SizeMain_Appready_Char, /* charValueLen */
                                       (uint8_t *)  pPayload);
      if (ret != BLE_STATUS_SUCCESS)
      {
        APP_DBG_MSG("  Fail   : aci_gatt_update_char_value MAIN_APPREADY_CHAR command, result : 0x%x \n\r", ret);
      }
      else
      {
        APP_DBG_MSG("  Success: aci_gatt_update_char_value MAIN_APPREADY_CHAR command\n\r");
      }
      /* USER CODE BEGIN CUSTOM_STM_App_Update_Service_3_Char_3*/

      /* USER CODE END CUSTOM_STM_App_Update_Service_3_Char_3*/
      break;

    case CUSTOM_STM_MAIN_ERRORCODE_CHAR:
      ret = aci_gatt_update_char_value(CustomContext.CustomMainserviceHdle,
                                       CustomContext.CustomMain_Errorcode_CharHdle,
                                       0, /* charValOffset */
                                       SizeMain_Errorcode_Char, /* charValueLen */
                                       (uint8_t *)  pPayload);
      if (ret != BLE_STATUS_SUCCESS)
      {
        APP_DBG_MSG("  Fail   : aci_gatt_update_char_value MAIN_ERRORCODE_CHAR command, result : 0x%x \n\r", ret);
      }
      else
      {
        APP_DBG_MSG("  Success: aci_gatt_update_char_value MAIN_ERRORCODE_CHAR command\n\r");
      }
      /* USER CODE BEGIN CUSTOM_STM_App_Update_Service_3_Char_4*/

      /* USER CODE END CUSTOM_STM_App_Update_Service_3_Char_4*/
      break;

    case CUSTOM_STM_DRIVE_DATA_CHAR:
      ret = aci_gatt_update_char_value(CustomContext.CustomDriveserviceHdle,
                                       CustomContext.CustomDrive_Data_CharHdle,
                                       0, /* charValOffset */
                                       SizeDrive_Data_Char, /* charValueLen */
                                       (uint8_t *)  pPayload);
      if (ret != BLE_STATUS_SUCCESS)
      {
        APP_DBG_MSG("  Fail   : aci_gatt_update_char_value DRIVE_DATA_CHAR command, result : 0x%x \n\r", ret);
      }
      else
      {
        APP_DBG_MSG("  Success: aci_gatt_update_char_value DRIVE_DATA_CHAR command\n\r");
      }
      /* USER CODE BEGIN CUSTOM_STM_App_Update_Service_4_Char_1*/

      /* USER CODE END CUSTOM_STM_App_Update_Service_4_Char_1*/
      break;

    case CUSTOM_STM_DRIVE_GYRODATA_CHAR:
      ret = aci_gatt_update_char_value(CustomContext.CustomDriveserviceHdle,
                                       CustomContext.CustomDrive_Gyrodata_CharHdle,
                                       0, /* charValOffset */
                                       SizeDrive_Gyrodata_Char, /* charValueLen */
                                       (uint8_t *)  pPayload);
      if (ret != BLE_STATUS_SUCCESS)
      {
        APP_DBG_MSG("  Fail   : aci_gatt_update_char_value DRIVE_GYRODATA_CHAR command, result : 0x%x \n\r", ret);
      }
      else
      {
        APP_DBG_MSG("  Success: aci_gatt_update_char_value DRIVE_GYRODATA_CHAR command\n\r");
      }
      /* USER CODE BEGIN CUSTOM_STM_App_Update_Service_4_Char_2*/

      /* USER CODE END CUSTOM_STM_App_Update_Service_4_Char_2*/
      break;

    case CUSTOM_STM_DRIVE_ACCELDATA_CHAR:
      ret = aci_gatt_update_char_value(CustomContext.CustomDriveserviceHdle,
                                       CustomContext.CustomDrive_Acceldata_CharHdle,
                                       0, /* charValOffset */
                                       SizeDrive_Acceldata_Char, /* charValueLen */
                                       (uint8_t *)  pPayload);
      if (ret != BLE_STATUS_SUCCESS)
      {
        APP_DBG_MSG("  Fail   : aci_gatt_update_char_value DRIVE_ACCELDATA_CHAR command, result : 0x%x \n\r", ret);
      }
      else
      {
        APP_DBG_MSG("  Success: aci_gatt_update_char_value DRIVE_ACCELDATA_CHAR command\n\r");
      }
      /* USER CODE BEGIN CUSTOM_STM_App_Update_Service_4_Char_3*/

      /* USER CODE END CUSTOM_STM_App_Update_Service_4_Char_3*/
      break;

    default:
      break;
  }

  /* USER CODE BEGIN Custom_STM_App_Update_Char_2 */

  /* USER CODE END Custom_STM_App_Update_Char_2 */

  return ret;
}
