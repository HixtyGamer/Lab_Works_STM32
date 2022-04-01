#include "stm32g474xx.h"
#include "stdlib.h"

#define TASK 4

void setup_clock();
void task_1();
void task_1_timer();
void task_2();
void task_2_timer();
void task_3();
void task_3_timer();
void cr_task_1();
void cr_task_1_timer();
void sleep(uint16_t time);
void cr_task_2();
void cr_task_2_interrupt();
void cr_task_2_timer();
void cr_task_3();
void cr_task_3_timer();
void cr_task_A();
void cr_task_A_timer();
void cr_task_B();
void cr_task_B_timer();

uint32_t interrupt_counter = 1; //для задания 3 и творческих заданий 1, 2 и A
int8_t duty_cycle = 0; //для творческого задания 2
int16_t period_PWM = 200; //для творческого задания B

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
	cr_task_2();
#elif TASK == 6
	cr_task_3();
#elif TASK == 7
	cr_task_A();
#elif TASK == 8
	cr_task_B();
#endif

}

void TIM2_IRQHandler (void)
{
#if TASK == 1
	task_1_timer();
#elif TASK == 2
	task_2_timer();
#elif TASK == 4
	cr_task_1_timer();
#elif TASK == 5
	cr_task_2_timer();
#endif
}

void TIM3_IRQHandler (void)
{
#if TASK == 3
	task_3_timer();
#elif TASK == 6
	cr_task_3_timer();
#elif TASK == 7
	cr_task_A_timer();
#elif TASK == 8
	cr_task_B_timer();
#endif
}

void EXTI15_10_IRQHandler()
{
#if TASK == 5
	cr_task_2_interrupt();
#endif
}

void setup_clock()
{
    FLASH->ACR &= ~FLASH_ACR_LATENCY_Msk;
    FLASH->ACR |= FLASH_ACR_LATENCY_2WS;

    RCC->CR |= RCC_CR_HSEON;

    while ((RCC->CR & RCC_CR_HSERDY) != RCC_CR_HSERDY) {}

    RCC->PLLCFGR&=~(RCC_PLLCFGR_PLLR_Msk | RCC_PLLCFGR_PLLM_Msk | RCC_PLLCFGR_PLLN_Msk);

    RCC->PLLCFGR |= 2 << RCC_PLLCFGR_PLLR_Pos // Установка делителя R
                    | RCC_PLLCFGR_PLLREN    // Включение R делителя PLL
                    | 15 << RCC_PLLCFGR_PLLN_Pos // Установка умножителя N
                    | 0 << RCC_PLLCFGR_PLLM_Pos // Установка делителя M
                    | RCC_PLLCFGR_PLLSRC_HSE; // HSE - источник сигнала для PLL

    RCC->CR |= RCC_CR_PLLON;

    while ((RCC->CR & RCC_CR_PLLRDY) != RCC_CR_PLLRDY){}

    RCC->CFGR |= RCC_CFGR_SW_Msk;
    RCC->CFGR &= ~(RCC_CFGR_SW_Msk ^ RCC_CFGR_SW_PLL);

    while ((RCC->CFGR & RCC_CFGR_SWS_PLL) != RCC_CFGR_SWS_PLL){}
}

void task_1()
{
	setup_clock();

    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOEEN;
    RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;

    GPIOE->MODER &= ~( GPIO_MODER_MODE8_Msk);
    GPIOE->MODER |= 1 <<  GPIO_MODER_MODE8_Pos;

    TIM2->PSC = 19999; // Предделитель = 19999
    TIM2->ARR = 999; // Задержка = 3 секунды
    TIM2->DIER |= TIM_DIER_UIE; // Разрешение прерывания по переполнению
    TIM2->CR1 |= TIM_CR1_CEN; // Включение таймера
    NVIC_EnableIRQ (TIM2_IRQn);

	while(1)
	{

	}
}

void task_1_timer()
{
    GPIOE->ODR ^= GPIO_ODR_OD8;
    TIM2->SR &= ~ TIM_SR_UIF; // Сброс прерывания по таймеру
}

void task_2()
{
	setup_clock();

    RCC->AHB2ENR |= RCC_AHB2ENR_GPIODEN;
    RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;

    GPIOD->MODER &= ~( GPIO_MODER_MODE1_Msk);
    GPIOD->MODER |= 1 <<  GPIO_MODER_MODE1_Pos;

    TIM2->PSC = 19999; // Предделитель = 19999
    TIM2->ARR = 250;
    TIM2->DIER |= TIM_DIER_UIE; // Разрешение прерывания по переполнению
    TIM2->CR1 |= TIM_CR1_CEN; // Включение таймера
    NVIC_EnableIRQ (TIM2_IRQn);

	while(1)
	{

	}
}

