// Created on Oct 15, 2010, Hexc, Kanishka
// Updated on April 21, 2014 for writing out stepping info to output file, by Olesya, hexc
// April 18, 2019: X. He and Hemendra
//   Added a new variable, stepl, to the output ntuple
// Sep 4, 2020: hexc
// removed fileOut

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "pINTSteppingAction.hh"

#include "pINTDetectorConstruction.hh"
#include "pINTEventAction.hh"
#include "G4Step.hh"
#include "G4SteppingManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

#include "pINTHistoManager.hh"

#include "G4Electron.hh"
#include "G4Proton.hh"
#include "G4Gamma.hh"
#include "G4Alpha.hh"
#include "G4DNAGenericIonsManager.hh"

//#include <stdlib.h>
//#include "Randomize.hh"

#include <iomanip>

////#include "G4RunManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

pINTSteppingAction::pINTSteppingAction(pINTDetectorConstruction* det,
				       pINTEventAction* evt, pINTHistoManager* his)
  :detector(det), eventaction(evt), Histo(his)
{
  // Get the reference for writing output to a file
  //  fileOut = pINTOutput::instance();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

pINTSteppingAction::~pINTSteppingAction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void pINTSteppingAction::UserSteppingAction(const G4Step* aStep)
{
  // get volume of the current step
  G4VPhysicalVolume* volume 
  = aStep->GetPreStepPoint()->GetTouchableHandle()->GetVolume();
  
  // collect energy and track length step by step
  G4double edep = aStep->GetTotalEnergyDeposit();
  
  G4double stepl = 0.;

  if (volume->GetName() == "Block") {
    //if (aStep->GetTrack()->GetTrackID() == 1)  // Need check this condition
      stepl = aStep->GetStepLength();
  }
  
  //
  // Update energy deposit and total tracklength.
  //
  eventaction->AddELossTrackLength(edep, stepl);

  // Here we collect all info and write to ntuple
  // Modeled from the dnaphysics example
  // This section should be modified further.  4/28/2014.  Olesya, hexc
  G4double flagParticle=0.;
  G4double flagProcess=0.;
  G4double x,y,z,xp,yp,zp;
  
  if (aStep->GetTrack()->GetDynamicParticle()->GetDefinition()->GetParticleName() == "e-")       flagParticle = 1;    
  if (aStep->GetTrack()->GetDynamicParticle()->GetDefinition()->GetParticleName() == "proton")   flagParticle = 2;
  if (aStep->GetTrack()->GetDynamicParticle()->GetDefinition()->GetParticleName() == "hydrogen") flagParticle = 3;
  if (aStep->GetTrack()->GetDynamicParticle()->GetDefinition()->GetParticleName() == "alpha")    flagParticle = 4;
  if (aStep->GetTrack()->GetDynamicParticle()->GetDefinition()->GetParticleName() == "alpha+")   flagParticle = 5;
  if (aStep->GetTrack()->GetDynamicParticle()->GetDefinition()->GetParticleName() == "helium")   flagParticle = 6;
  
  if (aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName()=="e-_G4DNAElastic")		flagProcess =11;
  if (aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName()=="e-_G4DNAExcitation")	flagProcess =12;
  if (aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName()=="e-_G4DNAIonisation")	flagProcess =13;
  if (aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName()=="e-_G4DNAAttachment")	flagProcess =14;
  if (aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName()=="e-_G4DNAVibExcitation")	flagProcess =15;
  if (aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName()=="e-_G4DNACapture")		flagProcess =16;
  
  if (aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName()=="proton_G4DNAExcitation")	flagProcess =17;
  if (aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName()=="proton_G4DNAIonisation")	flagProcess =18;
  if (aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName()=="proton_G4DNAChargeDecrease")	flagProcess =19;
  
  if (aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName()=="hydrogen_G4DNAExcitation")		flagProcess =20;
  if (aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName()=="hydrogen_G4DNAIonisation")		flagProcess =21;
  if (aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName()=="hydrogen_G4DNAChargeIncrease")	flagProcess =22;
  
  if (aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName()=="alpha_G4DNAExcitation")		flagProcess =23;
  if (aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName()=="alpha_G4DNAIonisation")		flagProcess =24;
  if (aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName()=="alpha_G4DNAChargeDecrease")	flagProcess =25;
  
  if (aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName()=="alpha+_G4DNAExcitation")	flagProcess =26;
  if (aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName()=="alpha+_G4DNAIonisation")	flagProcess =27;
  if (aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName()=="alpha+_G4DNAChargeDecrease")	flagProcess =28;
  if (aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName()=="alpha+_G4DNAChargeIncrease")	flagProcess =29;
  
  if (aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName()=="helium_G4DNAExcitation")	flagProcess =30;
  if (aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName()=="helium_G4DNAIonisation")	flagProcess =31;
  if (aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName()=="helium_G4DNAChargeIncrease")	flagProcess =32;
  
  if (aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName()=="hIoni")			flagProcess =33;
  if (aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName()=="eIoni")			flagProcess =34;
  
  if (aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName()!="Transportation")
    {  
      x=aStep->GetPreStepPoint()->GetPosition().x()/nanometer;
      y=aStep->GetPreStepPoint()->GetPosition().y()/nanometer;
      z=aStep->GetPreStepPoint()->GetPosition().z()/nanometer;
      xp=aStep->GetPostStepPoint()->GetPosition().x()/nanometer;
      yp=aStep->GetPostStepPoint()->GetPosition().y()/nanometer;
      zp=aStep->GetPostStepPoint()->GetPosition().z()/nanometer;
      /* NOT working yet
      Histo->FillNtupleDColumn(0, flagParticle);
      Histo->FillNtupleDColumn(1, flagProcess);
      Histo->FillNtupleDColumn(2, x);
      Histo->FillNtupleDColumn(3, y);
      Histo->FillNtupleDColumn(4, z);
      Histo->FillNtupleDColumn(5, aStep->GetTotalEnergyDeposit()/eV);
      Histo->FillNtupleDColumn(6, std::sqrt((x-xp)*(x-xp)+(y-yp)*(y-yp)+(z-zp)*(z-zp))/nm);
      Histo->FillNtupleDColumn(7, (aStep->GetPreStepPoint()->GetKineticEnergy() - aStep->GetPostStepPoint()->GetKineticEnergy())/eV );
      
      Histo->AddNtupleRow();
      */
    }
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
