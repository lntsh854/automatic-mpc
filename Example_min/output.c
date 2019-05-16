#include "C:\Users\Admin\Documents\GitHub\Cpp\Kompilator_skrosny\Libs\C\/defines.h"
#include "C:\Users\Admin\Documents\GitHub\Cpp\Kompilator_skrosny\Libs\C\/mpctools.h"
#include "C:\Users\Admin\Documents\GitHub\Cpp\Kompilator_skrosny\Libs\C\/simulated_signals.h"
#include "C:\Users\Admin\Documents\GitHub\Cpp\Kompilator_skrosny\Libs\C\/default_functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <math.h>
#include <sys/time.h>
#include "misc.h"
#include "stm32f4xx_conf.h"
#include "stm32f4xx_it.h"
#include "stm32f4xx.h"

#define VAL_MAX 0x0FFF
#define VAL_MIN 0x0000
#define PRECISION 1000.0f
/*STM32 Code*/
#define LEDG 0
#define LEDO 1
#define LEDR 2
#define LEDB 3

#define BUT0 0
#define BUT1 1
#define BUT2 2
#define BUT3 3
#define BUT4 4
__IO uint32_t TimingDelay = 0;
__IO uint32_t timer = 0;
volatile char str[1000];
volatile char * usart_str;
volatile int str_len = 0;

void Delay(__IO uint32_t nTime){ TimingDelay = nTime; while(TimingDelay != 0); }
void SysTick_Handler(void){ ++timer; if (TimingDelay != 0x00) --TimingDelay; }

uint32_t led2pin(int l){
    switch(l){
        case LEDG: return GPIO_Pin_12;
        case LEDO: return GPIO_Pin_13;
        case LEDR: return GPIO_Pin_14;
        case LEDB: return GPIO_Pin_15;
    }
    return GPIO_Pin_12;
}
uint32_t but2pin(int l){
    switch(l){
        case BUT1: return GPIO_Pin_1;
        case BUT2: return GPIO_Pin_2;
        case BUT3: return GPIO_Pin_3;
        case BUT4: return GPIO_Pin_4;
    }
    return GPIO_Pin_1;
}

void ledOn    (int l){ GPIO_SetBits(GPIOD, led2pin(l)); }
void ledOff   (int l){ GPIO_ResetBits(GPIOD, led2pin(l)); }
void ledToggle(int l){ GPIO_ToggleBits(GPIOD, led2pin(l)); }
void ledsSpin(){
    static int i = 0;
    int k = 0;
    for(k=0; k<4; ++k) ledOff(k);
    ledOn(i);
    i = (i+1)%4;
}
int readButton(int b){ 
    if(b==BUT0) return GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0); 
    else        return GPIO_ReadInputDataBit(GPIOD, but2pin(b));
}

void UsartInit(){
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    // USART 6
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_USART6);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    

    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_Init(USART6, &USART_InitStructure);
    USART_ITConfig(USART6, USART_IT_TXE, DISABLE);
    USART_Cmd(USART6, ENABLE);
}

void USART6_IRQHandler( void ){
    if(USART_GetITStatus(USART6, USART_IT_TXE) != RESET){
        if(*usart_str){
            USART_SendData(USART6, *usart_str++);
        }else{
            USART_ITConfig(USART6, USART_IT_TXE, DISABLE);
            usart_str = NULL;
        }
    }
}

void TIM3_IRQHandler(void){
    if(TIM_GetITStatus(TIM3,TIM_IT_Update) != RESET){
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
        timer_loop();
    }
}

void TimerInit(void){
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    /* TIM3 clock enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    /* Enable the TIM3 gloabal Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    // NVIC_SetPriorityGrouping(2);
    NVIC_SetPriority(TIM3_IRQn, 1);
 
    // PrescalerValue = (uint16_t) ((SystemCoreClock / 2) / 65536) - 1;
    TIM_TimeBaseStructure.TIM_Period = 2000 - 1;
    TIM_TimeBaseStructure.TIM_Prescaler = 4200 - 1;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
    /* TIM Interrupts enable */
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
    /* TIM3 enable counter */
    TIM_Cmd(TIM3, ENABLE);
}

void ClkInit(){
    SystemCoreClockUpdate();                                // Update system clock (needed to obtain SystemCoreClock)
    SysTick_Config(SystemCoreClock / 1000000);                 // Enable SysTick
}

void LedInit(){
    /* Configure the GPIO_LED pin */
    GPIO_InitTypeDef GPIO_InitStructure;
    
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);   // Enable the GPIO_LED Clock
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    ledOff(LEDO);
    ledOff(LEDR);
    ledOff(LEDG);
    ledOff(LEDB);
}