void task_2_timer()
{
    GPIOD->ODR ^= GPIO_ODR_OD1;
    TIM2->SR &= ~ TIM_SR_UIF; // Сброс прерывания по таймеру
}

void task_3()
{
	setup_clock();

    RCC->AHB2ENR |= RCC_AHB2ENR_GPIODEN;
    RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN | RCC_APB1ENR1_TIM3EN;

    GPIOD->MODER &= ~( GPIO_MODER_MODE3_Msk);
    GPIOD->MODER |= 2 <<  GPIO_MODER_MODE3_Pos;

    GPIOD->AFR[0] |= 2 << GPIO_AFRL_AFSEL3_Pos;

    TIM3->PSC = 19999;
    TIM3->ARR = 1499;
    TIM3->DIER |= TIM_DIER_UIE; // Разрешение прерывания по переполнению
    TIM3->CR1 |= TIM_CR1_CEN; // Включение таймера
    NVIC_EnableIRQ (TIM3_IRQn);

    TIM2->PSC = 0;
    TIM2->ARR = 99;
    // Настроить таймер в режим ШИМ
    TIM2->CCMR1 |=  TIM_CCMR1_OC1PE | 6 << TIM_CCMR1_OC1M_Pos;
    TIM2->CCER |= TIM_CCER_CC1E; // включить канал 1
    TIM2->CR1 |= TIM_CR1_ARPE;  // Разрешить автоматическую перегрузку счетчика
    TIM2->CR1 |= TIM_CR1_CEN; // Включить таймер
    TIM2->CCR1 = 20; //

	while(1)
	{

	}
}

void task_3_timer()
{
	switch(interrupt_counter)
	{
	case 0:
		interrupt_counter = 1;
	    TIM2->CCR1 = 50;
	    break;
	case 1:
		interrupt_counter = 2;
	    TIM2->CCR1 = 80;
	    break;
	case 2:
		interrupt_counter = 0;
	    TIM2->CCR1 = 20;
	    break;
	}

    TIM3->SR &= ~ TIM_SR_UIF;
}

void cr_task_1()
{
	setup_clock();

    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOEEN;
    RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;

    GPIOE->MODER &= ~( GPIO_MODER_MODE8_Msk);
    GPIOE->MODER |= 1 <<  GPIO_MODER_MODE8_Pos;

    TIM2->DIER |= TIM_DIER_UIE;
    TIM2->PSC = 19999; // Предделитель = 19999
    NVIC_EnableIRQ (TIM2_IRQn);

	while(1)
	{
		sleep(500);

	    GPIOE->ODR ^= GPIO_ODR_OD8;
	}
}

void cr_task_1_timer()
{
	interrupt_counter = 0;

    TIM2->SR &= ~ TIM_SR_UIF;
}

void sleep(uint16_t time)
{
	interrupt_counter = 1;

    TIM2->ARR = time - 1;
    TIM2->CR1 |= TIM_CR1_CEN;

    TIM2->SR &= ~ TIM_SR_UIF;

    while(interrupt_counter);

    TIM2->CR1 &= ~TIM_CR1_CEN;
    TIM2->CNT = 0;
}

void cr_task_2()
{
	setup_clock();

	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOEEN | RCC_AHB2ENR_GPIOBEN;
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
    RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;

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

    TIM2->PSC = 19999;
    TIM2->ARR = 999; //долгое нажатие - 1 секунда
    TIM2->DIER |= TIM_DIER_UIE;
    NVIC_EnableIRQ (TIM2_IRQn);

	while(1)
	{

	}
}

void cr_task_2_interrupt()
{
	if((GPIOB->IDR & GPIO_IDR_ID12) == 0)
	{
	    TIM2->CNT = 0;
	    TIM2->CR1 |= TIM_CR1_CEN;
	}
	else
	{
		if(interrupt_counter)
		{
		    TIM2->CR1 &= ~TIM_CR1_CEN;

			uint8_t counter = (GPIOE->ODR >> GPIO_ODR_OD12_Pos) & 0b1111;

			counter++;
			counter &= 0b1111;

			GPIOE->BSRR = GPIO_BSRR_BR12
						| GPIO_BSRR_BR13
						| GPIO_BSRR_BR14
						| GPIO_BSRR_BR15;

			GPIOE->BSRR = counter << GPIO_BSRR_BS12_Pos;
		}
		else
		{
			interrupt_counter = 1;
		}

	}

    EXTI->PR1 = EXTI_PR1_PIF12;
}

void cr_task_2_timer()
{
    TIM2->CR1 &= ~TIM_CR1_CEN;

	uint8_t counter = (GPIOE->ODR >> GPIO_ODR_OD12_Pos) & 0b1111;

	counter = 0;
	interrupt_counter = 0;

	GPIOE->BSRR = GPIO_BSRR_BR12
				| GPIO_BSRR_BR13
				| GPIO_BSRR_BR14
				| GPIO_BSRR_BR15;

	GPIOE->BSRR = counter << GPIO_BSRR_BS12_Pos;

    TIM2->SR &= ~ TIM_SR_UIF;
}

