// Created on Oct 15, 2010, Hexc, Kanishka
//
// Updated April 6, 2021: hexc for physics 8610 class
//    Adding run manager pointer to the stepping action class
//

#ifndef pINTSteppingAction_h
#define pINTSteppingAction_h 1

#include "G4UserSteppingAction.hh"

#include "globals.hh"
#include "pINTOutput.hh"

class pINTDetectorConstruction;
class pINTEventAction;
class pINTRunAction;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class pINTSteppingAction : public G4UserSteppingAction
{
public:
  //  pINTSteppingAction(pINTDetectorConstruction*, pINTEventAction*, pINTHistoManager*);
  pINTSteppingAction(pINTDetectorConstruction*, pINTEventAction*, pINTRunAction*);
  virtual ~pINTSteppingAction();

  void UserSteppingAction(const G4Step*);
    
private:
  pINTDetectorConstruction* detector;
  pINTEventAction*          eventaction;
  pINTRunAction*            run_action;

  pINTOutput*  fileOut;
  bool firstStep;
  G4double initKE;       // for storing the primary particle KE
  //  pINTHistoManager* Histo;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
