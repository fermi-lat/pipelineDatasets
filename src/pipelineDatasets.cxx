#include <TSQLServer.h>
#include <TSQLRow.h>
#include <TSQLResult.h> 
#include <TFrame.h>
#include <TH1.h>
#include <string>
#include <Riostream.h>
#include <string>
#include <sstream>
#include <TString.h>
#include <TObjString.h>
#include "pipelineDatasets.h"

 pipelineDatasets::~pipelineDatasets() { 
   if (m_datasetsChain) delete m_datasetsChain;
   if (m_db) delete m_db;
};


int pipelineDatasets::selectDatasets(char *taskName, char* datasetName, char* runMin, char* runMax){


  // This connects to the database
  char *connection="oracle://slac-oracle.slac.stanford.edu/SLACPROD";
  char *user="lat";
  char *pass="latread";

  if (m_db != 0) return 1;  // already connected!

  // Connect to oracle server
  TSQLServer *m_db = TSQLServer::Connect(connection, user, pass);
  if(m_db == NULL) {
    std::cout << "Error! No database connection" << std::endl;
    return 1;
  }


  //connect();

  // To get the list of datasets from input args
  char* sql12="select dsi.filepath, dsi.filename from glast_dp.dsinstance dsi, glast_dp.run r, glast_dp.dataset ds  where ds.dataset_pk = dsi.dataset_fk and ds.datasetname = '%s' and dsi.run_fk = r.run_pk and r.runstatus_fk = 1 and r.task_fk in (select task_pk from glast_dp.task  t where t.taskname='%s') ";


  char sql11[4096];

  // write the variables into the string (taskname and datasetname)
  std::sprintf(sql11, sql12, datasetName, taskName);
  TString sql1 = sql11;

  if (runMax > 0) {
    sql1.Strip();
    sql1.Append(" and r.runname between '");
    sql1.Append(runMin);
    sql1.Append("' and '");
    sql1.Append(runMax);
    sql1.Append("'");
  };

  sql1.Append(" order by r.run_pk asc");

 // do the query
  TSQLResult *res=m_db->Query(sql1);   
  if(res==NULL){
      std::cout<<"Error! No value returned for requested runs"<< std::endl;
    return 1;
  }
  TSQLRow *row1=res->Next();  
  if(row1==NULL){
    std::cout << "Error! No rows returned for systest_id and histogram_file" << std::endl;
    return 1;
  }

  // Fetch datasets and store in TObjArray

  do{
    TString datasetPathO = row1->GetField(0); datasetPathO = datasetPathO.Strip();
    TString datasetNameO = row1->GetField(1); datasetNameO = datasetNameO.Strip();
    TObjString* datasetSpec = new TObjString(datasetPathO.Append(datasetNameO).Strip().Data());
    m_datasetsCol.Add(datasetSpec);
  }while(res->Next());

  return 0;

};

int pipelineDatasets::selectDatasets(char *taskName, char* datasetName, std::vector<char*> &runList) {

  if (runList.size() == 0) {
    std::cout << "No runs in the runList!" << std::endl;
    return 1;
  }

  // This connects to the database
  char *connection="oracle://slac-oracle.slac.stanford.edu/SLACPROD";
  char *user="lat";
  char *pass="latread";

  if (m_db != 0) return 1;  // already connected!

  // Connect to oracle server
  TSQLServer *m_db = TSQLServer::Connect(connection, user, pass);
  if(m_db == NULL) {
    std::cout << "Error! No database connection" << std::endl;
    return 1;
  }
  //  connect();

  // To get the list of datasets from input args
  char* sql12="select dsi.filepath, dsi.filename from glast_dp.dsinstance dsi, glast_dp.run r, glast_dp.dataset ds  where ds.dataset_pk = dsi.dataset_fk and ds.datasetname = '%s' and dsi.run_fk = r.run_pk and r.runstatus_fk = 1 and r.task_fk in (select task_pk from glast_dp.task  t where t.taskname='%s') ";


  TString rListQuery("and r.runname in (");
  std::vector<char*>::iterator it;

  for (it = runList.begin(); it != runList.end(); it++) {
    if (it != runList.begin()) {rListQuery.Append(",");}
    rListQuery.Append( "'");
    rListQuery.Append((char*)(*it));
    rListQuery.Append( "'");
  }
  rListQuery.Append(")");



  char sql11[4096];

  // write the variables into the string (taskname and datasetname)
  std::sprintf(sql11, sql12, datasetName, taskName);
  TString sql1 = sql11;
  sql1.Append(rListQuery.Data());
  sql1.Append(" order by r.run_pk asc");



 // do the query
  TSQLResult *res=m_db->Query(sql1);   
  if(res==NULL){
      std::cout<<"Error! No value returned for requested runs"<< std::endl;
    return 1;
  }
  if((unsigned int)res->GetRowCount() != runList.size()){
      std::cout<<"Error! Did not find all requested runs"<< std::endl;
    return 1;
  }
  TSQLRow *row1=res->Next();  
  if(row1==NULL){
    std::cout << "Error! No rows returned for systest_id and histogram_file" << std:: endl;
    return 1;
  }

  // Fetch datasets and store in TObjArray

  do{
    TString datasetPathO = row1->GetField(0); datasetPathO = datasetPathO.Strip();
    TString datasetNameO = row1->GetField(1); datasetNameO = datasetNameO.Strip();
    TObjString* datasetSpec = new TObjString(datasetPathO.Append(datasetNameO).Strip().Data());

    m_datasetsCol.Add(datasetSpec);
  }while(res->Next());

  return 0;
};

TChain* pipelineDatasets::makeChain(char* treeName) {

  if (m_datasetsChain != 0) delete m_datasetsChain;
  m_datasetsChain = new TChain(treeName);

  for (Int_t i = 0; i < m_datasetsCol.GetEntries(); i++) {
    m_datasetsChain->Add((((TObjString*)m_datasetsCol.At(i)))->GetString().Data());
  }


  return m_datasetsChain;
};

void pipelineDatasets::connect() {

  // This connects to the database
  char *connection="oracle://slac-oracle.slac.stanford.edu/SLACPROD";
  char *user="lat";
  char *pass="latread";

  if (m_db != 0) return;  // already connected!

  // Connect to oracle server
  TSQLServer *m_db = TSQLServer::Connect(connection, user, pass);
  if(m_db == NULL) {
    std::cout << "Error! No database connection" << std::endl;
    return;
  }

}
