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
static float32_t sum[FFT_SIZE/8];

void tune(Tuning);
void frequencyComparison(uint16_t*, uint16_t);

int main()
{
    char c, command;
    string commands = "Commands:\nc - change tuning\nt - tune\ns - show actual tuning\nh - help. Show commands\nq - quit\n";
    int length;
    bool quit;
    Tuning tuning;

    greenLed::mode(Mode::OUTPUT);
    orangeLed::mode(Mode::OUTPUT);
    blueLed::mode(Mode::OUTPUT);
    //Intro
    cout << "Guitar Tuner version 1.0.0, by Lorenzo Binosi, Matheus Fim." << endl;
    cout << "Project for the Advanced Operating System course of M.Sc. course of Computer Science and Engineering of Politecnico di Milano.\n" << endl;
    //Commands
    quit = false;
    cout << commands;
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
		case 'c': cout << "Select the desired tuning:" << endl;
			  cout << "1 - E Standard" << endl;
                          cout << "2 - Eb Standard" << endl;
			  cout << "3 - Drop D" << endl;
			  cout << "4 - OPEN E" << endl;
                          cout << "5 - OPEN G" << endl;
			  cout << "6 - OPEN A" << endl;
			  cout << "Tuning: ";
			  length = 0;
			  while((c = getchar()) != '\n' && c != EOF) 
		   	  {
			      length++;
			      command = c;
			  };
		  	  if (length == 1)
			  { 
			      if (command >= 49 && command <= 54)
			          tuning = Tuning((TuningType) (command - 48));
			      else
                                  cout << "Invalid tuning." << endl;
			  }
			  break;
		case 't': tune (tuning);
			  break;
		case 's': cout << "Actual tuning: " << tuning.getTuningName() << endl; 
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
    int i;
    char c;
    Microphone& mic = Microphone::instance();

    mic.init(&frequencyComparison, FFT_SIZE);
    cout << "Instructions:" << endl;
    cout << "In order to tune your guitar play each string untill the green led is stable."; 
    cout << " If the orange led is blinking, it means that you should pull the string, otherwise if the blue led is blinking, it means that you should relax the string." << endl;
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
	    cout << " string. Press 'Enter' to end the tuning.>>";
	else
	    cout << " string. Press 'Enter' to procede to the next string.>>";
	currentNote = tuning.getStringFrequency(i-1);
	mic.start();
	while((c = getchar()) != '\n' && c != EOF);
	mic.stop();
    }
    greenLed::low();
    blueLed::low();
    orangeLed::low();
}


//Harmonic Product Spectrum 
/* to find the fundamenta frequency that is no always the strongest one in real guitars */ 
float32_t hps(float32_t *magFFT, int size)
{ 
    float32_t freq, maxValue =0;

    for(int k = 0; k < size/8; k++)
    {
	sum[k] = magFFT[k] * magFFT[2*k] * magFFT[3*k];
	// find fundamental frequency
	if( sum[k] > maxValue && k > 0 )
	{
  	    maxValue = sum[k];
	    freq = k;
	}
    }
    return freq * 11000 / size;
}

void frequencyComparison(uint16_t* PCM, uint16_t  size)
{
    arm_cfft_radix4_instance_f32 S;    // ARM CFFT module 
    uint16_t i;
    float32_t freq; 

    // first step is to adjust the pcm vector to the format of the fft function, alternating rea and imaginary parts
    for(i = 0; i < size; i++)
    {
        /* Real part, make offset by ADC / 2 */
        input[(uint16_t)i*2] = (float32_t)((float32_t) PCM[i] - 32768);  
        /* imaginary part */    
        input[(uint16_t)((i*2)+1)] = 0;
    }
    // now we calculate the frequency using FFT, the code below is pretty much a standard one and can be find in various websites
    // Initialize the CFFT/CIFFT module, intFlag = 0, doBitReverse = 1
    arm_cfft_radix4_init_f32(&S, FFT_SIZE, 0, 1);
    // Process the data through the CFFT/CIFFT module 
    arm_cfft_radix4_f32(&S, input);
    // Process the data through the Complex Magniture Module for calculating the magnitude at each bin
    arm_cmplx_mag_f32(input, output, FFT_SIZE);
    // Calculates maxValue and returns corresponding value 
    //arm_max_f32(output, FFT_SIZE, &maxValue, &maxIndex);
    freq = hps(output, FFT_SIZE);
    // Comparing the carrier with the frquency of the string
    if (freq == 0)
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
    	if(freq > fabsf(float (currentNote + 20)))
    	{
    	    empty = 0;
            greenLed::low();
            blueLed::low();
            orangeLed::low();		
   	}
    	else if (freq > (float (currentNote + 1.4)))
	{
	    empty = 0;
	    blueLed::high();
	    orangeLed::low();
	    greenLed::low();
	}
	else if (freq < (float (currentNote - 1.4)))
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
	
    printf("Desired Freq: %.2f hz\t\tAprox Freq: %.2f hz\n",(float) currentNote, freq); // prints frequency found 
    }

}


