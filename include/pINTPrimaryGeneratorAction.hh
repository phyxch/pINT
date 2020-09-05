// Created on Oct 15, 2010, Hexc, Kanishka

// Updated: 10/27/2010:  Add option for choosing a launch point

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef pINTPrimaryGeneratorAction_h
#define pINTPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"

#include "pINTOutput.hh"

class G4ParticleGun;
class G4Event;
class pINTDetectorConstruction;
class pINTPrimaryGeneratorMessenger;
class pINTOutput;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class pINTPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
  pINTPrimaryGeneratorAction(pINTDetectorConstruction*);    
  virtual ~pINTPrimaryGeneratorAction();

  void GeneratePrimaries(G4Event*);
  void SetRndmFlag(G4String val) { rndmFlag = val;}
  void SetLaunchPoint(G4double val) { launchPoint = val;}
  void SetCosmicRayFlag(G4String val) { cosmicRay = val;}
  G4double probProtonKE(G4double energy);

private:
  G4ParticleGun*                particleGun;	  //pointer a to G4  class
  pINTDetectorConstruction*    pINTDetector;    //pointer to the geometry
    
  pINTPrimaryGeneratorMessenger* gunMessenger;   //messenger of this class
  G4String                      rndmFlag;	  //flag for a rndm impact point
  G4String                      cosmicRay;	  //flag for cosmic ray
  G4double                      launchPoint;       // particle launching height

  pINTOutput*  fileOut;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif


