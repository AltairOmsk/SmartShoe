//******************************************************************************
// ������ include: ����� ������������ ������������ ���� � ������
//******************************************************************************
#include "glove.h" // �������� ���� ��������� ��� ������ ������
//******************************************************************************
// ������ ����������� ����������, ������������ � ������
//******************************************************************************
//------------------------------------------------------------------------------
// ����������
//------------------------------------------------------------------------------
uint8_t ADXL_Data[120];                                                         // ����� ��� ������ �������� ���������� ���������
uint8_t UART_Buf[129];                                                          // ����� ��� ������������ ��������� � ���� - ������� ����� ��������� � ���������
UART_t  U1;                                                                     // ����� ����1 ��� ������ � Bluetooth
//------------------------------------------------------------------------------
// ���������
//------------------------------------------------------------------------------
//static char LocalVar1;
//static char LocalVar2;

//******************************************************************************
// ������ ���������� ��������� �������
//******************************************************************************
static void    adxl_write_byte (uint8_t Addres, uint8_t Data);                  // �������� � ������� ���� ����
static uint8_t adxl_read_byte  (uint8_t Addres);                                // ��������� �� �������� ���� ����
static void adxl_read_FIFO  (uint8_t *Buf, uint8_t Cnt);                        // ��������� FIFO � ��������� Cnt ���

//******************************************************************************
// ������ �������� ������� (������� ����������, ����� ���������)
//******************************************************************************



//******************************************************************************
//   ��������� ������ (����� ������� �������)
//******************************************************************************
/*
- ��������� ���������� ���� ���������� (� FIFO ���� ��� ������)
- ���� ����� ���������� ������� - ���������� FIFO, ���� ��� - �������
- ����������� ������ ��� ������
- ���������� � UART
- ������� ������ ���������� ���� ����������
*/
void get_and_send (uint8_t Format){
int16_t Tmp;
static uint8_t MSG_cnt=0;
char    Str[256];


    Tmp = HAL_GPIO_ReadPin(INT_1_GPIO_Port, INT_1_Pin);                         // �������� ������� ���������� �� Trigger FIFO
    
    if (Tmp) {
      adxl_read_FIFO  (ADXL_Data, 20);                                          // �������� 20 �������� �� FIFO
    }
    else {
      __no_operation();
      return;
    }
    
    
    switch (Format) {
        case BIN:                                                               // ������� �������� �����
          UART_Buf[0] = 's';                                                    // ��������� ���������
          UART_Buf[1] = 't';            
          UART_Buf[2] = 'a';
          UART_Buf[3] = 'r';
          UART_Buf[4] = 't';
          UART_Buf[5] = '0';                                                    // ������ ���������
          UART_Buf[6] = MSG_cnt++;                                              // ��������� ���������������� ����� ���������
          
          memcpy((UART_Buf + 7), ADXL_Data, 120);                               // ������
          
          UART_Buf[127] = 0x0A; // CR
          UART_Buf[128] = 0x0D; // LF
          
          HAL_UART_Transmit_DMA(&huart1, UART_Buf, sizeof(UART_Buf));
          
          while(HAL_GPIO_ReadPin(INT_1_GPIO_Port, INT_1_Pin));                  // ������� ������ ������� ����������
          break;
          
        case TXT:                                                               // ��������� ��������� �����
          sprintf(Str, "%d; %d; %d\r\n", *((int16_t*)ADXL_Data+0), *((int16_t*)ADXL_Data+2), *((int16_t*)ADXL_Data+4));
          HAL_UART_Transmit(&huart1, (uint8_t*)Str, (uint8_t)strlen(Str),100);
          HAL_Delay(100);
          break;
            
        default:
          break;
        };
    
    
    
    
    
  

//  Tmp = adxl_read_byte  (ADXL_REG_ID);
//  printf("DEVID = %d\r\n", Tmp);
//
//  Tmp = adxl_read_byte  (ADXL_FIFO_CTRL);
//  printf("FIFO_CTRL = %d\r\n", Tmp); 
//    
//  Tmp = adxl_read_byte  (ADXL_FIFO_STATUS);
//  printf("FIFO_STATUS = %d\r\n", Tmp);  
//  
//  Tmp = adxl_read_byte  (ADXL_INT_ENABLE);
//  printf("INT_ENABLE = %d\r\n", Tmp);
//  
//  Tmp = adxl_read_byte  (ADXL_INT_MAP);
//  printf("INT_MAP = %d\r\n", Tmp);
//  
//  Tmp = adxl_read_byte  (ADXL_INT_SOURCE);
//  printf("INT_SOURCE = %d\r\n", Tmp);
  
//  Tmp =  adxl_read_byte  (ADXL_DATAX1);
//  Tmp <<= 8;
//  Tmp += adxl_read_byte  (ADXL_DATAX0);
  //printf("DEVID2 = %d\r\n", Tmp);
  //printf("%d\r\n", Tmp);
  
  
//  sprintf(Str, "%d\r\n", Tmp);
//  HAL_UART_Transmit(&huart1, (uint8_t*)Str, (uint8_t)strlen(Str),100);

  
//  HAL_Delay(1000);


}




