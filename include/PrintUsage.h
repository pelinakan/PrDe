//
//  PrintUsage.h
//  HiCap_ProbeDesigner
//
//  Created by Pelin Sahlen on 02/03/2016.
//  Copyright © 2016 Pelin Sahlen. All rights reserved.
//

#ifndef PrintUsage_h
#define PrintUsage_h


static int print_usage()
{
    fprintf(stderr, "\n");
    fprintf(stderr, "Program: Design Probes for HiCap \n");
    fprintf(stderr, "Contact: Pelin Akan <pelin.akan@scilifelab.se>\n\n");
    fprintf(stderr, "Usage:   DesignProbes which_chr \n");
    fprintf(stderr, "Parameter 1:  Which Chr to process in the format chrN where N is an integer[STR]\n");
    
    return 0;
}

#endif /* PrintUsage_h */
