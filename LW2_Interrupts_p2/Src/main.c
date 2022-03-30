#include "stm32g474xx.h"

#define TASK 1

void task1_3();
void task1_3_interrupt();

int main(void)
{
#if TASK == 1
	task1_3();
#endif
}

void EXTI15_10_IRQHandler()
{
#if TASK == 1
	task1_3_interrupt();
#endif
}

void task1_3()
{
    // Включить тактирование
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN | RCC_AHB2ENR_GPIODEN;
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

    // Выбрать порт, ножка с которого будет использоваться
    SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI12_PB
    				   | SYSCFG_EXTICR4_EXTI13_PB
					   | SYSCFG_EXTICR4_EXTI14_PB
					   | SYSCFG_EXTICR4_EXTI15_PB;

    // Настроить нужное внешнее прерывание
    EXTI->IMR1 |= EXTI_IMR1_IM12
    			| EXTI_IMR1_IM13
				| EXTI_IMR1_IM14
				| EXTI_IMR1_IM15;
    EXTI->RTSR1 |= EXTI_RTSR1_RT12
    			 | EXTI_RTSR1_RT13
				 | EXTI_RTSR1_RT14
				 | EXTI_RTSR1_RT15;
    EXTI->FTSR1 |= EXTI_FTSR1_FT12
    			 | EXTI_FTSR1_FT13
				 | EXTI_FTSR1_FT14
				 | EXTI_FTSR1_FT15;
    NVIC_EnableIRQ( EXTI15_10_IRQn );

    // Настроить входы-выходы
    GPIOB->MODER &= ~(GPIO_MODER_MODE12_Msk
    				| GPIO_MODER_MODE13_Msk
					| GPIO_MODER_MODE14_Msk
					| GPIO_MODER_MODE15_Msk);

    GPIOD->MODER &= ~(GPIO_MODER_MODE10_Msk
    				| GPIO_MODER_MODE11_Msk
					| GPIO_MODER_MODE12_Msk
					| GPIO_MODER_MODE13_Msk);
    GPIOD->MODER |= 1 << GPIO_MODER_MODE10_Pos
    			  | 1 << GPIO_MODER_MODE11_Pos
				  | 1 << GPIO_MODER_MODE12_Pos
				  | 1 << GPIO_MODER_MODE13_Pos;

	while(1)
	{

	}
}

void task1_3_interrupt()
{
    if(EXTI->PR1 & EXTI_PR1_PIF12)
    {
    	if(!(GPIOB->IDR & GPIO_IDR_ID12))
    	{
			GPIOD->BSRR = 1 << GPIO_BSRR_BS10_Pos;
    	}
    	else
    	{
			GPIOD->BSRR = 1 << GPIO_BSRR_BR10_Pos;
    	}

        EXTI->PR1 = EXTI_PR1_PIF12;
    }
    else if (EXTI->PR1 & EXTI_PR1_PIF13)
    {
    	if(!(GPIOB->IDR & GPIO_IDR_ID13))
    	{
    		GPIOD->BSRR = 1 << GPIO_BSRR_BS11_Pos;
    	}
    	else
    	{
    		GPIOD->BSRR = 1 << GPIO_BSRR_BR11_Pos;
    	}

        EXTI->PR1 = EXTI_PR1_PIF13;
    }
    else if (EXTI->PR1 & EXTI_PR1_PIF14)
    {
    	if(!(GPIOB->IDR & GPIO_IDR_ID14))
    	{
    		GPIOD->BSRR = 1 << GPIO_BSRR_BS12_Pos;
    	}
    	else
    	{
    		GPIOD->BSRR = 1 << GPIO_BSRR_BR12_Pos;
    	}

        EXTI->PR1 = EXTI_PR1_PIF14;
    }
    else if (EXTI->PR1 & EXTI_PR1_PIF15)
    {
    	if(!(GPIOB->IDR & GPIO_IDR_ID15))
    	{
    		GPIOD->BSRR = 1 << GPIO_BSRR_BS13_Pos;
    	}
    	else
    	{
    		GPIOD->BSRR = 1 << GPIO_BSRR_BR13_Pos;
    	}

        EXTI->PR1 = EXTI_PR1_PIF15;
    }
}
