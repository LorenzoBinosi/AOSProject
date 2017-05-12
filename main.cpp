#include <stdio.h>
#include <iostream>
#include <string>
#include "miosix.h"
#include "Microphone.h"
#include <math.h>
#include "miosix/arch/common/CMSIS/Device/ST/STM32F4xx/Include/stm32f4xx.h"
#include "Tuning.h"

#define ARM_MATH_CM4

#include "miosix/arch/common/CMSIS/Include/arm_math.h"


#define SAMPLES 8192
#define FFT_SIZE SAMPLES/2

using namespace std;
using namespace miosix;
using namespace std::tr1;

float32_t input[SAMPLES];
float32_t output[FFT_SIZE];

Tuning *tuning;

void frequency(uint16_t* PCM, uint16_t  size){

	arm_cfft_radix4_instance_f32 S;    // ARM CFFT module 
    float32_t maxValue;                // Max FFT value is stored here 
    uint32_t maxIndex;                // Index in Output array where max value is 
    uint16_t i;
    
    // first step is to adjust the pcm vector to the format of the fft function, alternating rea and imaginary parts
    for(i = 0; i < size; i++){
    
       	//iprintf("%d:%d\n", (uint16_t) i, PCM[(uint16_t) i] );
    
        /* Real part, make offset by ADC / 2 */
        input[(uint16_t)i*2] = (float32_t)((float32_t) PCM[i] - 32768);

        
        //printf("%d:%f\n", i, (float32_t)input[(uint16_t)i] ); // prints key:val
        
        /* imaginary part */    
        input[(uint16_t)((i*2)+1)] = 0;
    	
    	/*
    	if(i % 2){
    		input[(uint16_t)i] = 0;	
    	} else {
    		input[(uint16_t)i] = (float32_t)((float32_t) PCM[i] - 32768);
    	} */
    
    }

    // now we calculate the frequency using FFT, the code below is pretty much a standard one and can be find in various websites

    // Initialize the CFFT/CIFFT module, intFlag = 0, doBitReverse = 1
    arm_cfft_radix4_init_f32(&S, FFT_SIZE, 0, 1);

    // Process the data through the CFFT/CIFFT module 
    arm_cfft_radix4_f32(&S, input);
    
    // Process the data through the Complex Magniture Module for calculating the magnitude at each bin
    arm_cmplx_mag_f32(input, output, FFT_SIZE);
    
    // Calculates maxValue and returns corresponding value 
    arm_max_f32(output, FFT_SIZE, &maxValue, &maxIndex);

    printf("Value: %d. Index: %d. Aprox Freq: %.2f hz\n", (uint16_t) maxValue, (uint32_t)maxIndex, (float32_t)((float32_t) maxIndex * 2.69)); // prints frequency found 

    //return (float32_t)((float32_t) maxIndex * 10.77);
}
/*
void tuner(uint16_t *PCM, uint16_t size){
	float32_t freq;
	float32_t correctFreq;

	freq = frequency(PCM, size);

	if(freq>70 && freq<90){
    	correctFreq = 82.4;
  	}

	if(freq>100 && freq<120){
    	correctFreq = 110;
  	}
	
	if(freq>135 && freq<155){
    	correctFreq = 146.8;
  	}
  	
  	if(freq>186 && freq<205){
    	correctFreq = 196;
 	}
  	
  	if(freq>235 && freq<255){
    	correctFreq = 246.9;
  	}
  	
  	if(freq>320 && freq<340){
    	correctFreq = 329.6;
  	}

  	if (freq > correctFreq + 6){
    		cout << "++++++\n" << endl;
    	} else if (freq > correctFreq + 4){
    		cout << "++++\n" << endl;
    	} else if (freq > correctFreq + 1){
    		cout << "++\n" << endl; 
    	} else if (freq > correctFreq - 6){
    		cout << "------\n" << endl; 	
    	} else if (freq > correctFreq - 4){
    		cout << "----\n" << endl; 	
    	} else if (freq > correctFreq - 1){
    		cout << "--\n" << endl; 	
    	}

}

*/

int main(){

	char c, command;
	std::string commands = "Commands:\nc - change tuning\nt - tune\ns - show actual tuning\nh - help. Show commands\nq - quit\n";
	int length;
	bool quit;
	Microphone& mic = Microphone::instance();
	tuning = new Tuning();

	//welcome message
	cout << "Guitar Tuner version 1.0.0, by Lorenzo Binosi, Matheus Fim.\nProject for the Advanced Operating System course of M.Sc. course of Computer Science and Engineering of Politecnico di Milano.\n" << endl;
	
	cout <<  commands; 
	quit = false;

	while(quit == false){
		cout << "Command: ";
		length = 0;

		while((c = getchar()) != '\n' && c != EOF) 
		{
			length++;
			command = c;
		};
		if (length == 1){
			switch (command)
			{
				case 'c': delete tuning;
					/*cout << "1 - E standard\n2 - Eb standard\n3 - Drop D\n" << endl;
					
					while((c = getchar()) != '\n' && c != EOF) 
					{
						command = c;
					};	

					if(c == '1'){
						tuning = new Tuning (TuningType::E_STANDARD);
					} else if(c == '2'){
						tuning = new Tuning (TuningType::Eb_STANDARD);
					} else if(c == '3'){
						tuning = new Tuning (TuningType::DROP_D);
					} else{
						cout << "Invalid Option.\n Automatically setted to E Standard\n" << endl;
						tuning = new Tuning (TuningType::E_STANDARD);
					}*/

					tuning = new Tuning (TuningType::DROP_D);
					break;
					
				case 't': 
					mic.init(&frequency, FFT_SIZE);
					mic.start();
					break;
				
				case 's': cout << "Actual tuning: " << tuning->getTuningName() << endl; 
					break;
				
				case 'h': cout << commands;
					break;
				
				case 'q': quit = true;
					break;
				
				default : cout << "Invalid command." << endl;
					break;
			}
		} else{
			cout << "Invalid command." << endl;
		}
	}


	
	while(1){};

	return 0;
}