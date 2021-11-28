// Created on October 15, 2010, Hexc and his students
// April 11, 2019: X. He, Hemendra and James
//    Add soil material definitions for studying neutron scattering in soil with different moisture contents
//
// Nov 5, 2021: X. He
//    Removed forward class definition. Use headerfiles only.
//    Removed sensitive volume definitation for the front and back scintillating plate
//    
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef pINTDetectorConstruction_h
#define pINTDetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Sphere.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"

#include "G4GeometryManager.hh"

#include "G4UniformMagField.hh"
#include "pINTDetectorMessenger.hh"
#include "pINTScintillatorFrontSD.hh"
#include "pINTScintillatorBackSD.hh"

//#include <vector>

//class G4Box;
//class G4Tubs;
//class G4LogicalVolume;
//class G4VPhysicalVolume;
//class G4Material;


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class pINTDetectorConstruction : public G4VUserDetectorConstruction
{
public:
    
    pINTDetectorConstruction();
    ~pINTDetectorConstruction();
    
public:
    
    void SetMagField(G4double);
    void SetBlockThickness(G4double);
    void SetNewMaterial(G4String);
    
    G4VPhysicalVolume* Construct();
    
    void UpdateGeometry();
    
public:
    
    G4double GetWorldSizeZ()          {return WorldSizeZ;}; 
    G4double GetWorldSizeX()          {return WorldSizeX;};
    G4double GetWorldSizeY()          {return WorldSizeY;};
    
    const G4VPhysicalVolume* GetphysiWorld() {return physiWorld;}; 
    const G4VPhysicalVolume* GetFrontScintillator() { return physiScintFront;};
    const G4VPhysicalVolume* GetBackScintillator() { return physiScintBack;};
    
private:
    
    G4Material*        defaultMaterial;
    G4Material*        ScintMaterial;
    G4Material*        WorldMaterial;
    G4Material  *H2O, *Air, *Lead, *Alum, *CO2, *Copper, *Tungston, *Uranium;
    G4Material  *soilOne, *soilOne10W, *soilOne20W, *soilOne30W;
    
    G4Box*             solidWorld;    //pointer to the solid World 
    G4LogicalVolume*   logicWorld;    //pointer to the logical World
    G4VPhysicalVolume* physiWorld;    //pointer to the physical World
    
    G4double           WorldSizeX, WorldSizeY, WorldSizeZ;
    G4double           blockX, blockY, blockZ;
    G4double           scintX, scintY, scintZ;
    
    G4Box*             solidBlock;    //pointer to the solid block material
    G4LogicalVolume*   logicBlock;    //pointer to the logical block material
    G4VPhysicalVolume* physiBlock;    //pointer to the physical block material
    
    G4Box *solidScintFront, *solidScintBack;             //pointer to the two scintillators
    G4LogicalVolume   *logicScintFront, *logicScintBack;   //pointer to the logical scintillator
    G4VPhysicalVolume *physiScintFront, *physiScintBack;   //pointer to the physical scintillator
    
    G4UniformMagField* magField;      //pointer to the magnetic field
    
    pINTDetectorMessenger* detectorMessenger;  //pointer to the Messenger
    
    pINTScintillatorFrontSD* scintFrontSD;  // Pointer to the Scintillator sensitive detector.
    
    pINTScintillatorBackSD* scintBackSD;  // Pointer to the Scintillator sensitive detector.
    
private:
    
    void DefineMaterials();
    
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
