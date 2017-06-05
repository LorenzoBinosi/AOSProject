#include <stdio.h>
#include <stdbool.h>
#include <iostream>
#include <string>
#include <math.h>
#include "miosix.h"
#include "Tuning.h"
#include "miosix/arch/common/CMSIS/Device/ST/STM32F4xx/Include/stm32f4xx.h"
#include "Microphone.h"

#define ARM_MATH_CM4

#include "miosix/arch/common/CMSIS/Include/arm_math.h"

#define SAMPLES 8192
#define FFT_SIZE SAMPLES/2

using namespace std;
using namespace miosix;

typedef Gpio<GPIOD_BASE,12>  greenLed;
typedef Gpio<GPIOD_BASE,13>  orangeLed;
typedef Gpio<GPIOD_BASE,15>  blueLed;

static short empty = 0;
static float currentNote;
static float32_t input[SAMPLES];
static float32_t output[FFT_SIZE];

void tune(Tuning);
void frequencyComparison(uint16_t*, uint16_t);

int main()
{
	char c, command;
	std::string commands = "Commands:\nc - change tuning\nt - tune\ns - show actual tuning\nh - help. Show commands\nq - quit\n";
	int length;
	bool quit;
	Tuning *tuning;

	greenLed::mode(Mode::OUTPUT);
	orangeLed::mode(Mode::OUTPUT);
	blueLed::mode(Mode::OUTPUT);
	//Intro
	cout << "Guitar Tuner version 1.0.0, by Lorenzo Binosi, Matheus Fim.\nProject for the Advanced Operating System course of M.Sc. course of Computer Science and Engineering of Politecnico di Milano.\n" << endl;
	//Commands
	quit = false;
	cout << commands;
	tuning = new Tuning ();
	do
	{
		
		cout << "Command: ";
		length = 0;
		while((c = getchar()) != '\n' && c != EOF) 
		{
			length++;
			command = c;
		};
		if (length == 1)
		{
			switch (command)
			{
				case 'c': delete tuning;
					  cout << "Select the desired tuning:\n1 - Eb Standard\n2 - Drop D\n3 - E Standard\n";
					  cout << "Tuning: ";
					  length = 0;
					  while((c = getchar()) != '\n' && c != EOF) 
		   			  {
						length++;
						command = c;
					  };
					  if (length == 1)
					  { 
					  	switch (command)
					  	{
					  		case '1': tuning = new Tuning (TuningType::Eb_STANDARD);
					  			break;	
					  		case '2': tuning = new Tuning (TuningType::DROP_D);	
					  			break;
					  		case '3': tuning = new Tuning (TuningType::E_STANDARD);
					  			break;
					  		default: cout << "Invalid tuning." << endl;
					  			break;
					  	}
					  }	
					  break;
				case 't': tune (*tuning);
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
		}
		else if (length > 1)
			cout << "Invalid command." << endl;
	} while (quit == false);
	while (true);
  	return 0;
}

void tune(Tuning tuning)
{
	//Turning on the microphone with the FFT function, we must add a floating pointer in order to change the frequency of the note in which we want to tune
	int i;
	char c;
	Microphone& mic = Microphone::instance();

	mic.init(&frequencyComparison, FFT_SIZE);
	cout << "Instructions:" << endl;
	cout << "In order to tune your guitar play each string untill the green led is stable. If the red led is blinking, it means that you should pull the string, otherwise if the blue led is blinking, it means that you should relax the string." << endl;
	for (i = 6; i > 0; i--)
	{
		cout << "<<Tuning the " << i;
		switch (i)
		{
			case 1: cout << "st ";
				break;
			case 2: cout << "nd ";
				break;
			case 3: cout << "rd ";
				break;
			default: cout << "th ";
				 break;
		}
		if (i == 1)
			cout << " string. Press 'Enter' or the user button to end the tuning.>>";
		else
			cout << " string. Press 'Enter' or the user button to procede to the next string.>>";
		currentNote = tuning.getStringFrequency(i-1);
		mic.start();
		while((c = getchar()) != '\n' && c != EOF);
		mic.stop();
	}
}

void frequencyComparison(uint16_t* PCM, uint16_t  size)
{
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

    
    if (maxIndex == 0)
    {
        empty++;
	if (empty > 5)
	{
	    empty = 5;
            greenLed::low();
            blueLed::low();
            orangeLed::low();
	} 
    }
    else
    {
        if (((float32_t)((float32_t) (maxIndex * 2.69))) > (float (currentNote + 5)))
	{
	    empty = 0;
	    blueLed::high();
	    orangeLed::low();
	    greenLed::low();
	}
	else if (((float32_t)((float32_t) (maxIndex * 2.69))) < (float (currentNote - 5)))
	{
	    empty = 0;
	    blueLed::low();
	    orangeLed::high();
	    greenLed::low();
	}	
	else
	{
	    empty = 0;
	    greenLed::high();
	    blueLed::low();
	    orangeLed::low();
	}
    }

    //printf("Value: %d. Index: %d. Aprox Freq: %.2f hz\n", (uint16_t) maxValue, (uint32_t)maxIndex, (float32_t)((float32_t) maxIndex * 2.69)); // prints frequency found
    printf("Aprox Freq: %.2f hz\n", (float32_t)((float32_t) maxIndex * 2.69)); // prints frequency found  
}
