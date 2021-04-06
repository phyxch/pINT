// Created on Oct 15, 2010, Hexc, Kanishka
//
// Updated on April 6, 2021: hexc
//    Fixing a problem of writing ROOT output
//
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "pINTRunAction.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "g4root.hh"
//#include "G4AnalysisManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

pINTRunAction::pINTRunAction()
  : G4UserRunAction()
{}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

pINTRunAction::~pINTRunAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void pINTRunAction::BeginOfRunAction(const G4Run* aRun)
{ 
  G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;

  // Create analysis manager
 
  G4cout << "##### Create ECRS analysis manager " << "  " << this << G4endl;
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

  G4cout << "Using " << analysisManager->GetType() << " analysis manager" << G4endl;

  // Create directories  
  //analysisManager->SetHistoDirectoryName("histograms");
  //analysisManager->SetNtupleDirectoryName("ntuple");
  analysisManager->SetVerboseLevel(1);
  
  // Open an output file
  
  G4String fileName = "pINT_output";
  analysisManager->OpenFile(fileName);

  analysisManager->SetFirstNtupleId(1);

  analysisManager->CreateNtuple("pINT", "track");
  
  fNtColID[0] = analysisManager->CreateNtupleDColumn(1, "pID");
  fNtColID[1] = analysisManager->CreateNtupleDColumn(1, "Process");
  fNtColID[2] = analysisManager->CreateNtupleDColumn(1, "x");
  fNtColID[3] = analysisManager->CreateNtupleDColumn(1, "y");
  fNtColID[4] = analysisManager->CreateNtupleDColumn(1, "z");
  fNtColID[5] = analysisManager->CreateNtupleDColumn(1, "edep");
  fNtColID[6] = analysisManager->CreateNtupleDColumn(1, "size");
  fNtColID[7] = analysisManager->CreateNtupleDColumn(1, "diffKin");

  analysisManager->FinishNtuple(1);
  
    
  //inform the runManager to save random number seed
  G4RunManager::GetRunManager()->SetRandomNumberStore(true);
    
  //initialize cumulative quantities
  //
  aveELoss = 0.;
  aveTrackLength = 0.; 
  aveELoss2 = 0.;
  aveTrackLength2 = 0.; 

  currentRun = aRun;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void pINTRunAction::fillPerEvent(G4double totalEnergyDeposit, G4double totalTrackLength)
{
  // Find the number of events being processed so far
  G4int NbOfEvents = currentRun->GetNumberOfEvent();

  // Calculate the average of ELoss and track length using the following formulum
  //     aveX_N = aveX_(N-1) + (x_N - aveX_(N-1))/(N+1) 
  // Note that N starts from zero.  For N = 0, aveX_(0-1) is ZERO.
  //
  aveELoss += (totalEnergyDeposit - aveELoss)/(NbOfEvents + 1);
  aveTrackLength += (totalTrackLength - aveTrackLength)/(NbOfEvents + 1);

  aveELoss2 += (totalEnergyDeposit*totalEnergyDeposit - aveELoss2)/(NbOfEvents + 1);
  aveTrackLength2 += (totalTrackLength*totalTrackLength - aveTrackLength2)/(NbOfEvents + 1);

  // G4cout << " Number of events processed: " << NbOfEvents << " Average ELoss:  " << aveELoss  << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void pINTRunAction::EndOfRunAction(const G4Run* aRun)
{
  G4int NbOfEvents = aRun->GetNumberOfEvent();
  if (NbOfEvents == 0) return;
  
  //compute statistics: mean and rms
  //

  G4double rmsELoss = aveELoss2 - aveELoss*aveELoss;
  if (rmsELoss >0.) rmsELoss = std::sqrt(rmsELoss); else rmsELoss = 0.;

  G4double rmsTrackLength = aveTrackLength2 - aveTrackLength*aveTrackLength;
  if (rmsTrackLength >0.) rmsTrackLength = std::sqrt(rmsTrackLength); else rmsTrackLength = 0.;
  
  
  //print
  //
  G4cout
     << "\n--------------------End of Run------------------------------\n"
     << "\n mean energy loss : " << G4BestUnit(aveELoss,"Energy")
     << " +- "                          << G4BestUnit(rmsELoss,"Energy")  
     << G4endl;
     
  G4cout
     << "\n mean trackLength : " << G4BestUnit(aveTrackLength,"Length")
     << " +- "                               << G4BestUnit(rmsTrackLength,"Length")  
     << "\n------------------------------------------------------------\n"
     << G4endl;

  
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  
  // save histograms 
  
  analysisManager->Write();
  analysisManager->CloseFile();
  
  // complete cleanup
  
  delete G4AnalysisManager::Instance();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
