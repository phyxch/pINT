// Created on Oct 15, 2010, Hexc, Kanishka
//
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef pINTRunAction_h
#define pINTRunAction_h 1

#include "pINTHistoManager.hh"
#include "G4UserRunAction.hh"
#include "globals.hh"

#include "pINTOutput.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class G4Run;

class pINTRunAction : public G4UserRunAction
{
public:
  pINTRunAction();
  pINTRunAction(pINTHistoManager*);

  virtual ~pINTRunAction();

  void BeginOfRunAction(const G4Run*);
  void   EndOfRunAction(const G4Run*);
    
  void fillPerEvent(G4double, G4double); 

private:
  
  const G4Run*  currentRun;
  
  // 
  G4double aveELoss, aveTrackLength;
  G4double aveELoss2, aveTrackLength2;

  pINTOutput* fileOut;
  pINTHistoManager* Histo;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

