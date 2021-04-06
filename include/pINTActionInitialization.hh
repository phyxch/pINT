/// Created on Sep 4, 2020 by hexc
/// For running pINT in multithreaded mode
///
#ifndef pINTActionInitialization_h
#define pINTActionInitialization_h 1

#include "G4VUserActionInitialization.hh"
#include "pINTDetectorConstruction.hh"
//#include "pINTHistoManager.hh"

/// Action initialization class.
///

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class pINTActionInitialization : public G4VUserActionInitialization
{
public:
  pINTActionInitialization(pINTDetectorConstruction*);
  virtual ~pINTActionInitialization();
  
  virtual void BuildForMaster() const;
  virtual void Build() const;
  
private:
  pINTDetectorConstruction *detector;
  //  pINTHistoManager *histo;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

    
