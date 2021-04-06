// Created on Oct 15, 2010, Hexc, Kanishka
// Updated on April 21, 2014 for writing out stepping info to output file, by Olesya, hexc
// April 18, 2019: X. He and Hemendra
//   Added a new variable, stepl, to the output ntuple
// Sep 4, 2020: hexc
//       removed fileOut
// Apr 6, 2021: hexc
//       Follow ECRS scheme of adding ntuples. Make run action visiable in the stepping action

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "pINTSteppingAction.hh"

#include "pINTDetectorConstruction.hh"
#include "pINTEventAction.hh"
#include "pINTRunAction.hh"
#include "G4Step.hh"
#include "G4SteppingManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

//#include "pINTHistoManager.hh"
#include "g4root.hh"

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
				       pINTEventAction* evt, pINTRunAction* run)
  :detector(det), eventaction(evt), run_action(run)
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
      
  // get analysis manager
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  
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

      analysisManager->FillNtupleDColumn(1, run_action->GetNtColID(0), flagParticle);
      analysisManager->FillNtupleDColumn(1, run_action->GetNtColID(1), flagProcess);
      analysisManager->FillNtupleDColumn(1, run_action->GetNtColID(2), x);
      analysisManager->FillNtupleDColumn(1, run_action->GetNtColID(3), y);
      analysisManager->FillNtupleDColumn(1, run_action->GetNtColID(4), z);
      analysisManager->FillNtupleDColumn(1, run_action->GetNtColID(5), aStep->GetTotalEnergyDeposit()/eV );
      analysisManager->FillNtupleDColumn(1, run_action->GetNtColID(6), std::sqrt((x-xp)*(x-xp)+(y-yp)*(y-yp)+(z-zp)*(z-zp))/nm );
      analysisManager->FillNtupleDColumn(1, run_action->GetNtColID(7), (aStep->GetPreStepPoint()->GetKineticEnergy() - aStep->GetPostStepPoint()->GetKineticEnergy())/eV);
      
      analysisManager->AddNtupleRow(1);

    }
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
