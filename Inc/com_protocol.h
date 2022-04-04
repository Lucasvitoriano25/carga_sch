#ifndef __COM_PROTOCOL_H
#define __COM_PROTOCOL_H

#include "stm32f3xx_hal.h"

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
Load *Message_Received);

void COM_Protocol_Transceiver_Communication_Control(StatusMenssageTypeDef *Status_Message, 
uint8_t  *Message_To_Communication);

StatusMenssageTypeDef COM_Protocol_Check_Menssage(uint8_t *data, uint32_t length);

bool CHECKSUM(uint8_t *data, uint8_t length);

Load Convert_Received_Serial_Message_To_Load_State(uint8_t Received_Message[]);

float Convert_Data1_And_Data2_to_uint16_t(uint8_t Received_Datas[]);

uint8_t COM_Protocol_Report_Erro(StatusMenssageTypeDef Erro);
#endif /* __COM_PROTOCOL_H */