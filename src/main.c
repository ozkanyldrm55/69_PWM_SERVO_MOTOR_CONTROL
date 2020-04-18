/* motorun 3 ayrý konuma ceviren program */

#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

GPIO_InitTypeDef GPIO_InitStruct;
TIM_TimeBaseInitTypeDef TIM_InitStruct;
TIM_OCInitTypeDef TIMOC_InitStruct; // PWM icin


void GPIO_Config(void) {
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);

	// MODE AF olarak kullandigimiz icin AF ayarlarini yapmamiz gerekmektedir
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource12,GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource13,GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource14,GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource15,GPIO_AF_TIM4);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF; // timer olarak kullanildigi icin AF olarak kullandik.
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;

	GPIO_Init(GPIOD , &GPIO_InitStruct);
}

void TIM_Config(void) {
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);

	TIM_InitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_InitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_InitStruct.TIM_Period = 19999;
	TIM_InitStruct.TIM_Prescaler = 83;
	TIM_InitStruct.TIM_RepetitionCounter = 0;

	TIM_TimeBaseInit(TIM4 , &TIM_InitStruct);
	TIM_Cmd(TIM4,ENABLE);

	/* MOTOR OZELLIKLERI : 20ms(50Hz) PWM Periyod  1 - 2 ms duty cycle
	 *
	 * periyod = 20ms = 20000us demektir
	 *
	 * tick_freq = periyod*pwm_freq -> 20000*50 = 1MHz
	 *
	 * precaler = (timer_clk / tick_timer_freq)-1  -> (84MHz/1MHZ)-1 = 83
	 
	  PWM FREKANSI OLUSTURMAK ICIN ASAGIDAKI FORMULLER KULLANILIR
	   * -> periyod = (Timer_Tick_Freq / PWM_Freq)-1
	   * -> Time_Tick_Freq = Timer_CLK /(Prescaler + 1) 
	   * -> PWM_Freq = Timer_Tick_Freq / (periyod + 1)  
	 */

	TIMOC_InitStruct.TIM_OCMode = TIM_OCMode_PWM1; 	// PWM mod1 mod2 muhabbeti , doluluk oranlarýyla ilgili
	TIMOC_InitStruct.TIM_OutputState = ENABLE;	// cikisi aktif ettik
	TIMOC_InitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
}

void delay(uint32_t time) {
	while(time--); // 8 cycle da bir islem yapar
}

int main(void) {
	GPIO_Config();
	TIM_Config();
  while (1)
  {
	  TIMOC_InitStruct.TIM_Pulse = 500; // periyod 20ms oldugu icin  (0.5/20)*19999 = 500
	  TIM_OC1Init(TIM4,&TIMOC_InitStruct);
	  TIM_OC1PreloadConfig(TIM4,TIM_OCPreload_Enable);
	  delay(21000000); // 8cycle*168000000 = 21000000 , 1sn demek

	  TIMOC_InitStruct.TIM_Pulse = 1500; // (1,5/20)*19999 = 1500
	  TIM_OC1Init(TIM4,&TIMOC_InitStruct);
	  TIM_OC1PreloadConfig(TIM4,TIM_OCPreload_Enable);

	  TIMOC_InitStruct.TIM_Pulse = 2500; // (2,5/20)*19999 = 2500
	  TIM_OC1Init(TIM4,&TIMOC_InitStruct);
	  TIM_OC1PreloadConfig(TIM4,TIM_OCPreload_Enable);
  }
}

void EVAL_AUDIO_TransferComplete_CallBack(uint32_t pBuffer, uint32_t Size){
  /* TODO, implement your code here */
  return;
}

/*
 * Callback used by stm324xg_eval_audio_codec.c.
 * Refer to stm324xg_eval_audio_codec.h for more info.
 */
uint16_t EVAL_AUDIO_GetSampleCallBack(void){
  /* TODO, implement your code here */
  return -1;
}
