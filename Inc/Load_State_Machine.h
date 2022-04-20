#ifndef __LOAD_STATE_MACHINE_H
#define __LOAD_STATE_MACHINE_H

void Load_State_Machine_Init();

/* Para debug no IAR void Load_State_Machine(E_Carga_State Carga_State);*/

void Load_State_Machine();

void set_Current_Value(float Value);

void set_Potency_Value(float Value);

void set_Resistance_Value(float Value);

#endif /* __LOAD_STATE_MACHINE_H */