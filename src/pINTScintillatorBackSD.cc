// Created on 10/15/2010  Kanishka, Hexc
//
// This code is for defining a sensitive detector volume
// 


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#include "pINTScintillatorBackSD.hh"

#include "pINTScintHit.hh"
#include "pINTDetectorConstruction.hh"

#include "G4VPhysicalVolume.hh"
#include "G4Step.hh"
#include "G4VTouchable.hh"
#include "G4TouchableHistory.hh"
#include "G4SDManager.hh"
#include "G4UnitsTable.hh"

#include "G4ios.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

pINTScintillatorBackSD::pINTScintillatorBackSD(G4String name,
				       pINTDetectorConstruction* det)
  :G4VSensitiveDetector(name),Detector(det), nhits(0)
{
  collectionName.insert(name);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

pINTScintillatorBackSD::~pINTScintillatorBackSD()
{

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void pINTScintillatorBackSD::Initialize(G4HCofThisEvent* HCE)
{
  //  G4cout << "Initializing pINTScintillatorD" << G4endl;
  ScintCollection = new pINTScintHitsCollection (SensitiveDetectorName, collectionName[0]);   
  trackIDList.clear();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4bool pINTScintillatorBackSD::ProcessHits(G4Step* aStep,G4TouchableHistory* ROhist)
{
  G4double etot = aStep->GetTrack()->GetTotalEnergy();
  G4double globt = aStep->GetTrack()->GetGlobalTime();
  G4ThreeVector pos = aStep->GetTrack()->GetPosition();
  G4ThreeVector pvec = aStep->GetTrack()->GetMomentum();
  G4double xhit, yhit, zhit, px, py, pz;
  G4int tid = aStep->GetTrack()->GetTrackID();

  xhit = pos.x(); yhit = pos.y(); zhit = pos.z();
  px = pvec.x(); py = pvec.y(); pz = pvec.z();


  // Need to check to see if the track ID has been stored in the list or not
  for (G4int i = 0; i < trackIDList.size(); i++)
    {
      if (trackIDList[i] == aStep->GetTrack()->GetTrackID()) return true;
    }
  
  trackIDList.push_back(aStep->GetTrack()->GetTrackID());
  
  // The following lines are good for debugging the code!
  /*
  G4cout << "Particles hit scintillator: " << aStep->GetTrack()->GetDefinition()->GetParticleName() 
	 << " ID:  " << aStep->GetTrack()->GetTrackID()
	 << "   etot: " << G4BestUnit(etot, "Energy") <<  "   globt: " << G4BestUnit(globt, "Time") 
	 << " pos: " << pos << "  pvec: " << pvec << G4endl;
  */
  
  G4TouchableHistory* theTouchable
    = (G4TouchableHistory*)(aStep->GetPreStepPoint()->GetTouchable());
    
  G4VPhysicalVolume* physVol = theTouchable->GetVolume();

  // Create the hit object and fill the info
  pINTScintHit* scintHit = new pINTScintHit();
  scintHit->AddScintHit(etot, globt, xhit, yhit, zhit,
			px, py, pz, tid,
			aStep->GetTrack()->GetDefinition()->GetParticleName());

  if (physVol == Detector->GetBackScintillator()) {
    ScintCollection->insert(scintHit);
    nhits++;
  }
   
  return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void pINTScintillatorBackSD::EndOfEvent(G4HCofThisEvent* HCE)
{
  static G4int HCID = -1;
  if(HCID<0)
    { 
      HCID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]); 
    }

  HCE->AddHitsCollection(HCID,ScintCollection);
  // G4cout << " (pINTScintillatorBackSD) nhits: *** " << nhits << "  HCID: " << HCID << G4endl;

  // The line below is for debugging purposes
  // ScintCollection->PrintAllHits();
  trackIDList.clear();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void pINTScintillatorBackSD::clear()
{
  trackIDList.clear();
} 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void pINTScintillatorBackSD::DrawAll()
{} 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void pINTScintillatorBackSD::PrintAll()
{} 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

