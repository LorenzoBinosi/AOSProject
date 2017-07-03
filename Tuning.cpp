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
		s[4] = Ab_2;
		s[5] = Eb_2;
		tuningname = "Eb standard";
		break;
        case 3: s[0] = E_4;
		s[1] = B_3;
		s[2] = G_3;
		s[3] = D_3;
		s[4] = A_2;
		s[5] = D_2;
		tuningname = "Drop D";
		break;
        case 4: s[0] = E_4;
		s[1] = B_3;
		s[2] = Ab_3;
		s[3] = E_3;
		s[4] = B_2;
		s[5] = E_2;
		tuningname = "Open E";
		break;
        case 5: s[0] = D_4;
		s[1] = B_3;
		s[2] = G_3;
		s[3] = D_3;
		s[4] = G_2;
		s[5] = D_2;
		tuningname = "OPEN G";
		break;
        case 6: s[0] = E_4;
		s[1] = A_3;
		s[2] = E_3;
		s[3] = Db_3;
		s[4] = A_2;
		s[5] = E_2;
		tuningname = "OPEN A";
		break;
	default: s[0] = E_4;
		s[1] = B_3;
		s[2] = G_3;
		s[3] = D_3;
		s[4] = A_2;
		s[5] = E_2;
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
