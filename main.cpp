#include <stdio.h>
#include <stdbool.h>
#include <iostream>
#include <string>
#include "miosix.h"
#include "Tuning.h"

using namespace std;

void tune(Tuning);

int main()
{
	char c, command;
	std::string commands = "Commands:\nc - change tuning\nt - tune\ns - show actual tuning\nh - help. Show commands\nq - quit\n";
	int length;
	bool quit;
	Tuning *tuning;

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
					  tuning = new Tuning (TuningType::DROP_D);
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
	float *currentNote;
	int i;

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
			cout << " string. Press 'Enter' or the user button to end the tuning.>>" << endl;
		else
			cout << " string. Press 'Enter' or the user button to procede to the next string.>>" << endl;
		(*currentNote) = tuning.getStringFrequency(i-1);
	}
	//Turning on the microphone
}
