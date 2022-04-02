#include "stm32g474xx.h"
//#include "math.h"

#define TASK 5

void setup_clock();
void setup_adc_dma( uint16_t *array_to_write_to, uint8_t ch_count );
void adc_manually_get_data();
void dummy_delay(uint32_t duration);				//нужна для примера
void sleep(uint16_t time);
void example();
void task1_1();
void task1_2();
void task1_3();
void task2_1();
void task2_2();
void task2_3();


uint16_t adc_data[4] = { 0 };
uint16_t current_adc_value[1] = { 0 },  		//массивом, чтобы использовать чутка модифицированную функцию из примера для настройки DMA
		 current_adc_values[2] = { 0 },
		 prev_adc_values[2] = { 0 },			//для творческого задания 3
		delay_duration;	//для творческого задания 2
uint8_t interrupt_counter,						//нужна для работы точной задержки
		leds_to_light_up,
		leds_on = 0;

int main(void)
{
#if TASK == 0
	example();
#elif TASK == 1
	task1_1();
#elif TASK == 2
	task1_2();
#elif TASK == 3
	task1_3();
#elif TASK == 4
	task2_1();
#elif TASK == 5
	task2_2();
#elif TASK == 6
	task2_3();
#endif
}

void TIM2_IRQHandler (void)  //используется только для точной задержки
{
	interrupt_counter = 0;

	TIM2->SR &= ~ TIM_SR_UIF;
}

void example()
{
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;

	// Выбрать SYSCLK в качестве источника тактового сигнала для АЦП
	RCC->CCIPR |= (2U) << RCC_CCIPR_ADC12SEL_Pos;

	// Включить тактирование АЦП.
	RCC->AHB2ENR |= RCC_AHB2ENR_ADC12EN;

	// Выйти из режима power-down.
	ADC2->CR &= ~(ADC_CR_DEEPPWD);

	// Включить внутренний преобразователь питания и подождать, пока он запустится.
	ADC2->CR |= ADC_CR_ADVREGEN;
	dummy_delay( 10000 );

	// Включить автоматическую калибровку АЦП и подождать пока она закончится.
	ADC2->CR |= ADC_CR_ADCAL;
	while ( ADC2->CR & ADC_CR_ADCAL ){}

	// Включить АЦП.
	ADC2->ISR |= ADC_ISR_ADRDY; // Очистить флаг ADRDY.
	ADC2->CR |= ADC_CR_ADEN;
	while ( !(ADC2->ISR & ADC_ISR_ADRDY) ){} // Подождать до включения.

	// Увеличить время считывания показания для нужного нам канала
	ADC2->SMPR1 |= 2 << ADC_SMPR1_SMP9_Pos
				   | 2 << ADC_SMPR1_SMP8_Pos
				   | 2 << ADC_SMPR1_SMP7_Pos
				   | 2 << ADC_SMPR1_SMP6_Pos;

	// Настроить канал, из которого мы будем считывать данные.
	ADC2->SQR1 |= 9 << ADC_SQR1_SQ4_Pos
				  | 8 << ADC_SQR1_SQ3_Pos
				  | 7 << ADC_SQR1_SQ2_Pos
				  | 6 << ADC_SQR1_SQ1_Pos
				  | 3 << ADC_SQR1_L_Pos;

	// Включить работу с DMA в АЦП и настроить DMA.
	ADC2->CFGR |= ADC_CFGR_DMAEN | ADC_CFGR_DMACFG;
	setup_adc_dma(adc_data, 4);

	while ( 1 )
	{
		adc_manually_get_data();
		dummy_delay( 500000 );
	}

}

void task1_1()
{
	setup_clock();

    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;

    RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;

    TIM2->DIER |= TIM_DIER_UIE;
  //TIM2->PSC = 19999;
    NVIC_EnableIRQ (TIM2_IRQn);

    RCC->CCIPR |= (2U) << RCC_CCIPR_ADC12SEL_Pos;

    RCC->AHB2ENR |= RCC_AHB2ENR_ADC12EN;

    ADC2->CR &= ~(ADC_CR_DEEPPWD);

    ADC2->CR |= ADC_CR_ADVREGEN;
    sleep(1);

    ADC2->CR |= ADC_CR_ADCAL;
    while ( ADC2->CR & ADC_CR_ADCAL ){}

    ADC2->ISR |= ADC_ISR_ADRDY;
    ADC2->CR |= ADC_CR_ADEN;
    while ( !(ADC2->ISR & ADC_ISR_ADRDY) ){}

    ADC2->SMPR1 |= 2 << ADC_SMPR1_SMP6_Pos;

    ADC2->SQR1 |= 6 << ADC_SQR1_SQ1_Pos | 0 << ADC_SQR1_L_Pos;

    adc_manually_get_data();

    current_adc_value[0] = ADC2->DR;

    while(1)
    {

    }

}

