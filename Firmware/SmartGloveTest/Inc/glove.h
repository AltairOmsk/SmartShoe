#ifndef _GLOVE_H // Блокируем повторное включение этого модуля
#define _GLOVE_H
//******************************************************************************
// Секция include: здесь подключаются заголовочные файлы используемых модулей
//******************************************************************************
#include "stm32f1xx_hal.h"
#include "stdio.h"
#include "string.h"

//******************************************************************************
// Секция определения констант
//******************************************************************************
#define BIN 0                   // Формат вывода бинарный (быстро)
#define TXT 1                   // Формат вывода текстовый (медленно)   

                                
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


                                // Регистры акселерометра
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
// Секция определения типов
//******************************************************************************
typedef struct {                                                                // Контекст буфера УАРТ
  uint8_t  Buf[256];                                                            // Буфер
  uint8_t  Idx;                                                                 // Индекс буфера
  uint8_t  New_string;                                                          // Флаг "Принята новая строка"
} UART_t;
//******************************************************************************
// Секция определения глобальных переменных
//******************************************************************************
extern UART_HandleTypeDef huart1;
extern SPI_HandleTypeDef  hspi1;
extern uint8_t            TxtBinMode;
//******************************************************************************
// Секция прототипов глобальных функций
//******************************************************************************
void glove_init         (void);                                                 // Инициализация периферии
void test               (void);                                                 // Тест UART
void get_and_send       (uint8_t Format);                                       // Отправить данные (самая главная функция)
void UART1_IT_routine   (void);                                                 // Прием нового символа в прерывании
void USART1_read_command(void);                                                 // Обработка новой строки в буфере
//******************************************************************************
// Секция определения макросов
//******************************************************************************
#define __LED_ON        HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET)
#define __LED_OFF       HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET)
#define __LED_SW        HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin)


#define __SPI1_CS_0     HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_RESET)
#define __SPI1_CS_1     HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_SET)


#endif // Закрывающий #endif к блокировке повторного включения
//******************************************************************************
// ENF OF FILE
//******************************************************************************