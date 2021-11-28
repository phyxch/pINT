// Created on Oct 15, 2010, Hexc, Kanishka
//
// Updated Apr 6, 2021: hexc (for Physics 8610 class)
//    Added ntuple column ID's for writing out ROOT ntuple
//
// Updated Nov 5, 2021: hexc
//    Added event ID to the ntuple
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef pINTRunAction_h
#define pINTRunAction_h 1

//#include "pINTHistoManager.hh"
#include "G4UserRunAction.hh"
#include "globals.hh"

#include "pINTOutput.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class G4Run;

class pINTRunAction : public G4UserRunAction
{
public:
  pINTRunAction();
  // pINTRunAction(pINTHistoManager*);

  virtual ~pINTRunAction();

  void BeginOfRunAction(const G4Run*);
  void   EndOfRunAction(const G4Run*);
    
  void fillPerEvent(G4double, G4double);
  G4int GetNtColID(G4int id) { return fNtColID[id];};

private:
  
  const G4Run*  currentRun;
  
  // 
  G4double aveELoss, aveTrackLength;
  G4double aveELoss2, aveTrackLength2;

  pINTOutput* fileOut;
  G4int fNtColID[9];   
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