void task1_2()
{
	setup_clock();

    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;

    RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;

    TIM2->DIER |= TIM_DIER_UIE;
  //TIM2->PSC = 19999;
    NVIC_EnableIRQ (TIM2_IRQn);

    RCC->CCIPR |= (2U) << RCC_CCIPR_ADC12SEL_Pos;

    RCC->AHB2ENR |= RCC_AHB2ENR_ADC12EN;

    ADC2->CR &= ~(ADC_CR_DEEPPWD);

    ADC2->CR |= ADC_CR_ADVREGEN;
    sleep(1);

    ADC2->CR |= ADC_CR_ADCAL;
    while ( ADC2->CR & ADC_CR_ADCAL ){}

    ADC2->ISR |= ADC_ISR_ADRDY;
    ADC2->CR |= ADC_CR_ADEN;
    while ( !(ADC2->ISR & ADC_ISR_ADRDY) ){}

	ADC2->SMPR1 |= 2 << ADC_SMPR1_SMP9_Pos
				 | 2 << ADC_SMPR1_SMP7_Pos;

    ADC2->SQR1 |= 7 << ADC_SQR1_SQ1_Pos
    			| 9 << ADC_SQR1_SQ2_Pos
    			| 1 << ADC_SQR1_L_Pos;

	ADC2->CFGR |= ADC_CFGR_DMAEN | ADC_CFGR_DMACFG;
	setup_adc_dma(current_adc_values, 2);

	adc_manually_get_data();

    while ( 1 )
    {

    }
}

void task1_3()
{
	setup_clock();

    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;

    RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;

    TIM2->DIER |= TIM_DIER_UIE;
  //TIM2->PSC = 19999;
    NVIC_EnableIRQ (TIM2_IRQn);

    RCC->CCIPR |= (2U) << RCC_CCIPR_ADC12SEL_Pos;

    RCC->AHB2ENR |= RCC_AHB2ENR_ADC12EN;

    ADC2->CR &= ~(ADC_CR_DEEPPWD);

    ADC2->CR |= ADC_CR_ADVREGEN;
    sleep(1);

    ADC2->CR |= ADC_CR_ADCAL;
    while ( ADC2->CR & ADC_CR_ADCAL ){}

    ADC2->ISR |= ADC_ISR_ADRDY;
    ADC2->CR |= ADC_CR_ADEN;
    while ( !(ADC2->ISR & ADC_ISR_ADRDY) ){}

	ADC2->SMPR1 |= 2 << ADC_SMPR1_SMP9_Pos
				 | 2 << ADC_SMPR1_SMP7_Pos;

    ADC2->SQR1 |= 7 << ADC_SQR1_SQ1_Pos
    			| 9 << ADC_SQR1_SQ2_Pos
    			| 1 << ADC_SQR1_L_Pos;

	ADC2->CFGR |= ADC_CFGR_DMAEN | ADC_CFGR_DMACFG;
	setup_adc_dma(current_adc_values, 2);

    while(1)
    {
    	adc_manually_get_data();
    	sleep(250);
    }
}

