#include "stm32g474xx.h"

void dumb_delay(uint32_t duration);
void task_1();
void task_2();
void task_3();
void cr_task1();
void cr_task2();
void cr_task3();
void cr_task4();
void cr_taskA();

int main(void)
{
	//task_1();
	//task_2();
	//task_3();
	//cr_task1();
	//cr_task2();
	//cr_task3();
	//cr_task4();
	cr_taskA();
}

void task_1()
{
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIODEN | RCC_AHB2ENR_GPIOEEN;

	GPIOD->MODER &= ~(GPIO_MODER_MODE9_Msk);
	GPIOD->MODER |= 1 << GPIO_MODER_MODE9_Pos;

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
		{
			GPIOE->BSRR = GPIO_BSRR_BS2;
		}
		else
		{
			GPIOE->BSRR = GPIO_BSRR_BR2;
		}

		if((GPIOB->IDR & GPIO_IDR_ID14) == 0)
		{
			GPIOD->BSRR = GPIO_BSRR_BS2;
		}
		else
		{
			GPIOD->BSRR = GPIO_BSRR_BR2;
		}
	}
}

void task_3()
{
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOEEN | RCC_AHB2ENR_GPIOBEN;

	GPIOE->MODER &= ~(GPIO_MODER_MODE12
					| GPIO_MODER_MODE13
					| GPIO_MODER_MODE14
					| GPIO_MODER_MODE15);
	GPIOE->MODER |= 1 << GPIO_MODER_MODE12_Pos
				  | 1 << GPIO_MODER_MODE13_Pos
				  | 1 << GPIO_MODER_MODE14_Pos
				  | 1 << GPIO_MODER_MODE15_Pos;

	GPIOB->MODER &= ~(GPIO_MODER_MODE13 | GPIO_MODER_MODE15);

	uint8_t press_count = 0;
	uint8_t is_pressed = 0;

	while(1)
	{
		dumb_delay(10000);

		if((GPIOB->IDR & GPIO_IDR_ID13) == 0 && (press_count < 4) && is_pressed == 0)
		{
			GPIOE->ODR |= 1 << (GPIO_ODR_OD12_Pos + press_count);

			is_pressed = 1;
			press_count += 1;
		}
		else if(GPIOB->IDR & GPIO_IDR_ID13)
		{
			is_pressed = 0;
		}

		if((GPIOB->IDR & GPIO_IDR_ID15) == 0)
		{
			GPIOE->ODR &= ~(GPIO_ODR_OD12
						  | GPIO_ODR_OD13
						  | GPIO_ODR_OD14
						  | GPIO_ODR_OD15);

			press_count = 0;
		}
	}
}

void cr_task1()
{
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOEEN | RCC_AHB2ENR_GPIOBEN;

	GPIOE->MODER &= ~(GPIO_MODER_MODE12
					| GPIO_MODER_MODE13
					| GPIO_MODER_MODE14
					| GPIO_MODER_MODE15);
	GPIOE->MODER |= 1 << GPIO_MODER_MODE12_Pos
				  | 1 << GPIO_MODER_MODE13_Pos
				  | 1 << GPIO_MODER_MODE14_Pos
				  | 1 << GPIO_MODER_MODE15_Pos;

	GPIOB->MODER &= ~(GPIO_MODER_MODE12);

	uint8_t press_count = 0;
	uint8_t is_pressed = 0;
	//счётчик кадров анимации
	uint8_t frame = 0;

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
		dumb_delay(100000);

		if((GPIOB->IDR & GPIO_IDR_ID12) == 0 && is_pressed == 0)
		{
			is_pressed = 1;
			//изменение значения счётчика нажатий с 0 на 1 или с 1 на 0
			press_count ^= 1;
			frame = 0;
		}
		else if(GPIOB->IDR & GPIO_IDR_ID12)
		{
			is_pressed = 0;
		}

		//очистка состояния загораемых светодиодов
		GPIOE->ODR &= ~(GPIO_ODR_OD12
					  | GPIO_ODR_OD13
					  | GPIO_ODR_OD14
					  | GPIO_ODR_OD15);

		//изменение анимации после нажатия кнопки
		if(press_count == 0)
		{
			if(frame >= victory_num_of_frames)
			{
				frame = 0;
			}

			GPIOE->ODR |= victory_frames[frame] << GPIO_ODR_OD12_Pos;
		}
		else
		{
			if(frame >= error_num_of_frames)
			{
				frame = 0;
			}

			GPIOE->ODR |= error_frames[frame] << GPIO_ODR_OD12_Pos;
		}

		frame++;
	}
}

