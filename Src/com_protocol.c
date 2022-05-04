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
static uint8_t Message_To_Communication[8] = {0};


void COM_Protocol_Receive_Communication_Control(StatusMessageTypeDef *Status_Message, 
Load *Message_Received)
{
  #define Attempts 3
  uint8_t i=0;
  
  *Status_Message  = TIMEOUT;
  
  while((*Status_Message != OK) && i < Attempts)
  { 

    if((HAL_UART_Receive(&USART_MICRO_COMMUNICATION, Received_UART_Message, tamanho,100)) != HAL_OK)
    {    
      i++;
      *Status_Message = TIMEOUT;
      COM_Protocol_Reset_Serial();
    }
    else
    {
      if((*Status_Message = COM_Protocol_Check_Message(Received_UART_Message, tamanho)) != OK)
      {
        Create_Error_Message(Status_Message_Receiver, Status_Message);
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
    if(Received_Message[2] == 1){
      Load_Conversion_Aux.state_load = CURRENT;
      Load_Conversion_Aux.value_state_load = Convert_Data1_And_Data2_to_uint16_t(Received_Message);
      // se quiser desligar depois de um tempo = Load_Conversion_Aux.time_load_on = Received_Message[4];
      }
    else if(Received_Message[2] == 2){
      Load_Conversion_Aux.state_load = POTENCY;
      Load_Conversion_Aux.value_state_load = Convert_Data1_And_Data2_to_uint16_t(Received_Message);
      // se quiser desligar depois de um tempo = Load_Conversion_Aux.time_load_on = Received_Message[4];
      }
    else if(Received_Message[2] == 3){
      Load_Conversion_Aux.state_load = RESISTANCE;
      Load_Conversion_Aux.value_state_load = Convert_Data1_And_Data2_to_uint16_t(Received_Message);
      // se quiser desligar depois de um tempo = Load_Conversion_Aux.time_load_on = Received_Message[4];
      }
    else
    {
      Error_Setting_Value(Message_To_Communication, INCONSISTENCY_IN_SETED_MODE);
      COM_Protocol_Transceiver_Communication_Control(&Status_Message_Transceiver, Message_To_Communication);
    }
  }
  return Load_Conversion_Aux;  
}

void Convert_Load_Type_To_Serial_Message(Load load_to_be_converted, uint8_t *Message_Converted)
{
  uint8_t MSB = 0;
  uint8_t LSB = 0;
  Message_Converted[1] = 6;
  Message_Converted[2] = Convert_Load_Type_To_uint8(load_to_be_converted);
  Convert_float_to_2_uint8(load_to_be_converted.value_state_load,&LSB,&MSB);
  Message_Converted[3] = MSB;
  Message_Converted[4] = LSB;
  Create_Checksum(Message_Converted);
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

int Convert_Load_Type_To_uint8(Load load_to_be_converted)
{ 
  int converted_value = 0;
  if(load_to_be_converted.state_load == CURRENT){
    converted_value = 1;
  }
  else if(load_to_be_converted.state_load == POTENCY){
    converted_value = 2;
  }
  else if(load_to_be_converted.state_load == RESISTANCE){
    converted_value = 3;
  }
  return converted_value;
}

void Create_Error_Message(uint8_t * error_message, StatusMessageTypeDef *data)
{ 
  memset(error_message, 0, sizeof error_message);
  error_message[2] = COM_Protocol_Report_Erro(*data);
  error_message[7] = COM_Protocol_Report_Erro(*data);
}

void Create_Checksum(uint8_t * vector)
{
  vector[7] = vector[0] + vector[1] + vector[2] + vector[3] + vector[4] + vector[5] + vector[6];
  
}

void Error_Setting_Value(uint8_t * error_message,StatusMessageTypeDef Error_Type)
{
  memset(error_message, 0, sizeof error_message);
  if(Error_Type == OUTRANGE_VALUE){
    error_message[2] = 0xFA;
  }
  if(Error_Type == INCONSISTENCY_IN_SETED_MODE)
  {
    error_message[2] = 0xF9;

  }    
  Create_Checksum(error_message);

}