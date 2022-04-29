#ifndef __LOAD_H
#define __LOAD_H

void Load_Init();
void SET_CURRENT(float value);
void SET_POTENCY(float potency);
void SET_RESISTANCE(float resistance);
void TURN_LOAD_OFF();
float GET_CURRENT_SETED();
float GET_POTENCY_SETED();
float GET_RESISTANCE_SETED();

#endif /* __LOAD_H */