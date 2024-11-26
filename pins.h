#ifndef PINS_H_
#define PINS_H_



//RGB LED
#define RGB_Port gpioPortA
#define BLUE_Pin 6
#define RED_Pin 5
#define GREEN_Pin 7

//PWM
#define DRIVER_PORT gpioPortC
#define PWMA_PIN 3
#define PWMB_PIN 6

//BIN1
//Port C
#define PUMP2_PIN 7

//AIN1
#define PUMP1_PORT gpioPortA
#define PUMP1_PIN 0


//Connection LED
#define CONNECTION_PORT gpioPortB
#define CONNECTION_PIN 0

// Buzzer Connection
#define BUZZER_PORT gpioPortB
#define BUZZER_PIN 3


// [IADC0]
#define IADC_FSR1_READING_PORT_PIN iadcPosInputPortBPin1;
#define IADC_FSR1_READING_BUS BBUSALLOC;
#define IADC_FSR1_READING_BUSALLOC GPIO_BBUSALLOC_BODD0_ADC0;

#define IADC_FSR2_READING_PORT_PIN iadcPosInputPortBPin2;
#define IADC_FSR2_READING_BUS BBUSALLOC;
#define IADC_FSR2_READING_BUSALLOC GPIO_BBUSALLOC_BEVEN0_ADC0;


#endif /* PINS_H_ */