void ButtonsInit(){
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOD, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
}

void DACInit(){
    GPIO_InitTypeDef GPIO_InitStructure;
    DAC_InitTypeDef  DAC_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    DAC_DeInit();

    DAC_InitStructure.DAC_Trigger = DAC_Trigger_None;
    DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
    DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
    DAC_Init(DAC_Channel_1, &DAC_InitStructure);
    DAC_Cmd(DAC_Channel_1, ENABLE);
}

void ADCInit(){
  ADC_InitTypeDef       ADC_InitStructure;
  ADC_CommonInitTypeDef ADC_CommonInitStructure;
  GPIO_InitTypeDef      GPIO_InitStructure;

  /* Enable ADC3, DMA2 and GPIO clocks ****************************************/
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);

  /* Configure ADC3 Channel12 pin as analog input ******************************/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  ADC_DeInit();

  /* ADC Common Init **********************************************************/
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
  ADC_CommonInit(&ADC_CommonInitStructure);

  /* ADC3 Init ****************************************************************/
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  ADC_InitStructure.ADC_ExternalTrigConv = 0;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfConversion = 1;
  ADC_Init(ADC3, &ADC_InitStructure);

  /* ADC3 regular channel12 configuration *************************************/
  ADC_RegularChannelConfig(ADC3, ADC_Channel_12, 1, ADC_SampleTime_3Cycles);

  /* Enable ADC3 */
  ADC_Cmd(ADC3, ENABLE);
}

void FpuInit(void){
    SCB->CPACR |= ((3UL << 10*2)|(3UL << 11*2));  /* set CP10 and CP11 Full Access */
}

void initialize(){
    ClkInit();
    ADCInit();
    DACInit();
    LedInit();  
    ButtonsInit();
    UsartInit();
    FpuInit();
    TimerInit();
}

void write_string(char * s){
    usart_str = s;
    USART_ITConfig(USART6, USART_IT_TXE, ENABLE);
    while(usart_str){;}
    memset(s, 0, strlen(s));
}

char tmpstr[100];
char * float2str(float x){
    int sign = 1; // plus
    if(x < 0) sign = -1;
    x = x*sign; // remove sign

    int integer = (int)(x);
    int fractional = (int)( (x-integer)*10000 );
    if(sign > 0){
        sprintf(tmpstr, " %d.%04d",integer, fractional);
    } else {
        sprintf(tmpstr, "-%d.%04d",integer, fractional);
    }
    return tmpstr;
}

long get_time(){
    return timer;
}

uint16_t __measureOutput(){
    uint16_t value = 0;
    ADC_SoftwareStartConv(ADC3);
    while(ADC_GetFlagStatus(ADC3, ADC_FLAG_EOC) == RESET);
    value = (uint16_t)ADC_GetConversionValue(ADC3);
    return value;
}

float measureOutput(){
    return ((float)__measureOutput())/PRECISION;
}

void __setControlValue(uint16_t value){
    DAC_SetChannel1Data(DAC_Align_12b_R, value);
}

void setControlValue(float value){
    __setControlValue((uint16_t)(value*PRECISION));
}

void hardware_setup(){
    initialize();
}

//////////////////////////////////////////////////// TUTAJ ZACZYNA SIE ROBOTA SPECJALISTY MATLAB
void PID(ArchiveData * ad, CurrentControl * c){
	static float e0;
	static float e1;
	static float e2;
	if(ad == NULL) return;
	e0 = ad->yzad - ad->yk[M( 0)];
	e1 = ad->yzad - ad->yk[M(-1)];
	e2 = ad->yzad - ad->yk[M(-2)];
	setCurrentControlIncrement(c,4.950000*e0 + -4.050000*e1 + 0.000000*e2); // algorytm przyrostowy
}

