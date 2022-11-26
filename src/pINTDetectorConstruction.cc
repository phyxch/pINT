// Updated on Nov 15, 2013 for Fall2013 Phy3300 class, hexc
// Created on Oct 12, 2010, Hexc and his students
// Updated on 9/16/2014, Olesya, hexc:  Added CLHEP namespace
// April 11, 2019: X. He, Hemendra and James
//    Add soil material definitions for studying neutron scattering in soil with different moisture contents
// April 18, 2019: X. He and Hemendra
//    Add soil material with different moisture contents
// March 15, 2021: Hexc
//    Add scintillator material

// November 27, 2021: Hexc
//    Set the world size as a factor of 1.5 larger than the block size in x, y and z.
//

// November 15, 2022L Hexc
//   Added option for reading in a detector configuration file.
//

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
#include "pINTDetectorConstruction.hh"
#include "pINTDetectorMessenger.hh"

#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4UniformMagField.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"
#include "G4SDManager.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "pINTScintillatorFrontSD.hh"
#include "pINTScintillatorBackSD.hh"

#include "pINTPrimaryGeneratorAction.hh"

#include <math.h>

using namespace CLHEP;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

pINTDetectorConstruction::pINTDetectorConstruction()
  :defaultMaterial(0), ScintMaterial(0),
   solidWorld(0),logicWorld(0),physiWorld(0),
   WorldSizeX(0.), WorldSizeY(0.), WorldSizeZ(0.),
   blockX(0.), blockY(0.), blockZ(0.),
   scintX(0.), scintY(0.), scintZ(0.),
   solidScintFront(0), solidScintBack(0),
   logicScintFront(0), logicScintBack(0),
   physiScintFront(0), physiScintBack(0),
   magField(0), scintFrontSD(NULL), scintBackSD(NULL)
{

  // These numbers will be updated by reading in a configuration file
  // default parameter values of the atmospheric volume
  blockZ = 50.0*cm;
  blockX = 30.0*cm;
  blockY = 30.0*cm;

  scintZ = 1.0*mm;  // changed from 1.0*cm to 1.0*mm for neutron scattering studies, 5/12/19, hexc
  scintX = 30.0*cm;
  scintY = 30.0*cm;

  WorldSizeZ = 1.5*blockZ;
  WorldSizeX = 1.5*blockX;
  WorldSizeY = 1.5*blockY;
  
  // materials
  DefineMaterials();
  
  // create commands for interactive definition of the atmospheric layers
  detectorMessenger = new pINTDetectorMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

pINTDetectorConstruction::~pINTDetectorConstruction()
{ 
  delete detectorMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void pINTDetectorConstruction::DefineMaterials()
{ 
  //This function illustrates the possible ways to define materials
  
  G4String symbol;             //a=mass of a mole;
  G4double a, z, density;      //z=mean number of protons;  
  G4int iz, n;                 //iz=number of protons  in an isotope; 
  // n=number of nucleons in an isotope;
  
  G4int ncomponents, natoms;
  G4double abundance, fractionmass;
  
  //
  // define Elements
  //
  
  G4Element* H  = new G4Element("Hydrogen",symbol="H" , z= 1., a= 1.01*g/mole);
  G4Element* C  = new G4Element("Carbon"  ,symbol="C" , z= 6., a= 12.01*g/mole);
  G4Element* N  = new G4Element("Nitrogen",symbol="N" , z= 7., a= 14.01*g/mole);
  G4Element* O  = new G4Element("Oxygen"  ,symbol="O" , z= 8., a= 16.00*g/mole);
  G4Element* Mg = new G4Element("Magnesium", symbol="Mg", z = 12, a=24.305*g/mole);
  G4Element* Al = new G4Element("Aluminum", symbol="Al", z = 13, a=26.982*g/mole);
  G4Element* Si = new G4Element("Silicon",symbol="Si" , z= 14., a= 28.09*g/mole);
  G4Element* Ca = new G4Element("Calcium",symbol="Ca" , z= 20., a= 40.078*g/mole);
  G4Element* Ti = new G4Element("Titanium", symbol="Ti", z = 22, a=47.867*g/mole);
  G4Element* Fe = new G4Element("Iron",symbol="Fe" , z= 26., a= 55.845*g/mole);
  G4Element* W = new G4Element("Tungston",symbol="W" , z= 74., a= 183.85*g/mole);

  
  //
  // define an Element from isotopes, by relative abundance 
  //
  
  G4Isotope* U5 = new G4Isotope("U235", iz=92, n=235, a=235.01*g/mole);
  G4Isotope* U8 = new G4Isotope("U238", iz=92, n=238, a=238.03*g/mole);
 
  G4Element* U  = new G4Element("enriched Uranium",symbol="U",ncomponents=2);
  U->AddIsotope(U5, abundance= 90.*perCent);
  U->AddIsotope(U8, abundance= 10.*perCent);

  Uranium = new G4Material("Uranium", density=19.1*g/cm3, ncomponents=1);
  Uranium->AddElement(U, natoms=1);
  
  //
  // define simple materials
  //
  new G4Material("liquidArgon", z=18., a= 39.95*g/mole, density= 1.390*g/cm3);
  
  Alum = new G4Material("Aluminium", z=13., a=26.98*g/mole, density=2.700*g/cm3);
  Copper = new G4Material("Copper", z=29., a= 63.546*g/mole, density= 8.94*g/cm3);
  Tungston = new G4Material("Tungston", z=74., a= 183.85*g/mole, density= 15.63*g/cm3);
  Lead = new G4Material("Lead", z=82., a= 207.19*g/mole, density= 11.35*g/cm3);
  
  //
  // define a material from elements.   case 1: chemical molecule
  //

  // This is a fake cloud density.  Need a better definition.
  H2O = new G4Material("Water", density= 0.998*g/cm3, ncomponents=2);
  H2O->AddElement(H, natoms=2);
  H2O->AddElement(O, natoms=1);
  // overwrite computed meanExcitationEnergy with ICRU recommended value 
  H2O->GetIonisation()->SetMeanExcitationEnergy(75.0*eV);

  // Define the material:  BC408 plastic scintillator
  G4Material* Sci = new G4Material("Scintillator", density= 1.032*g/cm3, ncomponents=2);
  Sci->AddElement(C, natoms=10);
  Sci->AddElement(H, natoms=11);
  
  ScintMaterial = Sci;
  
  G4Material* Myl = new G4Material("Mylar", density= 1.397*g/cm3, ncomponents=3);
  Myl->AddElement(C, natoms=10);
  Myl->AddElement(H, natoms= 8);
  Myl->AddElement(O, natoms= 4);
  
  G4Material* SiO2 = new G4Material("quartz",density= 2.200*g/cm3, ncomponents=2);
  SiO2->AddElement(Si, natoms=1);
  SiO2->AddElement(O , natoms=2);
  
  //
  // define a material from elements.   case 2: mixture by fractional mass
  //
  
  Air = new G4Material("Air", density= 1.290*mg/cm3, ncomponents=2);
  Air->AddElement(N, fractionmass=0.7);
  Air->AddElement(O, fractionmass=0.3);

  //
  // define a material from elements and/or others materials (mixture of mixtures)
  // Most of the soild density info is from Wikipedia
  
  G4Material* Aerog = new G4Material("Aerogel", density = 0.200*g/cm3, ncomponents=3);
  Aerog->AddMaterial(SiO2, fractionmass=62.5*perCent);
  Aerog->AddMaterial(H2O , fractionmass=37.4*perCent);
  Aerog->AddElement (C   , fractionmass= 0.1*perCent);

  G4Material* Al2O3 = new G4Material("AlOxide", density = 3.95*g/cm3, ncomponents = 2);
  Al2O3 -> AddElement(Al, natoms=2);
  Al2O3 -> AddElement(O, natoms=3);

  G4Material* Fe2O3 = new G4Material("FeOxide", density = 5.24*g/cm3, ncomponents = 2);
  Fe2O3 -> AddElement(Fe, natoms=2);
  Fe2O3 -> AddElement(O, natoms=3);

  G4Material* CaO = new G4Material("CaOxide", density = 3.34*g/cm3, ncomponents = 2);
  CaO -> AddElement(Ca, natoms=1);
  CaO -> AddElement(O, natoms=1);

  G4Material* MgO = new G4Material("MgOxide", density = 3.54*g/cm3, ncomponents = 2);
  MgO -> AddElement(Mg, natoms=1);
  MgO -> AddElement(O, natoms=1);
    
  G4Material* TiO2 = new G4Material("TiOxide", density = 4.23*g/cm3, ncomponents = 2);
  TiO2 -> AddElement(Ti, natoms=1);
  TiO2 -> AddElement(O, natoms=2);

  // We assume organic soil components have 1.33 g/cm3 density. This number should be updated with
  // specifric soil type simulation. 4/12/2019
  G4Material* OrganicMat = new G4Material("Organic", density = 1.33*g/cm3, ncomponents = 4);
  OrganicMat -> AddElement(C, natoms=50);
  OrganicMat -> AddElement(O, natoms=42);
  OrganicMat -> AddElement(H, natoms=5);
  OrganicMat -> AddElement(N, natoms=3);

  // Define soil material
  // Based on: http://gfnun.unal.edu.co/fileadmin/content/gruposdeinvestigacion/fisicanuclear/Tesis/DanielAndrade_TG.pdf
  soilOne = new G4Material("DrySoil", density = 0.6*g/cm3, ncomponents=7);
  soilOne->AddMaterial(SiO2, fractionmass=61.3*perCent);
  soilOne->AddMaterial(Al2O3, fractionmass=13.0*perCent);
  soilOne->AddMaterial(Fe2O3, fractionmass=2.5*perCent);
  soilOne->AddMaterial(CaO, fractionmass=1.6*perCent);
  soilOne->AddMaterial(MgO, fractionmass=0.7*perCent);
  soilOne->AddMaterial(TiO2, fractionmass=0.6*perCent);
  soilOne->AddMaterial(OrganicMat, fractionmass=20.3*perCent);

  // 10% moisture content
  soilOne10W = new G4Material("DrySoil10W", density = 0.6*g/cm3, ncomponents=8);
  soilOne10W->AddMaterial(SiO2, fractionmass=55.17*perCent);
  soilOne10W->AddMaterial(Al2O3, fractionmass=11.7*perCent);
  soilOne10W->AddMaterial(Fe2O3, fractionmass=2.25*perCent);
  soilOne10W->AddMaterial(CaO, fractionmass=1.44*perCent);
  soilOne10W->AddMaterial(MgO, fractionmass=0.63*perCent);
  soilOne10W->AddMaterial(TiO2, fractionmass=0.54*perCent);
  soilOne10W->AddMaterial(OrganicMat, fractionmass=18.27*perCent);
  soilOne10W->AddMaterial(H2O, fractionmass=10.0*perCent);

  // 20% moisture content. Need new density value?
  soilOne20W = new G4Material("DrySoil20W", density = 0.6*g/cm3, ncomponents=8);
  soilOne20W->AddMaterial(SiO2, fractionmass=49.04*perCent);
  soilOne20W->AddMaterial(Al2O3, fractionmass=10.4*perCent);
  soilOne20W->AddMaterial(Fe2O3, fractionmass=2.0*perCent);
  soilOne20W->AddMaterial(CaO, fractionmass=1.28*perCent);
  soilOne20W->AddMaterial(MgO, fractionmass=0.56*perCent);
  soilOne20W->AddMaterial(TiO2, fractionmass=0.48*perCent);
  soilOne20W->AddMaterial(OrganicMat, fractionmass=16.24*perCent);
  soilOne20W->AddMaterial(H2O, fractionmass=20.0*perCent);

  // 30% moisture content. Need new density value?
  soilOne30W = new G4Material("DrySoil30W", density = 0.6*g/cm3, ncomponents=8);
  soilOne30W->AddMaterial(SiO2, fractionmass=42.91*perCent);
  soilOne30W->AddMaterial(Al2O3, fractionmass=9.1*perCent);
  soilOne30W->AddMaterial(Fe2O3, fractionmass=1.75*perCent);
  soilOne30W->AddMaterial(CaO, fractionmass=1.12*perCent);
  soilOne30W->AddMaterial(MgO, fractionmass=0.49*perCent);
  soilOne30W->AddMaterial(TiO2, fractionmass=0.42*perCent);
  soilOne30W->AddMaterial(OrganicMat, fractionmass=14.21*perCent);
  soilOne30W->AddMaterial(H2O, fractionmass=30.0*perCent);

  //
  // examples of gas in non STP conditions
  //
  
  CO2 = new G4Material("CarbonicGas", density= 1.842*mg/cm3, ncomponents=2,
		   kStateGas, 325.*kelvin, 50.*atmosphere);
  CO2->AddElement(C, natoms=1);
  CO2->AddElement(O, natoms=2);
  
  G4Material* steam = 
    new G4Material("WaterSteam", density= 0.3*mg/cm3, ncomponents=1,
		   kStateGas, 500.*kelvin, 2.*atmosphere);
  steam->AddMaterial(H2O, fractionmass=1.);
  
  //
  // examples of vacuum
  //
  
  G4Material* Vacuum =
    new G4Material("Galactic", z=1., a=1.01*g/mole, density= universe_mean_density,
		   kStateGas, 2.73*kelvin, 3.e-18*pascal);
  
  G4Material* beam = 
    new G4Material("Beam", density= 1.e-5*g/cm3, ncomponents=1,
		   kStateGas, STP_Temperature, 2.e-2*bar);
  beam->AddMaterial(Air, fractionmass=1.);
  
  G4cout << *(G4Material::GetMaterialTable()) << G4endl;
  
  // 
  // Define the default materials
  //
  defaultMaterial  = H2O;

  //
  // Set the world material
  //
  // Note: 4/4/2011. One can not use air material for the world volume
  //   for low energy particles since they will interact with the air
  //   molecules.  We simply make the world material as vacuum. 
  WorldMaterial = Vacuum;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4VPhysicalVolume* pINTDetectorConstruction::Construct()
{  
  // Clean old geometry, if any
  //
  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();

  // read the data through configuration file
  std::ifstream infile ("config_pINT.txt");
  
  std::string line;

  // Skip two comment lines
  std::getline(infile, line);
  G4cout << line << G4endl;
  std::getline(infile, line);
  G4cout << line << G4endl;

  while (std::getline(infile, line))
  {
    vector<string> row_values;

    split(line, ',', row_values);

    if ( row_values[0] == "h2o") {
      defaultMaterial = H2O;
    } else if ( row_values[0] == "co2") {
      defaultMaterial = CO2;
    } else if ( row_values[0] == "lead") {
      defaultMaterial = Lead;
    } else if ( row_values[0] == "copper") {
      defaultMaterial = Copper;
    } else if ( row_values[0] == "uranium") {
      defaultMaterial = Uranium;
    } else if ( row_values[0] == "tungston") {
      defaultMaterial = Tungston;
    } else if ( row_values[0] == "alum") {
      defaultMaterial = Alum;
    } else if ( row_values[0] == "air") {
      defaultMaterial = Air;
    } else if ( row_values[0] == "scint") {
      defaultMaterial = ScintMaterial;
    } else if ( row_values[0] == "soilOne") {
      defaultMaterial = soilOne;
    } else if ( row_values[0] == "soilOne10W") {
      defaultMaterial = soilOne10W;
    } else if ( row_values[0] == "soilOne20W") {
      defaultMaterial = soilOne20W;
    } else if ( row_values[0] == "soilOne30W") {
      defaultMaterial = soilOne30W;
    } else {
      G4cout << "Material is not found!!! Check your list of available materials" << G4endl;
    }
    
    blockX = stod(row_values[1])*cm;
    blockY = stod(row_values[2])*cm;
    blockZ = stod(row_values[3])*cm;   
  }

  infile.close();
  
  //     
  // World
  //
  
  WorldSizeX = 1.5*blockX; // Make sure that we have the updated blockX size
  WorldSizeY = 1.5*blockY; // Make sure that we have the updated blockY size
  WorldSizeZ = 1.5*blockZ; // Make sure that we have the updated blockZ size

  solidWorld = new G4Box("World",				//its name
			 WorldSizeX/2,WorldSizeY/2,WorldSizeZ/2);	//its size

  logicWorld = new G4LogicalVolume(solidWorld,		//its solid
                                  WorldMaterial,	//
                                   "World");		//its name
  
  physiWorld = new G4PVPlacement(0,			//no rotation
  				 G4ThreeVector(),	//at (0,0,0)
                                 "World",		//its name
                                 logicWorld,		//its logical volume				 
                                 0,			//its mother  volume
                                 false,			//no boolean operation
                                 0);			//copy number
  
  // 
  // Make block material
  //

  solidBlock = new G4Box("Block",			 //its name
			 blockX/2,blockY/2,blockZ/2);	//its size

  logicBlock = new G4LogicalVolume(solidBlock,		//its solid
                                  defaultMaterial,	//
                                   "Block");		//its name
  
  physiBlock = new G4PVPlacement(0,			//no rotation
  				 G4ThreeVector(),	//at (0,0,0)
                                 "Block",		//its name
                                 logicBlock,		//its logical volume				 
                                 physiWorld,			//its mother  volume
                                 false,			//no boolean operation
                                 0);			//copy number
  
  // 
  // construct front and back scintillators
  //
  scintX = blockX;
  scintY = blockY;
  
  solidScintFront = new G4Box("ScintFront",			 //its name
			      scintX/2,scintY/2,scintZ/2);	//its size
  
  logicScintFront = new G4LogicalVolume(solidScintFront,		//its solid
					ScintMaterial,	//
					"ScintFront");		//its name
  
  physiScintFront = new G4PVPlacement(0,     //no rotation
				      G4ThreeVector(0.0, 0.0, blockZ/2.+scintZ/2.),	//at (0,0,0)
				      "ScintFront",		//its name
				      logicScintFront,		//its logical volume				 
				      physiWorld,	        //its mother  volume
				      false,			//no boolean operation
				      0);			//copy number
  
  solidScintBack = new G4Box("ScintBack",		//its name
			      scintX/2,scintY/2,scintZ/2);	//its size
  
  logicScintBack = new G4LogicalVolume(solidScintBack,		//its solid
					ScintMaterial,	//
					"ScintBack");		//its name
  
  physiScintBack = new G4PVPlacement(0,			//no rotation
				     G4ThreeVector(0.0, 0.0, -(blockZ/2.+scintZ/2.)),	//at (0,0,0)
				      "ScintBack",		//its name
				      logicScintBack,		//its logical volume				 
				      physiWorld,			//its mother  volume
				      false,			//no boolean operation
				      0);			//copy number

  //                                        
  // Visualization attributes
  //
  // logicWorld->SetVisAttributes(G4VisAttributes::Invisible);
  
  G4VisAttributes* simpleWorldVisAtt= new G4VisAttributes(G4Colour(0.,1.0, 0.));
  simpleWorldVisAtt->SetVisibility(false);
  simpleWorldVisAtt->SetForceWireframe(true);
  logicWorld->SetVisAttributes(simpleWorldVisAtt);

  G4VisAttributes* blockColor = new G4VisAttributes(G4Colour(0.5, 0.5, 0.5));
  blockColor->SetVisibility(true);
  blockColor->SetForceWireframe(true);
  blockColor->SetForceSolid(false);
  
  G4VisAttributes* scintColorFront = new G4VisAttributes(G4Colour(0.2, 0.4, 0.3));
  scintColorFront->SetVisibility(true);
  scintColorFront->SetForceWireframe(false);
  scintColorFront->SetForceSolid(true);

  G4VisAttributes* scintColorBack = new G4VisAttributes(G4Colour(0.2, 0.5, 0.2));
  scintColorBack->SetVisibility(true);
  scintColorBack->SetForceWireframe(false);
  scintColorBack->SetForceSolid(true);
  
  logicBlock->SetVisAttributes(blockColor);
  logicScintFront->SetVisAttributes(scintColorFront);
  logicScintBack->SetVisAttributes(scintColorBack);

  /* Removed sensitive volume use 11/5/2021 HE
  // Implement the sensitive detector and add to the SD manager
  G4SDManager* SDman = G4SDManager::GetSDMpointer();

  if (!scintFrontSD) {
    scintFrontSD = new pINTScintillatorFrontSD("scintFrontSD", this);
    SDman->AddNewDetector(scintFrontSD);
  }

  if (!scintBackSD) {
    scintBackSD = new pINTScintillatorBackSD("scintBackSD", this);
    SDman->AddNewDetector(scintBackSD);
  }
  
  logicScintFront->SetSensitiveDetector(scintFrontSD);
  logicScintBack->SetSensitiveDetector(scintBackSD);
  
  */
  
  //
  //always return the physical World
  //
  return physiWorld;
  
}
  
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"

void pINTDetectorConstruction::SetMagField(G4double fieldValue)
{
  //apply a global uniform magnetic field along Z axis
  G4FieldManager* fieldMgr
   = G4TransportationManager::GetTransportationManager()->GetFieldManager();

  if(magField) delete magField;		//delete the existing magn field

  if(fieldValue!=0.)			// create a new one if non nul
  { magField = new G4UniformMagField(G4ThreeVector(0.,0.,fieldValue));
    fieldMgr->SetDetectorField(magField);
    fieldMgr->CreateChordFinder(magField);
  } else {
    magField = 0;
    fieldMgr->SetDetectorField(magField);
  }
}

void pINTDetectorConstruction::SetBlockThickness(G4double thickness)
{
  blockZ = thickness;
}


void pINTDetectorConstruction::SetNewMaterial(G4String mat)
{
  if ( mat.compareTo("h2o") == 0) {
    defaultMaterial = H2O;
    return;
  } else if ( mat.compareTo("co2") == 0) {
    defaultMaterial = CO2;
    return;
  } else if ( mat.compareTo("lead") == 0) {
    defaultMaterial = Lead;
    return;
  } else if ( mat.compareTo("copper") == 0) {
    defaultMaterial = Copper;
    return;
  } else if ( mat.compareTo("uranium") == 0) {
    defaultMaterial = Uranium;
    return;
  } else if ( mat.compareTo("tungston") == 0) {
    defaultMaterial = Tungston;
    return;
  } else if ( mat.compareTo("alum") == 0) {
    defaultMaterial = Alum;
    return;
  } else if ( mat.compareTo("air") == 0) {
    defaultMaterial = Air;
    return;
  } else if ( mat.compareTo("scint") == 0) {
    defaultMaterial = ScintMaterial;
    return;
  } else if ( mat.compareTo("soilOne") == 0) {
    defaultMaterial = soilOne;
    return;
  } else if ( mat.compareTo("soilOne10W") == 0) {
    defaultMaterial = soilOne10W;
    return;
  } else if ( mat.compareTo("soilOne20W") == 0) {
    defaultMaterial = soilOne20W;
    return;
  } else if ( mat.compareTo("soilOne30W") == 0) {
    defaultMaterial = soilOne30W;
    return;
  } else {
    G4cout << "Material is not found!!! Check your list of available materials" << G4endl;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4RunManager.hh"

void pINTDetectorConstruction::UpdateGeometry()
{

  //G4RunManager::GetRunManager()->DefineWorldVolume(Construct());
    G4RunManager::GetRunManager()->InitializeGeometry();
  //  G4RunManager::GetRunManager()->ReinitializeGeometry();
  //  G4RunManager::GetRunManager()->SetUserAction(new pINTPrimaryGeneratorAction(this));
}

void pINTDetectorConstruction::split(const std::string &s, char delim, std::vector<std::string> &elems)
{
  std::stringstream ss;
  ss.str(s);
  std::string item;
  while (std::getline(ss, item, delim)) {
    elems.push_back(item);
  }
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
