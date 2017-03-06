//
//  main.cpp
//  PrDe
//
//  Created by Pelin Sahlen and Anandashankar Anil on 28/02/2017.
//  Copyright (c) 2015 Pelin Sahlen. All rights reserved.
//

#include <iostream>
#include <string>
#include <algorithm>

#include "PrintUsage.h"
#include "PrDe.h"


int main(int argc, const char * argv[]) {
	
	std::string whichMod;
	std::string whichChr;
	std::string proxType;
	std::string statsOption;
	std::string printOption; 
	
	PrDe prde;
	
    if (argc < 2) {
        print_usage();
        return -1;
    }
    
    whichMod = argv[1];
    
    if(whichMod=="ProbeDesigner"){
		if (argc != 4) {
			print_usage();
			return -1;
		}
		else{
			if(argv[2]=="-c" || argv[2]=="--chr"){
				prde.ProbeDesignMain(argv[2]);
			}
			else{
				print_usage();
				return -1;
			}
		}
		
	}
	
	else if(whichMod=="ProximityDetector"){
		if (argc != 10) {
			print_usage();
			return -1;
		}
		else{
			for(int i=2; i<argc; ++i){
				if(argv[i]=="-c" || argv[i]=="--chr"){
					whichChr=argv[i+1];
				}
				else if(argv[i]=="-p" || argv[i]=="--proximitytype"){
					if(argv[i+1]=="PrintProbeProximities" || argv[i+1] =="PrintEnhancerProximities")
						proxType=argv[i+1];
					else{
						std::cout<<"Invalid argument for -p"<<std::endl;
						print_usage();
						return -1;
					}
				}
				else if(argv[i]=="-d" || argv[i]=="--displaytype"){
					if(argv[i+1]=="ComputeStatsOnly" || argv[i+1] == "PrintProximities")
						statsOption=argv[i+1];
					else{
						std::cout<<"Invalid argument for -d"<<std::endl;
						print_usage();
						return -1;
					}
				}
				else if(argv[i]=="-n" || argv[i]=="--printtype"){
					if(argv[i+1]=="Neg" || argv[i+1] == "NonNeg"||argv[i+1]=="Both")
						printOption=argv[i+1];
						else{
						std::cout<<"Invalid argument for -n"<<std::endl;
						print_usage();
						return -1;
					}
				}
				else{
					print_usage();
					return -1;
				}
			}
			prde.ProxDetectMain(whichChr, proxType, statsOption, printOption);
			
		}
		
	}
	else{
		print_usage();
		return -1;
	}
}
