#include <stdio.h>
#include "Tuning.h"
#include "Note.h"

Tuning::Tuning ()
{
	setTune(TuningType::E_STANDARD);
}

Tuning::Tuning (TuningType tuningType)
{
	setTune(tuningType);
} 

void Tuning::setTune(TuningType tuningType)
{
	switch (tuningType)
	{
		case 2: s[1] = Eb_4;
			s[2] = Bb_3;
			s[3] = Gb_3;
			s[4] = Db_3;
			s[5] = Ab_2;
			s[6] = Eb_2;
			tuningname = "Eb standard";
			break;
		case 3: s[1] = E_4;
			s[2] = B_3;
			s[3] = G_3;
			s[4] = D_3;
			s[5] = A_2;
			s[6] = D_2;
			tuningname = "Drop D";
			break;
		default: s[1] = E_4;
			s[2] = B_3;
			s[3] = G_3;
			s[4] = D_3;
			s[5] = A_2;
			s[6] = E_2;
			tuningname = "E standard";
			break;
	}
}

float Tuning::getStringFrequency(int stringNumber) 
{
	if (stringNumber > 0 && stringNumber < 7)
		return s[stringNumber];
	else
		return -1;
}

std::string Tuning::getTuningName () {return tuningname;}
