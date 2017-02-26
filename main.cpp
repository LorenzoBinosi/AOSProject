#include <stdio.h>
#include <stdbool.h>
#include <iostream>
#include <string>
#include "miosix.h"
#include "Tuning.h"

using namespace std;

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
				case 't': break;
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
