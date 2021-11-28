// Created on 10/15/2010  Kanishka, Hexc
//
// This code is used for storing hits info in the sensitive detector. 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#ifndef pINTScintHit_h
#define pINTScintHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

class pINTScintHit : public G4VHit
{
  public:
  
  pINTScintHit();
  ~pINTScintHit();
  pINTScintHit(const pINTScintHit&);
  const pINTScintHit& operator=(const pINTScintHit&);
  int operator==(const pINTScintHit&) const;
  
  inline void* operator new(size_t);
  inline void  operator delete(void*);
  
  void Draw();
  void Print();
  
public:
  
  void AddScintHit( G4double e,  G4double gt, G4double x, G4double y, G4double z,
		    G4double px, G4double py, G4double pz, G4int id,  G4String name) 
  {
    EScint = e; GlobalTimeScint = gt; 
    particleName = name;
    xScint = x;
    yScint = y;
    zScint = z;
    pxScint = px;
    pyScint = py;
    pzScint = pz;
    trackID = id;
  }
  
  G4double GetEnergy() 
  { 
    return EScint ; 
  }
  
  G4double GetGlobalTime()  
  {  
    return GlobalTimeScint ; 
  }

  G4String GetParticleName()
  {
    return particleName;
  }

  
private:
  
  G4double EScint, GlobalTimeScint;
  G4double xScint, yScint, zScint;
  G4double pxScint, pyScint, pzScint;
  G4int trackID;
  G4String particleName;
  
};


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

typedef G4THitsCollection<pINTScintHit> pINTScintHitsCollection;

extern G4Allocator<pINTScintHit> pINTScintHitAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

inline void* pINTScintHit::operator new(size_t)
{
  void* aHit;
  aHit = (void*) pINTScintHitAllocator.MallocSingle();
  return aHit;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

inline void pINTScintHit::operator delete(void* aHit)
{
  pINTScintHitAllocator.FreeSingle((pINTScintHit*) aHit);
}

#endif
