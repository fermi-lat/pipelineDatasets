#include "pipelineDatasets/pruneTuple.h"

pruneTuple::~pruneTuple() { 
  if (m_chain) delete m_chain    ;
  if (m_newFile) delete m_newFile    ;

};


pruneTuple::pruneTuple(TChain* c, char* newFileName, char* cut) { 
  m_chain = c;
  m_newFile = new TFile(newFileName,"new");
  m_cut = TCut(cut);

};


int pruneTuple::prune(UInt_t maxPerFile) {

  // Concatenate AnalysisNtuple ntuple, applying cuts on active distance and
  // number of tracks
  //
  // Example of usage: from CINT -
  // .L PruneTuple.C
  // PruneTuple("IndividualRuns/nt*.root")
  //


  // cd to the new file
  m_newFile->cd();

  // copy the tree entries that pass the cuts

  TTree *newTree = m_chain->CopyTree(m_cut);

  m_newFile->Write();

  if (newTree->GetEntries() > maxPerFile) {
    Long64_t numEntries = newTree->GetEntries();
    UInt_t numTrees = Int_t(numEntries/maxPerFile);
    if (numTrees*maxPerFile < numEntries) ++numTrees;
    UInt_t i;
    for (i=0; i<numTrees; i++) {
      Int_t total = maxPerFile;
      if (i==numTrees-1) total = numEntries-i*maxPerFile;
      Int_t first = 0 + i*maxPerFile;
      char filename[50];
      sprintf(filename, "%s%d%s", "ntuple_prune", i, ".root");
      //    printf("processing %d tree starting evt = %d tot = %d\n", i, first, total);
      TFile *file=new TFile(filename,"new");
      TTree *t = newTree->CopyTree("","", total, first);
      file->Write();
      delete file;
    }
  }
  return 0;
}
