#include "main.h"
#include "stm32f3xx_hal.h"
#include "stdbool.h"

#include "com_protocol.h"
#include "com_HA.h"
//uint8_t mensagem[20];

extern UART_HandleTypeDef huart1;
uint16_t teste, teste2;
uint8_t Mensagem[tamanho];
uint8_t Status_Message_Receiver;
uint8_t Received_UART_Message[tamanho];
E_Carga_State Returned_Load_State;
StatusMenssageTypeDef Status_Message_Transceiver;


void COM_Protocol_Receive_Communication_Control(StatusMenssageTypeDef *Status_Message, 
Load *Message_Received)
{
  #define Attempts 3
  uint8_t i=0;
  
  *Status_Message  = TIMEOUT;
  
  while((*Status_Message != OK) && i < Attempts)
  { 
    __HAL_UART_CLEAR_OREFLAG(&huart1);

    __HAL_UART_CLEAR_NEFLAG(&huart1);
    
    if((HAL_UART_Receive(&huart1, Received_UART_Message, sizeof(Received_UART_Message),100)) != HAL_OK)
    {
      i++;
      *Status_Message = TIMEOUT;
    }
    else
    {
      if((*Status_Message = COM_Protocol_Check_Menssage(Received_UART_Message, tamanho)) != OK)
      {
        i++;
        Status_Message_Receiver = COM_Protocol_Report_Erro(*Status_Message);
        COM_Protocol_Transceiver_Communication_Control(&Status_Message_Transceiver, &Status_Message_Receiver);
      }
      else
      {
        Status_Message_Receiver = COM_Protocol_Report_Erro(*Status_Message);
        COM_Protocol_Transceiver_Communication_Control(&Status_Message_Transceiver, &Status_Message_Receiver);
        *Message_Received = Convert_Received_Serial_Message_To_Load_State(Received_UART_Message);
      }
    }
  }
}

void COM_Protocol_Transceiver_Communication_Control(StatusMenssageTypeDef *Status_Message, 
uint8_t  *Message_To_Communication)
{
  #define Attempts 3
  uint8_t i=0;
  
  *Status_Message  = TIMEOUT;
  
  while((*Status_Message != OK) && i < Attempts)
  {
    if (HAL_UART_Transmit(&USART_MICRO_COMMUNICATION, Message_To_Communication, tamanho, 100) != HAL_OK)
    {
      i++;
    }
    else
    {
      if((*Status_Message = COM_Protocol_Check_Menssage(Message_To_Communication, tamanho)) != OK)
      {
        i++;
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
  
  else if(CHECKSUM(data, length) != true)                 //Teste da integridade da mensagem
  {
    return ERRO_CHECKSUM;
  }
  
  else if(data[1]== 0xFF)
  {
    return ERRO_Length;
  }
  
  else if(data[1]== 0xFE)
  {
    return ERRO_CHECKSUM;
  }
  
  else if(data[1]== 0xFD)
  {
    return ERRO_EMERGENCY;
  }    
  
  else if(data[1]== 0xFC)
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

uint8_t COM_Protocol_Report_Erro(StatusMenssageTypeDef Erro)
{
  switch(Erro)
  {
  case ERRO_Length:
    return(0xFF);
    break;
    
  case ERRO_CHECKSUM:
    return(0xFE);
    break;
    
  case ERRO_EMERGENCY:
    return(0xFD);
    break;
    
  case BUSY:
    return(0xFC);
    break;
    
  case TIMEOUT:
    return(0xFB);
    break;
  }
}

Load Convert_Received_Serial_Message_To_Load_State(uint8_t Received_Message[])
{
  Load Load_Conversion_Aux = {IDLE, 0, 0};
  if(Received_Message[1] == 0x06 ){
    Load_Conversion_Aux.state_load = CURRENT;
    Load_Conversion_Aux.value_state_load = Convert_Data1_And_Data2_to_uint16_t(Received_Message);
    Load_Conversion_Aux.time_load_on = Received_Message[4];
    }
  return Load_Conversion_Aux;  
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

float Convert_Data1_And_Data2_to_uint16_t(uint8_t Received_Datas[])
{
  float Value_Conversion_Aux = 0;
  Value_Conversion_Aux = (((uint16_t) Received_Datas[3] << 8) | Received_Datas[4]);
  return (Value_Conversion_Aux * 0.001);
}