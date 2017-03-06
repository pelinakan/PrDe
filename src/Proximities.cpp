#include "Proximities.h"

void ProximityClass::AnnotateDistalInteractor(std::string feature_id, std::string anchored_chr, std::string interactor_chr, int *interactor_resite, int ExperimentNo){
    
    n = interactor_resite[0];
    // Intra chromosomal interaction
    if(anchored_chr.compare(interactor_chr) == 0){
        PopulateInteractions(Features[feature_id].proximities.junctions, interactor_resite, ExperimentNo);
    }
    else{  // inter chromosomal interaction
        chrfound = 0;
        for(auto itx = Features[feature_id].proximities_ctx.begin() ; itx < Features[feature_id].proximities_ctx.end();++itx){
            if (interactor_chr.compare(itx->maptochrname) == 0){
                if(itx->junctions_ctx.find(n) == itx->junctions_ctx.end()){
                    itx->junctions_ctx[n].paircount = new int[NOFEXPERIMENTS]; // add a new entry
                    itx->junctions_ctx[n].strandcombination = new int[((NOFEXPERIMENTS)*4)];
                    for(int z = 0; z < (NOFEXPERIMENTS); ++z)
                        itx->junctions_ctx[n].paircount[z] = 0;
                    for (int z = 0; z < ((NOFEXPERIMENTS)*4); ++z) {
                        itx->junctions_ctx[n].strandcombination[z] = 0;
                    }
                    itx->junctions_ctx[n].refragend = interactor_resite[1];
                    itx->junctions_ctx[n].paircount[ExperimentNo] = 1;
                    
                 }
                else{ // if inserted before
                    itx->junctions_ctx[n].paircount[ExperimentNo] += 1;
                }
                
                chrfound = 1;
                break;
            }
        }
        if(!chrfound){
            Features[feature_id].proximities_ctx.push_back(SignalStruct_CTX());
            Features[feature_id].proximities_ctx.back().maptochrname.append(interactor_chr);
            Features[feature_id].proximities_ctx.back().junctions_ctx[n].paircount = new int[NOFEXPERIMENTS];
            Features[feature_id].proximities_ctx.back().junctions_ctx[n].strandcombination = new int[NOFEXPERIMENTS*4];
            for(int z = 0; z < (NOFEXPERIMENTS); ++z)
                Features[feature_id].proximities_ctx.back().junctions_ctx[n].paircount[z] = 0;
            Features[feature_id].proximities_ctx.back().junctions_ctx[n].strandcombination = new int[((NOFEXPERIMENTS)*4)];
            for (int z = 0; z < ((NOFEXPERIMENTS)*4); ++z) {
                Features[feature_id].proximities_ctx.back().junctions_ctx[n].strandcombination[z] = 0;
            }
            Features[feature_id].proximities_ctx.back().junctions_ctx[n].refragend = interactor_resite[1];
            Features[feature_id].proximities_ctx.back().junctions_ctx[n].paircount[ExperimentNo] = 1;
           
        }
    }
}


void ProximityClass::AnnotateEnhancerDistalInteractor(std::string enid, std::string anchored_chr, std::string interactor_chr, int *interactor_resite, int ExperimentNo, EnhancerSet& enClass){
    
    n = interactor_resite[0];
    // Intra chromosomal interaction
    if(anchored_chr.compare(interactor_chr) == 0){
        PopulateInteractions(enClass.Enhancers[enid].proximities.junctions, interactor_resite, ExperimentNo);
    }
    else{  // inter chromosomal interaction
        chrfound = 0;
        for(auto itx = enClass.Enhancers[enid].proximities_ctx.begin() ; itx < enClass.Enhancers[enid].proximities_ctx.end();++itx){
            if (interactor_chr.compare(itx->maptochrname) == 0){
                if(itx->junctions_ctx.find(n) == itx->junctions_ctx.end()){
                    itx->junctions_ctx[n].paircount = new int[NOFEXPERIMENTS]; // add a new entry
                    itx->junctions_ctx[n].strandcombination = new int[((NOFEXPERIMENTS)*4)];
                    for(int z = 0; z < (NOFEXPERIMENTS); ++z)
                        itx->junctions_ctx[n].paircount[z] = 0;
                    for (int z = 0; z < ((NOFEXPERIMENTS)*4); ++z) {
                        itx->junctions_ctx[n].strandcombination[z] = 0;
                    }
                    itx->junctions_ctx[n].refragend = interactor_resite[1];
                    itx->junctions_ctx[n].paircount[ExperimentNo] = 1;
                 }
                else{ // if inserted before
                    itx->junctions_ctx[n].paircount[ExperimentNo] += 1;
                }
                
                chrfound = 1;
                break;
            }
        }
        if(!chrfound){
            enClass.Enhancers[enid].proximities_ctx.push_back(SignalStruct_CTX());
            enClass.Enhancers[enid].proximities_ctx.back().maptochrname.append(interactor_chr);
            enClass.Enhancers[enid].proximities_ctx.back().junctions_ctx[n].paircount = new int[NOFEXPERIMENTS];
            enClass.Enhancers[enid].proximities_ctx.back().junctions_ctx[n].strandcombination = new int[NOFEXPERIMENTS*4];
            for(int z = 0; z < (NOFEXPERIMENTS); ++z)
                enClass.Enhancers[enid].proximities_ctx.back().junctions_ctx[n].paircount[z] = 0;
            enClass.Enhancers[enid].proximities_ctx.back().junctions_ctx[n].strandcombination = new int[((NOFEXPERIMENTS)*4)];
            for (int z = 0; z < ((NOFEXPERIMENTS)*4); ++z) {
                enClass.Enhancers[enid].proximities_ctx.back().junctions_ctx[n].strandcombination[z] = 0;
            }
            enClass.Enhancers[enid].proximities_ctx.back().junctions_ctx[n].refragend = interactor_resite[1];
            enClass.Enhancers[enid].proximities_ctx.back().junctions_ctx[n].paircount[ExperimentNo] = 1;
        }
    }
}

