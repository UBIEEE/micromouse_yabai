#include "Vision/vision.hpp"

#include "custom_stm.h"

//
// External variables.
//

extern ADC_HandleTypeDef hadc1; // main.c

//
// Constants.
//

// Number of ticks waiting for the IR emitters to turn on.
static constexpr uint8_t WAITING_TICKS = 2;

static constexpr GPIO_TypeDef* EMIT_PORTS[] = {
    IR_FAR_RIGHT_EMIT_GPIO_Port,
    IR_MID_RIGHT_EMIT_GPIO_Port,
    IR_MID_LEFT_EMIT_GPIO_Port,
    IR_FAR_LEFT_EMIT_GPIO_Port,
};

static constexpr uint16_t EMIT_PINS[] = {
    IR_FAR_RIGHT_EMIT_Pin,
    IR_MID_RIGHT_EMIT_Pin,
    IR_MID_LEFT_EMIT_Pin,
    IR_FAR_LEFT_EMIT_Pin,
};

//
// Static variables.
//

static volatile bool s_adc_ready = false;

//
// Vision functions.
//

void Vision::process() {
  if (!m_enabled) {
    if (m_state != State::IDLE) {
      // Turn off the emitter.
      HAL_GPIO_WritePin(EMIT_PORTS[m_sensor], EMIT_PINS[m_sensor],
                        GPIO_PIN_RESET);
      m_state = State::IDLE;
    }
    return;
  }

  switch (m_state) {
  case State::READING:
    if (!s_adc_ready) return;
    s_adc_ready = false;
    m_state     = State::IDLE;

    // Turn off the emitter.
    HAL_GPIO_WritePin(EMIT_PORTS[m_sensor], EMIT_PINS[m_sensor],
                      GPIO_PIN_RESET);

    m_readings[m_sensor] = m_raw_readings[m_sensor];

    // Next sensor.
    m_sensor = static_cast<Sensor>((m_sensor + 1) % 4);
    break;

  case State::IDLE:
    // Turn on the emitter.
    HAL_GPIO_WritePin(EMIT_PORTS[m_sensor], EMIT_PINS[m_sensor], GPIO_PIN_SET);

    m_state         = State::WAITING;
    m_waiting_ticks = 0;
    break;

  case State::WAITING:
    if (++m_waiting_ticks < WAITING_TICKS) return;

    // Start DMA read.
    HAL_ADC_Start_DMA(&hadc1, reinterpret_cast<uint32_t*>(&m_raw_readings), 4);
    m_state = State::READING;
    break;
  }
}

void Vision::send_feedback() {
  static bool was_enabled = false;

  if (m_enabled || was_enabled) {
    Custom_STM_App_Update_Char(CUSTOM_STM_VISIONDATACHAR, m_readings);
  }
  was_enabled = m_enabled;
}

//
// Callbacks.
//

// ADC conversion complete callback.
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc) {
  assert_param(hadc->Instance == ADC1);
  UNUSED(hadc);
  s_adc_ready = true;
}
