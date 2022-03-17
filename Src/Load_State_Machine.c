/**

  ******************************************************************************
   *CORRENTE M�XIMA LIMITADA PELO DAC = 3.3 A
  ******************************************************************************

*/

#include "main.h"
#include "Load.h"
#include "com_protocol.h"

/* USER CODE BEGIN PV */
StatusMenssageTypeDef Status_Message;
Load load;
uint32_t original_time = Hal_getTick();
uint32_t time_passed = 0;
/* USER CODE END PV */

/* USER CODE BEGIN PFP */


/* USER CODE END PFP */


/* USER CODE BEGIN 0 */
  
  
  float current_A, potency_W, resistance_Ohms;
  
/* USER CODE END 0 */
  
void Load_State_Machine_Init(){
  current_A = 0.20;
  potency_W = 0;
  resistance_Ohms = 0;
  Carga_Init();
}
/* Para debug no IAR void Load_State_Machine(E_Carga_State Carga_State){ */
void Load_State_Machine(){

  COM_Protocol_Receive_Communication_Control(&Status_Message, &load);

  if(Status_Mensage == OK){
  time_passed = Hal_getTick;
  SET_CURRENT(load.value_state_load);
  }

  if(hal_getTick - time_passed > load.time_load_on){
    SET_CURRENT(0);

  }
   switch(New_Message_Received){
      
      case CURRENT:
        SET_CURRENT(current_A);
        break;
        
      case POTENCY:
        SET_POTENCY(potency_W);
        break;
        
      case RESISTANCE:
         SET_RESISTANCE(resistance_Ohms);
        break;    
    }
  }
}

void set_Current_Value(float Value){
  if(Value != current_A){
    current_A = Value;
  }
}

void set_Potency_Value(float Value){
  if(Value != potency_W){
    potency_W = Value;
  }
}

void set_Resistance_Value(float Value){
  if(Value != resistance_Ohms){  
    resistance_Ohms = Value;
  }
}