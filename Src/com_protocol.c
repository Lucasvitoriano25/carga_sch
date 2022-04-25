#include "main.h"
#include "stm32f3xx_hal.h"
#include "stdbool.h"
#include <string.h> 

#include "com_protocol.h"
#include "com_HA.h"
//uint8_t message[20];

extern UART_HandleTypeDef huart1;

static uint8_t Status_Message_Receiver[tamanho];
static uint8_t Received_UART_Message[tamanho];
static StatusMessageTypeDef Status_Message_Transceiver;


void COM_Protocol_Receive_Communication_Control(StatusMessageTypeDef *Status_Message, 
Load *Message_Received)
{
  #define Attempts 3
  uint8_t i=0;
  
  *Status_Message  = TIMEOUT;
  
  while((*Status_Message != OK) && i < Attempts)
  { 

    if((HAL_UART_Receive(&USART_MICRO_COMMUNICATION, Received_UART_Message, tamanho,200)) != HAL_OK)
    {    
      i++;
      *Status_Message = TIMEOUT;
      COM_Protocol_Reset_Serial();
    }
    else
    {
      if((*Status_Message = COM_Protocol_Check_Message(Received_UART_Message, tamanho)) != OK)
      {
        Status_Message_Receiver[2] = COM_Protocol_Report_Erro(*Status_Message);
        COM_Protocol_Transceiver_Communication_Control(&Status_Message_Transceiver, Status_Message_Receiver);
        COM_Protocol_Reset_Serial();
      }
      else
      {
        memcpy(Status_Message_Receiver, Received_UART_Message, sizeof(Received_UART_Message));
        *Message_Received = Convert_Received_Serial_Message_To_Load_State(Received_UART_Message);
      }
    }
  }
}

void COM_Protocol_Transceiver_Communication_Control(StatusMessageTypeDef *Status_Message, 
uint8_t  *Message_To_Communication)
{
  #define Attempts 3
  uint8_t i=0;
  
  *Status_Message  = TIMEOUT;
  
  while((*Status_Message != OK) && i < Attempts)
  {
    if ((HAL_UART_Transmit(&USART_MICRO_COMMUNICATION, Message_To_Communication, tamanho,10)) != HAL_OK)
    {
      i++;
    }
    else
    {
      *Status_Message  = OK;
    }
  }
}


StatusMessageTypeDef COM_Protocol_Check_Message(uint8_t *data, uint32_t length)
{
  if(length != 8)                                   //Recebido com numero incorreto de bytes
  {
    return ERRO_Length;
  }
  
  else if(CHECKSUM(data, length) != true)                 //Teste da integridade da message
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

uint8_t COM_Protocol_Report_Erro(StatusMessageTypeDef Erro)
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
  return 0;
}

void COM_Protocol_Reset_Serial()
{
 uint8_t dump[tamanho] = {0}; 
 
 HAL_UART_Receive(&USART_MICRO_COMMUNICATION, dump, tamanho, 0);

 HAL_UART_DeInit(&USART_MICRO_COMMUNICATION);

 HAL_UART_Init(&USART_MICRO_COMMUNICATION);
}

Load Convert_Received_Serial_Message_To_Load_State(uint8_t *Received_Message)
{
  Load Load_Conversion_Aux = {IDLE, 0, 0};
  if(Received_Message[1] == 0x06 ){
    Load_Conversion_Aux.state_load = CURRENT;
    Load_Conversion_Aux.value_state_load = Convert_Data1_And_Data2_to_uint16_t(Received_Message);
    // se quiser desligar depois de um tempo = Load_Conversion_Aux.time_load_on = Received_Message[4];
    }
  return Load_Conversion_Aux;  
}

void Convert_Load_Type_To_Serial_Message(Load load_to_be_converted, uint8_t *Message_Converted)
{
  if(load_to_be_converted.state_load == CURRENT)
  {
    uint8_t MSB = 0;
    uint8_t LSB = 0;
    Message_Converted[1] = 6;
    Convert_float_to_2_uint8(load_to_be_converted.value_state_load,&LSB,&MSB);
    Message_Converted[3] = MSB;
    Message_Converted[4] = LSB;
    Message_Converted[7] =  Message_Converted[6] + Message_Converted[5] + Message_Converted[4] + Message_Converted[3] +
      Message_Converted[2]+ Message_Converted[1]+ Message_Converted[0];
    
    
  }
}

float Convert_Data1_And_Data2_to_uint16_t(uint8_t Received_Datas[])
{
  float Value_Conversion_Aux = 0;
  Value_Conversion_Aux = (((uint16_t) Received_Datas[3] << 8) | Received_Datas[4]);
  return (Value_Conversion_Aux * 0.001);
}

void Convert_float_to_2_uint8(float message_to_be_converted, uint8_t *LSB,uint8_t *MSB)
{
  uint16_t aux =0;
  aux = (uint16_t) (message_to_be_converted*1000);
  *LSB = (uint8_t) aux; // Get lower byte of 16-bit var
  *MSB = (uint8_t)(aux >> 8); // Get upper byte of 16-bit var
}