//
//  main.cpp
//  HiCap_ProbeDesigner
//
//  Created by Pelin Sahlen on 12/03/2015.
//  Copyright (c) 2015 Pelin Sahlen. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include <vector>
#include <algorithm>
#include <cstddef>
#include <cstdio>
#include "../../include/OutStream.h"
#include <limits>


int mai(int argc, const char * argv[]) {
	
	unsigned int n=95, s=10;;
	double b;
	b=double(n)/s;
	std::cout<< b<<std::endl;
	
	//std::cout<<std::numeric_limits<unsigned long int>::min()<<std::endl;
	//std::cout<< std::numeric_limits<unsigned long int>::max()<<std::endl;
	//std::string fname = "sd";
   // fname.append(".AllProbes.gff3");   
    
    //std::ifstream f(fname);
 //   if(!f.good())
	//	std::cout<<"Exists"<<std::endl;
	
	//std::ofstream of("log.txt");
	//OutStream log(&std::cout, &of);
	
	//log<<"Hello "<<n <<std::endl;
	//log<<"Tjena "<<n+20<<std::endl ;

	/***std::map< std::string, std::vector < Interval < std::string > > > genechrIntervals;
	std::string transcriptfile = "ipf.txt";
	
	struct genetemp{
		std::string chr;
		int start;
		int end;
	};
	
	std::map <std::string, genetemp> genemap; //key gene name, value struct
	
	std::string chr, start, end, target, SOTerm, dbid, strand, attributes, gene_id, intId, temp, lineReadin;	
	bool flag=true;
	//struct 
	std::string genename, tr_id, exonCounts, exonStarts, exonEnds;
	std::ifstream fileReadin;
	
	fileReadin.open(transcriptfile, std::ifstream::in);
	getline(fileReadin,temp); //discard header
	
		
	 while (getline(fileReadin, lineReadin)){
      		
		std::stringstream geneline ( lineReadin );
		
		//Gene Name \t Name \t chr \t Strand \t start \t end \t target
		//name2	 name	chrom	strand	txStart	txEnd	exonCount	exonStarts	exonEnds
		getline(geneline,genename,'\t');
		 
		getline(geneline, tr_id,'\t'); 
		
		getline(geneline, chr,'\t'); 
         
		  
		getline(geneline, strand,'\t');
    
		getline(geneline, start,'\t');
		getline(geneline, end,'\t');
		
		getline(geneline, exonCounts,'\t');
		getline(geneline, exonStarts,'\t');
		getline(geneline, exonEnds,'\t');
		
		genetemp gt;
		gt.chr=chr;
		gt.start=std::stoi(start);
		gt.end=std::stoi(end);
		auto it =genemap.find(genename);
		if (it!=genemap.end()){		
			if(it->second.chr ==gt.chr){
				if(abs(gt.end-gt.start)>abs(it->second.end-it->second.start)){
					it->second=gt;
				}
			}
		}
		else{
			genemap.emplace(genename, gt);
		}
		
		size_t eStarts=std::count(exonStarts.begin(), exonStarts.end(), ',');
		size_t eEnds=std::count(exonEnds.begin(), exonEnds.end(), ',');
		if(eStarts==eEnds){
			if((exonStarts.back()!=','&&exonEnds.back()!=',')){
				exonStarts.push_back(',');
				exonEnds.push_back(',');
				eStarts++;
				eEnds++;
			}
			size_t pos11, pos12, pos21, pos22;
			int exS, exE, intS, intE;
			intS=gt.start;
			intE=gt.start;
						
			while(eStarts>0&&eEnds>0){
				pos11=0; pos21=0;
				pos12=exonStarts.find_first_of(','); pos22=exonEnds.find_first_of(',');    
				exS=std::stoi(exonStarts.substr(pos11, pos12));
				exE=std::stoi(exonEnds.substr(pos21, pos22));
				//std::cout<<exS<< " "<<exE<<std::endl; 
				exonStarts.erase(pos11, pos12+1); exonEnds.erase(pos21, pos22+1);
				--eStarts; --eEnds;
				
				if(intE<exS){
					intE=exS;
				}
				
				if(intS<intE)
				//add to intr interval intS intE
				
				intS=exE;
				intE=exE; 
				
			}
			//if(intE<end)
				std::cout<<"Intron :"<<intE<< " "<<end<<std::endl; 
			
		}
	
}***/
 
}
