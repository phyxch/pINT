// Sep 22, 2022: hexc and Mayur
//     Removed g4root.hh, replaced with G4AnalysisManager.hh
//
#ifndef pINTHistoManager_h
#define pINTHistoManager_h 1

#include "globals.hh"
//#include "g4root.hh"
#include "G4AnalysisManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class pINTHistoManager
{
  public:

    pINTHistoManager();
   ~pINTHistoManager();

    void SetFileName   (const G4String& name) { fileName[0] = name;};
    void book();
    void save();
    void FillNtuple(G4int id, G4int col, G4double e, G4double weight = 1.0);
    void FillNtupleIColumn(G4int icol, G4int ival);
    void FillNtupleFColumn(G4int icol, G4float ival);
    void FillNtupleDColumn(G4int icol, G4double ival);
    void AddNtupleRow();
    
  private:

    G4String         fileName[2];
    G4bool           factoryOn;       
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
