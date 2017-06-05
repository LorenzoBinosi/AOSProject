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
	s = new float[6];
	switch (tuningType)
	{
		case 2: s[0] = Eb_4;
			s[1] = Bb_3;
			s[2] = Gb_3;
			s[3] = Db_3;
			s[4] = (Eb_2 || Ab_3);
			s[5] = (Eb_2 || Eb_3);
			tuningname = "Eb standard";
			break;
		case 3: s[0] = E_4;
			s[1] = B_3;
			s[2] = G_3;
			s[3] = D_3;
			s[4] = (A_2 || A_3);
			s[5] = (D_2 || D_3);
			tuningname = "Drop D";
			break;
		default: s[0] = E_4;
			s[1] = B_3;
			s[2] = G_3;
			s[3] = D_3;
			s[4] = (A_2 || A_3);
			s[5] = (E_2 || E_3);
			tuningname = "E standard";
			break;
	}
}

float Tuning::getStringFrequency(int stringNumber) 
{
	if (stringNumber >= 0 && stringNumber < 6)
		return s[stringNumber];
	else
		return -1;
}

std::string Tuning::getTuningName () {return tuningname;}
