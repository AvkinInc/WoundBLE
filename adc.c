#include "adc.h"


//Declare Init Structures
IADC_Init_t init = IADC_INIT_DEFAULT;
IADC_AllConfigs_t initAllConfigs = IADC_ALLCONFIGS_DEFAULT;
IADC_InitSingle_t initSingle = IADC_INITSINGLE_DEFAULT;
IADC_SingleInput_t FSR1ReadingSingleInput = IADC_SINGLEINPUT_DEFAULT;
IADC_SingleInput_t FSR2ReadingSingleInput = IADC_SINGLEINPUT_DEFAULT;



volatile uint32_t fsr1Reading = 0x00;
volatile uint32_t fsr2Reading = 0x00;


uint8_t fsr1Activated = 0;
uint8_t fsr2Activated = 0;

uint32_t fsr1LocalLow = 0;
uint32_t fsr2LocalLow = 0;

uint32_t fsr1LocalHigh = 0;
uint32_t fsr2LocalHigh = 0;

bool fsr1changeInActivationState = false;
bool fsr2changeInActivationState = false;

uint32_t fsrThreshold = 500;
uint32_t thresholdOn = 0;
uint32_t thresholdOff = 0;

static bool blockActivation;

void resetBlockActivation(void){
  blockActivation = false;
}

void initIADC (void){

  CMU_ClockEnable(cmuClock_IADC0, true);
  CMU_ClockEnable(cmuClock_GPIO, true);


  //reset ADC
  IADC_reset(IADC0);


  //Set clock
  CMU_ClockSelectSet(cmuClock_IADCCLK, cmuSelect_FSRCO);  // FSRCO - 20MHz

  // Modify init structs and initialize
  init.warmup = iadcWarmupKeepWarm;

  // Set the HFSCLK prescale value here
  init.srcClkPrescale = IADC_calcSrcClkPrescale(IADC0, CLK_SRC_ADC_FREQ, 0);

  // Configuration 0 is used by both scan and single conversions by default
  // Use unbuffered AVDD (supply voltage in mV) as reference
  // Resolution is not configurable directly but is based on the
  // selected oversampling ratio (osrHighSpeed), which defaults to
  // 2x and generates 12-bit results.
  initAllConfigs.configs[0].reference = iadcCfgReferenceVddx;
  initAllConfigs.configs[0].vRef = 3300;
  // initAllConfigs.configs[0].osrHighSpeed = iadcCfgOsrHighSpeed2x;

  // Divides CLK_SRC_ADC to set the CLK_ADC frequency
  initAllConfigs.configs[0].adcClkPrescale = IADC_calcAdcClkPrescale(IADC0,
                                                                     CLK_ADC_FREQ,
                                                                     0,
                                                                     iadcCfgModeNormal,
                                                                     init.srcClkPrescale);

  FSR1ReadingSingleInput.posInput = IADC_FSR1_READING_PORT_PIN;
  FSR1ReadingSingleInput.negInput = iadcNegInputGnd;

  FSR2ReadingSingleInput.posInput = IADC_FSR2_READING_PORT_PIN;
  FSR2ReadingSingleInput.negInput = iadcNegInputGnd;


  //init IADC
  IADC_init(IADC0, &init, &initAllConfigs);
   }

void FSR1InitialReading(void){


  //initialize the single converson inputs
  IADC_initSingle(IADC0, &initSingle, &FSR1ReadingSingleInput);

  //Analog Bus for ADC0 Input
  GPIO->BBUSALLOC |= IADC_FSR1_READING_BUSALLOC;

  IADC_command(IADC0, iadcCmdStartSingle);
  //wait for conversion to complete
  while((IADC0->STATUS & (_IADC_STATUS_CONVERTING_MASK
                           | _IADC_STATUS_SINGLEFIFODV_MASK)) != IADC_STATUS_SINGLEFIFODV);

  fsr1Reading = IADC_pullSingleFifoResult(IADC0).data;

  sl_bt_gatt_server_write_attribute_value(gattdb_fsr1,0,4,&fsr1Reading);
  sl_bt_gatt_server_notify_all(gattdb_fsr1, 4, &fsr1Reading);

}

void FSR2InitialReading(void){

  //initialize the single conversion inputs
  IADC_initSingle(IADC0, &initSingle, &FSR2ReadingSingleInput);

  //Analog Bus for ADC0 Input
  GPIO->BBUSALLOC |= IADC_FSR2_READING_BUSALLOC;

  IADC_command(IADC0, iadcCmdStartSingle);
  //wait for conversion to complete
  while((IADC0->STATUS & (_IADC_STATUS_CONVERTING_MASK
                          | _IADC_STATUS_SINGLEFIFODV_MASK)) != IADC_STATUS_SINGLEFIFODV);

  fsr2Reading = IADC_pullSingleFifoResult(IADC0).data;

  sl_bt_gatt_server_write_attribute_value(gattdb_fsr2,0,4,&fsr2Reading);
  sl_bt_gatt_server_notify_all(gattdb_fsr2, 4, &fsr2Reading);

}

