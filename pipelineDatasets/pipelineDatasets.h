#ifndef pipelineDatasets_h
#define pipelineDatasets_h 1

#include <TChain.h>
#include <TObjArray.h>
#include <vector>

/** @class pipelineDatasets
 * @brief query the pipeline dataset catalogue by task, datasetname and run
 * 
 * Supply either a run range or list of runs (as char*) along with task name
 * and datasetname to get create a TChain of those files. The paths to those
 * datasets will be created.
 * 
 * rc=0 is success for the int functions
 *
 * Example 1 (in interactive Root):
 * 
 * .L ~/GLAST/Pipeline/pipelineDatasets.cxx+
 * pipelineDatasets* p = new pipelineDatasets();
 * int sc = p->selectDatasets("allGamma-GR-v5r0p2","recon","001", "005");
 * if (sc==0) TChain* c = p->makeTChain("Recon");
 *
 * Example 2 (in interactive Root):
 * 
 * .L ~/GLAST/Pipeline/pipelineDatasets.cxx+
 * pipelineDatasets* p = new pipelineDatasets();
 * #include <vector>
 * std::vector<char*> run;
 * run.push_back("012");
 * run.push_back("015");
 * int sc = p->selectDatasets("allGamma-GR-v5r0p2","recon",run);
 * if (sc==0) TChain* c = p->makeTChain("Recon");
 */
 

class pipelineDatasets {
public :

  pipelineDatasets() { m_datasetsChain = 0;m_db = 0;};
 ~pipelineDatasets();

 /// find datasets by task name, dataset namde and a run range (rc=0 for success)
 int selectDatasets(char* taskName, char* datasetName, char* runMin="", 
		     char* runMax="");

 /// find datasets by task name, dataset name and a list of runs (rc=0 for success
 int selectDatasets(char* taskName, char* datasetName, std::vector<char*> &runList);

 /// create a TChain based on the selectDataSets query: supply the tree name
 TChain* makeChain(char* treeName);
 TChain* getChain() {return m_datasetsChain;};

 protected:
 void connect();

 private:

 /// list of datasets - TObjStrings
 TObjArray m_datasetsCol;

 /// chain of datasets
 TChain* m_datasetsChain;

 /// pointer to db connection
 TSQLServer *m_db;
};

#endif