void DMC_analitic(ArchiveData * ad, CurrentControl * c){
	static long i,j,k;
	static float du;
	static float dUp[98] = {0};
	static float Y[10] = {0};
	static float Y0[10] = {0};
	static float K1[10] = {1.959015317862772e-03,7.100210280605816e-02,1.069511201181385e-01,1.195219445154055e-01,1.166528094409197e-01,1.030149272550155e-01,9.180318051940477e-02,8.158882978483079e-02,7.236533916169056e-02,6.188918861371388e-02};
	static float Mp[10][98] = {{1.015000000000000e-01,9.070000000000000e-02,8.165000000000000e-02,7.414999999999999e-02,6.555000000000000e-02,6.020000000000003e-02,5.425000000000002e-02,4.899999999999993e-02,4.110000000000003e-02,4.000000000000004e-02,3.554999999999997e-02,3.120000000000001e-02,2.839999999999998e-02,2.664999999999995e-02,2.160000000000006e-02,2.185000000000004e-02,1.894999999999991e-02,1.510000000000000e-02,1.519999999999999e-02,1.475000000000004e-02,1.239999999999997e-02,9.450000000000070e-03,9.749999999999925e-03,1.035000000000008e-02,9.549999999999947e-03,6.700000000000039e-03,4.750000000000032e-03,5.999999999999894e-03,6.100000000000105e-03,5.899999999999905e-03,2.200000000000091e-03,6.549999999999945e-03,9.000000000000119e-04,4.449999999999954e-03,1.900000000000013e-03,3.149999999999986e-03,3.250000000000086e-03,1.299999999999968e-03,1.199999999999979e-03,1.850000000000018e-03,5.499999999999394e-04,1.950000000000007e-03,1.149999999999984e-03,3.800000000000026e-03,-9.999999999998899e-04,2.999999999999670e-04,2.999999999999892e-03,-1.499999999998725e-04,-2.999999999999670e-04,4.999999999999449e-04,0.000000000000000e+00,2.650000000000041e-03,3.499999999998504e-04,-1.449999999999951e-03,-6.999999999999229e-04,3.349999999999964e-03,-5.999999999999339e-04,-1.350000000000184e-03,8.000000000001339e-04,6.999999999999229e-04,-1.549999999999940e-03,1.049999999999995e-03,1.999999999999780e-04,1.100000000000101e-03,4.999999999988347e-05,-6.500000000000394e-04,-3.499999999998504e-04,1.949999999999896e-03,4.999999999999449e-04,-6.500000000000394e-04,-1.599999999999824e-03,1.499999999998725e-04,1.500000000000945e-04,9.500000000000064e-04,-6.500000000000394e-04,9.999999999998899e-04,8.000000000001339e-04,-1.649999999999929e-03,9.500000000000064e-04,1.499999999998725e-04,-9.999999999998899e-05,2.500000000000835e-04,9.999999999998899e-04,-2.399999999999958e-03,1.049999999999995e-03,8.999999999999009e-04,-1.799999999999802e-03,3.499999999998504e-04,4.999999999999449e-04,1.000000000002110e-04,-5.000000000010552e-05,6.500000000000394e-04,-1.200000000000090e-03,8.000000000001339e-04,-5.000000000001670e-04,-2.499999999998614e-04,-1.500000000000945e-04,1.249999999999973e-03},{1.922000000000000e-01,1.723500000000000e-01,1.558000000000000e-01,1.397000000000000e-01,1.257500000000000e-01,1.144500000000001e-01,1.032500000000000e-01,9.009999999999996e-02,8.110000000000006e-02,7.555000000000001e-02,6.674999999999998e-02,5.959999999999999e-02,5.504999999999993e-02,4.825000000000002e-02,4.345000000000010e-02,4.079999999999995e-02,3.404999999999991e-02,3.029999999999999e-02,2.995000000000003e-02,2.715000000000001e-02,2.185000000000004e-02,1.920000000000000e-02,2.010000000000001e-02,1.990000000000003e-02,1.624999999999999e-02,1.145000000000007e-02,1.074999999999993e-02,1.210000000000000e-02,1.200000000000001e-02,8.099999999999996e-03,8.750000000000036e-03,7.449999999999957e-03,5.349999999999966e-03,6.349999999999967e-03,5.049999999999999e-03,6.400000000000072e-03,4.550000000000054e-03,2.499999999999947e-03,3.049999999999997e-03,2.399999999999958e-03,2.499999999999947e-03,3.099999999999992e-03,4.950000000000010e-03,2.800000000000136e-03,-6.999999999999229e-04,3.299999999999859e-03,2.850000000000019e-03,-4.499999999998394e-04,1.999999999999780e-04,4.999999999999449e-04,2.650000000000041e-03,2.999999999999892e-03,-1.100000000000101e-03,-2.149999999999874e-03,2.650000000000041e-03,2.750000000000030e-03,-1.950000000000118e-03,-5.500000000000505e-04,1.500000000000057e-03,-8.500000000000174e-04,-4.999999999999449e-04,1.249999999999973e-03,1.300000000000079e-03,1.149999999999984e-03,-6.000000000001560e-04,-9.999999999998899e-04,1.600000000000046e-03,2.449999999999841e-03,-1.500000000000945e-04,-2.249999999999863e-03,-1.449999999999951e-03,2.999999999999670e-04,1.100000000000101e-03,2.999999999999670e-04,3.499999999998504e-04,1.800000000000024e-03,-8.499999999997954e-04,-6.999999999999229e-04,1.099999999999879e-03,4.999999999988347e-05,1.500000000000945e-04,1.249999999999973e-03,-1.400000000000068e-03,-1.349999999999962e-03,1.949999999999896e-03,-8.999999999999009e-04,-1.449999999999951e-03,8.499999999997954e-04,6.000000000001560e-04,5.000000000010552e-05,5.999999999999339e-04,-5.500000000000505e-04,-3.999999999999560e-04,2.999999999999670e-04,-7.500000000000284e-04,-3.999999999999560e-04,1.099999999999879e-03,1.249999999999973e-03},{2.738500000000000e-01,2.465000000000000e-01,2.213500000000000e-01,1.999000000000000e-01,1.800000000000001e-01,1.634500000000000e-01,1.443500000000000e-01,1.301000000000000e-01,1.166500000000000e-01,1.067500000000000e-01,9.514999999999996e-02,8.624999999999994e-02,7.665000000000000e-02,7.010000000000005e-02,6.240000000000001e-02,5.589999999999995e-02,4.924999999999991e-02,4.505000000000003e-02,4.235000000000000e-02,3.660000000000008e-02,3.159999999999996e-02,2.955000000000008e-02,2.964999999999995e-02,2.660000000000007e-02,2.100000000000002e-02,1.744999999999997e-02,1.685000000000003e-02,1.799999999999991e-02,1.420000000000010e-02,1.464999999999994e-02,9.650000000000047e-03,1.189999999999991e-02,7.249999999999979e-03,9.499999999999953e-03,8.300000000000085e-03,7.700000000000040e-03,5.750000000000033e-03,4.349999999999965e-03,3.599999999999937e-03,4.349999999999965e-03,3.649999999999931e-03,6.900000000000017e-03,3.950000000000120e-03,3.100000000000103e-03,2.299999999999969e-03,3.149999999999986e-03,2.550000000000052e-03,5.000000000010552e-05,1.999999999999780e-04,3.149999999999986e-03,2.999999999999892e-03,1.549999999999940e-03,-1.800000000000024e-03,1.200000000000090e-03,2.050000000000107e-03,1.399999999999846e-03,-1.149999999999984e-03,1.499999999998725e-04,-4.999999999988347e-05,1.999999999999780e-04,-2.999999999999670e-04,2.350000000000074e-03,1.349999999999962e-03,4.999999999999449e-04,-9.500000000000064e-04,9.500000000000064e-04,2.099999999999991e-03,1.799999999999802e-03,-1.749999999999918e-03,-2.099999999999991e-03,-1.299999999999857e-03,1.249999999999973e-03,4.500000000000615e-04,1.299999999999857e-03,1.149999999999984e-03,1.500000000000945e-04,1.000000000002110e-04,-5.500000000000505e-04,9.999999999998899e-04,2.999999999999670e-04,1.149999999999984e-03,-1.149999999999984e-03,-3.500000000000725e-04,-4.500000000000615e-04,1.500000000000945e-04,-5.500000000000505e-04,-9.500000000000064e-04,9.500000000000064e-04,5.500000000000505e-04,7.000000000001450e-04,-6.000000000001560e-04,2.500000000000835e-04,-9.000000000001229e-04,5.000000000010552e-05,-9.000000000001229e-04,8.500000000000174e-04,1.099999999999879e-03,1.249999999999973e-03},{3.480000000000000e-01,3.120500000000000e-01,2.815500000000000e-01,2.541500000000000e-01,2.290000000000000e-01,2.045500000000000e-01,1.843500000000000e-01,1.656500000000000e-01,1.478500000000000e-01,1.351500000000000e-01,1.217999999999999e-01,1.078500000000000e-01,9.850000000000003e-02,8.904999999999996e-02,7.750000000000001e-02,7.109999999999994e-02,6.399999999999995e-02,5.745000000000000e-02,5.180000000000007e-02,4.635000000000000e-02,4.195000000000004e-02,3.910000000000002e-02,3.634999999999999e-02,3.135000000000010e-02,2.699999999999991e-02,2.355000000000007e-02,2.274999999999994e-02,2.020000000000000e-02,2.075000000000005e-02,1.554999999999995e-02,1.410000000000000e-02,1.379999999999992e-02,1.039999999999997e-02,1.275000000000004e-02,9.600000000000053e-03,8.900000000000019e-03,7.600000000000051e-03,4.899999999999904e-03,5.549999999999944e-03,5.499999999999949e-03,7.449999999999957e-03,5.900000000000127e-03,4.250000000000087e-03,6.099999999999994e-03,2.150000000000096e-03,2.850000000000019e-03,3.049999999999997e-03,5.000000000010552e-05,2.850000000000019e-03,3.499999999999837e-03,1.549999999999940e-03,8.500000000000174e-04,1.549999999999940e-03,6.000000000001560e-04,6.999999999999229e-04,2.199999999999980e-03,-4.500000000000615e-04,-1.400000000000068e-03,1.000000000000112e-03,3.999999999999560e-04,8.000000000001339e-04,2.399999999999958e-03,6.999999999999229e-04,1.500000000000945e-04,9.999999999998899e-04,1.449999999999951e-03,1.449999999999951e-03,1.999999999999780e-04,-1.600000000000046e-03,-1.949999999999896e-03,-3.499999999998504e-04,5.999999999999339e-04,1.449999999999951e-03,2.099999999999991e-03,-4.999999999999449e-04,1.100000000000101e-03,2.500000000000835e-04,-6.500000000000394e-04,1.249999999999973e-03,1.299999999999857e-03,-1.249999999999973e-03,-9.999999999998899e-05,5.499999999998284e-04,-2.249999999999863e-03,4.999999999999449e-04,-5.000000000010552e-05,-8.499999999997954e-04,8.999999999999009e-04,1.200000000000090e-03,-4.999999999999449e-04,1.999999999999780e-04,-2.500000000000835e-04,-1.149999999999984e-03,-9.999999999998899e-05,3.499999999998504e-04,8.500000000000174e-04,1.099999999999879e-03,1.249999999999973e-03},{4.135500000000000e-01,3.722500000000000e-01,3.358000000000000e-01,3.031500000000000e-01,2.701000000000000e-01,2.445500000000001e-01,2.199000000000000e-01,1.968500000000000e-01,1.762500000000000e-01,1.617999999999999e-01,1.434000000000000e-01,1.297000000000000e-01,1.174499999999999e-01,1.041500000000000e-01,9.270000000000001e-02,8.584999999999998e-02,7.639999999999991e-02,6.690000000000007e-02,6.154999999999999e-02,5.670000000000008e-02,5.149999999999999e-02,4.580000000000006e-02,4.110000000000003e-02,3.734999
//////////////////////////////////////////////////// TUTAJ ZACZYNA SIE ROBOTA UZYTKOWNIKA KONCOWEGO
enum {ALG_PID, ALG_DMC_ANALITIC};
volatile int algorithm = ALG_PID;

ArchiveData ad;
CurrentControl cc;

void measurements(){
    new_output(&ad,measureOutput());
}

void controller_setup(){
    init_archive_data(&ad, 200, 200, 0, 0, 1.0);
    get_time();
    PID(NULL,NULL);
    DMC_analitic(NULL,NULL);
}

void idle(){
    // Komunikacja z komputerem zbierającym dane 
    sprintf(str, "%d,%s,"  , algorithm, float2str(ad.yk[M(0)]));
    write_string(str);
    sprintf(str,       "%s,", float2str(ad.yzad));
    write_string(str);
    sprintf(str,          "%s;\n", float2str(ad.uk1[M(-1)]));
    write_string(str);

    if(readButton(BUT0)) algorithm = ALG_PID;
    else                 algorithm = ALG_DMC_ANALITIC;

    if(readButton(BUT1)) ad.yzad = 1.0;
    if(readButton(BUT2)) ad.yzad = 2.0;
    if(readButton(BUT3)) ad.yzad = 3.0;
    
    if(readButton(BUT4)) while(1);
}

void loop(){
    if(algorithm == ALG_PID){
        PID(&ad,&cc);
    }
    if(algorithm == ALG_DMC_ANALITIC) {
        DMC_analitic(&ad,&cc);
    }
    ledsSpin();
}

void controls(){
    pushCurrentControlsToArchiveData(&cc,&ad);
    projectOnFeasibleSet(&ad, -1.0,1.0, 0.0, 4.0);
    setControlValue(last_control(&ad));
}

void timeout(){
    // Zapal diode czerwona
    ledOn(LEDR);
    // Wystaw zerowe sterowanie
    setControlValue(0.0);
    // Wyswietl wyniki dzialania profilera
    print_profiler();
    sprintf(str,"\n");
    write_string(str);
    // Wejdz w petle nieskonczona
    while(1);
}
