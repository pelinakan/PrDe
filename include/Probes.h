#ifndef INTSUITE_INC_PROBES_H_
#define INTSUITE_INC_PROBES_H_

#include <map>
#include "Promoters.h"

class ProbeSet{
    friend class PromoterClass;
public:
   
	void ReadProbeCoordinates(std::string, std::map <std::string, std::string>&, int, bool, PrDes::RENFileInfo&);
    
	int FindOverlaps(std::string, unsigned long int, unsigned long int, std::string);
    int FindOverlaps_NegCtrls(std::string, unsigned long int, unsigned long int, std::string);
    void AnnotateProbeswithPromoters(PromoterClass);
    
    ProbeSet(OutStream& prlog, int fcount, int fRCount) : prLog (prlog), fileCount(fCount), filesReadCount (fRCount) {}
    
private:
	OutStream& prLog;
	std::map< std::string, std::string> probeTypeMap;
	int fileCount;
	int filesReadCount;
	
	void GetProbeFeats(std::stringstream&, CaptureProbes&, std::string&);
    int FindClosestFeature(int, std::vector<int>, std::string);
    int AddTotheIntervalTree(std::vector<Interval< int > >&, std::vector<Interval< int > >&, std::string, std::string, bool); 
    void ProcessProbeLine(std::map< std::string, std::vector < std::string > >::iterator, std::vector < int>&, CaptureProbes, int&, std::map< std::string, std::vector<Interval<int>>>&, std::map< std::string, std::vector<Interval<int>>>&, int);
   
};


#endif //INTSUITE_INC_PROBES_H_
