// Created on Oct 15, 2010, Hexc, Kanishka
// Updated on March 5, 2011 for calcuating the total energy loss and tracklength.  hexc

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef pINTEventAction_h
#define pINTEventAction_h 1

#include "pINTDetectorConstruction.hh"

#include "G4UserEventAction.hh"
#include "pINTOutput.hh"

class pINTRunAction;
class pINTEventActionMessenger;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class pINTEventAction : public G4UserEventAction
{
public:
  pINTEventAction(pINTRunAction*);
  virtual ~pINTEventAction();
  
  void  BeginOfEventAction(const G4Event*);
  void    EndOfEventAction(const G4Event*);
  
  void SetPrintModulo(G4int val)  {printModulo = val;};
  
  void AddELossTrackLength(G4double, G4double);
  
  G4int getCurrentEventID() { return eventID; }
  G4int GetEventID();
  
private:
  pINTRunAction*  runAct;

  G4double totalEnergyDeposit, totalTrackLength;

  G4int     scintFrontID, scintBackID;  
  G4int     printModulo, eventID;
  
  pINTEventActionMessenger*  eventMessenger;
  
  pINTOutput*  fileOut;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
