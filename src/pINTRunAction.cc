// Created on Oct 15, 2010, Hexc, Kanishka
//
// Updated on April 6, 2021: hexc
//    Fixing a problem of writing ROOT output
//
// Updated on November 5, 2021: hexc
//    Turn on ntuple merging
//
// Updated on April 26, 2022: hexc
//    Removed g4root.hh and implemented G4AnalysisManager. Added ".root" extension to the ourput filename.
//
// Updated on March 19, 2024: hexc
//    Cleaning up code and adding more comments. Also, added time and date info to the output filename.
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "pINTRunAction.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

//#include "g4root.hh"
//#include "G4RootAnalysisManager.hh"
#include "G4GenericAnalysisManager.hh"

using G4AnalysisManager = G4GenericAnalysisManager;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

pINTRunAction::pINTRunAction()
  : G4UserRunAction()
{;}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

pINTRunAction::~pINTRunAction()
{;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void pINTRunAction::BeginOfRunAction(const G4Run* aRun)
{ 
  G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;

  // Get the analysis manager
  // Create analysis manager
  auto analysisManager = G4AnalysisManager::Instance();
  G4cout << "Using " << analysisManager->GetType() << G4endl;

  // set printing event number per each event
  G4RunManager::GetRunManager()->SetPrintProgress(1);     

  // Create directories 
  //analysisManager->SetHistoDirectoryName("histograms");
  //analysisManager->SetNtupleDirectoryName("ntuple");
  analysisManager->SetVerboseLevel(1);
  analysisManager->SetNtupleMerging(true);
  // Note: merging ntuples is available only with Root output
  
  // Get time and date to creating a unique filename
  time_t rawtime;
  struct tm * timeinfo;
  char buffer[80];
  
  time (&rawtime);
  timeinfo = localtime(&rawtime);
  
  strftime(buffer,80,"-%d-%m-%Y-%H-%M-%S",timeinfo);
  G4String DateTime(buffer);
  
  // Open an output file
  G4String fileName = "pINT_output" + DateTime + ".root";  
  //  G4String fileName = "pINT_output.root";
  analysisManager->OpenFile(fileName);

  analysisManager->CreateH1("Eloss","Edep in shell", 100, 0., 100*MeV);
  //analysisManager->CreateH1("Egap","Edep in gap", 100, 0., 100*MeV);
  analysisManager->CreateH1("TrackL","trackL in shell", 100, 0., 1*m);
  
  analysisManager->SetFirstNtupleId(1);
  analysisManager->CreateNtuple("pINT", "track");  
  fNtColID[0] = analysisManager->CreateNtupleIColumn(1, "evtID");
  fNtColID[1] = analysisManager->CreateNtupleIColumn(1, "pID");
  fNtColID[2] = analysisManager->CreateNtupleDColumn(1, "Process");
  fNtColID[3] = analysisManager->CreateNtupleDColumn(1, "x");
  fNtColID[4] = analysisManager->CreateNtupleDColumn(1, "y");
  fNtColID[5] = analysisManager->CreateNtupleDColumn(1, "z");
  fNtColID[6] = analysisManager->CreateNtupleDColumn(1, "edep");
  fNtColID[7] = analysisManager->CreateNtupleDColumn(1, "size");
  fNtColID[8] = analysisManager->CreateNtupleDColumn(1, "diffKin");

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
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
