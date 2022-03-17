#include "main.h"
#include "stm32f1xx_hal.h"
#include "stdbool.h"

#include "com_protocol.h"
#include "com_HA.h"
//uint8_t mensagem[20];

extern UART_HandleTypeDef huart1;
uint16_t teste, teste2;
uint8_t Mensagem[tamanho];
E_Carga_State Returned_Load_State;
uint8_t Received_UART_Message[tamanho];

void COM_Protocol_Receive_Communication_Control(StatusMenssageTypeDef *Status_Message, 
Load *Message_Received)
{
  #define Attempts 3
  uint8_t i=0;
  
  *Status_Message  = TIMEOUT;
  
  while(((*Status_Message != OK) && i < Attempts)
  {
    if (HAL_UART_Receive(&USART_MICRO_COMMUNICATION, Received_UART_Message, tamanho, 100) != HAL_OK)
    {
      i++;
      *Status_Message = TIMEOUT;
    }
    else
    {
      if((*Status = COM_Protocol_Check_Menssage(Received_UART_Message, tamanho)) != OK)
      {
        i++;
      }
      else
      {
        *Message_Received = Convert_Received_Serial_Message_To_Load_State(Received_UART_Message);
        *Status_Message = OK;
      }
    }
  }
}

void COM_Protocol_Transceiver_Communication_Control(StatusMenssageTypeDef *Status_Message, 
uint8_t *Message_To_Communication)
{
  #define Attempts 3
  uint8_t i=0;
  
  *Status_Message  = TIMEOUT;
  
  while(((*Status_Message != OK) && i < Attempts)
  {
    if (HAL_UART_Transmit(&USART_MICRO_COMMUNICATION, Message_To_Communication, tamanho, 100) != HAL_OK)
    {
      i++;
      *Status_Message = TIMEOUT;
    }
    else
    {
      if((*Status = COM_Protocol_Check_Menssage(Message_To_Communication, tamanho)) != OK)
      {
        i++;
      }
      else
      {
        *Status_Message = OK;
      }
    }
  }
}


StatusMenssageTypeDef COM_Protocol_Check_Menssage(uint8_t *data, uint32_t length)
{
  if(length != 8)                                   //Recebido com numero incorreto de bytes
  {
    return ERRO_Length;
  }
  
  if(CHECKSUM(data, length) != true)                 //Teste da integridade da mensagem
  {
    return ERRO_CHECKSUM;
  }
  
  if(data[1]== 0xFF)
  {
    return ERRO_Length;
  }
  
  if(data[1]== 0xFE)
  {
    return ERRO_CHECKSUM;
  }
  
  if(data[1]== 0xFD)
  {
    return ERRO_EMERGENCY;
  }    
  
  if(data[1]== 0xFC)
  {
    return BUSY;
  }
  
  return OK;
} 

bool CHECKSUM(uint8_t *data, uint8_t length)
{
  uint8_t soma=0, i=0;
  
  for(i=0; i<length-1; i++)
  {
    soma += data[i];
  }
  if(soma == data[i])
  {
    return true;
  }
  else
  {
    return false;
  }
}

Load Convert_Received_Serial_Message_To_Load_State(uint8_t Received_Message)
{
  Load Load_Conversion_Aux = {IDLE, 0, 0};
  uint16_t Value_Conversion_Aux = 0;
  if(Received_Message[0] == 0x06 ){
    Load_Conversion_Aux.state = CURRENT;
    Value_Conversion_Aux = Received_Message[2];
    Value_Conversion_Aux << 8;
    Value_Conversion_Aux = Value_Conversion_Aux + Received_Message[3];
    Load_Conversion_Aux.value = Value_Conversion_Aux;
    }
  /*
  else if(Received_Message == ){
    Returned_Load_State = POTENCY;
    return Returned_Load_State;
  }
  else if(Received_Message == ){
    Returned_Load_State = RESISTANCE;
    return Returned_Load_State;
  }
  else{
    Returned_Load_State = IDLE;
    return Returned_Load_State;
  }
  */
}