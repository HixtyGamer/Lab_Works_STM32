#include "stm32g474xx.h"
#include <math.h>

#define TASK 3

void setup_clock();
void task1_3();
void task1_3_tim();
void task2_3();
void task2_3_tim();
void task3_b();
void task3_b_tim();

int8_t direction = 1;

int main(void)
{
#if TASK == 1
	task1_3();
#elif TASK == 2
	task2_3();
#elif TASK == 3
	task3_b();
#endif
}

void TIM3_IRQHandler (void)
{
#if TASK == 1
	task1_3_tim();
#elif TASK == 2
	task2_3_tim();
#elif TASK == 3
	task3_b_tim();
#endif
}

void setup_clock()
{
    // Установить задержку на доступ к Flash
    FLASH->ACR &= ~FLASH_ACR_LATENCY_Msk;
    FLASH->ACR |= FLASH_ACR_LATENCY_2WS;

    // Включить HSE как и в прошлом примере
    RCC->CR |= RCC_CR_HSEON;
    while ((RCC->CR & RCC_CR_HSERDY) != RCC_CR_HSERDY) {}

    // Сбросить начальные значения регистра настройки PLL
    RCC->PLLCFGR&=~(RCC_PLLCFGR_PLLR_Msk | RCC_PLLCFGR_PLLM_Msk | RCC_PLLCFGR_PLLN_Msk);

    // Настроить PLL по значениям, полученными в ioc файле
    RCC->PLLCFGR |= 0 << RCC_PLLCFGR_PLLR_Pos // Установка делителя R
                    | RCC_PLLCFGR_PLLREN    // Включение R делителя PLL
                    | 16 << RCC_PLLCFGR_PLLN_Pos // Установка умножителя N
                    | 1 << RCC_PLLCFGR_PLLM_Pos // Установка делителя M
                    | RCC_PLLCFGR_PLLSRC_HSE; // HSE - источник сигнала для PLL

    // Включить PLL
    RCC->CR |= RCC_CR_PLLON;

    // Подождать, пока PLL включится
    while ((RCC->CR & RCC_CR_PLLRDY) != RCC_CR_PLLRDY){}

    // Использовать PLL в качестве источника SYSCLK
    RCC->CFGR |= RCC_CFGR_SW_Msk;
    RCC->CFGR &= ~(RCC_CFGR_SW_Msk ^ RCC_CFGR_SW_PLL);

    // Подождать, до момента, пока источник частоты поменяется
    while ((RCC->CFGR & RCC_CFGR_SWS_PLL) != RCC_CFGR_SWS_PLL){}

}

void task1_3()
{
    setup_clock();

    // Включить тактирование
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIODEN;
    RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN | RCC_APB1ENR1_TIM3EN;

    // Настроить ножки на переменную функцию
    GPIOD->MODER &= ~GPIO_MODER_MODE3_Msk;
    GPIOD->MODER |= 2 << GPIO_MODER_MODE3_Pos;

    GPIOD->AFR[0] |= 2 << GPIO_AFRL_AFSEL3_Pos;

    // Настроить таймер
    TIM2->PSC = 0; // Предделитель = 1, то есть считаем каждый такт
    TIM2->ARR = 999;

    // Настроить таймер в режим ШИМ
    TIM2->CCMR1 |=  TIM_CCMR1_OC1PE | 6 << TIM_CCMR1_OC1M_Pos;
    TIM2->CCER |= TIM_CCER_CC1E;
    TIM2->CR1 |= TIM_CR1_ARPE;  // Разрешить автоматическую перегрузку счетчика
    TIM2->CR1 |= TIM_CR1_CEN; // Включить таймер
    TIM2->CCR1 = 199;

    TIM3->PSC = 31999;
    TIM3->ARR = 999;
    TIM3->DIER |= TIM_DIER_UIE;
    TIM3->CR1 |= TIM_CR1_CEN;
    NVIC_EnableIRQ (TIM3_IRQn);

    while(1)
    {

    }
}

void task1_3_tim()
{
	if(TIM2->CCR1 >= 799)
	{
		TIM2->CCR1 = 199;
	}
	else
	{
		TIM2->CCR1 += 300;
	}

	TIM3->SR &= ~ TIM_SR_UIF;
}

void task2_3()
{
    setup_clock();

    // Включить тактирование
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIODEN;
    RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN | RCC_APB1ENR1_TIM3EN;

    // Настроить ножки на переменную функцию
    GPIOD->MODER &= ~GPIO_MODER_MODE3_Msk;
    GPIOD->MODER |= 2 << GPIO_MODER_MODE3_Pos;

    GPIOD->AFR[0] |= 2 << GPIO_AFRL_AFSEL3_Pos;

    // Настроить таймер
    TIM2->PSC = 0; // Предделитель = 1, то есть считаем каждый такт
    TIM2->ARR = 999;

    // Настроить таймер в режим ШИМ
    TIM2->CCMR1 |=  TIM_CCMR1_OC1PE | 6 << TIM_CCMR1_OC1M_Pos;
    TIM2->CCER |= TIM_CCER_CC1E;
    TIM2->CR1 |= TIM_CR1_ARPE;  // Разрешить автоматическую перегрузку счетчика
    TIM2->CR1 |= TIM_CR1_CEN; // Включить таймер
    TIM2->CCR1 = 0;

    TIM3->PSC = 31999;
    TIM3->ARR = 4;
    TIM3->DIER |= TIM_DIER_UIE;
    TIM3->CR1 |= TIM_CR1_CEN;
    NVIC_EnableIRQ (TIM3_IRQn);

    while(1)
    {

    }
}

void task2_3_tim()
{
	TIM2->CCR1 += direction;

	if(TIM2->CCR1 == 999)
	{
		direction = -1;
	}
	else if(TIM2->CCR1 == 0)
	{
		direction = 1;
	}

	TIM3->SR &= ~ TIM_SR_UIF;
}

void task3_b()
{
	SCB->CPACR |= 0x3 << 20 ;
    setup_clock();

    // Включить тактирование
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIODEN;
    RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN | RCC_APB1ENR1_TIM3EN;

    // Настроить ножки на переменную функцию
    GPIOD->MODER &= ~GPIO_MODER_MODE3_Msk;
    GPIOD->MODER |= 2 << GPIO_MODER_MODE3_Pos;

    GPIOD->AFR[0] |= 2 << GPIO_AFRL_AFSEL3_Pos;

    // Настроить таймер
    TIM2->PSC = 7999; // Предделитель = 1, то есть считаем каждый такт
    TIM2->ARR = 999;

    // Настроить таймер в режим ШИМ
    TIM2->CCMR1 |=  TIM_CCMR1_OC1PE | 6 << TIM_CCMR1_OC1M_Pos;
    TIM2->CCER |= TIM_CCER_CC1E;
    TIM2->CR1 |= TIM_CR1_ARPE;  // Разрешить автоматическую перегрузку счетчика
    TIM2->CR1 |= TIM_CR1_CEN; // Включить таймер
    TIM2->CCR1 = 0;

    TIM3->PSC = 31999;
    TIM3->ARR = 8;
    TIM3->DIER |= TIM_DIER_UIE;
    TIM3->CR1 |= TIM_CR1_CEN;
    NVIC_EnableIRQ (TIM3_IRQn);

	direction = 0;

    while(1)
    {

    }
}

void task3_b_tim()
{
	direction += 1;

	TIM2->ARR = (uint16_t) ((sin(2 * 3.14 * direction / 256) + 3) * 100);
    TIM2->CCR1 = TIM2->ARR / 2;

	TIM3->SR &= ~ TIM_SR_UIF;
}
