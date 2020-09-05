// Created on Oct 15, 2010, Hexc, Kanishka
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class pINTSteppingVerbose;

#ifndef pINTSteppingVerbose_h
#define pINTSteppingVerbose_h 1

#include "G4SteppingVerbose.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class pINTSteppingVerbose : public G4SteppingVerbose
{
 public:   

   pINTSteppingVerbose();
  ~pINTSteppingVerbose();

   void StepInfo();
   void TrackingStarted();

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
