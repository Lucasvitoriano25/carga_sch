/*

  ******************************************************************************
   *CORRENTE M�XIMA LIMITADA PELO DAC = 3.3 A
  ******************************************************************************

*/

#include "main.h"
#include "stm32f3xx_hal.h"
#include "stdbool.h"

extern uint16_t ADC_VALUES[];
    
extern ADC_HandleTypeDef hadc1;
extern DMA_HandleTypeDef hdma_adc1;

extern DAC_HandleTypeDef hdac1;

extern TIM_HandleTypeDef htim1;






/* USER CODE BEGIN PFP */
void Load_Init();
void SET_CURRENT(float value);
void SET_POTENCY(float potency);
void SET_RESISTANCE(float resistance);
/* USER CODE END PFP */

void Load_Init()
{
  SET_CURRENT(0);
}

void SET_CURRENT(float Value){
  if(Value < 3.3){
    int Ad_Value = 0;
    
    Ad_Value = Value * I_To_AD;
    
    HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, Ad_Value);
    HAL_DAC_Start(&hdac1, DAC_CHANNEL_1);
  }
  else
  {} 
  //Value bigger than dac max value in Volts of micro
}

void SET_POTENCY(float potency){
   
  float Current_Value = 0;
  float Tension_Value = 0;
    
  Tension_Value = ADC_VALUES[1] * AD_To_Tension;
  Current_Value = potency/Tension_Value;
    
  SET_CURRENT(Current_Value);
}

void SET_RESISTANCE(float resistance){
   
  float Tension_Value = 0;
  float Current_Value = 0;
    
  Tension_Value = (float)(ADC_VALUES[1]) * AD_To_Tension;
  Current_Value = Tension_Value/resistance;
    
  SET_CURRENT(Current_Value);
}

void TURN_LOAD_OFF(){
  HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, 0);
  HAL_DAC_Start(&hdac1, DAC_CHANNEL_1);
  }

float GET_CURRENT_SETED()
{
  float Current_Value = 0;
  Current_Value = ADC_VALUES[0] * AD_To_mA;

  return Current_Value;
}

float GET_POTENCY_SETED()
{
  float Current_Value = 0;
  float Tension_Value = 0;
  float Potency_Value = 0;
  Tension_Value = ADC_VALUES[1] * AD_To_Tension;
  Current_Value = ADC_VALUES[0] * AD_To_mA;
  Potency_Value = Current_Value * Tension_Value;
  
  return Potency_Value;
}

float GET_RESISTANCE_SETED()
{
  float Tension_Value = 0;
  float Current_Value = 0;
  float Resistance_Value = 0;
    
  Tension_Value = ADC_VALUES[1] * AD_To_Tension;
  Current_Value = ADC_VALUES[0] * AD_To_mA;
  Resistance_Value = Tension_Value/Current_Value;

  return Resistance_Value;
}

