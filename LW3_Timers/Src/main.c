#include "stm32g474xx.h"

#define TASK 4

void task_1();
void task_1_timer();
void task_2();
void task_2_timer();
void task_3();
void task_3_timer();
void cr_task_1();
void cr_task_1_timer();
void sleep(uint16_t time);

int interrupt_counter = 1; //для задания 3 и творческого задания 1

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
#endif
}

void TIM3_IRQHandler (void)
{
#if TASK == 3
	task_3_timer();
#endif
}

void task_1()
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

    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOEEN;
    RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;

    GPIOE->MODER &= ~( GPIO_MODER_MODE8_Msk);
    GPIOE->MODER |= 1 <<  GPIO_MODER_MODE8_Pos;

    TIM2->PSC = 19999; // Предделитель = 19999
    TIM2->ARR = 2999; // Задержка = 3 секунды
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

    RCC->AHB2ENR |= RCC_AHB2ENR_GPIODEN;
    RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;

    GPIOD->MODER &= ~( GPIO_MODER_MODE1_Msk);
    GPIOD->MODER |= 1 <<  GPIO_MODER_MODE1_Pos;

    TIM2->PSC = 19999; // Предделитель = 19999
    TIM2->ARR = 250; // Задержка = 3 секунды
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

    RCC->AHB2ENR |= RCC_AHB2ENR_GPIODEN;
    RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN | RCC_APB1ENR1_TIM3EN;

    GPIOD->MODER &= ~( GPIO_MODER_MODE3_Msk);
    GPIOD->MODER |= 2 <<  GPIO_MODER_MODE3_Pos;

    GPIOD->AFR[0] |= 2 << GPIO_AFRL_AFSEL3_Pos;

    TIM3->PSC = 19999;
    TIM3->ARR = 1500;
    TIM3->DIER |= TIM_DIER_UIE; // Разрешение прерывания по переполнению
    TIM3->CR1 |= TIM_CR1_CEN; // Включение таймера
    NVIC_EnableIRQ (TIM3_IRQn);

    TIM2->PSC = 0;
    TIM2->ARR = 100;
    // Настроить таймер в режим ШИМ
    TIM2->CCMR1 |=  TIM_CCMR1_OC1PE | 6 << TIM_CCMR1_OC1M_Pos;
    TIM2->CCER |= TIM_CCER_CC1E; // включить канал 1
    TIM2->CR1 |= TIM_CR1_ARPE;  // Разрешить автоматическую перегрузку счетчика
    TIM2->CR1 |= TIM_CR1_CEN; // Включить таймер
    TIM2->CCR1 = 20; // Установить коэффициент заполнения на канал 3 - 0.625

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

    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOEEN;
    RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;

    GPIOE->MODER &= ~( GPIO_MODER_MODE8_Msk);
    GPIOE->MODER |= 1 <<  GPIO_MODER_MODE8_Pos;

    TIM2->DIER |= TIM_DIER_UIE;
    NVIC_EnableIRQ (TIM2_IRQn);

	while(1)
	{
		sleep(1000);

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

    TIM2->PSC = 19999; // Предделитель = 19999
    TIM2->ARR = time;
    TIM2->CR1 |= TIM_CR1_CEN;

    while(interrupt_counter);

    TIM2->CR1 &= ~TIM_CR1_CEN;

}
