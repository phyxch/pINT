// Created on Oct 15, 2010, Hexc and his students

// Updated: 10/27/2010:  Add option for choosing a launch point

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "pINTPrimaryGeneratorMessenger.hh"

#include "pINTPrimaryGeneratorAction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

pINTPrimaryGeneratorMessenger::pINTPrimaryGeneratorMessenger(
                                          pINTPrimaryGeneratorAction* pINTGun)
:pINTAction(pINTGun)
{
  gunDir = new G4UIdirectory("/pINT/gun/");
  gunDir->SetGuidance("PrimaryGenerator control");
   
  RndmCmd = new G4UIcmdWithAString("/pINT/gun/rndm",this);
  RndmCmd->SetGuidance("Shoot randomly the incident particle.");
  RndmCmd->SetGuidance("  Choice : on(default), off");
  RndmCmd->SetParameterName("choice",true);
  RndmCmd->SetDefaultValue("on");
  RndmCmd->SetCandidates("on off");
  RndmCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  // Define "CosmicRay" option
  CosmicRay = new G4UIcmdWithAString("/pINT/gun/cosmicRay",this);
  CosmicRay->SetGuidance("Incident primary cosmic protons (based on PDG).");
  CosmicRay->SetGuidance("  Choice : on, off(default)");
  CosmicRay->SetParameterName("choice",true);
  CosmicRay->SetDefaultValue("off");
  CosmicRay->SetCandidates("on off");
  CosmicRay->AvailableForStates(G4State_PreInit,G4State_Idle);

  // Define "LaunchPoint" option
  LaunchPoint = new G4UIcmdWithADoubleAndUnit("/pINT/gun/LaunchPoint", this);
  LaunchPoint->SetGuidance("Set the height at which particle is launched.");
  LaunchPoint->SetGuidance("You may include unit.  Default unit is km.");
  LaunchPoint->SetParameterName("LaunchPoint",true);
  LaunchPoint->SetUnitCategory("Length");
  LaunchPoint->SetDefaultUnit("km");


}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

pINTPrimaryGeneratorMessenger::~pINTPrimaryGeneratorMessenger()
{
  delete RndmCmd;
  delete CosmicRay;
  delete LaunchPoint;
  delete gunDir;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void pINTPrimaryGeneratorMessenger::SetNewValue(
                                        G4UIcommand* command, G4String newValue)
{ 
  if( command == RndmCmd )
   { pINTAction->SetRndmFlag(newValue);}

  if( command == CosmicRay )
   { pINTAction->SetCosmicRayFlag(newValue);}

  if( command == LaunchPoint )
    { pINTAction->SetLaunchPoint(LaunchPoint->GetNewDoubleValue(newValue));}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
