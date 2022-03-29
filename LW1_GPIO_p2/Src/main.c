#include "stm32g474xx.h"

#define TASK 3

void example();
void task1_3();
void task2_3();
void task3_b2();

void dummy_delay (uint32_t duration);

int main(void)
{
#if TASK == 0
	example();
#elif TASK == 1
	task1_3();
#elif TASK == 2
	task2_3();
#elif TASK == 3
	task3_b2();
#endif

}

void dummy_delay (uint32_t duration)
{
	for(uint32_t i = 0; i < duration; ++i);
}

void example()
{
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN_Msk | RCC_AHB2ENR_GPIODEN_Msk;

    GPIOB->MODER &= ~(GPIO_MODER_MODE13_Msk);
    GPIOB->MODER |= 0<<GPIO_MODER_MODE13_Pos;
    GPIOE->MODER &= ~(GPIO_MODER_MODE5_Msk | GPIO_MODER_MODE6_Msk);
    GPIOE->MODER |= 1 << GPIO_MODER_MODE5_Pos | 1 << GPIO_MODER_MODE6_Pos;

    uint32_t button_pressed = 0;

	while(1)
	{
		dummy_delay(100000);
		if(!(GPIOB->IDR & GPIO_IDR_ID13) && button_pressed==0)
		{
			button_pressed = 1;
			GPIOD->ODR ^= GPIO_ODR_OD5 | GPIO_ODR_OD6;
		}
		else if(GPIOB->IDR & GPIO_IDR_ID13)
		{
			button_pressed = 0;
		}
	}
}

void task1_3()
{
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN_Msk | RCC_AHB2ENR_GPIOEEN_Msk;

	GPIOB->MODER &= ~(GPIO_MODER_MODE13_Msk
					| GPIO_MODER_MODE15_Msk);

	GPIOE->MODER &= ~(GPIO_MODER_MODE13_Msk
					| GPIO_MODER_MODE14_Msk
					| GPIO_MODER_MODE15_Msk
					| GPIO_MODER_MODE0_Msk);
	GPIOE->MODER |= 1 << GPIO_MODER_MODE13_Pos
				  | 1 << GPIO_MODER_MODE14_Pos
				  | 1 << GPIO_MODER_MODE15_Pos
				  | 1 << GPIO_MODER_MODE0_Pos;

	int32_t button = 0,
			presses = 0,
			position = 0;

	while(1)
	{
		dummy_delay(100000);

		if(!(GPIOB->IDR & GPIO_IDR_ID13) && button==0 && presses < 4)
		{
			button = 1;
			position = GPIO_ODR_OD13_Pos + presses;

			if(position >= 16)
			{
				position -= 16;
			}

			GPIOE->ODR |= 1 << position;

			presses += 1;
		}
		else if(GPIOB->IDR & GPIO_IDR_ID13)
		{
			button = 0;
		}

		if(!(GPIOB->IDR & GPIO_IDR_ID15))
		{
			GPIOE->ODR &= ~(GPIO_ODR_OD13_Msk
						 | GPIO_ODR_OD14_Msk
						 | GPIO_ODR_OD15_Msk
						 | GPIO_ODR_OD0_Msk);
			presses = 0;
		}
	}


}

void task2_3()
{
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN_Msk | RCC_AHB2ENR_GPIOEEN_Msk;

	GPIOB->MODER &= ~(GPIO_MODER_MODE12_Msk
					| GPIO_MODER_MODE13_Msk
					| GPIO_MODER_MODE14_Msk);

	GPIOE->MODER &= ~(GPIO_MODER_MODE12_Msk
					| GPIO_MODER_MODE13_Msk
					| GPIO_MODER_MODE14_Msk
					| GPIO_MODER_MODE15_Msk);
	GPIOE->MODER |= 1 << GPIO_MODER_MODE12_Pos
				  | 1 << GPIO_MODER_MODE13_Pos
				  | 1 << GPIO_MODER_MODE14_Pos
				  | 1 << GPIO_MODER_MODE15_Pos;

	int32_t button1 = 0,
			button2 = 0,
			counter = 0;

	while(1)
	{
		dummy_delay(100000);

		if(!(GPIOB->IDR & GPIO_IDR_ID12) && button1 == 0)
		{
			button1 = 1;
			counter++;
		}
		else if(GPIOB->IDR & GPIO_IDR_ID12)
		{
			button1 = 0;
		}

		if(!(GPIOB->IDR & GPIO_IDR_ID13) && button2 == 0)
		{
			button2 = 1;
			counter--;
		}
		else if(GPIOB->IDR & GPIO_IDR_ID13)
		{
			button2 = 0;
		}

		if(!(GPIOB->IDR & GPIO_IDR_ID14))
		{
			counter = 0;
		}

		counter &= 0b1111;

		if(((GPIOE->ODR >> GPIO_ODR_OD12_Pos) & 0b1111) != counter)
		{
			GPIOE->ODR &= ~(GPIO_ODR_OD12_Msk
						  | GPIO_ODR_OD13_Msk
						  | GPIO_ODR_OD14_Msk
						  | GPIO_ODR_OD15_Msk);
			GPIOE->ODR |= counter << GPIO_ODR_OD12_Pos;
		}
	}
}

