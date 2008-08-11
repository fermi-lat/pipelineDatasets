#include "commonRootData/FileHeader.h"

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


int pruneTuple::prune(Long64_t maxPerFile) {

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

  // TTree *newTree = m_chain->CopyTree(m_cut);
  TTree *newTree = m_chain->CloneTree(-1, "fast");
  newTree->SetMaxTreeSize(maxPerFile);

  newTree->BuildIndex("m_runId", "m_eventId");

  if (m_newFile->Write()) return 0;

  return 1;
}

int pruneTuple::copyHeader(const char* srcFilename)
{
  TFile from(srcFilename);
  if (from.IsZombie()) return -1;

  TObject* headerObj = from.Get("header");
  if (!headerObj) return 1;
  FileHeader* header = (FileHeader*)headerObj;

  m_newFile->cd();
  Int_t nBytes = header->Write("header");
  if (!nBytes) return 2;

  delete header;

  return 0;
}
