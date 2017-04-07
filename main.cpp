#include <stdio.h>
#include <iostream>
#include <string>
#include "miosix.h"
#include "Microphone.h"
#include <math.h>
#include "miosix/arch/common/CMSIS/Device/ST/STM32F4xx/Include/stm32f4xx.h"
//#include "miosix/kernel/scheduler/scheduler.h"

#define ARM_MATH_CM4
//#define __FPU_PRESENT 1


#include "miosix/arch/common/CMSIS/Include/arm_math.h"
//#include "tm_stm32f4_dac_signal.h"
//#include "tm_stm32f4_adc.h"



#define SAMPLES 512
#define FFT_SIZE SAMPLES/2

using namespace std;
using namespace miosix;
using namespace std::tr1;

float32_t input[SAMPLES];
float32_t output[FFT_SIZE];


void visualize(uint16_t* PCM, uint16_t  size){

	arm_cfft_radix4_instance_f32 S;    /* ARM CFFT module */
    float32_t maxValue;                /* Max FFT value is stored here */
    uint32_t maxIndex;                /* Index in Output array where max value is */
    uint16_t i;

 //   iprintf("new\n");
    
    for(i = 0; i < size; i++){
       	//iprintf("%d:%d\n", (uint16_t) i, PCM[(uint16_t) i] );
     
        /* Real part, make offset by ADC / 2 */
        input[(uint16_t)i*2] = (float32_t)((float32_t) PCM[i] - 32768);
        //printf("%d:%f\n", i, (float32_t)input[(uint16_t)i] ); // prints key:val
        
        /* imaginary part */    
        input[(uint16_t)((i*2)+1)] = 0;
    }

    /* Initialize the CFFT/CIFFT module, intFlag = 0, doBitReverse = 1 */
    arm_cfft_radix4_init_f32(&S, FFT_SIZE, 0, 1);

    /* Process the data through the CFFT/CIFFT module */
    arm_cfft_radix4_f32(&S, input);
    
    /* Process the data through the Complex Magniture Module for calculating the magnitude at each bin */
    arm_cmplx_mag_f32(input, output, FFT_SIZE);
    
    /* Calculates maxValue and returns corresponding value */
    arm_max_f32(output, FFT_SIZE, &maxValue, &maxIndex);

    //printf("%f:%d\n", (float32_t) maxValue, (uint32_t)maxIndex); // prints key:val

	  
    /*for(int i=0; i < size/2 ; i++ ){
    	printf("%d:%f\n", i, (float32_t)output[(uint16_t)i] ); // prints key:val
    }*/
}


int main(){
	/*
	TM_DAC_SIGNAL_Init(TM_DAC2, TIM4);
	//TM_DAC_SIGNAL_Init(
	TM_DAC_SIGNAL_SetSignal(TM_DAC2, TM_DAC_SIGNAL_Signal_Sinus, 10000);

	TM_ADC_Init(ADC1, ADC_Channel_0);
	*/
	Microphone& mic = Microphone::instance(); 
	mic.init(&visualize, FFT_SIZE);
	mic.start();

	while(1){
		/*
		for(int i=0; i < SAMPLES; i += 2){
			delayUs(21);
			input[(uint16_t) i] = (float32_t)((float32_t) TM_ADC_Read(ADC1, ADC_Channel_0) - 2048.0);
			input[(uint16_t)(i+1)] = 0;
			printf("%d:%f\n", i, input[i]);
		}*/



	};

	return 0;
}