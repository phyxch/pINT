// Created on Oct 15, 2010, Hexc, Kanishka
// Updated on March 5, 2011 for calcuating the total energy loss and tracklength.  hexc
// Updated on Arpil 21, 2014 for writing our the total energy loss and tracklength 
//   in each event.  Olesya, hexc
// 
// Updated on April 26, 2022: hexc
//    Removed g4root.hh and implemented G4AnalysisManager.
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "pINTEventAction.hh"

#include "pINTRunAction.hh"
#include "pINTEventActionMessenger.hh"

#include "pINTScintHit.hh"

#include "G4Event.hh"
#include "G4TrajectoryContainer.hh"
#include "G4VTrajectory.hh"
#include "G4VVisManager.hh"
#include "G4UnitsTable.hh"
//#include "G4SDManager.hh"

//#include "g4root.hh"
#include "G4GenericAnalysisManager.hh"
using G4AnalysisManager = G4GenericAnalysisManager;


#include "Randomize.hh"
#include <iomanip>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

pINTEventAction::pINTEventAction(pINTRunAction* run)
  :runAct(run),scintFrontID(-1), scintBackID(-1), 
   printModulo(1),eventMessenger(0)
{
  eventMessenger = new pINTEventActionMessenger(this);
  
  // Get the reference for writing output to a file
  //  fileOut = pINTOutput::instance();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

pINTEventAction::~pINTEventAction()
{
  delete eventMessenger;
}

// 
G4int pINTEventAction::GetEventID()
{
  return eventID;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void pINTEventAction::BeginOfEventAction(const G4Event* evt)
{  
  eventID = evt->GetEventID();
  
  if (eventID%printModulo == 0) { 
    G4cout << "\n---> Begin of event: " << eventID << G4endl;
    CLHEP::HepRandom::showEngineStatus();
  }

  /* 11/5/2021 
  if ( (scintFrontID == -1) || (scintBackID == -1) )   {
    G4SDManager *SDman = G4SDManager::GetSDMpointer();
    scintFrontID = SDman->GetCollectionID("scintFrontSD");
    scintBackID = SDman->GetCollectionID("scintBackSD");
  }
  */

  totalEnergyDeposit = 0.0;
  totalTrackLength = 0.0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void pINTEventAction::EndOfEventAction(const G4Event* evt)
{
  //print per event (modulo n)
  //
  G4int evtNb = evt->GetEventID();
  
  // 
  // Printout the total energy loss and tracklength 
  //
  G4cout << "  ================= Per event total energy deposit ========== " << G4endl;
  G4cout << "  Total energy loss:  " << G4BestUnit(totalEnergyDeposit, "Energy") << G4endl;
  G4cout << "  Total track length: " << G4BestUnit(totalTrackLength, "Length") << G4endl;
  G4cout << "  =========================================================== " << G4endl;

  runAct->fillPerEvent(totalEnergyDeposit, totalTrackLength);

  // get analysis manager
  auto analysisManager = G4AnalysisManager::Instance();

  // accumulate statistic
  // in the order of the histograms, ntuple columns declarations
  G4int counter = 0;
  analysisManager->FillH1(counter++, totalEnergyDeposit);
  analysisManager->FillH1(counter++, totalTrackLength);
  
}  


void pINTEventAction::AddELossTrackLength(G4double edep, G4double stepl)
{  
  totalEnergyDeposit += edep;
  totalTrackLength += stepl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
