
#include "Enhancers.h"
#include "Global.h"
#include <utility>
#include <fstream>
#include <sstream>
#include <string>


void EnhancerSet::ReadEnhancerCoordinates(std::string filename, int padding, OutStream& log){
    
    int numEnRead;
    numEnRead=0;
	
	std::ifstream enFile(filename.c_str());
	std::string line, enID, enChr, enStart, enEnd, temp, line1;
    std::map< std::string, std::vector < Interval < std::string > > > chrIntervals; //key - chr, mapped to vector containing different intervals of enhancers in the list containing  

    
	log<<"Reading Enhancer List File "<< filename<<std::endl;
	getline(enFile, temp); //header
	while (getline(enFile, line)) {
      		
   		std::stringstream enline ( line );
		getline(enline,enChr,'\t');
		getline(enline,enStart,'\t');
		getline(enline,enEnd,'\t');
		
		enID=enChr+":"+enStart+"-"+enEnd;

		EnhancerStruct enSTemp;
		enSTemp.enId=enID; // enhancer Id
		enSTemp.chr=enChr;
			
		enSTemp.start=std::stoi(enStart); // fragment Start Site 
		enSTemp.end=std::stoi(enEnd);
		auto check = Enhancers.emplace(enID, enSTemp);
		
		enTree.EnhancerList.push_back(enID);
   	
		if(!std::get<1>(check)){
			log<<"Duplicate Enhancer:"<<enID<<std::endl;
		}
	
		if(chrIntervals.count(enChr)==0){
			chrIntervals[enChr].push_back(Interval<std::string>((enSTemp.start -padding),(enSTemp.end + padding),(enID)));
		}
		else{
			std::vector < Interval < std::string > > tempvector ;
			tempvector.push_back(Interval<std::string>((enSTemp.start -padding),(enSTemp.end + padding),(enID)));
			chrIntervals.emplace(enChr, tempvector);
		}
		++numEnRead;
	}
	
	enFile.close();
	
    for(auto it = chrIntervals.begin(); it != chrIntervals.end();++it){
		
		std::vector<Interval< std::string > > temp;
        enTree.En_Tree[it->first] = IntervalTree< std::string >(it->second);
        it->second.swap(temp);
    }
    
	log <<"Number of Enhancer List Coordinates Read : "<<numEnRead << std::endl;
    
}




std::string EnhancerSet::FindOverlaps(std::string chr, unsigned long int readstart, unsigned long int readend){
	
    std::vector<Interval< std::string > > enresults;

    enTree.En_Tree[chr].findOverlapping(readstart, readend, enresults);
    if (enresults.size() > 0){ // value = probe_index
        return enresults[0].value;
    }
    else
        return "null";
}



