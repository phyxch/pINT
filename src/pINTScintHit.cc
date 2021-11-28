// Created on 10/15/2010 Hexc and his students
//
// This code is used for storing hits info in the sensitive detector. 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#include "pINTScintHit.hh"
#include "G4UnitsTable.hh"
#include <iomanip>

G4Allocator<pINTScintHit> pINTScintHitAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

pINTScintHit::pINTScintHit()
{
   EScint = 0.; GlobalTimeScint = -100.;
   xScint = yScint = zScint = 0;
   pxScint = pyScint = pzScint = 0;
   trackID = -99;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

pINTScintHit::~pINTScintHit()
{;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

pINTScintHit::pINTScintHit(const pINTScintHit& right)
{
  EScint = right.EScint; GlobalTimeScint = right.GlobalTimeScint;
  xScint = right.xScint; yScint = right.yScint; zScint = right.zScint; 
  pxScint = right.pxScint; pyScint = right.pyScint; pzScint = right.pzScint; 
  particleName = right.particleName; trackID = right.trackID;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

const pINTScintHit& pINTScintHit::operator=(const pINTScintHit& right)
{
  EScint = right.EScint; GlobalTimeScint = right.GlobalTimeScint;
  xScint = right.xScint; yScint = right.yScint; zScint = right.zScint; 
  pxScint = right.pxScint; pyScint = right.pyScint; pzScint = right.pzScint; 
  particleName = right.particleName; trackID = right.trackID;
  return *this;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

int pINTScintHit::operator==(const pINTScintHit& right) const
{
  return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void pINTScintHit::Draw()
{;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void pINTScintHit::Print()
{
  G4cout << std::setw(6) << particleName
	 << " TrID: " << std::setw(6) << trackID
	 << " E: " << std::setw(8) <<  G4BestUnit(EScint, "Energy") 
	 << " gt: " << std::setw(11) << G4BestUnit(GlobalTimeScint, "Time") 
	 << " x: "  << std::setw(6) << G4BestUnit(xScint, "Length")
	 << " y: "  << std::setw(6) << G4BestUnit(yScint, "Length")
	 << " z: "  << std::setw(6) << G4BestUnit(zScint, "Length")
	 << " px: "  << std::setw(6) << G4BestUnit(pxScint, "Energy")
	 << " py: "  << std::setw(6) << G4BestUnit(pyScint, "Energy")
	 << " pz: "  << std::setw(6) << G4BestUnit(pzScint, "Energy")
	 <<G4endl;;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

