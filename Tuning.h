#ifndef TUNING_H
#define TUNING_H

#include <string>

enum TuningType
{
    E_STANDARD = 1,
    Eb_STANDARD,
    DROP_D
};

class Tuning
{
	private:
		float  s1;
		float  s2;
		float  s3;
		float  s4;
		float  s5;
		float  s6;
		std::string tuningname;

	public:
		Tuning ();
		Tuning (TuningType);
		virtual ~Tuning () = default;
		float  getFirstStringFrequency();
		float  getSecondStringFrequency();
		float  getThirdStringFrequency();
		float  getFourthStringFrequency();
		float  getFifthStringFrequency();
		float  getSixthStringFrequency();
		std::string getTuningName();
};

#endif
