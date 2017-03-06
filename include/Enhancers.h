#ifndef INTSUITE_INC_ENHANCERS_H_
#define INTSUITE_INC_ENHANCERS_H_

#include <map>
#include "ProbeRESitesCount.h"
#include "Data_Structures.h"
#include "IntervalTree.h"

class EnhancerSet{

public:

	void ReadEnhancerCoordinates(std::string, int, OutStream&);
	std::string FindOverlaps(std::string , unsigned long int , unsigned long int);

	std::map< std::string, EnhancerStruct > Enhancers;
	
private:

	EnhancerTree enTree;
};


#endif //INTSUITE_INC_PROBES_H_
