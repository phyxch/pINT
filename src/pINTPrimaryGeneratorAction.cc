// Created on Oct 15, 2010, Hexc, Kanishka

// Updated on 10/27/2010:  Add option for choosing a launch point.
// Updated on 4/28/2014, Olesya, hexc
// Updated on 9/16/2014, Olesya, hexc:  Added CLHEP namespace
// Updated on 11/27/2021: hexc
//    Updated launch position in z to follow the block size
//


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "pINTPrimaryGeneratorAction.hh"

#include "pINTDetectorConstruction.hh"
#include "pINTPrimaryGeneratorMessenger.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "Randomize.hh"

using namespace CLHEP;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

pINTPrimaryGeneratorAction::pINTPrimaryGeneratorAction(
                                             pINTDetectorConstruction* pINTDC)
  : pINTDetector(pINTDC), rndmFlag("off"),
   cosmicRay("false"),
   launchPoint(100.0*cm)
{

  G4int n_particle = 1;
  particleGun  = new G4ParticleGun(n_particle);
  
  //create a messenger for this class
  gunMessenger = new pINTPrimaryGeneratorMessenger(this);

  // default particle kinematic

  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;
  G4ParticleDefinition* particle
                    = particleTable->FindParticle(particleName="mu-");
  particleGun->SetParticleDefinition(particle);
  particleGun->SetParticleEnergy(5.*GeV);
  //particleGun->SetParticleMomentum(1.*GeV);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

pINTPrimaryGeneratorAction::~pINTPrimaryGeneratorAction()
{
  delete particleGun;
  delete gunMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void pINTPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  G4double positionX = 0.49*(pINTDetector->GetWorldSizeX());
  G4double positionY = 0.49*(pINTDetector->GetWorldSizeY());
  G4double positionZ = 0.49*(pINTDetector->GetWorldSizeZ());

  launchPoint = positionZ;
  
  //this function is called at the begining of event
  // 
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;

  G4double z0 = -launchPoint;
  G4double y0 = 1.0*cm;
  G4double x0 = 1.0*cm;

 
  if (rndmFlag == "on")
    {
      z0 = (pINTDetector->GetWorldSizeZ())*(G4UniformRand()-0.5);
    }
  
  if (cosmicRay == "on") 
    {
      G4ParticleDefinition* particle
	= particleTable->FindParticle(particleName="proton");
      particleGun->SetParticleDefinition(particle);
      // We set the proton KE below 100 GeV.
      G4double protonKE;
      while(1) {
	protonKE = 1.0 + 99.0 * G4UniformRand();
	if (probProtonKE(protonKE) > G4UniformRand()) break;
      }
      particleGun->SetParticleEnergy(protonKE*GeV);
    } 
  
  if (cosmicRay == "off") 
    {
      //G4ParticleDefinition* particle
      //= particleTable->FindParticle(particleName="mu-");
      //particleGun->SetParticleDefinition(particle);
      //particleGun->SetParticleMomentum(5.*GeV);
      G4cout << " Do nothing!" << G4endl;      
      
    }
  
  particleGun->SetParticlePosition(G4ThreeVector(x0,y0,z0));
  particleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
  particleGun->GeneratePrimaryVertex(anEvent);

}

G4double pINTPrimaryGeneratorAction::probProtonKE(G4double energy)
{
  // We used the primary proton KE distribution from PDG for selecting proton KE
  // The constant, 1.0584, was determined from a normalization.  This is only valid for choosing KE in a
  // range from 1 to 100 GeV.  For higher KE, one has to determine this constant again.  See the code
  // "primaryProtonDist.C".
  return 1.8*pow(energy, -2.7) / 1.0584;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

