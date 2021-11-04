#include "stm32g474xx.h"

void task_1();
void task_1_interrupt();
void task_2();
void task_2_interrupt();
void task_3();
void task_3_interrupt();

int main(void)
{
	//task_1();
	//task_2();
	task_3();
}

void EXTI15_10_IRQHandler()
{
	//task_1_interrupt();
	//task_2_interrupt();
	task_3_interrupt();
}

void task_1()
{
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN | RCC_AHB2ENR_GPIOEEN;
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

	SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI12_PB;

    EXTI->IMR1 |= EXTI_IMR1_IM12;
    EXTI->FTSR1 |= EXTI_FTSR1_FT12;
    NVIC_EnableIRQ( EXTI15_10_IRQn );

    GPIOB->MODER &= ~GPIO_MODER_MODE12;

    GPIOE->MODER &= ~GPIO_MODER_MODE11;
    GPIOE->MODER |= 1 << GPIO_MODER_MODE11_Pos;

	while(1)
	{

	}
}

void task_1_interrupt()
{
	GPIOE->ODR ^= GPIO_ODR_OD11;

	EXTI->PR1 = EXTI_PR1_PIF12;
}

void task_2()
{
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN | RCC_AHB2ENR_GPIODEN | RCC_AHB2ENR_GPIOEEN;
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

	SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI14_PB;

    EXTI->IMR1 |= EXTI_IMR1_IM14;
    EXTI->FTSR1 |= EXTI_FTSR1_FT14;
    EXTI->RTSR1 |= EXTI_RTSR1_RT14;
    NVIC_EnableIRQ( EXTI15_10_IRQn );

    GPIOB->MODER &= ~GPIO_MODER_MODE14;

    GPIOD->MODER &= ~GPIO_MODER_MODE0;
    GPIOD->MODER |= 1 << GPIO_MODER_MODE0_Pos;
    GPIOE->MODER &= ~GPIO_MODER_MODE14;
    GPIOE->MODER |= 1 << GPIO_MODER_MODE14_Pos;

	while(1)
	{

	}
}

void task_2_interrupt()
{
	if((GPIOB->IDR & GPIO_IDR_ID14) == 0)
	{
		GPIOD->BSRR = GPIO_BSRR_BS0;
		GPIOE->BSRR = GPIO_BSRR_BS14;
	}
	else
	{
		GPIOD->BSRR = GPIO_BSRR_BR0;
		GPIOE->BSRR = GPIO_BSRR_BR14;
	}

	EXTI->PR1 = EXTI_PR1_PIF14;
}

void task_3()
{
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN | RCC_AHB2ENR_GPIODEN;
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

	SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI12_PB
					   | SYSCFG_EXTICR4_EXTI13_PB
					   | SYSCFG_EXTICR4_EXTI14_PB
					   | SYSCFG_EXTICR4_EXTI15_PB;

    EXTI->IMR1 |= EXTI_IMR1_IM12
    			| EXTI_IMR1_IM13
				| EXTI_IMR1_IM14
				| EXTI_IMR1_IM15;

    EXTI->FTSR1 |= EXTI_FTSR1_FT12
    			 | EXTI_FTSR1_FT13
				 | EXTI_FTSR1_FT14
				 | EXTI_FTSR1_FT15;

    EXTI->RTSR1 |= EXTI_RTSR1_RT12
    			 | EXTI_RTSR1_RT13
				 | EXTI_RTSR1_RT14
				 | EXTI_RTSR1_RT15;

    NVIC_EnableIRQ( EXTI15_10_IRQn );

    GPIOB->MODER &= ~(GPIO_MODER_MODE12
    				| GPIO_MODER_MODE13
					| GPIO_MODER_MODE14
					| GPIO_MODER_MODE15);

    GPIOD->MODER &= ~(GPIO_MODER_MODE13
    				| GPIO_MODER_MODE12
					| GPIO_MODER_MODE11
					| GPIO_MODER_MODE10);
    GPIOD->MODER |= 1 << GPIO_MODER_MODE13_Pos
    			  | 1 << GPIO_MODER_MODE12_Pos
				  | 1 << GPIO_MODER_MODE11_Pos
				  | 1 << GPIO_MODER_MODE10_Pos;

	while(1)
	{

	}
}

void task_3_interrupt()
{
    if(EXTI->PR1 & EXTI_PR1_PIF12)
    {
    	if((GPIOB->IDR & GPIO_IDR_ID12) == 0)
    	{
    		GPIOD->BSRR = GPIO_BSRR_BS13;
    	}
    	else
    	{
    		GPIOD->BSRR = GPIO_BSRR_BR13;
    	}

        EXTI->PR1 = EXTI_PR1_PIF12;
    }
    else if (EXTI->PR1 & EXTI_PR1_PIF13)
    {
    	if((GPIOB->IDR & GPIO_IDR_ID13) == 0)
    	{
    		GPIOD->BSRR = GPIO_BSRR_BS12;
    	}
    	else
    	{
    		GPIOD->BSRR = GPIO_BSRR_BR12;
    	}

        EXTI->PR1 = EXTI_PR1_PIF13;
    }
    else if (EXTI->PR1 & EXTI_PR1_PIF14)
	{
    	if((GPIOB->IDR & GPIO_IDR_ID14) == 0)
    	{
    		GPIOD->BSRR = GPIO_BSRR_BS11;
    	}
    	else
    	{
    		GPIOD->BSRR = GPIO_BSRR_BR11;
    	}

		EXTI->PR1 = EXTI_PR1_PIF14;
	}
    else
	{
    	if((GPIOB->IDR & GPIO_IDR_ID15) == 0)
    	{
    		GPIOD->BSRR = GPIO_BSRR_BS10;
    	}
    	else
    	{
    		GPIOD->BSRR = GPIO_BSRR_BR10;
    	}

		EXTI->PR1 = EXTI_PR1_PIF15;
	}
}
