// Created on Oct 15, 2010, Hexc, Kanishka

#ifndef pINTSteppingAction_h
#define pINTSteppingAction_h 1

#include "G4UserSteppingAction.hh"

#include "globals.hh"
#include "pINTOutput.hh"

class pINTDetectorConstruction;
class pINTEventAction;
class pINTHistoManager;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class pINTSteppingAction : public G4UserSteppingAction
{
public:
  pINTSteppingAction(pINTDetectorConstruction*, pINTEventAction*, pINTHistoManager*);
  virtual ~pINTSteppingAction();

  void UserSteppingAction(const G4Step*);
    
private:
  pINTDetectorConstruction* detector;
  pINTEventAction*          eventaction;

  pINTOutput*  fileOut;
  pINTHistoManager* Histo;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