void cr_task2()
{
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOEEN | RCC_AHB2ENR_GPIOBEN;

	GPIOE->MODER &= ~(GPIO_MODER_MODE12
					| GPIO_MODER_MODE13
					| GPIO_MODER_MODE14
					| GPIO_MODER_MODE15);
	GPIOE->MODER |= 1 << GPIO_MODER_MODE12_Pos
				  | 1 << GPIO_MODER_MODE13_Pos
				  | 1 << GPIO_MODER_MODE14_Pos
				  | 1 << GPIO_MODER_MODE15_Pos;

	GPIOB->MODER &= ~(GPIO_MODER_MODE12
					| GPIO_MODER_MODE13
					| GPIO_MODER_MODE14
					| GPIO_MODER_MODE15);

	while(1)
	{
		dumb_delay(10000);

		if((GPIOB->IDR & GPIO_IDR_ID12) == 0)
		{
			GPIOE->BSRR = GPIO_BSRR_BS12;
		}
		else
		{
			GPIOE->BSRR = GPIO_BSRR_BR12;
		}

		if((GPIOB->IDR & GPIO_IDR_ID13) == 0)
		{
			GPIOE->BSRR = GPIO_BSRR_BS13;
		}
		else
		{
			GPIOE->BSRR = GPIO_BSRR_BR13;
		}

		if((GPIOB->IDR & GPIO_IDR_ID14) == 0)
		{
			GPIOE->BSRR = GPIO_BSRR_BS14;
		}
		else
		{
			GPIOE->BSRR = GPIO_BSRR_BR14;
		}

		if((GPIOB->IDR & GPIO_IDR_ID15) == 0)
		{
			GPIOE->BSRR = GPIO_BSRR_BS15;
		}
		else
		{
			GPIOE->BSRR = GPIO_BSRR_BR15;
		}
	}
}

void cr_task3()
{
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOEEN | RCC_AHB2ENR_GPIOBEN;

	GPIOE->MODER &= ~(GPIO_MODER_MODE12
					| GPIO_MODER_MODE13
					| GPIO_MODER_MODE14
					| GPIO_MODER_MODE15);
	GPIOE->MODER |= 1 << GPIO_MODER_MODE12_Pos
				  | 1 << GPIO_MODER_MODE13_Pos
				  | 1 << GPIO_MODER_MODE14_Pos
				  | 1 << GPIO_MODER_MODE15_Pos;

	GPIOB->MODER &= ~(GPIO_MODER_MODE12
					| GPIO_MODER_MODE13
					| GPIO_MODER_MODE14);

	uint8_t counter = 0,
			b1_is_pressed = 0,
			b2_is_pressed = 0;

	while(1)
	{
		dumb_delay(10000);

		if((GPIOB->IDR & GPIO_IDR_ID12) == 0 && b1_is_pressed == 0)
		{
			b1_is_pressed = 1;
			counter++;
			counter &= 0b1111;
		}
		else if(GPIOB->IDR & GPIO_IDR_ID12)
		{
			b1_is_pressed = 0;
		}

		if((GPIOB->IDR & GPIO_IDR_ID13) == 0 && b2_is_pressed == 0)
		{
			b2_is_pressed = 1;
			counter--;
			counter &= 0b1111;
		}
		else if(GPIOB->IDR & GPIO_IDR_ID13)
		{
			b2_is_pressed = 0;
		}

		if((GPIOB->IDR & GPIO_IDR_ID14) == 0)
		{
			counter = 0;
		}

		if(((GPIOE->ODR >> GPIO_ODR_OD12_Pos) & 0b1111) ^ counter)
		{
			GPIOE->ODR &= ~(GPIO_ODR_OD12
						  | GPIO_ODR_OD13
						  | GPIO_ODR_OD14
						  | GPIO_ODR_OD15);

			GPIOE->BSRR = counter << GPIO_BSRR_BS12_Pos;
		}
	}

}

