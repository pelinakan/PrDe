#include <cstdlib>
#include <iostream>
#include <ctime>
#include <cstring>
#include <fstream>
using namespace std;

#include "Global.h"
#include "ProcessPairs.h"
#include "Find_Interactions.h"
#include "OutStream.h"
#include "HiCapTools.h"


unsigned int totalNumberofPairs=0;
unsigned int NumberofPairs=0;
unsigned int NofPairs_One_on_Probe=0;
unsigned int NofPairs_Both_on_Probe=0;
unsigned int NofPairsNoAnn=0;
const int coreprom_upstream = 1000;
const int coreprom_downstream = 1000;

const int BinSize  = 1000; // Only To Calculate Background Interaction Frequencies


const int BinSizeProbeProbe=20000;



int HiCapTools::ProxDetectMain(std::string whichchr, std::string interactionOption, std::string statsOption, std::string interactiontype){
	
	
	struct Experiment{
		std::string filepath;
		std::string name;
		std::string celltype;
		std::string designname;
	};
	int NOFEXPERIMENTS; // Number of Experiments
	int padding = 300; //For Sequence Capture Probes
	std::string DigestedGenomeFileName;
	std::string TranscriptListFileName;
	std::string SNPFile;
	std::string negCtrlRegFile;
	std::string ProbeFileName;
	std::string NegCtrlProbeFileName;
	std::string ExpFileName;
	std::string EnhancerListFileName;
	std::string BaseFileName;
    std::vector <Experiment> Experiments; 
	Experiment Exptemp;
	std::string enhancerOption;
	enhancerOption="No";
	std::map <std::string, std::string> probeType;
	std::locale l;
	std::ofstream statsFile;
	bool emptyErrFlag=false;
	char currTime[100];
	int featFileCount = 3; //4 - transcript and SNV files, 2 - transcript file only, 1 - snp file only
    int countFeatFiles = 2;
    int MinNumberofSupportingPairs = 1; // To be entered by the user
    int ReadLen = 80;//ReadLength
    const int ClusterPromoters  = 1200;
    int MinimumJunctionDistance = 1000; // To be entered by the user
	bool CALCULATE_P_VALUES;
	int WindowSize = 101; 
	int WindowSizeProbeProbe = 3; 
	
	PrDes::RENFileInfo reFileInfo;
   
	std::ifstream configFile("config/configFile.txt");
	std::string line;
   
   /***
	if (argc !=5) {
		print_usage();
		return -1;
	}
	//whichchr = argv[1];
	//interactionOption = argv[2];
	//statsOption = argv[3];
	//interactiontype = argv[4];
	***/
	std::time_t now_time = std::time(NULL);
    std::strftime(reFileInfo.currTime, sizeof(currTime), "%H.%M.%S_%F", std::localtime(&now_time));//Date and time when run starts
    
    std::ofstream logFile(std::string("ProxDetLog_")+currTime+".log");
    
    OutStream log;
    log.AddStreams(&std::cout, &logFile);
	
	if(configFile.good()){
		while (!configFile.eof()){
			getline(configFile, line);
			if(line.find("=")!=string::npos){
				if(line.substr(0, line.find('=')).find("Experiment File Name Path")){
					string s;
					s=line.substr(line.find('=')+1);
					s.erase(std::remove_if(begin(s), end(s), [l](char ch) { return std::isspace(ch, l); }), end(s));
					if(s.empty()){
						log<<"!!Error!! : Experiment File Name Path is empty. It is required!" <<std::endl;
						emptyErrFlag = true;
					}
					ExpFileName=s;
				}
				if(line.substr(0, line.find('=')).find("Minimum Number of Supporting Pairs")){
					if(!(line.substr(line.find('=')+1).empty()))
						MinNumberofSupportingPairs=std::stoi(line.substr(line.find('=')+1));
					else{
						log<<"##Warning## : Minimum Number of Supporting Pairs is empty. Set to default : "<<MinNumberofSupportingPairs<<std::endl;
					}
				}
				if(line.substr(0, line.find('=')).find("Minimum Junction Distance")){
					if(!(line.substr(line.find('=')+1).empty()))
						MinimumJunctionDistance=std::stoi(line.substr(line.find('=')+1));
					else
						log<<"##Warning## : Minimum Junction Distance is empty. Set to default : "<< MinimumJunctionDistance<<std::endl;
				}
				if(line.substr(0, line.find('=')).find("Padding")){
					if(!(line.substr(line.find('=')+1).empty()))
						padding=std::stoi(line.substr(line.find('=')+1));
					else
						log<<"##Warning## : Padding is empty. Set to default : "<< padding<<std::endl;
				}
				if(line.substr(0, line.find('=')).find("Read Length")){
					if(!(line.substr(line.find('=')+1).empty()))
						ReadLen=std::stoi(line.substr(line.find('=')+1));
					else
						log<<"##Warning## : Read Length is empty. Set to default : "<< ReadLen<<std::endl;
					
				}
				if(line.substr(0, line.find('=')).find("Base File Name")){
					string s;
					s=line.substr(line.find('=')+1);
					s.erase(std::remove_if(begin(s), end(s), [l](char ch) { return std::isspace(ch, l); }), end(s));
					if(s.empty()){
						log<<"##Warning## : Base File Name is empty. Set to default: ProxDetect!" <<std::endl;
						s = "ProxDetect";
					}
					BaseFileName=s;
				}
				if(line.substr(0, line.find('=')).find("Calculate p_values")){
					string s;
					s=line.substr(line.find('=')+1);
					s.erase(std::remove_if(begin(s), end(s), [l](char ch) { return std::isspace(ch, l); }), end(s));
					if(s=="Yes" || s=="yes"|| s=="Y" || s== "y"){
						CALCULATE_P_VALUES=true;
					}
					else{
						CALCULATE_P_VALUES=false;
						if(s.empty())
							log<<"##Note## : Calculate p values is empty. Set to default: No!" <<std::endl;
					}
				}
				if(line.substr(0, line.find('=')).find("Window Size for Probe-Distal")){
					if(!(line.substr(line.find('=')+1).empty()))
						WindowSize=std::stoi(line.substr(line.find('=')+1));
					else
						log<<"##Warning## :Window Size for Probe-Distal is empty. Set to default : "<< WindowSize<<std::endl;
				}
				if(line.substr(0, line.find('=')).find("Window Size for Probe-Probe")){
					if(!(line.substr(line.find('=')+1).empty()))
						WindowSizeProbeProbe=std::stoi(line.substr(line.find('=')+1));
					else
						log<<"##Warning## :Window Size for Probe-Probe is empty. Set to default : "<< WindowSizeProbeProbe<<std::endl;
				}
			}
		}
	}
	else{
		log<<"ConfigFile.txt not found in config directory."<<std::endl;
	}
	
	//------------------------
	
	if(emptyErrFlag){
		log<< " Enter all required fields."<<std::endl;
		return 0;
	}
	
	//------------------------
	

	if(statsOption=="ComputeStatsOnly"){
		if(CALCULATE_P_VALUES){
			CALCULATE_P_VALUES=false;
			log << "Calculate p values set to No for computing Stats only"<< std::endl;
		}
		//interactiontype="Both";		
	}

	log << "Min Number of Supporting Pairs      " << MinNumberofSupportingPairs << std::endl;
	log << "Min Junction Distance               " << MinimumJunctionDistance << std::endl;
	log << "Padding around Probes               " << padding << std::endl;
	log << "Calculate p values                  " ;
	if(CALCULATE_P_VALUES)
		log<< "Yes" << std::endl;
	if(!CALCULATE_P_VALUES)
		log<< "No" << std::endl;
    
    ifstream ExpFile(ExpFileName.c_str());
    if(ExpFile.good()){
		while (!ExpFile.eof()){
			getline(ExpFile, line);
			if(line.find("=")!=string::npos){
				if(line.substr(0, line.find('=')).find("Feature Probe File")){
					string s;
					s=line.substr(line.find('=')+1);
					s.erase(std::remove_if(s.begin(), s.end(), [l](char ch) { return std::isspace(ch, l); }), s.end());
					if(s.empty()){
						log<<"!!Error!! : Feature Probe File Path is empty. It is required!" <<std::endl;
						emptyErrFlag=true;
					}
					ProbeFileName = s;
				}
				if(line.substr(0, line.find('=')).find("Negative Control Probe File")){
					string s;
					s=line.substr(line.find('=')+1);
					s.erase(std::remove_if(s.begin(), s.end(), [l](char ch) { return std::isspace(ch, l); }), s.end());
					if(s.empty() && CALCULATE_P_VALUES){
						log<<"!!Error!! : Negative Control Probe File Path is empty when Calculate p_values is Yes. It is required!" <<std::endl;
						emptyErrFlag=true;
					}
					NegCtrlProbeFileName = s;
				}
				if(line.substr(0, line.find('=')).find("Digested Genome File")){
					string s;
					s=line.substr(line.find('=')+1);
					s.erase(std::remove_if(s.begin(), s.end(), [l](char ch) { return std::isspace(ch, l); }), s.end());
					if(s.empty()){
						log<<"!!Error!! : Digested Genome File Path is empty. It is required!" <<std::endl;
						emptyErrFlag=true;
					}
					DigestedGenomeFileName = s;
				}
				if(line.substr(0, line.find('=')).find("Transcript List File")){
					string s;
					s=line.substr(line.find('=')+1);
					s.erase(std::remove_if(s.begin(), s.end(), [l](char ch) { return std::isspace(ch, l); }), s.end());
					if(s.empty()){
						log<<"##Warning## : Either Transcript List File or SNV List File is required" <<std::endl;
						featFileCount=featFileCount-2;
						countFeatFiles-=1;
					}
					TranscriptListFileName = s;
				}
				if(line.substr(0, line.find('=')).find("SNV List File")!=std::string::npos){
					std::string s;
					s=line.substr(line.find('=')+1);
					s.erase(std::remove_if(begin(s), end(s), [l](char ch) { return std::isspace(ch, l); }), end(s));
					if(s.empty()){
						log<<"##Warning## : Either Transcript List File or SNV List File is required" <<std::endl;
						featFileCount=featFileCount-1;
						countFeatFiles-=1;
						//emptyErrFlag = true;
					}
					SNPFile=s;
				}
				if(line.substr(0, line.find('=')).find("Negative control region File")!=std::string::npos){
					std::string s;
					s=line.substr(line.find('=')+1);
					s.erase(std::remove_if(begin(s), end(s), [l](char ch) { return std::isspace(ch, l); }), end(s));
					if(s.empty() && CALCULATE_P_VALUES){
						log<<"!!Error!! : Negative Control region File Path is empty when Calculate p_values is Yes. It is required!" <<std::endl;
						//featFileCount=featFileCount-1;
						//countFeatFiles-=1;
						emptyErrFlag = true;
					}
					negCtrlRegFile=s;
				}
				if(line.substr(0, line.find('=')).find("Promoters")){
					string s;
					s=line.substr(line.find('=')+1);
					s.erase(std::remove_if(s.begin(), s.end(), [l](char ch) { return std::isspace(ch, l); }), s.end());
					if(s.empty()){
						s="promoter";
						log<<"##Warning## : Promoters set to default value : "<< s<<std::endl;
						
					}
					probeType["promoter"]=s;
				}
				if(line.substr(0, line.find('=')).find("SNVs")){
					string s;
					s=line.substr(line.find('=')+1);
					s.erase(std::remove_if(s.begin(), s.end(), [l](char ch) { return std::isspace(ch, l); }), s.end());
					if(s.empty()){
						s="SNV";
						log<<"##Warning## : SNVs set to default value : "<< s<<std::endl;
						
					}
					probeType["SNP"]=s;
				}
				if(line.substr(0, line.find('=')).find("Negative controls")){
					string s;
					s=line.substr(line.find('=')+1);
					s.erase(std::remove_if(s.begin(), s.end(), [l](char ch) { return std::isspace(ch, l); }), s.end());
					if(s.empty()){
						s="neg_ctrl";
						log<<"##Warning## : Negative Controls set to default value : "<< s<<std::endl;
						
					}
					probeType["negctrl"]=s;
				}
				if(line.substr(0, line.find('=')).find("Other")){
					string s;
					s=line.substr(line.find('=')+1);
					s.erase(std::remove_if(s.begin(), s.end(), [l](char ch) { return std::isspace(ch, l); }), s.end());
					if(s.empty()){
						s="other";
						log<<"##Warning## : Other set to default value : "<< s<<std::endl;
						
					}
					probeType["other"]=s;
				}
				if(line.substr(0, line.find('=')).find("Number of Experiments")){
					NOFEXPERIMENTS=std::stoi(line.substr(line.find('=')+1));
				}
				if(interactionOption.find("PrintEnhancerProximities")){
					if(line.substr(0, line.find('=')).find("Enhancer List File")){
						string s;
						s=line.substr(line.find('=')+1);
						s.erase(std::remove_if(s.begin(), s.end(), [l](char ch) { return std::isspace(ch, l); }), s.end());
						if(s.empty()){
						log<<"!!Error!! : Enhancer List File Path is empty. It is required!" <<std::endl;
						emptyErrFlag=true;
						}
						EnhancerListFileName = s;
					}
				}
				if(line.substr(0, line.find('=')).find("Experiment BAM File Name Path")){
					
					string s;
					s=line.substr(line.find('=')+1);
					s.erase(std::remove_if(s.begin(), s.end(), [l](char ch) { return std::isspace(ch, l); }), s.end());
					Exptemp.filepath=s;
					
					getline(ExpFile, line);
					Exptemp.name=line.substr(line.find('=')+1);
					getline(ExpFile, line);
					Exptemp.designname=line.substr(line.find('=')+1);
					getline(ExpFile, line);
					Exptemp.celltype=line.substr(line.find('=')+1);
					Experiments.push_back(Exptemp);
				}
			}
		}
	}
	else{
		log<<"ExperimentFile.txt not found in config directory."<<std::endl;
	}
    
    if(TranscriptListFileName.empty()&&SNPFile.empty()){
		log << "Both transcript list file and SNV list file paths are not entered"<< std::endl;
		emptyErrFlag=true;
	}
	
	if(emptyErrFlag){ // a required field is empty
		log<< " Enter all required fields."<<std::endl;
		return 0;
	}
    
    //------------------------------------------------------------------------------------
    RESitesClass dpnII(log);
    dpnII.InitialiseVars(DigestedGenomeFileName);
    //-------------------------------------------------------------------------------------
    
    ProcessBAM bamfile(log);
    //-------------------------------------------------------------------------------------
    DetectInteractions Interactions(log, MinNumberofSupportingPairs, CALCULATE_P_VALUES, MinimumJunctionDistance);
   //-------------------------------------------------------------------------------------
    
    std::string BAMFILENAME, ExperimentName;
    std::vector < std::string > ExperimentNames;
    int ExperimentNo = 0;
    
    FeatureClass proms(log);
   
    //-------------------------------------------------------------------------------
	log << "Reading Feature files and annotating features: Starting!" << std::endl;
    proms.InitialiseData(ClusterPromoters, countFeatFiles+CALCULATE_P_VALUES);
    
   // proms.ReadPromoterAnnotation(dpnII, TranscriptListFileName, ClusterPromoters);
    switch(featFileCount){
		case 1:	
			proms.ReadFeatureAnnotation(dpnII, SNPFile, "SNV");
			break;
		case 2:
			proms.ReadFeatureAnnotation(dpnII, TranscriptListFileName, "transcript");	
			break;
		case 3:
			proms.ReadFeatureAnnotation(dpnII, TranscriptListFileName, "transcript");
			proms.ReadFeatureAnnotation(dpnII, SNPFile, "SNV");
			break;
		default:
			break;
			
	}
	if(CALCULATE_P_VALUES)
		proms.ReadFeatureAnnotation(dpnII, negCtrlRegFile, "neg_ctrl");
		
	log << "Reading Feature files and annotating features: Done!" << std::endl;

	//---------------------------------------------------------------------------------
	
	ProbeSet ProbeClass(log, (1+CALCULATE_P_VALUES), 0); // 1 for feature probe file, 2 if Neg ctrl probe file.
    ProbeClass.ReadProbeCoordinates(ProbeFileName, probeType, padding, false, reFileInfo);
    if(CALCULATE_P_VALUES)	
		ProbeClass.ReadProbeCoordinates(NegCtrlProbeFileName, probeType, padding, true, reFileInfo);
    
    //---------------------------------------------------------------------------------
    if(statsOption=="ComputeStatsOnly"){
		statsFile.open(BaseFileName+"."+reFileInfo.genomeAssembly.substr(0, reFileInfo.genomeAssembly.find_first_of(',')) +".computeStats."+reFileInfo.currTime+".txt");
		statsFile << "Total_Number_of_Pairs"<<'\t'<< "Total_Number_of_Pairs_on_Probes"<<'\t' << "Number_of_Pairs_Both_Reads_on_Probe" << '\t'<< "Number_of_Pairs_One_Read_on_Probe" << '\t' << "Number_of_Pairs_None_on_Probe" << '\t'<< "On_Probe_Pair_Fraction" <<std::endl;
	}
    
    //---------------------------------------------------------------------------------
    vector < DetermineBackgroundLevels > background;
    
    ProximityClass proximities(NOFEXPERIMENTS);
        
    if(interactionOption=="PrintProbeProximities"){
		
		for (unsigned i=0; i<Experiments.size(); ++i){ // Reads all the pairs in each experiment and fills the interaction maps

			Exptemp=Experiments[i];
			bamfile.Initialize(Exptemp.filepath, NOFEXPERIMENTS, padding, ReadLen);
			ExperimentNames.push_back(Exptemp.name);
		
		
		
			if(CALCULATE_P_VALUES){ //Fill NegCtrl proximities to calculate background interaction frequencies
				bamfile.ProcessSortedBamFile_NegCtrls(ProbeClass, dpnII, proximities, Exptemp.filepath, ExperimentNo, Exptemp.designname, statsOption);
		
				background.push_back(DetermineBackgroundLevels());
			
				//background.back().CalculateMeanandStdRegress(Exptemp.name, ExperimentNo, Exptemp.designname);
				background.back().CalculateMeanandStdRegress(Exptemp.name+".Probe_Distal", ExperimentNo, Exptemp.designname, background.back().bglevels, BinSize, "ProbeDistal", MinimumJunctionDistance, log, WindowSize);
				background.back().CalculateMeanandStdRegress(Exptemp.name+".Probe_Probe", ExperimentNo, Exptemp.designname, background.back().bglevelsProbeProbe, BinSizeProbeProbe, "ProbeProbe", MinimumJunctionDistance, log, WindowSizeProbeProbe);
				
				log << "Total_Number_of_Pairs" << '\t' << totalNumberofPairs << std::endl;
				log << "Total_Number_of_Pairs on Probes" << '\t' << NumberofPairs << std::endl;
				log << "Number_of_Pairs_Both_Reads_on_Probe" << '\t' << NofPairs_Both_on_Probe << std::endl;
				log << "Number_of_Pairs_One_Read_on_Probe" << '\t' << NofPairs_One_on_Probe << std::endl;
				log << "Number_of_Pairs_None_on_Probe" << '\t' << NofPairsNoAnn << std::endl;
				log << "FractionofPairsOnProbe" << '\t' << (NumberofPairs)/double(totalNumberofPairs) << std::endl; 	    
				NumberofPairs = 0; NofPairs_Both_on_Probe = 0; NofPairs_One_on_Probe = 0; NofPairsNoAnn = 0;
			}
        	bamfile.ProcessSortedBAMFile(ProbeClass, dpnII, proximities, Exptemp.filepath, ExperimentNo, whichchr, Exptemp.designname, statsOption);
          
			log << "Total_Number_of_Pairs" << '\t' << totalNumberofPairs << std::endl;
			log << "Total_Number_of_Pairs" << '\t' << NumberofPairs << std::endl;
			log << "Number_of_Pairs_Both_Reads_on_Probe" << '\t' << NofPairs_Both_on_Probe << std::endl;
			log << "Number_of_Pairs_One_Read_on_Probe" << '\t' << NofPairs_One_on_Probe << std::endl;
			log << "Number_of_Pairs_None_on_Probe" << '\t' << NofPairsNoAnn << std::endl;
			log << "On_Probe_Pair_Fraction" << '\t' << (NumberofPairs)/double(totalNumberofPairs) << std::endl; 	    
			
			if(statsOption=="ComputeStatsOnly"){
				statsFile <<  Exptemp.name << std::endl;
				statsFile << totalNumberofPairs << '\t' <<NumberofPairs << '\t' << NofPairs_Both_on_Probe<< '\t' << NofPairs_One_on_Probe<< '\t' << NofPairsNoAnn << '\t' << (NumberofPairs)/double(totalNumberofPairs) << std::endl;
			}
        
			NumberofPairs = 0; 
			NofPairs_Both_on_Probe = 0; 
			NofPairs_One_on_Probe = 0; 
			NofPairsNoAnn = 0;       
			++ExperimentNo;

			log << Exptemp.filepath << "     finished" << std::endl;

		}
    
		if(interactiontype=="Neg"){
			
			Interactions.CalculatePvalAndPrintInteractionsProbeDistal_NegCtrls(ProbeClass, background, BaseFileName, NOFEXPERIMENTS, ExperimentNames, whichchr, BinSize, reFileInfo); //Print all same type of interactions
			Interactions.CalculatePvalAndPrintInteractionsProbeProbe_NegCtrls(ProbeClass, background, BaseFileName, NOFEXPERIMENTS, ExperimentNames, whichchr, BinSizeProbeProbe, reFileInfo); //Print all same type of interactions
		}
		else if(interactiontype=="NonNeg"){
			
			Interactions.CalculatePvalAndPrintInteractionsProbeDistal(ProbeClass, background, BaseFileName, NOFEXPERIMENTS, ExperimentNames, whichchr, BinSize, reFileInfo);//Print all same type of interactions
			Interactions.CalculatePvalAndPrintInteractionsProbeProbe(ProbeClass, background, BaseFileName, NOFEXPERIMENTS, ExperimentNames, whichchr, BinSizeProbeProbe, reFileInfo);//Print all same type of interactions
		}
		else{	
		
			Interactions.CalculatePvalAndPrintInteractionsProbeDistal(ProbeClass, background, BaseFileName, NOFEXPERIMENTS, ExperimentNames, whichchr, BinSize, reFileInfo);//Print all same type of interactions
			Interactions.CalculatePvalAndPrintInteractionsProbeDistal_NegCtrls(ProbeClass, background, BaseFileName, NOFEXPERIMENTS, ExperimentNames, whichchr, BinSize, reFileInfo); //Print all same type of interactions
			Interactions.CalculatePvalAndPrintInteractionsProbeProbe(ProbeClass, background, BaseFileName, NOFEXPERIMENTS, ExperimentNames, whichchr, BinSizeProbeProbe, reFileInfo);//Print all same type of interactions
			Interactions.CalculatePvalAndPrintInteractionsProbeProbe_NegCtrls(ProbeClass, background, BaseFileName, NOFEXPERIMENTS, ExperimentNames, whichchr, BinSizeProbeProbe, reFileInfo); //Print all same type of interactions
		}
	}
	
	if(interactionOption=="PrintEnhancerProximities"){
		
		EnhancerSet enhancerClass;
		log<<"Enhancer File is "<<EnhancerListFileName<<std::endl;
		enhancerClass.ReadEnhancerCoordinates(EnhancerListFileName, padding, log);
		
		for (unsigned i=0; i<Experiments.size(); i++){ // Reads all the pairs in each experiment and fills the interaction maps

			Exptemp=Experiments[i];
			bamfile.Initialize(Exptemp.filepath, NOFEXPERIMENTS, padding, ReadLen);
			ExperimentNames.push_back(Exptemp.name);
		          
			bamfile.ProcessSortedBAMFileForEnhancers(ProbeClass, dpnII, proximities, Exptemp.filepath, ExperimentNo, whichchr, Exptemp.designname, statsOption, enhancerClass);     
			++ExperimentNo;

			log << Exptemp.filepath << "     finished" << std::endl;

		}
    	
		Interactions.PrintEnhancerEnhancerInteractions(ProbeClass, background, BaseFileName, ExperimentNo, ExperimentNames, whichchr, enhancerClass, reFileInfo);

	}

}
