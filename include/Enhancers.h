#ifndef INTSUITE_INC_ENHANCERS_H_
#define INTSUITE_INC_ENHANCERS_H_

#include <map>
#include "ProbeRESitesCount.h"

class EnhancerSet{

public:

	void ReadEnhancerCoordinates(std::string, int);
	std::string FindOverlaps(std::string , unsigned long int , unsigned long int);

	std::map< std::string, EnhancerStruct > Enhancers;
	
private:

	EnhancerTree enTree;
};


#endif //INTSUITE_INC_PROBES_H_
