#include <stdio.h>
#include "Tuning.h"

Tuning::Tuning ()
{
	s1 = 329.628;
	s2 = 246.942;
	s3 = 195.998;
	s4 = 146.832;
	s5 = 110.00;
	s6 = 82.407;
	tuningname = "E standard";
}

Tuning::Tuning (TuningType tuningtype)
{
	switch (tuningtype)
	{
		case 2: s1 = 311.127;
			s2 = 233.082;
			s3 = 184.997;
			s4 = 138.591;
			s5 = 103.826;
			s6 = 77.782;
			tuningname = "Eb standard";
			break;
		case 3: s1 = 293.665;
			s2 = 246.942;
			s3 = 195.998;
			s4 = 146.832;
			s5 = 110.00;
			s6 = 82.407;
			tuningname = "Drop D";
			break;
		default: s1 = 329.628;
			s2 = 246.942;
			s3 = 195.998;
			s4 = 146.832;
			s5 = 110.00;
			s6 = 82.407;
			tuningname = "E standard";
			break;
	}
} 

float Tuning::getFirstStringFrequency() {return s1;}
float Tuning::getSecondStringFrequency() {return s2;}
float Tuning::getThirdStringFrequency() {return s3;}
float Tuning::getFourthStringFrequency() {return s4;}
float Tuning::getFifthStringFrequency() {return s5;}
float Tuning::getSixthStringFrequency() {return s6;}	
std::string Tuning::getTuningName () {return tuningname;}
