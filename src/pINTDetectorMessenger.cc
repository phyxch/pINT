// Created on Oct 15, 2010, Hexc, Kanishka

// Updated: Oct 17, 2010, hexc, Add detector update message.
//          Oct 18, 2010, hexc, Add option for changing material types

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "pINTDetectorMessenger.hh"

#include "pINTDetectorConstruction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

pINTDetectorMessenger::pINTDetectorMessenger(
                                           pINTDetectorConstruction* pINTDet)
:pINTDetector(pINTDet)
{ 
  pINTDir = new G4UIdirectory("/pINT/");
  pINTDir->SetGuidance("UI commands for this detector");
  
  detDir = new G4UIdirectory("/pINT/det/");
  detDir->SetGuidance("detector control");
       
  UpdateCmd = new G4UIcmdWithoutParameter("/pINT/det/update",this);
  UpdateCmd->SetGuidance("Update calorimeter geometry.");
  UpdateCmd->SetGuidance("This command MUST be applied before \"beamOn\" ");
  UpdateCmd->SetGuidance("if you changed geometrical value(s).");
  UpdateCmd->AvailableForStates(G4State_Idle);
      
  MagFieldCmd = new G4UIcmdWithADoubleAndUnit("/pINT/det/setField",this);  
  MagFieldCmd->SetGuidance("Define magnetic field.");
  MagFieldCmd->SetGuidance("Magnetic field will be in Z direction.");
  MagFieldCmd->SetParameterName("Bz",false);
  MagFieldCmd->SetUnitCategory("Magnetic flux density");
  MagFieldCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  ThicknessCmd = new G4UIcmdWithADoubleAndUnit("/pINT/det/setThickness",this);  
  ThicknessCmd->SetGuidance("Define the thickness of the material.");
  ThicknessCmd->SetGuidance("Change the thickness of the material block.");
  ThicknessCmd->SetParameterName("LengthZ",false);
  ThicknessCmd->SetUnitCategory("Length");
  ThicknessCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  MaterialCmd = new G4UIcmdWithAString("/pINT/det/setMaterial",this);  
  MaterialCmd->SetGuidance("Define the material type.");
  MaterialCmd->SetGuidance(" Input (pick one): h2o, co2, air, copper, lead, alum, tungston, uranium, soilOne, soilOne10W, soilOne20W, and soilOne30W");
  MaterialCmd->SetParameterName("newMaterial",true);
  MaterialCmd->SetDefaultValue("H2O");
  //MaterialCmd->SetCandidates("H2O, Air, CO2, Cu, Fe, Au, Pb");
  MaterialCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

pINTDetectorMessenger::~pINTDetectorMessenger()
{
  delete UpdateCmd;
  delete MagFieldCmd;
  delete ThicknessCmd;
  delete MaterialCmd;
  delete detDir;
  delete pINTDir;  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void pINTDetectorMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{ 
   
  if( command == UpdateCmd )
   { pINTDetector->UpdateGeometry(); }

  if( command == MagFieldCmd )
   { pINTDetector->SetMagField(MagFieldCmd->GetNewDoubleValue(newValue));}

  if( command == ThicknessCmd )
   { pINTDetector->SetBlockThickness(ThicknessCmd->GetNewDoubleValue(newValue));}

  if( command == MaterialCmd )
   { pINTDetector->SetNewMaterial(newValue);}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