void task2_1()
{
	setup_clock();

    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN | RCC_AHB2ENR_GPIOEEN;

    RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;

    GPIOE->MODER &= ~(0xFFFF << GPIO_MODER_MODE0_Pos); //0b1111111111111111
    GPIOE->MODER |= 0x5555 << GPIO_MODER_MODE0_Pos;	   //0b0101010101010101

    TIM2->DIER |= TIM_DIER_UIE;
  //TIM2->PSC = 19999;
    NVIC_EnableIRQ (TIM2_IRQn);

    RCC->CCIPR |= (2U) << RCC_CCIPR_ADC12SEL_Pos;

    RCC->AHB2ENR |= RCC_AHB2ENR_ADC12EN;

    ADC2->CR &= ~(ADC_CR_DEEPPWD);

    ADC2->CR |= ADC_CR_ADVREGEN;
    sleep(1);

    ADC2->CR |= ADC_CR_ADCAL;
    while ( ADC2->CR & ADC_CR_ADCAL ){}

    ADC2->ISR |= ADC_ISR_ADRDY;
    ADC2->CR |= ADC_CR_ADEN;
    while ( !(ADC2->ISR & ADC_ISR_ADRDY) ){}

	ADC2->SMPR1 |= 2 << ADC_SMPR1_SMP6_Pos;

    ADC2->SQR1 |= 6 << ADC_SQR1_SQ1_Pos
    			| 0 << ADC_SQR1_L_Pos;

	ADC2->CFGR |= ADC_CFGR_DMAEN | ADC_CFGR_DMACFG;
	setup_adc_dma(current_adc_value, 1);

    while(1)
    {
    	adc_manually_get_data();
    	leds_to_light_up = current_adc_value[0] / 455; //455 ~ 4096 / 9
    	if(leds_on != leds_to_light_up)
    	{
    		leds_on = leds_to_light_up;
        	GPIOE->BSRR = 0xFF << GPIO_BSRR_BR0_Pos;
        	GPIOE->BSRR = ((1 << leds_to_light_up) - 1) << GPIO_BSRR_BS0_Pos;
    	}
    	sleep(50);     //обновление ~20 раз/с
    }
}

void task2_2()
{
	setup_clock();

    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN | RCC_AHB2ENR_GPIOEEN;

    RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;

    GPIOE->MODER &= ~(0xFFFF << GPIO_MODER_MODE0_Pos); //0b1111111111111111
    GPIOE->MODER |= 0x5555 << GPIO_MODER_MODE0_Pos;	   //0b0101010101010101

    TIM2->DIER |= TIM_DIER_UIE;
  //TIM2->PSC = 19999;
    NVIC_EnableIRQ (TIM2_IRQn);

    RCC->CCIPR |= (2U) << RCC_CCIPR_ADC12SEL_Pos;

    RCC->AHB2ENR |= RCC_AHB2ENR_ADC12EN;

    ADC2->CR &= ~(ADC_CR_DEEPPWD);

    ADC2->CR |= ADC_CR_ADVREGEN;
    sleep(1);

    ADC2->CR |= ADC_CR_ADCAL;
    while ( ADC2->CR & ADC_CR_ADCAL ){}

    ADC2->ISR |= ADC_ISR_ADRDY;
    ADC2->CR |= ADC_CR_ADEN;
    while ( !(ADC2->ISR & ADC_ISR_ADRDY) ){}

	ADC2->SMPR1 |= 2 << ADC_SMPR1_SMP6_Pos;

    ADC2->SQR1 |= 6 << ADC_SQR1_SQ1_Pos
    			| 0 << ADC_SQR1_L_Pos;

	ADC2->CFGR |= ADC_CFGR_DMAEN | ADC_CFGR_DMACFG;
	setup_adc_dma(current_adc_value, 1);

    while(1)
    {
		GPIOE->BSRR = 0xFF << GPIO_BSRR_BR0_Pos;
		GPIOE->BSRR = (1 << leds_to_light_up) << GPIO_BSRR_BS0_Pos;

    	adc_manually_get_data();

		//значения частоты должны меняться от 0,5 Гц до 10 Гц, а задержка равна 1/частота
		//если считать задержку через частоту, то можно добиться линейного изменения частоты
		delay_duration = 8192000 / (19 * current_adc_value[0] + 4096);  //если нужно будет, то объясню как я пришёл к этой формуле

    	sleep(delay_duration);

    	if(leds_to_light_up == 7)
    	{
    		leds_to_light_up = 0;
    	}
    	else
    	{
    		leds_to_light_up++;
    	}
    }
}

