// Created on Oct 15, 2010, Hexc, Kanishka
//
// Updated on 10/27/2010: Added command option for picking a launch point
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef pINTPrimaryGeneratorMessenger_h
#define pINTPrimaryGeneratorMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"

class pINTPrimaryGeneratorAction;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithADoubleAndUnit;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class pINTPrimaryGeneratorMessenger: public G4UImessenger
{
public:
  pINTPrimaryGeneratorMessenger(pINTPrimaryGeneratorAction*);
  virtual ~pINTPrimaryGeneratorMessenger();
    
  void SetNewValue(G4UIcommand*, G4String);
    
private:
  pINTPrimaryGeneratorAction* pINTAction;
  G4UIdirectory*               gunDir; 
  G4UIcmdWithAString*          RndmCmd;
  G4UIcmdWithAString*          CosmicRay;
  G4UIcmdWithADoubleAndUnit*   LaunchPoint;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

