#ifndef pruneTuple_h
#define pruneTuple_h 1

#include <TChain.h>
#include <TObjArray.h>

/** @class pruneTuple
 * @brief query the pipeline dataset catalogue by task, datasetname and run
 * 
 */
 

class pruneTuple {
public :

  pruneTuple(char* newFileName = "ntuple-prune.root", char* cut = "") {};
 ~pruneTuple();

 /// find datasets by task name, dataset namde and a run range (rc=0 for success)
 int prune(UInt_t maxPerFile=200000);



 private:


 /// chain of datasets
 TChain* m_chain;

 /// new output file
 TFile* m_newFile;

 /// cut to apply
 TCut* m_cut;



};

#endif
