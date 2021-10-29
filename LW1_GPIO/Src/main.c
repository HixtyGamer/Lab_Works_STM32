#include "stm32g474xx.h"

void dumb_delay(uint32_t duration);
void task_1();
void task_2();

int main(void)
{
	//task_1();
	task_2();
}

void task_1()
{
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIODEN | RCC_AHB2ENR_GPIOEEN;

	GPIOD->MODER &= ~(GPIO_MODER_MODE2_Msk);
	GPIOD->MODER |= 1 << GPIO_MODER_MODE2_Pos;

	GPIOE->MODER &= ~(GPIO_MODER_MODE7_Msk);
	GPIOE->MODER |= 1 << GPIO_MODER_MODE7_Pos;

	GPIOD->ODR |= GPIO_ODR_OD9;

	while(1)
	{
		dumb_delay(1000000);

	    GPIOD->ODR ^= GPIO_ODR_OD9;
	    GPIOE->ODR ^= GPIO_ODR_OD7;
	}
}

void task_2()
{
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIODEN | RCC_AHB2ENR_GPIOEEN | RCC_AHB2ENR_GPIOBEN;

	GPIOD->MODER &= ~(GPIO_MODER_MODE2);
	GPIOD->MODER |= 1 << GPIO_MODER_MODE2_Pos;

	GPIOE->MODER &= ~(GPIO_MODER_MODE2);
	GPIOE->MODER |= 1 << GPIO_MODER_MODE2_Pos;

	GPIOB->MODER &= ~(GPIO_MODER_MODE12 | GPIO_MODER_MODE14);

	while(1)
	{
		dumb_delay(10000);

		if((GPIOB->IDR & GPIO_IDR_ID12) == 0)
			GPIOE->ODR = 1 << GPIO_ODR_OD2_Pos;
		else
			GPIOE->ODR = 0 << GPIO_ODR_OD2_Pos;

		if((GPIOB->IDR & GPIO_IDR_ID14) == 0)
			GPIOD->ODR = 1 << GPIO_ODR_OD2_Pos;
		else
			GPIOD->ODR = 0 << GPIO_ODR_OD2_Pos;
	}
}

void dumb_delay(uint32_t duration)
{
	for(uint32_t i=0; i<duration; i++);
}
