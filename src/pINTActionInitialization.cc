/// Created on Sep 4, 2020 by hexc
/// for running pINT in multithreaded mode
///

#include "pINTActionInitialization.hh"
#include "pINTRunAction.hh"
#include "pINTEventAction.hh"
#include "pINTPrimaryGeneratorAction.hh"
#include "pINTSteppingAction.hh"
// #include "pINTHistoManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

pINTActionInitialization::pINTActionInitialization(pINTDetectorConstruction *pINTDetector)
 : G4VUserActionInitialization()
{
  detector = pINTDetector;  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

pINTActionInitialization::~pINTActionInitialization()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void pINTActionInitialization::BuildForMaster() const
{
  SetUserAction(new pINTRunAction);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void pINTActionInitialization::Build() const
{
  pINTRunAction* runAction = new pINTRunAction();
  SetUserAction(runAction);

  pINTEventAction* eventAction = new pINTEventAction(runAction);
  SetUserAction(eventAction);

  SetUserAction(new pINTSteppingAction(detector, eventAction, runAction));
  SetUserAction(new pINTPrimaryGeneratorAction(detector));
}  

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
