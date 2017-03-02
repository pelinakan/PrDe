//
//  DesignProbes.h
//  HiCap_ProbeDesigner
//
//  Created by Pelin Sahlen on 12/03/2015.
//  Copyright (c) 2015 Pelin Sahlen. All rights reserved.
//
#ifndef PRSUITE_INC_DESPR_H_
#define PRSUITE_INC_DESPR_H_

#include "ProbePromoterClass.h"
#include "RepeatOverlaps.h"

class DesignClass{ //Design probe class
    friend class ProbePromoterClass;
public:
    std::unordered_map< std::string, int > chrIndex; // key = chrname, value = index of REfragments
    std::vector < PrDes::DesignLayers > OneDesign; // TSS is on plus strand
    
    void InitialiseDesign(ProbePromoterClass&, std::vector< PrDes::REPosMap >&);
    void DesignProbes(ProbePromoterClass&, ProbeRESitesClass&, Repeats, std::string, std::string, std::string, int, int, PrDes::RENFileInfo&, int);
    void MergeAllChrOutputs(ProbePromoterClass&, PrDes::RENFileInfo&);
    DesignClass(OutStream& dlog) : dLog (dlog) {}
    
protected:
	int ProbeLen;
	std::string bigwigsummarybinary, mappabilityfile;
	int reLeftCut, reRightCut;
	OutStream& dLog;
	double mapThreshold;
	int BUFSIZE;
	
    bool overlap(ProbeRESitesClass&, Repeats, int&, int, int, std::string, int, int, bool, bool);
    double BigWigSummary(std::string, int, int);
    
private:
	
	int MaxDistancetoTSS;
	
    bool CheckRepeatOverlaps(ProbeRESitesClass&, std::string, int, int&, bool, Repeats, bool, bool);
    bool createNewEntry(std::unordered_map<int, PrDes::REposStruct >&, std::unordered_map<int, PrDes::REposStruct >&, int, std::string, int,bool);
    int CheckREsiteAroundProbe(ProbeRESitesClass&, std::string, int, int);
    bool WritetoFile(std::ofstream&, std::string, int, int, std::vector< std::string >, bool, std::string, ProbePromoterClass&);
    int CheckDistanceofProbetoTSS(ProbeRESitesClass&, std::string, int, int, bool);
    
    
};

#endif //PRSUITE_INC_DESPR_H_
