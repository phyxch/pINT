// Created on Oct 15, 2010, Hexc, Kanishka

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef pINTEventActionMessenger_h
#define pINTEventActionMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class pINTEventAction;
class G4UIdirectory;
class G4UIcmdWithAnInteger;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class pINTEventActionMessenger: public G4UImessenger
{
public:
  pINTEventActionMessenger(pINTEventAction*);
  virtual ~pINTEventActionMessenger();
    
  void SetNewValue(G4UIcommand*, G4String);
    
private:
  pINTEventAction*     eventAction;
  G4UIdirectory*        eventDir;   
  G4UIcmdWithAnInteger* PrintCmd;    
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
