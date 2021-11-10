#include "stm32g474xx.h"

#define TASK 4

void task_1();
void task_1_interrupt();
void task_2();
void task_2_interrupt();
void task_3();
void task_3_interrupt();
void cr_task_1();
void cr_task_1_interrupt();
void cr_task_3();
void cr_task_3_interrupt();
void dumb_delay();

uint8_t b1_pressed = 0,
		b2_pressed = 0,
		b3_pressed = 0,
		b4_pressed = 0;
uint32_t combination_input = 0,
		 time_passed = 0;

int main(void)
{
#if TASK == 1
	task_1();
#elif TASK == 2
	task_2();
#elif TASK == 3
	task_3();
#elif TASK == 4
	cr_task_1();
#elif TASK == 5

#elif TASK == 6
	cr_task_3();
#endif

}

void EXTI15_10_IRQHandler()
{
#if TASK == 1
	task_1_interrupt();
#elif TASK == 2
	task_2_interrupt();
#elif TASK == 3
	task_3_interrupt();
#elif TASK == 4
	cr_task_1_interrupt();
#elif TASK == 5

#elif TASK == 6
	cr_task_3_interrupt();
#endif

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
    else if (EXTI->PR1 & EXTI_PR1_PIF15)
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

void cr_task_1()
{
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOEEN | RCC_AHB2ENR_GPIOBEN;
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

	GPIOE->MODER &= ~(GPIO_MODER_MODE12
					| GPIO_MODER_MODE13
					| GPIO_MODER_MODE14
					| GPIO_MODER_MODE15);
	GPIOE->MODER |= 1 << GPIO_MODER_MODE12_Pos
				  | 1 << GPIO_MODER_MODE13_Pos
				  | 1 << GPIO_MODER_MODE14_Pos
				  | 1 << GPIO_MODER_MODE15_Pos;

	GPIOB->MODER &= ~(GPIO_MODER_MODE12);

	SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI12_PB;

    EXTI->IMR1 |= EXTI_IMR1_IM12;
    EXTI->FTSR1 |= EXTI_FTSR1_FT12;
    EXTI->RTSR1 |= EXTI_RTSR1_RT12;
    NVIC_EnableIRQ( EXTI15_10_IRQn );

	while(1)
	{
		for(time_passed = 0; b1_pressed == 1; time_passed++);
	}
}

void cr_task_1_interrupt()
{
	uint32_t long_press_delay = 500000;

	if((GPIOB->IDR & GPIO_IDR_ID12) == 0)
	{
		b1_pressed = 1;
	}
	else
	{
		b1_pressed = 0;
		uint8_t counter = (GPIOE->ODR >> GPIO_ODR_OD12_Pos) & 0b1111;

		if(time_passed < long_press_delay)
		{
			counter++;
			counter &= 0b1111;
		}
		else
		{
			counter = 0;
		}

		GPIOE->BSRR = GPIO_BSRR_BR12
					| GPIO_BSRR_BR13
					| GPIO_BSRR_BR14
					| GPIO_BSRR_BR15;

		GPIOE->BSRR = counter << GPIO_BSRR_BS12_Pos;
	}


    EXTI->PR1 = EXTI_PR1_PIF12;
}

void cr_task_3()
{
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN | RCC_AHB2ENR_GPIOEEN;
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

    NVIC_EnableIRQ( EXTI15_10_IRQn );

    GPIOB->MODER &= ~(GPIO_MODER_MODE12
    				| GPIO_MODER_MODE13
					| GPIO_MODER_MODE14
					| GPIO_MODER_MODE15);

    GPIOE->MODER &= ~(GPIO_MODER_MODE12
    				| GPIO_MODER_MODE13
					| GPIO_MODER_MODE14
					| GPIO_MODER_MODE15);
    GPIOE->MODER |= 1 << GPIO_MODER_MODE12_Pos
    			  | 1 << GPIO_MODER_MODE13_Pos
				  | 1 << GPIO_MODER_MODE14_Pos
				  | 1 << GPIO_MODER_MODE15_Pos;

	uint32_t correct_combination = 1234;
	uint32_t frame_time = 100000;

	uint8_t frame = 0,
			mistakes = 0;

	//кадры анимации "победы"
	uint8_t victory_frames[] = { 0b0001,
								 0b0010,
								 0b0100,
								 0b1000,
								 0b0100,
								 0b0010 };

	//кадры анимации "ошибки"
	uint8_t error_frames[] = { 0b0000,
							   0b1000,
							   0b1100,
							   0b1110,
							   0b1111,
							   0b0111,
							   0b0011,
							   0b0001 };

	//количество кадров анимации "победы"
	uint8_t victory_num_of_frames = sizeof(victory_frames) / sizeof(victory_frames[0]);
	//количество кадров анимации "ошибки"
	uint8_t error_num_of_frames = sizeof(error_frames) / sizeof(error_frames[0]);

	while(1)
	{
		if(b1_pressed && b2_pressed && b3_pressed && b4_pressed)
		{
			if(combination_input == correct_combination)
			{
				while(1)
				{
					dumb_delay(frame_time);

					if(frame >= victory_num_of_frames)
					{
						frame = 0;
					}

					GPIOE->BSRR = GPIO_BSRR_BR12
								| GPIO_BSRR_BR13
								| GPIO_BSRR_BR14
								| GPIO_BSRR_BR15;

					GPIOE->BSRR = victory_frames[frame] << GPIO_BSRR_BS12_Pos;
					frame++;
				}
			}
			else if(mistakes < 2)  //если считать от нуля, то не более 3х ошибок
			{
				for(uint8_t i = 0; i < error_num_of_frames * 2; i++)
				{
					dumb_delay(frame_time);

					if(frame >= error_num_of_frames)
					{
						frame = 0;
					}

					GPIOE->BSRR = GPIO_BSRR_BR12
								| GPIO_BSRR_BR13
								| GPIO_BSRR_BR14
								| GPIO_BSRR_BR15;

					GPIOE->BSRR = error_frames[frame] << GPIO_BSRR_BS12_Pos;
					frame++;
				}

				mistakes++;
				combination_input = 0;
				b1_pressed = 0;
				b2_pressed = 0;
				b3_pressed = 0;
				b4_pressed = 0;

				GPIOE->BSRR = GPIO_BSRR_BR12
							| GPIO_BSRR_BR13
							| GPIO_BSRR_BR14
							| GPIO_BSRR_BR15;
			}
			else
			{
				while(1)
				{
					dumb_delay(frame_time);

					if(frame >= error_num_of_frames)
					{
						frame = 0;
					}

					GPIOE->BSRR = GPIO_BSRR_BR12
								| GPIO_BSRR_BR13
								| GPIO_BSRR_BR14
								| GPIO_BSRR_BR15;

					GPIOE->BSRR = error_frames[frame] << GPIO_BSRR_BS12_Pos;
					frame++;
				}
			}
		}
	}
}

void cr_task_3_interrupt()
{
    if(EXTI->PR1 & EXTI_PR1_PIF12)
    {
    	if(((GPIOB->IDR & GPIO_IDR_ID12) == 0) && b1_pressed == 0)
    	{
			GPIOE->BSRR = GPIO_BSRR_BS12;
			b1_pressed = 1;
			combination_input = combination_input * 10 + 1;
    	}

        EXTI->PR1 = EXTI_PR1_PIF12;
    }
    else if (EXTI->PR1 & EXTI_PR1_PIF13)
    {
    	if(((GPIOB->IDR & GPIO_IDR_ID13) == 0) && b2_pressed == 0)
    	{
    		GPIOE->BSRR = GPIO_BSRR_BS13;
			b2_pressed = 1;
			combination_input = combination_input * 10 + 2;
    	}

        EXTI->PR1 = EXTI_PR1_PIF13;
    }
    else if (EXTI->PR1 & EXTI_PR1_PIF14)
	{
    	if(((GPIOB->IDR & GPIO_IDR_ID14) == 0) && b3_pressed == 0)
    	{
    		GPIOE->BSRR = GPIO_BSRR_BS14;
			b3_pressed = 1;
			combination_input = combination_input * 10 + 3;
    	}

		EXTI->PR1 = EXTI_PR1_PIF14;
	}
    else if (EXTI->PR1 & EXTI_PR1_PIF15)
	{
    	if(((GPIOB->IDR & GPIO_IDR_ID15) == 0) && b4_pressed == 0)
    	{
    		GPIOE->BSRR = GPIO_BSRR_BS15;
			b4_pressed = 1;
			combination_input = combination_input * 10 + 4;
    	}

		EXTI->PR1 = EXTI_PR1_PIF15;
	}
}

void dumb_delay(uint32_t delay)
{
	for(uint32_t i = 0; i < delay; i++);
}
