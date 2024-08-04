#include "Vision/vision.hpp"

#include "custom_stm.h"

//
// External variables.
//

extern ADC_HandleTypeDef hadc1; // main.c

//
// Constants.
//

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
// Vision functions.
//

void Vision::process() {
  if (!m_enabled) {
    if (m_state != State::IDLE) {
      // Turn off the emitter.
      set_emitter(m_sensor, GPIO_PIN_RESET);
      m_state = State::IDLE;
    }
    return;
  }

  switch (m_state) {
  case State::READING:
    if (!m_adc_ready) return;
    m_adc_ready = false;
    m_state     = State::IDLE;

    // Turn off the emitter.
    set_emitter(m_sensor, GPIO_PIN_RESET);

    m_readings[m_sensor] = m_raw_readings[m_sensor];

    // Next sensor.
    m_sensor = static_cast<Sensor>((m_sensor + 1) % 4);
    break;

  case State::IDLE:
    // Turn on the emitter.
    set_emitter(m_sensor, GPIO_PIN_SET);

    m_state = State::WAITING; // Wait one tick for the emitter to turn on.
    break;

  case State::WAITING:
    // Start DMA read.
    HAL_ADC_Start_DMA(&hadc1, reinterpret_cast<uint32_t*>(&m_raw_readings), 4);
    m_state = State::READING;
    break;
  }
}

void Vision::set_emitter(Sensor sensor, GPIO_PinState state) {
  HAL_GPIO_WritePin(EMIT_PORTS[sensor], EMIT_PINS[sensor], state);
}

void Vision::read_complete_handler() { m_adc_ready = true; }

void Vision::send_feedback() {
  if (!m_enabled) return;

  Custom_STM_App_Update_Char(CUSTOM_STM_VISION_DATA_CHAR, m_readings);
}

#include "Basic/robot.hpp"

// ADC conversion complete callback.
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc) {
  assert_param(hadc->Instance == ADC1);
  UNUSED(hadc);

  Robot::get().vision().read_complete_handler();
}