void task3_b2()
{
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN_Msk | RCC_AHB2ENR_GPIOEEN_Msk;

	GPIOB->MODER &= ~(GPIO_MODER_MODE12_Msk
					| GPIO_MODER_MODE13_Msk
					| GPIO_MODER_MODE14_Msk
					| GPIO_MODER_MODE15_Msk);

	GPIOE->MODER &= ~(GPIO_MODER_MODE12_Msk
					| GPIO_MODER_MODE13_Msk
					| GPIO_MODER_MODE14_Msk
					| GPIO_MODER_MODE15_Msk);
	GPIOE->MODER |= 1 << GPIO_MODER_MODE12_Pos
				  | 1 << GPIO_MODER_MODE13_Pos
				  | 1 << GPIO_MODER_MODE14_Pos
				  | 1 << GPIO_MODER_MODE15_Pos;

	int32_t button1 = 0,
			button2 = 0,
			button3 = 0,
			button4 = 0,
			allbuttons = 0,
			//00 -> 1, 01 -> 2, 10 -> 3, 11 -> 4
			//12344321
			right_combination = 0b00 << 14
							  | 0b01 << 12
							  | 0b10 << 10
							  | 0b11 << 8
							  | 0b11 << 6
							  | 0b10 << 4
							  | 0b01 << 2
							  | 0b00,
			curr_combination = 0;

	while(1)
	{
		dummy_delay(100000);

		if(!(GPIOB->IDR & GPIO_IDR_ID12) && button1 == 0)
		{
			button1 = 1;

			curr_combination = curr_combination << 2;
			curr_combination |= 0b00;
			curr_combination &= 0xFFFF;
			GPIOE->ODR |= 1 << GPIO_ODR_OD12_Pos;
		}
		else if(GPIOB->IDR & GPIO_IDR_ID12)
		{
			button1 = 0;
			GPIOE->ODR &= ~GPIO_ODR_OD12;
		}

		if(!(GPIOB->IDR & GPIO_IDR_ID13) && button2 == 0)
		{
			button2 = 1;

			curr_combination = curr_combination << 2;
			curr_combination |= 0b01;
			curr_combination &= 0xFFFF;
			GPIOE->ODR |= 1 << GPIO_ODR_OD13_Pos;
		}
		else if(GPIOB->IDR & GPIO_IDR_ID13)
		{
			button2 = 0;
			GPIOE->ODR &= ~GPIO_ODR_OD13;
		}

		if(!(GPIOB->IDR & GPIO_IDR_ID14) && button3 == 0)
		{
			button3 = 1;

			curr_combination = curr_combination << 2;
			curr_combination |= 0b10;
			curr_combination &= 0xFFFF;
			GPIOE->ODR |= 1 << GPIO_ODR_OD14_Pos;
		}
		else if(GPIOB->IDR & GPIO_IDR_ID14)
		{
			button3 = 0;
			GPIOE->ODR &= ~GPIO_ODR_OD14;
		}

		if(!(GPIOB->IDR & GPIO_IDR_ID15) && button4 == 0)
		{
			button4 = 1;

			curr_combination = curr_combination << 2;
			curr_combination |= 0b11;
			curr_combination &= 0xFFFF;
			GPIOE->ODR |= 1 << GPIO_ODR_OD15_Pos;
		}
		else if(GPIOB->IDR & GPIO_IDR_ID15)
		{
			button4 = 0;
			GPIOE->ODR &= ~GPIO_ODR_OD15;
		}

		if(curr_combination == right_combination)
		{
			GPIOE->ODR |= 1 << GPIO_ODR_OD12_Pos
						| 1 << GPIO_ODR_OD13_Pos
						| 1 << GPIO_ODR_OD14_Pos
						| 1 << GPIO_ODR_OD15_Pos;

			while(1)
			{
				dummy_delay(100000);

				//Если нажаты и отпущены все 4 кнопки, выходить из бесконечного цикла
				if(!((GPIOB->IDR >> GPIO_IDR_ID12_Pos) & 0b1111))
				{
					allbuttons = 1;
					curr_combination = 0;
				}
				else if((((GPIOB->IDR >> GPIO_IDR_ID12_Pos) & 0b1111) == 0b1111) && allbuttons == 1)
				{
					allbuttons = 0;

					GPIOE->ODR &= ~(GPIO_ODR_OD12
								  | GPIO_ODR_OD13
								  | GPIO_ODR_OD14
								  | GPIO_ODR_OD15);

					break;
				}
			}
		}

	}
}
