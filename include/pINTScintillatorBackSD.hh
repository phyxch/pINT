// Created on 10/15/2010  Kanishka, Hexc
//
// This code is for defining a sensitive detector volume
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#ifndef pINTScintillatorBackSD_h
#define pINTScintillatorBackSD_h 1

#include "G4VSensitiveDetector.hh"
#include "globals.hh"

class pINTDetectorConstruction;
class G4HCofThisEvent;
class G4Step;

#include "pINTScintHit.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

class pINTScintillatorBackSD : public G4VSensitiveDetector
{
public:
  
  pINTScintillatorBackSD(G4String, pINTDetectorConstruction* );
  ~pINTScintillatorBackSD();
  
  void Initialize(G4HCofThisEvent*);
  G4bool ProcessHits(G4Step*,G4TouchableHistory*);
  void EndOfEvent(G4HCofThisEvent*);
  void clear();
  void DrawAll();
  void PrintAll();
  
private:
  
  pINTScintHitsCollection*  ScintCollection;      
  pINTDetectorConstruction* Detector;
  G4int                   nhits;
  std::vector<G4int> trackIDList;

};

#endif
