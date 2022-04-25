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
static Load load = {IDLE, 0, 0};
static uint32_t original_time = 0; 
static uint32_t initial_time = 0;
static uint8_t Message_To_Communication[8] = {0};
/* USER CODE END PV */

/* USER CODE BEGIN PFP */


/* USER CODE END PFP */


/* USER CODE BEGIN 0 */
  
  
static float current_A = 0;
static float potency_W = 0;
static float resistance_Ohms = 0;
  
/* USER CODE END 0 */
  
void Load_State_Machine_Init()
{
  current_A = 0.20;
  potency_W = 0;
  resistance_Ohms = 0;
  original_time = HAL_GetTick();
  Load_Init();
}
/* Para debug no IAR void Load_State_Machine(E_Carga_State Carga_State){ */
void Load_State_Machine()
{
  COM_Protocol_Receive_Communication_Control(&Status_Message, &load);
  
  if( (Status_Message == OK && load.state_load == IDLE) || ((HAL_GetTick() - initial_time) > 60000) )
  {        
      initial_time = HAL_GetTick();
      TURN_LOAD_OFF();
      load.state_load = IDLE;
  }    
  else if(Status_Message == OK && load.state_load == CURRENT)
  {      
    SET_CURRENT(load.value_state_load);
    initial_time = HAL_GetTick();
    HAL_Delay(1);
    if(GET_CURRENT_SETED() > 0.5*load.value_state_load && GET_CURRENT_SETED() < 1.5*load.value_state_load)
   { 
      Convert_Load_Type_To_Serial_Message(load, Message_To_Communication);
      COM_Protocol_Transceiver_Communication_Control(&Status_Message_Transceiver, Message_To_Communication);
    }
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