/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/

#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "stm32f4xx.h"
#include "FreeRTOS.h"
#include "task.h"


static void prvSetupUart(void);
static void prvSetupHardware(void);
void printmsg(char *msg);
char usr_msg[250];

void led_task_handler(void *params);
void button_task_handler(void *params);


int main(void)
{

	//1. Reset the RCC clock configuration to the default reset state.
	//HSI on, PLL off, HSE off, CPU clock = 16MhZ
	RCC_DeInit();

	//2. Update the SystemCoreClock variable
	SystemCoreClockUpdate();

	prvSetupHardware();

	//Create tasks
	xTaskCreate(led_task_handler,
				"LED task",
				configMINIMAL_STACK_SIZE,
				NULL,
				1,
				NULL);

	xTaskCreate(button_task_handler,
				"Button task",
				configMINIMAL_STACK_SIZE,
				NULL,
				1,
				NULL);

	//Start scheduler
	vTaskStartScheduler();

	for(;;);
}

void led_task_handler(void *params){

	while(1){

	}
}

void button_task_handler(void *params){

	while(1){

	}
}

static void prvSetupHardware(void){

	//Setup UART
	prvSetupUart();

}


static void prvSetupUart(void){

	GPIO_InitTypeDef gpio_uart_pins;
		USART_InitTypeDef uart2_init;

		//Enable the UART2 and GPIOA peripheral clock
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

		//PA2 is Tx. PA3 is Rx
		//Alternate function configuration of MCU to behave as UART2

		memset(&gpio_uart_pins, 0, sizeof(gpio_uart_pins));

		gpio_uart_pins.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
		gpio_uart_pins.GPIO_Mode = GPIO_Mode_AF;
		gpio_uart_pins.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_Init(GPIOA, &gpio_uart_pins);

		//AF mode setting
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2); //PA2
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2); //PA3

		//UART parameter initializations

		memset(&uart2_init, 0, sizeof(uart2_init));

		uart2_init.USART_BaudRate = 115200;
		uart2_init.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		uart2_init.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
		uart2_init.USART_Parity = USART_Parity_No;
		uart2_init.USART_StopBits = USART_StopBits_1;
		uart2_init.USART_WordLength = USART_WordLength_8b;
		USART_Init(USART2, &uart2_init);

		//Enable the UART2 peripheral
		USART_Cmd(USART2, ENABLE);
}

void printmsg(char *msg){

	for(uint32_t i = 0; i < strlen(msg); i++){

		while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) != SET);
		USART_SendData(USART2, msg[i]);
	}
}
