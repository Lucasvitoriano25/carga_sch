#ifndef __COM_PROTOCOL_H
#define __COM_PROTOCOL_H

#include "stdbool.h"

#define tamanho 8
typedef enum
{
  Teste = 0,
  Escrita,
  Leitura,
  LerADHipot,
} ComandoTypeDef;

typedef enum
{
  OK = 0,
  ERRO_Length,
  ERRO_CHECKSUM,
  ERRO_EMERGENCY,
  BUSY,
  TIMEOUT,
} StatusMenssageTypeDef;

typedef struct
{
  uint8_t          Identificator;
  ComandoTypeDef   Comando;
  uint8_t          Subcomando;
  uint8_t          Data_1;
  uint8_t          Data_2;
  uint8_t          Data_3;
  uint8_t          Data_4;
  uint8_t          CHECKSUM;  
} MensagemTypeDef;

void COM_Protocol_Receive_Communication_Control(StatusMenssageTypeDef *Status_Message, 
E_Carga_State *Message_Received);
StatusMenssageTypeDef COM_Protocol_Check_Menssage(uint8_t *data, uint32_t length);
bool CHECKSUM(uint8_t *data, uint8_t length);
E_Carga_State Convert_Received_Serial_Message_To_Load_State(uint_t Received_Message);

#endif /* __COM_PROTOCOL_H */