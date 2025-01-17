#ifndef PWM_INIT_PWMB_CONFIG_H
#define PWM_INIT_PWMB_CONFIG_H

#ifdef __cplusplus
extern "C"
{
#endif

// <<< Use Configuration Wizard in Context Menu >>>

// <h>PWM configuration

// <o SL_PWM_PWMB_FREQUENCY> PWM frequency [Hz]
// <i> Default: 10000
#define SL_PWM_PWMB_FREQUENCY       10000

// <o SL_PWM_PWMB_POLARITY> Polarity
// <PWM_ACTIVE_HIGH=> Active high
// <PWM_ACTIVE_LOW=> Active low
// <i> Default: PWM_ACTIVE_HIGH
#define SL_PWM_PWMB_POLARITY        PWM_ACTIVE_HIGH
// </h> end pwm configuration

// <<< end of configuration section >>>

// <<< sl:start pin_tool >>>

// <timer channel=OUTPUT> SL_PWM_PWMB
// $[TIMER_SL_PWM_PWMB]
#define SL_PWM_PWMB_PERIPHERAL                   TIMER1
#define SL_PWM_PWMB_PERIPHERAL_NO                1

#define SL_PWM_PWMB_OUTPUT_CHANNEL               0
// TIMER1 CC0 on PC06
#define SL_PWM_PWMB_OUTPUT_PORT                  gpioPortC
#define SL_PWM_PWMB_OUTPUT_PIN                   6

// <<< sl:end pin_tool >>>

#ifdef __cplusplus
}
#endif

#endif // PWM_INIT_PWMB_CONFIG_H
