// This code is used to write out his info to a ntuple based the dnaphysics example
// Created on 4/21/2014, Olesya, hexc
// 2/16/2018: hexc
//   Renamed the root output filename to "pINT_hits.root"
// April 18, 2019: X. He and Hemendra
//   Added a new variable, stepl, to the output ntuple
// May 15, 2019: X. He and J. Shirk
//   Added "FinishNtuple()" call for ntuple creation for geant4.10.05 version requirement
// 

#include "pINTHistoManager.hh"
#include "G4UnitsTable.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

pINTHistoManager::pINTHistoManager()
{
  fileName[0]  = "pINT_hits";
  factoryOn = false;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

pINTHistoManager::~pINTHistoManager()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void pINTHistoManager::book()
{
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  analysisManager->SetVerboseLevel(1);
  
  G4String extension = analysisManager->GetFileType();
  fileName[1] = fileName[0] + "." + extension;
  
  // open output file
  
  G4bool fileOpen = analysisManager->OpenFile(fileName[0]);
  if (!fileOpen) {
    G4cout << "\n---> pINTHistoManager::book(): cannot open " << fileName[1] 
           << G4endl;
    return;
  }  

  // create ntuple
  
  // analysisManager->SetFirstHistoId(1);   // commented out on 5/15/2019 by hexc & James
  
  analysisManager->CreateNtuple("pINT","trackStructure");
  
  analysisManager->CreateNtupleDColumn("flagParticle");
  analysisManager->CreateNtupleDColumn("flagProcess");
  analysisManager->CreateNtupleDColumn("x");
  analysisManager->CreateNtupleDColumn("y");
  analysisManager->CreateNtupleDColumn("z");
  analysisManager->CreateNtupleDColumn("edep");
  analysisManager->CreateNtupleDColumn("size");
  analysisManager->CreateNtupleDColumn("diffKin");
  analysisManager->FinishNtuple();    // added 5/15/2019 by hexc & James
  
  factoryOn = true;
  
  G4cout << "\n----> Histogram file is opened in " << fileName[1] << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void pINTHistoManager::save()
{
  if (factoryOn) {
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();    
    analysisManager->Write();
    analysisManager->CloseFile();
    G4cout << "\n----> pINTHistograms are saved in " << fileName[1] << G4endl;
      
    delete G4AnalysisManager::Instance();
    factoryOn = false;
  }         
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void pINTHistoManager::FillNtupleIColumn(G4int icol, G4int ival)
{
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  analysisManager->FillNtupleIColumn(icol,ival);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void pINTHistoManager::FillNtupleFColumn(G4int icol, G4float fval)
{
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  analysisManager->FillNtupleFColumn(icol,fval);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void pINTHistoManager::FillNtupleDColumn(G4int icol, G4double dval)
{
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  analysisManager->FillNtupleDColumn(icol,dval);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void pINTHistoManager::AddNtupleRow()
{
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  analysisManager->AddNtupleRow();
}