void readFSR1ADC(void){

  uint16_t change;

  //Initialize the single conversion inputs
  IADC_initSingle(IADC0, &initSingle, &FSR1ReadingSingleInput);
  //Analog bus for ADC0 input
  GPIO->BBUSALLOC |= IADC_FSR1_READING_BUSALLOC;

  IADC_command(IADC0, iadcCmdStartSingle);
  //wait for conversion to be complete
  while((IADC0->STATUS & (_IADC_STATUS_CONVERTING_MASK
                            | _IADC_STATUS_SINGLEFIFODV_MASK)) != IADC_STATUS_SINGLEFIFODV);

  fsr1Reading = IADC_pullSingleFifoResult(IADC0).data;

  sl_bt_gatt_server_write_attribute_value(gattdb_fsr1,0,4, &fsr1Reading);
  sl_bt_gatt_server_notify_all(gattdb_fsr1, 4, &fsr1Reading);


  if(fsr1Activated == 0){
      change = abs(fsr1LocalHigh - fsr1Reading);
      if(fsr1Reading >= fsr1LocalHigh){
          //change is positive
          //reading is increasing or staying the same
          fsr1LocalHigh = fsr1Reading;
          fsr1changeInActivationState = 0;
          GPIO_PinOutSet(RGB_Port, RED_Pin);
      }
      else{
          if(change < thresholdOn){
              //decreasing but not past threshold
              //localHigh stays the same, keep measuring from that point
              //do not need to set local low till threshold is crossed
              fsr1changeInActivationState = 0;
          }
          else if(change >= thresholdOn){
              //decreasing and over the threshold
              //set local low
              fsr1LocalLow = fsr1Reading;
              fsr1Activated = 1;
              fsr1changeInActivationState = 1;
              //skin has been released, block reactivation
              //write sleep timer activation function

          }
        }
  }

  else if(fsr1Activated == 1){
      change = abs(fsr1Reading - fsr1LocalLow);
      if(fsr1Reading <= fsr1LocalLow){
          //decreasing or staying the same
          fsr1LocalLow = fsr1Reading;
          fsr1changeInActivationState = 0;
      }
      else{
          if(change < thresholdOff){
              //going higher, but not past the threshold
              //localLow stays the same, keep measuring from that point
              //no need to set local high
              //till the threshold is crossed
              fsr1changeInActivationState = 0;
          }
          else if (change >= thresholdOff){
              //going higher and past the threshold
              //set local high, go below to reactivate
              fsr1LocalHigh = fsr1Reading;
              fsr1Activated = 0;
              fsr1changeInActivationState = 1;
              //sleepTimerActivationOff();
          }

      }

  }
  if(fsr1Activated){
      if(fsr1changeInActivationState == 1 && blockActivation == false){
          //turn on LED and Buzzer
          GPIO_PinOutClear(BUZZER_PORT, BUZZER_PIN);
          //sleepTimerBuzzerOff();
          blockActivation = true;

      }
  }
  if(fsr1changeInActivationState == 1 && blockActivation == true){
   //no more activation
   //turn led off and stop buzzer

  }

}

void readFSR2ADC(void){

  uint16_t change;

  //Initialize the single conversoin inputs
  IADC_initSingle(IADC0, &initSingle, &FSR2ReadingSingleInput);
  //Analog bus for ADC0 input
  GPIO->BBUSALLOC |= IADC_FSR2_READING_BUSALLOC;

  IADC_command(IADC0, iadcCmdStartSingle);
  //wait for conversion to be complete
  while((IADC0->STATUS & (_IADC_STATUS_CONVERTING_MASK
                            | _IADC_STATUS_SINGLEFIFODV_MASK)) != IADC_STATUS_SINGLEFIFODV);

  fsr2Reading = IADC_pullSingleFifoResult(IADC0).data;

  sl_bt_gatt_server_write_attribute_value(gattdb_fsr2,0,4, &fsr2Reading);
  sl_bt_gatt_server_notify_all(gattdb_fsr2, 4, &fsr2Reading);


  if(fsr2Activated == 0){
      change = abs(fsr2LocalHigh - fsr2Reading);
      if(fsr2Reading >= fsr2LocalHigh){
          //change is positive
          //reading is increasing or staying the same
          fsr2LocalHigh = fsr2Reading;
          fsr2changeInActivationState = 0;
          GPIO_PinOutSet(RGB_Port, RED_Pin);
      }
      else{
          if(change < thresholdOn){
              //decreasing but not past threshold
              //localHigh stays the same, keep measuring from that point
              //do not need to set local low till threshold is crossed
              fsr2changeInActivationState = 0;
          }
          else if(change >= thresholdOn){
              //decreasing and over the threshold
              //set local low
              fsr2LocalLow = fsr2Reading;
              fsr2Activated = 1;
              fsr2changeInActivationState = 1;
              //skin has been released, block reactivation
              //write sleep timer activation function

          }
        }
  }

  else if(fsr2Activated == 1){
      change = abs(fsr2Reading - fsr2LocalLow);
      if(fsr2Reading <= fsr2LocalLow){
          //decreasing or staying the same
          fsr2LocalLow = fsr2Reading;
          fsr2changeInActivationState = 0;
      }
      else{
          if(change < thresholdOff){
              //going higher, but not past the threshold
              //localLow stays the same, keep measuring from that point
              //no need to set local high
              //till the threshold is crossed
              fsr2changeInActivationState = 0;
          }
          else if (change >= thresholdOff){
              //going higher and past the threshold
              //set local high, go below to reactivate
              fsr2LocalHigh = fsr2Reading;
              fsr2Activated = 0;
              fsr2changeInActivationState = 1;
              //sleepTimerActivationOff();
          }

      }

  }
  if(fsr2Activated){
      if(fsr2changeInActivationState == 1 && blockActivation == false){
          //turn on LED and Buzzer
          GPIO_PinOutClear(BUZZER_PORT, BUZZER_PIN);
          //sleepTimerBuzzerOff();
          blockActivation = true;

      }
  }
  if(fsr2changeInActivationState == 1 && blockActivation == true){
   //no more activation
   //turn led off and stop buzzer

  }
}











