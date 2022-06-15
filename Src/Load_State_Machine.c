/**

  ******************************************************************************
   *CORRENTE M�XIMA LIMITADA PELO DAC = 3.3 A
  ******************************************************************************

*/

#include "main.h"
#include "Load.h"
#include "com_protocol.h"
#include "stm32f3xx_hal.h"

/* USER CODE BEGIN PV */
static StatusMessageTypeDef Status_Message;
static StatusMessageTypeDef Status_Message_Transceiver;
Load load = {IDLE, 0, 60000, ON};
uint32_t initial_time = 0;
static uint8_t Message_To_Communication[8] = {0};
/* USER CODE END PV */

/* USER CODE BEGIN PFP */


/* USER CODE END PFP */


/* USER CODE BEGIN 0 */
  
  
static float current_A = 0;
static float potency_W = 0;
static float resistance_Ohms = 0;

static float Current_Seted = 0;
static float Potency_Seted = 0;
static float Resistance_Seted = 0;

  
void Load_State_Machine_Init()
{
  current_A = 0.20;
  potency_W = 0;
  resistance_Ohms = 0;
  Load_Init();
}
/* Para debug no IAR void Load_State_Machine(E_Carga_State Carga_State){ */
void Load_State_Machine()
{
  COM_Protocol_Receive_Communication_Control(&Status_Message, &load);
  if(load.security_time_state == ON) 
  {
    if((load.state_load == IDLE) || ((HAL_GetTick() - initial_time) > load.time_load_on))
    {        
        initial_time = HAL_GetTick();
        TURN_LOAD_OFF();
        load.state_load = IDLE;
    }   
  } 
  if(load.state_load == CURRENT)
  {      
    SET_CURRENT(load.value_load);
    HAL_Delay(2);
    if( Status_Message == OK)
    {
      Current_Seted = GET_CURRENT_SETED();
      if((Current_Seted - (10 * AD_To_mA )) < load.value_load && (Current_Seted + (10 * AD_To_mA)) > load.value_load)
      {  
        Convert_Load_Information_To_Serial_Message(load, Message_To_Communication, Current_Seted);
        COM_Protocol_Transceiver_Communication_Control(&Status_Message_Transceiver, Message_To_Communication);
      }
    else 
    {
      Error_Setting_Value(Message_To_Communication, OUTRANGE_VALUE);
      COM_Protocol_Transceiver_Communication_Control(&Status_Message_Transceiver, Message_To_Communication);
    }
   }
  }
  else if( load.state_load == POTENCY)
  {      
    SET_POTENCY(load.value_load);
    HAL_Delay(2);
    if( Status_Message == OK)
    {
      Potency_Seted = GET_POTENCY_SETED();
      if(Potency_Seted < 2* load.value_load && Potency_Seted  > 0.5 * load.value_load)
      {
        Convert_Load_Information_To_Serial_Message(load, Message_To_Communication, Potency_Seted);
        COM_Protocol_Transceiver_Communication_Control(&Status_Message_Transceiver, Message_To_Communication);
      }
      else 
      {
        Error_Setting_Value(Message_To_Communication, OUTRANGE_VALUE);
        COM_Protocol_Transceiver_Communication_Control(&Status_Message_Transceiver, Message_To_Communication);
      }
    }
  }
  else if( load.state_load == RESISTANCE)
  {      
    SET_RESISTANCE(load.value_load);
    HAL_Delay(2);
    if( Status_Message == OK)
    {
      Resistance_Seted = GET_RESISTANCE_SETED();
      if(Resistance_Seted < 2 * load.value_load && Resistance_Seted > 0.5 * load.value_load)
      { 
        Convert_Load_Information_To_Serial_Message(load, Message_To_Communication, Resistance_Seted);
        COM_Protocol_Transceiver_Communication_Control(&Status_Message_Transceiver, Message_To_Communication);
      }
      else 
      {
        Error_Setting_Value(Message_To_Communication, OUTRANGE_VALUE);
        COM_Protocol_Transceiver_Communication_Control(&Status_Message_Transceiver, Message_To_Communication);
      }
    }
  }  
  else if( load.state_load == ALTERATING_TIME_ON)
  {      
    Convert_Load_Information_To_Serial_Message(load, Message_To_Communication, load.time_load_on);
    if( Status_Message == OK)
      {
        COM_Protocol_Transceiver_Communication_Control(&Status_Message_Transceiver, Message_To_Communication);
      }
  }
  else if(Status_Message == OK)
  {
    load.state_load = IDLE;
    TURN_LOAD_OFF();
    Convert_Load_Information_To_Serial_Message(load, Message_To_Communication, 0);
    COM_Protocol_Transceiver_Communication_Control(&Status_Message_Transceiver, Message_To_Communication);
  }
}




void set_Current_Value(float Value)
{
  if(Value != current_A){
    current_A = Value;
  }
}

void set_Potency_Value(float Value)
{
  if(Value != potency_W){
    potency_W = Value;
  }
}

void set_Resistance_Value(float Value)
{
  if(Value != resistance_Ohms){  
    resistance_Ohms = Value;
  }
}

/* USER CODE END 0 */
  
