// Created on Oct 15, 2010, Hexc, Kanishka

// Updated:  Oct 17, Hexc,  Add detector update messages
//           Oct 18, Hexc,  Add option for changing material tyoe.  

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef pINTDetectorMessenger_h
#define pINTDetectorMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class pINTDetectorConstruction;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithAnInteger;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithoutParameter;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class pINTDetectorMessenger: public G4UImessenger
{
public:
  pINTDetectorMessenger(pINTDetectorConstruction* );
  ~pINTDetectorMessenger();
  
  void SetNewValue(G4UIcommand*, G4String);
  
private:
  
  pINTDetectorConstruction* pINTDetector;
  
  G4UIdirectory*             pINTDir;
  G4UIdirectory*             detDir;
  G4UIcmdWithADoubleAndUnit* MagFieldCmd;
  G4UIcmdWithADoubleAndUnit* ThicknessCmd;
  G4UIcmdWithAString* MaterialCmd;
  G4UIcmdWithoutParameter*   UpdateCmd;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

