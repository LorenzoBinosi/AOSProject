#ifndef TUNING_H
#define TUNING_H

#include <string>

enum TuningType
{
    E_STANDARD = 1,
    Eb_STANDARD,
    DROP_D,
    OPEN_E,
    OPEN_G,
    OPEN_A
};

class Tuning
{
	private:
		float s[6];
		std::string tuningname;
		void setTune(TuningType);

	public:
		Tuning ();
		Tuning (TuningType);
		virtual ~Tuning () = default;
		float getStringFrequency(int stringNumber);
		std::string getTuningName();
};

#endif