void task2_3()
{
	setup_clock();

    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN | RCC_AHB2ENR_GPIOEEN;

    RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN | RCC_APB1ENR1_TIM3EN;

    GPIOE->MODER &= ~(GPIO_MODER_MODE2_Msk
    				| GPIO_MODER_MODE3_Msk);
    GPIOE->MODER |= 2 << GPIO_MODER_MODE2_Pos
    			  | 2 << GPIO_MODER_MODE3_Pos;

    GPIOE->AFR[0] |= 2 << GPIO_AFRL_AFSEL2_Pos | 2 << GPIO_AFRL_AFSEL3_Pos;

    TIM3->PSC = 0;
    TIM3->ARR = 4095;
    TIM3->CCMR1 |=  TIM_CCMR1_OC1PE | 6 << TIM_CCMR1_OC1M_Pos
    			   | TIM_CCMR1_OC2PE | 6 << TIM_CCMR1_OC2M_Pos;
    TIM3->CCER |= TIM_CCER_CC1E | TIM_CCER_CC2E;
    TIM3->CR1 |= TIM_CR1_ARPE;
    TIM3->CR1 |= TIM_CR1_CEN;

    TIM2->DIER |= TIM_DIER_UIE;
    NVIC_EnableIRQ (TIM2_IRQn);

    RCC->CCIPR |= (2U) << RCC_CCIPR_ADC12SEL_Pos;

    RCC->AHB2ENR |= RCC_AHB2ENR_ADC12EN;

    ADC2->CR &= ~(ADC_CR_DEEPPWD);

    ADC2->CR |= ADC_CR_ADVREGEN;
    sleep(1);

    ADC2->CR |= ADC_CR_ADCAL;
    while ( ADC2->CR & ADC_CR_ADCAL ){}

    ADC2->ISR |= ADC_ISR_ADRDY;
    ADC2->CR |= ADC_CR_ADEN;
    while ( !(ADC2->ISR & ADC_ISR_ADRDY) ){}

	ADC2->SMPR1 |= 2 << ADC_SMPR1_SMP6_Pos
				 | 2 << ADC_SMPR1_SMP7_Pos;

    ADC2->SQR1 |= 6 << ADC_SQR1_SQ1_Pos
    			| 7 << ADC_SQR1_SQ2_Pos
    			| 1 << ADC_SQR1_L_Pos;

	ADC2->CFGR |= ADC_CFGR_DMAEN | ADC_CFGR_DMACFG;
	setup_adc_dma(current_adc_values, 2);

    while(1)
    {
    	adc_manually_get_data();

    	if(prev_adc_values[0] != current_adc_values[0])
    	{
        	TIM3->CCR1 = current_adc_values[0];
        	prev_adc_values[0] = current_adc_values[0];
    	}

    	if(prev_adc_values[1] != current_adc_values[1])
    	{
        	TIM3->CCR2 = current_adc_values[1];
        	prev_adc_values[1] = current_adc_values[1];
    	}

    	sleep(50);
    }
}

void setup_adc_dma( uint16_t *array_to_write_to, uint8_t ch_count )
{
    // Включить тактирование DMA
    RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN | RCC_AHB1ENR_DMAMUX1EN;

    // Настроить адрес откуда будет происходить чтение
    DMA1_Channel1->CPAR = (uint32_t)&(ADC2->DR);

    // Настроить адрес куда будет происходить записью надо напрямую преобразовать
    // адрес в число типа uin32_t, так как все адреса - 32 значения.
    DMA1_Channel1->CMAR = (uint32_t)array_to_write_to;

    // Настроить количество значений, которые мы хотим считать.
    DMA1_Channel1->CNDTR = ch_count;

    // Настроить канал DMA: размер данных 16 бит источник и приемник, циклический
    // режи, увеличивать указатель при записи.
    DMA1_Channel1->CCR |= 1 << DMA_CCR_MSIZE_Pos | 1 << DMA_CCR_PSIZE_Pos
                       | DMA_CCR_MINC | DMA_CCR_CIRC;

    // Настроить мультиплексор каналов DMA
    DMAMUX1_Channel0->CCR |= (36 << DMAMUX_CxCR_DMAREQ_ID_Pos);
    // Включить DMA
    DMA1_Channel1->CCR |= DMA_CCR_EN;
}

void adc_manually_get_data()
{
    // Начать считывание данных из АЦП
    ADC2->CR |= ADC_CR_ADSTART;

    // Подождать до завершения измерения
    while ( !(ADC2->ISR & ADC_ISR_EOS) ){}

    // Сбросить флаг завершения измерения
    ADC2->ISR |= ADC_ISR_EOS;
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

void sleep(uint16_t time)
{
	interrupt_counter = 1;

	if(time == 0)
	{
		interrupt_counter = 0;
	}
	else if(time == 1)
	{
	    TIM2->PSC = 9999;
	    TIM2->ARR = 1;
	}
	else
	{
	    TIM2->PSC = 19999;
		TIM2->ARR = time - 1;
	}

    TIM2->CR1 |= TIM_CR1_CEN;

    TIM2->SR &= ~ TIM_SR_UIF;

    while(interrupt_counter);

    TIM2->CR1 &= ~TIM_CR1_CEN;
    TIM2->CNT = 0;
}

void dummy_delay(uint32_t duration)
{
	for(uint32_t i = 0; i < duration; i++);
}
