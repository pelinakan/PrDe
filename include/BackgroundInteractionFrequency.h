#ifndef HCT_PROXDET_INC_BIF_H_
#define HCT_PROXDET_INC_BIF_H_
/*
Background interaction frequency is calculated 
* 
*/

#include "Probes.h"

class DetermineBackgroundLevels{
    friend class ProbeSet;
public:
	BG_signals bglevels;
	BG_signals bglevelsProbeProbe;
	
	void CalculateMeanandStdRegress(std::string, int, std::string, BG_signals&, int, std::string, int, OutStream&, int);

};

#endif  // HCT_PROXDET_INC_BIF_H_
