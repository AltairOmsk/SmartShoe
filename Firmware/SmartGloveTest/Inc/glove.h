#ifndef _GLOVE_H // ��������� ��������� ��������� ����� ������
#define _GLOVE_H
//******************************************************************************
// ������ include: ����� ������������ ������������ ����� ������������ �������
//******************************************************************************
#include "stm32f1xx_hal.h"
#include "stdio.h"
#include "string.h"

//******************************************************************************
// ������ ����������� ��������
//******************************************************************************
#define BIN 0                   // ������ ������ �������� (������)
#define TXT 1                   // ������ ������ ��������� (��������)   

                                
#define ADXL_ID             0xE5
#define ADXL_CMD_READ       0x80
#define ADXL_CMD_MULTIBYTE  0x40

#define DRATE_800           0x0D
#define DRATE_400           0x0C    
#define DRATE_200           0x0B
#define DRATE_100           0x0A

#define RANGE_2G            0x00
#define RANGE_4G            0x01
#define RANGE_8G            0x02
#define RANGE_16G           0x03
#define FULL_RES_BIT        0x08


                                // �������� �������������
#define ADXL_REG_ID         0x00
#define ADXL_REG_FREQ       0x2C
#define ADXL_INT_ENABLE     0x2E    
#define ADXL_INT_MAP        0x2F
#define ADXL_INT_SOURCE     0x30
#define ADXL_DATAX0         0x32
#define ADXL_DATAX1         0x33
#define ADXL_DATAY0         0x34
#define ADXL_DATAY1         0x35
#define ADXL_DATAZ0         0x36
#define ADXL_DATAZ1         0x37
#define ADXL_FIFO_CTRL      0x38  
#define ADXL_FIFO_STATUS    0x39
#define ADXL_POWER_CTL      0x2D
#define ADXL_DATA_FORMAT    0x31

//******************************************************************************
// ������ ����������� �����
//******************************************************************************
typedef struct {                                                                // �������� ������ ����
  uint8_t  Buf[256];                                                            // �����
  uint8_t  Idx;                                                                 // ������ ������
  uint8_t  New_string;                                                          // ���� "������� ����� ������"
} UART_t;
//******************************************************************************
// ������ ����������� ���������� ����������
//******************************************************************************
extern UART_HandleTypeDef huart1;
extern SPI_HandleTypeDef  hspi1;
extern uint8_t            TxtBinMode;
//******************************************************************************
// ������ ���������� ���������� �������
//******************************************************************************
void glove_init         (void);                                                 // ������������� ���������
void test               (void);                                                 // ���� UART
void get_and_send       (uint8_t Format);                                       // ��������� ������ (����� ������� �������)
void UART1_IT_routine   (void);                                                 // ����� ������ ������� � ����������
void USART1_read_command(void);                                                 // ��������� ����� ������ � ������
//******************************************************************************
// ������ ����������� ��������
//******************************************************************************
#define __LED_ON        HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET)
#define __LED_OFF       HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET)
#define __LED_SW        HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin)


#define __SPI1_CS_0     HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_RESET)
#define __SPI1_CS_1     HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_SET)


#endif // ����������� #endif � ���������� ���������� ���������
//******************************************************************************
// ENF OF FILE
//******************************************************************************