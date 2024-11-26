/***************************************************************************//**
 * @file
 * @brief PWM Driver Instance Initialization
 *******************************************************************************
 * # License
 * <b>Copyright 2020 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/

#include "sl_pwm.h"

#include "sl_pwm_init_PWMA_config.h"

#include "sl_pwm_init_PWMB_config.h"

#include "sl_pwm_init_PWMC_config.h"


#include "em_gpio.h"


sl_pwm_instance_t sl_pwm_PWMA = {
  .timer = SL_PWM_PWMA_PERIPHERAL,
  .channel = (uint8_t)(SL_PWM_PWMA_OUTPUT_CHANNEL),
  .port = (uint8_t)(SL_PWM_PWMA_OUTPUT_PORT),
  .pin = (uint8_t)(SL_PWM_PWMA_OUTPUT_PIN),
#if defined(SL_PWM_PWMA_OUTPUT_LOC)
  .location = (uint8_t)(SL_PWM_PWMA_OUTPUT_LOC),
#endif
};

sl_pwm_instance_t sl_pwm_PWMB = {
  .timer = SL_PWM_PWMB_PERIPHERAL,
  .channel = (uint8_t)(SL_PWM_PWMB_OUTPUT_CHANNEL),
  .port = (uint8_t)(SL_PWM_PWMB_OUTPUT_PORT),
  .pin = (uint8_t)(SL_PWM_PWMB_OUTPUT_PIN),
#if defined(SL_PWM_PWMB_OUTPUT_LOC)
  .location = (uint8_t)(SL_PWM_PWMB_OUTPUT_LOC),
#endif
};

sl_pwm_instance_t sl_pwm_PWMC = {
  .timer = SL_PWM_PWMC_PERIPHERAL,
  .channel = (uint8_t)(SL_PWM_PWMC_OUTPUT_CHANNEL),
  .port = (uint8_t)(SL_PWM_PWMC_OUTPUT_PORT),
  .pin = (uint8_t)(SL_PWM_PWMC_OUTPUT_PIN),
#if defined(SL_PWM_PWMC_OUTPUT_LOC)
  .location = (uint8_t)(SL_PWM_PWMC_OUTPUT_LOC),
#endif
};


void sl_pwm_init_instances(void)
{

  sl_pwm_config_t pwm_PWMA_config = {
    .frequency = SL_PWM_PWMA_FREQUENCY,
    .polarity = SL_PWM_PWMA_POLARITY,
  };

  sl_pwm_init(&sl_pwm_PWMA, &pwm_PWMA_config);

  sl_pwm_config_t pwm_PWMB_config = {
    .frequency = SL_PWM_PWMB_FREQUENCY,
    .polarity = SL_PWM_PWMB_POLARITY,
  };

  sl_pwm_init(&sl_pwm_PWMB, &pwm_PWMB_config);

  sl_pwm_config_t pwm_PWMC_config = {
    .frequency = SL_PWM_PWMC_FREQUENCY,
    .polarity = SL_PWM_PWMC_POLARITY,
  };

  sl_pwm_init(&sl_pwm_PWMC, &pwm_PWMC_config);

}
