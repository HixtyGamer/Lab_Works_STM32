#include "stm32g474xx.h"

#define TASK 1

void task1_3();
void task1_3_interrupt();

uint8_t buttons_curr = 0,
		buttons_prev = 0;

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
		if(buttons_prev != buttons_curr)
		{
			buttons_prev = buttons_curr;
			GPIOD->BSRR = GPIO_BSRR_BR10
						| GPIO_BSRR_BR11
						| GPIO_BSRR_BR12
						| GPIO_BSRR_BR13;
			GPIOD->BSRR = buttons_curr << GPIO_BSRR_BS10_Pos;
		}
	}
}

void task1_3_interrupt()
{
    if(EXTI->PR1 & EXTI_PR1_PIF12)
    {
    	if(!(GPIOB->IDR & GPIO_IDR_ID12))
    	{
    		buttons_curr |= 1 << 3;
    	}
    	else
    	{
    		buttons_curr &= 0b0111;
    	}

        EXTI->PR1 = EXTI_PR1_PIF12;
    }
    else if (EXTI->PR1 & EXTI_PR1_PIF13)
    {
    	if(!(GPIOB->IDR & GPIO_IDR_ID13))
    	{
    		buttons_curr |= 1 << 2;
    	}
    	else
    	{
    		buttons_curr &= 0b1011;
    	}

        EXTI->PR1 = EXTI_PR1_PIF13;
    }
    else if (EXTI->PR1 & EXTI_PR1_PIF14)
    {
    	if(!(GPIOB->IDR & GPIO_IDR_ID14))
    	{
    		buttons_curr |= 1 << 1;
    	}
    	else
    	{
    		buttons_curr &= 0b1101;
    	}

        EXTI->PR1 = EXTI_PR1_PIF14;
    }
    else if (EXTI->PR1 & EXTI_PR1_PIF15)
    {
    	if(!(GPIOB->IDR & GPIO_IDR_ID15))
    	{
    		buttons_curr |= 1;
    	}
    	else
    	{
    		buttons_curr &= 0b1110;
    	}

        EXTI->PR1 = EXTI_PR1_PIF15;
    }
}
