#ifndef pruneTuple_h
#define pruneTuple_h 1

#include "TChain.h"
#include "TCut.h"
#include "TFile.h"

/** @class pruneTuple
 * @brief query the pipeline dataset catalogue by task, datasetname and run
 * 
 */
 

class pruneTuple {
public :

  pruneTuple(TChain* c, char* newFileName = "ntuple-prune.root", char* cut = "");
 ~pruneTuple();

 /// find datasets by task name, dataset namde and a run range (rc=0 for success)
 int prune(Long64_t maxPerFile=500000000000LL);

 /// copy header from another file to this one (rc=0 for success)
 int copyHeader(const char* srcFilename);

 private:


 /// chain of datasets
 TChain* m_chain;

 /// new output file
 TFile* m_newFile;

 /// cut to apply
 TCut m_cut;



};

#endif