//******************************************************************************
//   ����� ������ ������� � ����������
//******************************************************************************
void UART1_IT_routine (void) {

  U1.Buf[U1.Idx] = USART1->DR;                                                  // �������� ������ �� ���� � ��

  if (U1.Buf[U1.Idx] == 0x0A) {                                                 // ���� ��������� �������� ������ - LF
    if (U1.Buf[U1.Idx - 1] == 0x0D) {                                           // � ���������� CR,
      
      if (U1.Idx < 5) {                                                         // ������ �� ��������� ������� �������� ���������
        U1.Idx = 0;
        return;
      }

      U1.Buf[U1.Idx + 1] = 0;                                                   // ���������� ���������� ������ ���������  
      U1.Idx = 0;   
      U1.New_string = 1;                                                        // ������ ���� ������ ������ ���������      
      return;                                                                   // ��� �� �� ���������� ������ ������ � ����� ����������� ����������

    }
  } 
  U1.Idx++;
}




//******************************************************************************
// ��������� ����� ������ � ������
//******************************************************************************
void USART1_read_command (void){
  

  if (U1.New_string == 0)    return;                                            // ������� ���� ��� ����� ������                 
  
  //----------------------------------------------------------------------------
  U1.New_string = 0;                                                            // �������� ����� ������
  
char Tmp_buf[12];
char TmpStr[64];
char *Ptr;
uint8_t i, ii;
int32_t Tmp32;


  //----------------------------------------------------------------------------
  Ptr=strstr(U1.Buf, "AT+TXTMODE");                                             // AT+TXTMODE
  if (Ptr){
    TxtBinMode = TXT;
    sprintf(TmpStr, "+TXTMODE\r\n");
    HAL_UART_Transmit(&huart1, (uint8_t*)TmpStr, strlen(TmpStr), 500);
  }
    
    
  //----------------------------------------------------------------------------
  Ptr=strstr(U1.Buf, "AT+BINMODE");                                             // AT+BINMODE
  if (Ptr){
    TxtBinMode = BIN;
    sprintf(TmpStr, "+BINMODE\r\n");
    HAL_UART_Transmit(&huart1, (uint8_t*)TmpStr, strlen(TmpStr), 500);
  }
  
  
  
  
  //----------------------------------------------------------------------------
  

  for (ii=0;ii<255;ii++) U1.Buf[ii]=0;                                        // �������� ������ ����� �������
  
}


//******************************************************************************
//   �������� ������� UART
//******************************************************************************
void test (void){
static uint8_t i=0;
char    Str[256];
    i++;
    sprintf(Str, "Test %d\r\n", i);
    HAL_UART_Transmit_DMA(&huart1, (uint8_t*)Str, (uint8_t)strlen(Str));
}

//******************************************************************************
//   ������������� ��������� ��������
//******************************************************************************
void glove_init (void){
    HAL_GPIO_WritePin(BT_RESET_GPIO_Port, BT_RESET_Pin, GPIO_PIN_SET);          // ����� BT
    HAL_Delay(10);
    HAL_GPIO_WritePin(BT_RESET_GPIO_Port, BT_RESET_Pin, GPIO_PIN_RESET);
    HAL_Delay(100);
  
    __SPI1_CS_1;
    HAL_Delay(1);
    adxl_write_byte (ADXL_POWER_CTL, (1<<3));                                   // ��������� ������ ���������
    adxl_write_byte (ADXL_REG_FREQ, DRATE_400);                                 // �������� ���������
    adxl_write_byte (ADXL_DATA_FORMAT, RANGE_16G | FULL_RES_BIT);               // �������� +/-16G ��� ������ ����������
    adxl_write_byte (ADXL_INT_ENABLE, (1<<1));                                  // Watermark Enable
    adxl_write_byte (ADXL_FIFO_CTRL, 0xD4);         // 1100 1010 ����� FIFO "�������", �� INT1, ����� 20 �������
    HAL_Delay(1);
}

//******************************************************************************
//   �������� � ������� ���� ����
//******************************************************************************
static void    adxl_write_byte (uint8_t Addres, uint8_t Data){
uint8_t Tmp[2];

    Tmp[0] = 0;
    Tmp[1] = 0;
    
    if (Addres > 0x39) while(1);                                                // ����� �� ����� ���� ������ 0�39
    
    Tmp[0] = Addres;
    Tmp[1] = Data;
    
    __SPI1_CS_0;
    HAL_SPI_Transmit(&hspi1, Tmp, 2, 10);                                    // ����� � �����
    __SPI1_CS_1;
    
}                  

//******************************************************************************
//   ��������� �� �������� ���� ����
//******************************************************************************
static uint8_t adxl_read_byte  (uint8_t Addres){
uint8_t Tmp = 0;
    
    if (Addres > 0x39) while(1);                                                // ����� �� ����� ���� ������ 0�39
    
    Tmp  = Addres | ADXL_CMD_READ;                                              // ���������� ���� ������
    
    __SPI1_CS_0;
    HAL_SPI_Transmit(&hspi1, &Tmp, 1, 10);                                    // ����� � �����
    
    HAL_SPI_Receive (&hspi1, &Tmp, 1, 10);                                    // ������
    __SPI1_CS_1;
    
    return Tmp;
}                       


//******************************************************************************
//   ��������� FIFO � ��������� Cnt ���
//******************************************************************************
static void adxl_read_FIFO  (uint8_t *Buf, uint8_t Cnt){
uint8_t Tmp = 0, i;

    for (i=0;i<Cnt;i++){
        
        Tmp  = ADXL_DATAX0 | ADXL_CMD_READ | ADXL_CMD_MULTIBYTE;                // ���������� ���� ������

        __SPI1_CS_0;
        HAL_SPI_Transmit(&hspi1, &Tmp, 1, 10);                                  // ����� � �����

        HAL_SPI_Receive (&hspi1, Buf + (6 * i), 6, 10);                         // �������� 6 ���� ������
        __SPI1_CS_1;
    }

} 





//******************************************************************************
// ENF OF FILE
//******************************************************************************