void cr_task4()
{
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOEEN | RCC_AHB2ENR_GPIOBEN;

	GPIOE->MODER &= ~(GPIO_MODER_MODE12
					| GPIO_MODER_MODE13
					| GPIO_MODER_MODE14
					| GPIO_MODER_MODE15);
	GPIOE->MODER |= 1 << GPIO_MODER_MODE12_Pos
				  | 1 << GPIO_MODER_MODE13_Pos
				  | 1 << GPIO_MODER_MODE14_Pos
				  | 1 << GPIO_MODER_MODE15_Pos;

	GPIOB->MODER &= ~(GPIO_MODER_MODE12);

	uint8_t counter = 0,
			is_pressed = 0;

	uint32_t long_press_delay = 500000;

	while(1)
	{
		dumb_delay(100000);

		uint32_t i = 0;

		if((GPIOB->IDR & GPIO_IDR_ID12) == 0 && is_pressed == 0)
		{
			//for нужен, чтобы после короткого нажатия не нужно было ждать время как для длинного нажатия
			for(; i < long_press_delay; i++)
			{
				if(GPIOB->IDR & GPIO_IDR_ID12)
				{
					counter++;
					counter &= 0b1111;
					break;
				}
			}

			if (i == long_press_delay)
			{
				counter = 0;
			}
		}

		//проверка, нажата ли кнопка после выполнения действия, чтобы с новой итерацией цикла while не происходили действия,
		//пока кнопка не будет отпущена
		if((GPIOB->IDR & GPIO_IDR_ID12) == 0)
		{
			is_pressed = 1;
		}
		else
		{
			is_pressed = 0;
		}

		if(((GPIOE->ODR >> GPIO_ODR_OD12_Pos) & 0b1111) ^ counter)
		{
			GPIOE->ODR &= ~(GPIO_ODR_OD12
						  | GPIO_ODR_OD13
						  | GPIO_ODR_OD14
						  | GPIO_ODR_OD15);

			GPIOE->BSRR = counter << GPIO_BSRR_BS12_Pos;
		}
	}
}

void cr_taskA()
{
	/* BSRR используют, когда требуется выставить определёные логические уровни на выходах,
	 * вне зависимости от их прошлого состояния.
	 * ORD же используют, когда нужно изменить значения на выходах из одного состояния в другое.
	 * Из минусов - могут быть проблемы при одновременном использовании данного регистра в основной программе и в прерывании.*/
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOEEN | RCC_AHB2ENR_GPIOBEN;

	GPIOE->MODER &= ~(GPIO_MODER_MODE12
					| GPIO_MODER_MODE13
					| GPIO_MODER_MODE14
					| GPIO_MODER_MODE15);
	GPIOE->MODER |= 1 << GPIO_MODER_MODE12_Pos
				  | 1 << GPIO_MODER_MODE13_Pos
				  | 1 << GPIO_MODER_MODE14_Pos
				  | 1 << GPIO_MODER_MODE15_Pos;

	GPIOB->MODER &= ~(GPIO_MODER_MODE12);

	uint8_t press_count = 0;
	uint8_t is_pressed = 0;
	//счётчик кадров анимации
	uint8_t frame = 0;

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
		dumb_delay(100000);

		if((GPIOB->IDR & GPIO_IDR_ID12) == 0 && is_pressed == 0)
		{
			is_pressed = 1;
			//изменение значения счётчика нажатий с 0 на 1 или с 1 на 0
			press_count ^= 1;
			frame = 0;
		}
		else if(GPIOB->IDR & GPIO_IDR_ID12)
		{
			is_pressed = 0;
		}

		//очистка состояния загораемых светодиодов
		GPIOE->BSRR = GPIO_BSRR_BR12
					| GPIO_BSRR_BR13
					| GPIO_BSRR_BR14
					| GPIO_BSRR_BR15;

		//изменение анимации после нажатия кнопки
		if(press_count == 0)
		{
			if(frame >= victory_num_of_frames)
			{
				frame = 0;
			}

			GPIOE->BSRR = victory_frames[frame] << GPIO_BSRR_BS12_Pos;
		}
		else
		{
			if(frame >= error_num_of_frames)
			{
				frame = 0;
			}

			GPIOE->BSRR = error_frames[frame] << GPIO_BSRR_BS12_Pos;
		}

		frame++;
	}
}

void dumb_delay(uint32_t duration)
{
	for(uint32_t i = 0; i < duration; i++);
}
