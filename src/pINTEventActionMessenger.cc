// Created on Oct 15, 2010, Hexc, Kanishka

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "pINTEventActionMessenger.hh"

#include "pINTEventAction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

pINTEventActionMessenger::pINTEventActionMessenger(pINTEventAction* EvAct)
:eventAction(EvAct)
{
  eventDir = new G4UIdirectory("/pINT/event/");
  eventDir->SetGuidance("event control");
   
  PrintCmd = new G4UIcmdWithAnInteger("/pINT/event/printModulo",this);
  PrintCmd->SetGuidance("Print events modulo n");
  PrintCmd->SetParameterName("EventNb",false);
  PrintCmd->SetRange("EventNb>0");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

pINTEventActionMessenger::~pINTEventActionMessenger()
{
  delete PrintCmd;
  delete eventDir;   
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void pINTEventActionMessenger::SetNewValue(
                                        G4UIcommand* command,G4String newValue)
{ 
  if(command == PrintCmd)
    {eventAction->SetPrintModulo(PrintCmd->GetNewIntValue(newValue));}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
