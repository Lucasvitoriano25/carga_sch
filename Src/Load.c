/*

  ******************************************************************************
   *CORRENTE M�XIMA LIMITADA PELO DAC = 3.3 A
  ******************************************************************************

*/

#include "main.h"
#include "stm32f3xx_hal.h"
#include "stdbool.h"


uint16_t ADC_VALUES[2];
    
extern ADC_HandleTypeDef hadc1;
extern DMA_HandleTypeDef hdma_adc1;

extern DAC_HandleTypeDef hdac1;

extern TIM_HandleTypeDef htim1;


float I_To_AD_Converter = 1363.64; 
float AD_To_Tension_Converter = 0.017;



/* USER CODE BEGIN PFP */
void Carga_Init();
void SET_CURRENT(float value);
void SET_POTENCY(float potency);
void SET_RESISTANCE(float resistance);
/* USER CODE END PFP */


void Carga_Init(){

  HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);
  HAL_TIM_Base_Start(&htim1);
  HAL_ADC_Start_DMA(&hadc1, (uint32_t*) ADC_VALUES, 2);



}

void SET_CURRENT(float Value){
  
  int Ad_Value;
  
  Ad_Value = Value * I_To_AD_Converter;
  
  HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, Ad_Value);
  HAL_DAC_Start(&hdac1, DAC_CHANNEL_1);
}

void SET_POTENCY(float potency){
   
  float Current_Value;
  float Tension_Value;
    
  Tension_Value = ADC_VALUES[1] * AD_To_Tension_Converter;
  Current_Value = potency/Tension_Value;
    
  SET_CURRENT(Current_Value);
}

void SET_RESISTANCE(float resistance){
   
  float Tension_Value;
  float Current_Value;
    
  Tension_Value = (float)(ADC_VALUES[1]) * AD_To_Tension_Converter;
  Current_Value = Tension_Value/resistance;
    
  SET_CURRENT(Current_Value);
}

void TURN_LOAD_OFF(){
  HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, 0);
  HAL_DAC_Start(&hdac1, DAC_CHANNEL_1);
  }