void cr_task_3()
{
	setup_clock();

    RCC->AHB2ENR |= RCC_AHB2ENR_GPIODEN;
    RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN | RCC_APB1ENR1_TIM3EN;

    GPIOD->MODER &= ~( GPIO_MODER_MODE3_Msk);
    GPIOD->MODER |= 2 <<  GPIO_MODER_MODE3_Pos;

    GPIOD->AFR[0] |= 2 << GPIO_AFRL_AFSEL3_Pos;

    TIM2->PSC = 0;
    TIM2->ARR = 127;
    // Настроить таймер в режим ШИМ
    TIM2->CCMR1 |=  TIM_CCMR1_OC1PE | 6 << TIM_CCMR1_OC1M_Pos;
    TIM2->CCER |= TIM_CCER_CC1E; // включить канал 1
    TIM2->CR1 |= TIM_CR1_ARPE;  // Разрешить автоматическую перегрузку счетчика
    TIM2->CR1 |= TIM_CR1_CEN; // Включить таймер
    TIM2->CCR1 = abs(duty_cycle);

    TIM3->PSC = 19999; // Предделитель = 19999
    TIM3->ARR = 24;
    TIM3->DIER |= TIM_DIER_UIE;
    TIM3->CR1 |= TIM_CR1_CEN;
    NVIC_EnableIRQ (TIM3_IRQn);

	while(1)
	{

	}
}

void cr_task_3_timer()
{
	duty_cycle++;

	TIM2->CCR1 = abs(duty_cycle);

    TIM3->SR &= ~ TIM_SR_UIF;
}

void cr_task_A()
{
	/*в мс; период сигнала от 2 до 65535 мс (частота ~ от 0.032 до 1000 Гц)*/
	uint16_t period = 500,
			 imp_count = 15;

	period--;

	interrupt_counter = imp_count * 2;

	setup_clock();

    RCC->AHB2ENR |= RCC_AHB2ENR_GPIODEN;
    RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN | RCC_APB1ENR1_TIM3EN;

    GPIOD->MODER &= ~( GPIO_MODER_MODE3_Msk);
    GPIOD->MODER |= 1 <<  GPIO_MODER_MODE3_Pos;

    TIM3->PSC = 9999;
    TIM3->ARR = period;
    TIM3->DIER |= TIM_DIER_UIE; // Разрешение прерывания по переполнению
    TIM3->CR1 |= TIM_CR1_CEN; // Включение таймера
    NVIC_EnableIRQ (TIM3_IRQn);

	while(1)
	{

	}
}

void cr_task_A_timer()
{
	interrupt_counter--;

	if(interrupt_counter == 0)
	{
	    TIM3->CR1 &= ~TIM_CR1_CEN;
	}

	GPIOD->ODR ^= GPIO_ODR_OD3;

	TIM3->SR &= ~ TIM_SR_UIF;
}

void cr_task_B()
{
	setup_clock();

    RCC->AHB2ENR |= RCC_AHB2ENR_GPIODEN;
    RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN | RCC_APB1ENR1_TIM3EN;

    GPIOD->MODER &= ~( GPIO_MODER_MODE3_Msk);
    GPIOD->MODER |= 2 <<  GPIO_MODER_MODE3_Pos;

    GPIOD->AFR[0] |= 2 << GPIO_AFRL_AFSEL3_Pos;

    TIM2->PSC = 19999;
    TIM2->ARR = abs(period_PWM);
    TIM2->CCMR1 |=  TIM_CCMR1_OC1PE | 6 << TIM_CCMR1_OC1M_Pos;
    TIM2->CCER |= TIM_CCER_CC1E;
    TIM2->CR1 |= TIM_CR1_ARPE;
    TIM2->CR1 |= TIM_CR1_CEN;
    TIM2->CCR1 = abs(period_PWM) / 2;

    TIM3->PSC = 19999;
    TIM3->ARR = 24;
    TIM3->DIER |= TIM_DIER_UIE;
    TIM3->CR1 |= TIM_CR1_CEN;
    NVIC_EnableIRQ (TIM3_IRQn);

	while(1)
	{

	}
}

void cr_task_B_timer()
{
	period_PWM++;

	//частота меняется от 5 Гц до 1 Гц (период от 1000 мс до 200 мс)
	if(period_PWM == 999)
	{
		period_PWM = -998;
	}

	if(period_PWM == -199)
	{
		period_PWM = 200;
	}

    TIM2->ARR = abs(period_PWM);
    TIM2->CCR1 = abs(period_PWM) / 2;

    TIM3->SR &= ~ TIM_SR_UIF;
}
