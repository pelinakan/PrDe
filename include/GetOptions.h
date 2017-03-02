#ifndef INTSUITE_INC_GO_H_
#define INTSUITE_INC_GO_H_

static int print_usage()
{
  fprintf(stderr, "\n");
  fprintf(stderr, "Program: Chromosome Conformation Analysis \n");
  fprintf(stderr, "Contact: Pelin Akan <pelin.akan@scilifelab.se>\n\n");
  fprintf(stderr, "Usage:   AnalyseHiCap_chr_interact_stats_neg chrN interactionoption statsoption printoption\n");
  fprintf(stderr, "Parameter 1:  Which Chromosome to process in the format chrN, where N can be the name/number of the chromosome or All if processing all available chromosomes [string] \n");
  fprintf(stderr, "Parameter 2:  interactionoption can be 'PrintProbeInteractions' to find and print all the Probe interactions or 'PrintEnhancerInteractions' to find and print all the Probe interactions \n");
  fprintf(stderr, "Parameter 3:  statsoption can be 'ComputeStatsOnly' to compute only the stats or 'PrintInteractions' to also find and print interactions (currently only for Probe interactions)\n");
  fprintf(stderr, "Parameter 4:  printoption can be 'Neg' to print only negative control Probe interactions or 'Nonneg' to print only non negative control Probe interactions or 'Both' to print both (currently only for Probe interactions)\n"
				  "printOption will be set to 'Both' if statsoption is 'ComputeStatsOnly'");
  return 0;
}

#endif // INTSUITE_INC_GO_H_
