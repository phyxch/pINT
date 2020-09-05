// Created on Oct 15, 2010, Hexc, Kanishka
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "pINTRunAction.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

pINTRunAction::pINTRunAction()
  : G4UserRunAction()
{}

pINTRunAction::pINTRunAction(pINTHistoManager* his) : Histo(his), G4UserRunAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

pINTRunAction::~pINTRunAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void pINTRunAction::BeginOfRunAction(const G4Run* aRun)
{ 
  G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;

  // This part is not working yet
  // Book the histogram, i.e., ntuple
  //  Histo->book();

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

  // Not working yet
  //Histo->save();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