void ProximityClass::AnnotateFeatFeatInteraction(std::string feature_id1, std::string feature_id2, int ExperimentNo){
    foundbefore = 0;
    for (auto it = Features[feature_id1].Inter_feature_ints.begin(); it < Features[feature_id1].Inter_feature_ints.end(); ++it){ // Check if the interaction with that promoter is seen before
        if (it->interacting_feature_id == feature_id2){
            it->signal[ExperimentNo] += 1.0;
            foundbefore = 1;
            break;
        }
    }
    if(!foundbefore){ // Create a new entry for that promoter
        Features[feature_id1].Inter_feature_ints.push_back(FeattoFeatSignalStruct());
        Features[feature_id1].Inter_feature_ints.back().interacting_feature_id = feature_id2;
        Features[feature_id1].Inter_feature_ints.back().signal.resize(NOFEXPERIMENTS);
        Features[feature_id1].Inter_feature_ints.back().signal[ExperimentNo] = 1;
        Features[feature_id1].Inter_feature_ints.back().strandcombination = new int[((NOFEXPERIMENTS)*4)];
        for (int z = 0; z < ((NOFEXPERIMENTS)*4); ++z)
            Features[feature_id1].Inter_feature_ints.back().strandcombination[z] = 0;
    }
}

void ProximityClass::AnnotateEnEnInteraction(std::string enid1, std::string enid2, int ExperimentNo, EnhancerSet& enClass){
    foundbefore = 0;
    for (auto it = enClass.Enhancers[enid1].Inter_feature_ints.begin(); it < enClass.Enhancers[enid1].Inter_feature_ints.end(); ++it){ // Check if the interaction with that promoter is seen before
        if (it->interacting_feature_id == enid2){
            it->signal[ExperimentNo] += 1.0;
            foundbefore = 1;
            break;
        }
    }
    if(!foundbefore){ // Create a new entry for that promoter
        enClass.Enhancers[enid1].Inter_feature_ints.push_back(FeattoFeatSignalStruct()); //call constructor
        enClass.Enhancers[enid1].Inter_feature_ints.back().interacting_feature_id = enid2;
        enClass.Enhancers[enid1].Inter_feature_ints.back().signal.resize(NOFEXPERIMENTS);
        enClass.Enhancers[enid1].Inter_feature_ints.back().signal[ExperimentNo] = 1;
        enClass.Enhancers[enid1].Inter_feature_ints.back().strandcombination = new int[((NOFEXPERIMENTS)*4)];
        for (int z = 0; z < ((NOFEXPERIMENTS)*4); ++z)
            enClass.Enhancers[enid1].Inter_feature_ints.back().strandcombination[z] = 0;
    }
}


void ProximityClass::PopulateInteractions(std::map<int, Junction >& signals, int *interactor_resite, int ExperimentNo){
int n = interactor_resite[0];
    
    if(signals.find(n) == signals.end()){
        signals[n].paircount = new int[NOFEXPERIMENTS];
        for(int z = 0; z < (NOFEXPERIMENTS); ++z)
            signals[n].paircount[z] = 0; // Initialise
        
        signals[n].strandcombination = new int[4*(NOFEXPERIMENTS)];
        for (int z = 0; z < 4*(NOFEXPERIMENTS); ++z)
            signals[n].strandcombination[z] = 0;
        
        signals[n].paircount[ExperimentNo] = 1;
        signals[n].refragend = interactor_resite[1];
        
        
    }
    else{
        signals[n].paircount[ExperimentNo] += 1;
    }
    
}

void ProximityClass::RecordProximities(Alignment pair, std::string feature_id1, std::string feature_id2, int ExperimentNo) {
    
    if ((feature_id1.length() != 4 && feature_id2.length() != 4)) { //"null"
        AnnotateFeatFeatInteraction(feature_id1, feature_id2, ExperimentNo);
        AnnotateFeatFeatInteraction(feature_id2, feature_id1, ExperimentNo);
        
    }
    else{
        
        if (feature_id1.length() != 4 ) // If first read is annotated with a probe, second read is the interactor
            AnnotateDistalInteractor(feature_id1, pair.chr1, pair.chr2, pair.resites2, ExperimentNo);
        else
            AnnotateDistalInteractor(feature_id2, pair.chr2, pair.chr1, pair.resites1, ExperimentNo);
    }
}

void ProximityClass::RecordEnhancerProximities(enAlignment pair, std::string enid1, std::string enid2, int ExperimentNo, EnhancerSet& enClass) {
    
    if (( enid2 != "null")) { //for both enhaners in the provided list 
        AnnotateEnEnInteraction(enid1, enid2, ExperimentNo, enClass);
        AnnotateEnEnInteraction(enid2, enid1, ExperimentNo, enClass);
        
    }
    else{
        AnnotateEnhancerDistalInteractor(enid1, pair.chr1, pair.chr2, pair.resites2, ExperimentNo, enClass);
    }
}
