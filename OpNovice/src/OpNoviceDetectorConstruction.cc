//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
/// \file OpNovice/src/OpNoviceDetectorConstruction.cc
/// \brief Implementation of the OpNoviceDetectorConstruction class
//
//
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "OpNoviceDetectorConstruction.hh"
#include "OpNoviceDetectorMessenger.hh"

#include "G4NistManager.hh"
#include "G4Material.hh"
#include "G4Element.hh"
#include "G4VisAttributes.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4OpticalSurface.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

OpNoviceDetectorConstruction::OpNoviceDetectorConstruction()
 : G4VUserDetectorConstruction()
{

  fDetectorMessenger = new OpNoviceDetectorMessenger(this);

  fDetectorMode = 0;

  fWorld_x = fWorld_y = fWorld_z = 1.0*m;

  fBGOCrystal_x  = fBGOCrystal_y  = 21.*mm; fBGOCrystal_z  = 23.*cm;
  fPbF2Crystal_x = fPbF2Crystal_y = 30.*mm; fPbF2Crystal_z = 14.*cm;

  fPaint = 100.*um; // Thickness of paint coating around crystal

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

OpNoviceDetectorConstruction::~OpNoviceDetectorConstruction(){;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* OpNoviceDetectorConstruction::Construct()
{

  // ------------- Materials -------------

  G4NistManager* man = G4NistManager::Instance();

  // Standard air from NIST DB
  G4Material* Air = man->FindOrBuildMaterial("G4_AIR");

  // Standard BGO from NIST DB
  G4Material* BGO = man->FindOrBuildMaterial("G4_BGO");

  // EJ510 reflective paint
  man->FindOrBuildElement("Ti");
  man->FindOrBuildElement("C");
  man->FindOrBuildElement("H");
  man->FindOrBuildElement("O");
  G4Material* EJ510Paint = new G4Material("EJ510Paint",1.182*g/cm3,4);
  EJ510Paint->AddElement(G4Element::GetElement("Ti"),41.08*perCent);
  EJ510Paint->AddElement(G4Element::GetElement("C"), 17.18*perCent);
  EJ510Paint->AddElement(G4Element::GetElement("H"),  2.88*perCent);
  EJ510Paint->AddElement(G4Element::GetElement("O"), 38.86*perCent);

  // Lead fluoride PbF2
  man->FindOrBuildElement("Pb");
  man->FindOrBuildElement("F");
  G4Material* PbF2 = new G4Material("PbF2",7.77*g/cm3,2);
  PbF2->AddElement(G4Element::GetElement("Pb"),84.5*perCent);
  PbF2->AddElement(G4Element::GetElement("F"), 15.5*perCent);

  //Print all the materials defined.
  //G4cout << G4endl << "The elements defined are : " << G4endl << G4endl;
  //G4cout << *(G4Element::GetElementTable()) << G4endl;
  //G4cout << G4endl << "The materials defined are : " << G4endl << G4endl;
  //G4cout << *(G4Material::GetMaterialTable()) << G4endl;

//
// ------------ Generate & Add Material Properties Table ------------
//


  //
  // BGO
  //

  G4double bgoPhotonEnergy[] =
    { 2.034*eV, 2.068*eV, 2.103*eV, 2.139*eV,
      2.177*eV, 2.216*eV, 2.256*eV, 2.298*eV,
      2.341*eV, 2.386*eV, 2.433*eV, 2.481*eV,
      2.532*eV, 2.585*eV, 2.640*eV, 2.697*eV,
      2.757*eV, 2.820*eV, 2.885*eV, 2.954*eV,
      3.026*eV, 3.102*eV, 3.181*eV, 3.265*eV,
      3.353*eV, 3.446*eV, 3.545*eV, 3.649*eV,
      3.760*eV, 3.877*eV, 4.002*eV, 4.136*eV };

  const G4int bgoNEntries = sizeof(bgoPhotonEnergy)/sizeof(G4double);

  G4double bgoRefractiveIndex[] =
    { 2.1030, 2.1056, 2.1084, 2.1114,
      2.1145, 2.1179, 2.1214, 2.1251,
      2.1291, 2.1334, 2.1379, 2.1428,
      2.1480, 2.1537, 2.1597, 2.1662,
      2.1732, 2.1809, 2.1891, 2.1982,
      2.2081, 2.2190, 2.2308, 2.2440,
      2.2586, 2.2748, 2.2932, 2.3136,
      2.3370, 2.3635, 2.3940, 2.4297 };
  assert(sizeof(bgoRefractiveIndex) == sizeof(bgoPhotonEnergy));

  // Need to find the right table
  G4double bgoAbsorption[] =
    { 3.*m, 3.*m, 3.*m, 3.*m, 3.*m, 3.*m, 3.*m, 3.*m,
      3.*m, 3.*m, 3.*m, 3.*m, 3.*m, 3.*m, 3.*m, 3.*m,
      3.*m, 3.*m, 3.*m, 3.*m, 3.*m, 3.*m, 3.*m, 3.*m,
      3.*m, 3.*m, 3.*m, 3.*m, 3.*m, 3.*m, 3.*m, 3.*m };
  assert(sizeof(bgoAbsorption) == sizeof(bgoPhotonEnergy));

  G4double bgoScintFastComp[] = 
    { 0.23, 0.29, 0.33, 0.39, 0.45, 0.52, 0.60, 0.67,
      0.75, 0.84, 0.89, 0.95, 0.99, 1.00, 0.97, 0.93,
      0.85, 0.74, 0.60, 0.48, 0.33, 0.22, 0.13, 0.04,
      0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00 };
  assert(sizeof(bgoScintFastComp) == sizeof(bgoPhotonEnergy));
  
  G4MaterialPropertiesTable* bgoMPT = new G4MaterialPropertiesTable();
  bgoMPT->AddProperty("RINDEX",bgoPhotonEnergy,bgoRefractiveIndex,bgoNEntries)->SetSpline(true);
  bgoMPT->AddProperty("ABSLENGTH",bgoPhotonEnergy,bgoAbsorption,bgoNEntries)->SetSpline(true);
  bgoMPT->AddProperty("FASTCOMPONENT",bgoPhotonEnergy, bgoScintFastComp, bgoNEntries)->SetSpline(true);
  bgoMPT->AddConstProperty("SCINTILLATIONYIELD",8000./MeV);
  bgoMPT->AddConstProperty("RESOLUTIONSCALE",1.0);
  bgoMPT->AddConstProperty("FASTTIMECONSTANT",300.*ns);

  //G4cout << ">>> BGO G4MaterialPropertiesTable <<<" << G4endl;
  //bgoMPT->DumpTable();

  BGO->SetMaterialPropertiesTable(bgoMPT);

  //
  // PbF2
  //

  G4double pbf2PhotonEnergy[] = { 1.550*eV, 1.771*eV, 2.066*eV, 2.480*eV, 3.100*eV, 4.133*eV };
  const G4int pbf2NEntries = sizeof(pbf2PhotonEnergy)/sizeof(G4double);
  G4double pbf2RefractiveIndex[] = { 1.749, 1.755, 1.765, 1.782, 1.818, 1.937 };
  assert(sizeof(pbf2RefractiveIndex) == sizeof(pbf2PhotonEnergy));
  G4double pbf2Absorption[] = { 3.*m, 3.*m, 3.*m, 3.*m, 3.*m, 3.*m };
  assert(sizeof(pbf2Absorption) == sizeof(pbf2PhotonEnergy));

  G4MaterialPropertiesTable* pbf2MPT = new G4MaterialPropertiesTable();
  pbf2MPT->AddProperty("RINDEX",pbf2PhotonEnergy,pbf2RefractiveIndex,pbf2NEntries)->SetSpline(true);
  pbf2MPT->AddProperty("ABSLENGTH",pbf2PhotonEnergy,pbf2Absorption,pbf2NEntries)->SetSpline(true);

  //G4cout << ">>> PbF2 G4MaterialPropertiesTable <<<" << G4endl;
  //pbf2MPT->DumpTable();

  PbF2->SetMaterialPropertiesTable(pbf2MPT);

  //
  // Air
  //

  G4double airPhotonEnergy[] = { 1.550*eV, 4.136*eV };
  const G4int airNEntries = sizeof(airPhotonEnergy)/sizeof(G4double);
  G4double airRefractiveIndex[] = { 1.00, 1.00 };
  G4MaterialPropertiesTable* airMPT = new G4MaterialPropertiesTable();
  airMPT->AddProperty("RINDEX", airPhotonEnergy, airRefractiveIndex, airNEntries);

  //G4cout << ">>> Air G4MaterialPropertiesTable <<<" << G4endl;
  //airMPT->DumpTable();

  Air->SetMaterialPropertiesTable(airMPT);

//
// ------------- Volumes --------------

// The experimental Hall
//
  G4Box* world_box = new G4Box("World",0.5*fWorld_x,0.5*fWorld_y,0.5*fWorld_z);
  G4LogicalVolume* world_log = new G4LogicalVolume(world_box,Air,"World",0,0,0);
  world_log->SetVisAttributes(G4VisAttributes::Invisible);
  G4VPhysicalVolume* world_phys = new G4PVPlacement(0,G4ThreeVector(),world_log,"World",0,false,0);

// The paint coating outside BGO
  //G4Box* paint_box = new G4Box("Paint",0.5*(fCrystal_x+2.*fPaint),0.5*(fCrystal_y+2.*fPaint),0.5*(fCrystal_z+2.*fPaint));
  //G4LogicalVolume* paint_log = new G4LogicalVolume(paint_box,EJ510Paint,"Paint",0,0,0);
  //G4VPhysicalVolume* paint_phys = new G4PVPlacement(0,G4ThreeVector(),paint_log,"Paint",world_log,false,0);

// The BGO crystal
  G4Box* bgo_crystal_box = new G4Box("BGO_Crystal",0.5*fBGOCrystal_x,0.5*fBGOCrystal_y,0.5*fBGOCrystal_z);
  G4LogicalVolume* bgo_crystal_log = new G4LogicalVolume(bgo_crystal_box,BGO,"BGO_Crystal",0,0,0);
  //G4VPhysicalVolume* crystal_phys = new G4PVPlacement(0,G4ThreeVector(),crystal_log,"Crystal",paint_log,false,0);

// The PbF2 crystal
  G4Box* pbf2_crystal_box = new G4Box("PbF2_Crystal",0.5*fPbF2Crystal_x,0.5*fPbF2Crystal_y,0.5*fPbF2Crystal_z);
  G4LogicalVolume* pbf2_crystal_log = new G4LogicalVolume(pbf2_crystal_box,PbF2,"PbF2_Crystal",0,0,0);

  if ( fDetectorMode == 0 ) {

    G4cout << G4endl << ">>> Using BGO crystal with size " << fBGOCrystal_x/mm << "x" << fBGOCrystal_y/mm << "x" << fBGOCrystal_z/mm << " mm <<<" << G4endl << G4endl;
    G4VPhysicalVolume* crystal_phys = new G4PVPlacement(0,G4ThreeVector(),bgo_crystal_log,"BGO_Crystal",world_log,false,0);

  } else {

    G4cout << G4endl << ">>> Using PbF2 crystal with size " << fPbF2Crystal_x/mm << "x" << fPbF2Crystal_y/mm << "x" << fPbF2Crystal_z/mm << " mm <<<" << G4endl << G4endl;
    G4VPhysicalVolume* crystal_phys = new G4PVPlacement(0,G4ThreeVector(),pbf2_crystal_log,"PbF2_Crystal",world_log,false,0);

  }

// ------------- Surfaces --------------

  //
  // BGO
  //

  G4OpticalSurface* opBGOSurface =  new G4OpticalSurface("BGOSurface");
  // BGO surface with TiO painting
  //opBGOSurface->SetType(dielectric_LUT);
  //opBGOSurface->SetModel(LUT);
  //opBGOSurface->SetFinish(polishedtioair);
  // BGO surface with mirror
  opBGOSurface->SetType(dielectric_metal);
  opBGOSurface->SetModel(glisur);
  opBGOSurface->SetFinish(polished);

  const G4int bgo_num = 2;
  G4double bgo_ephoton[bgo_num] = {2.034*eV, 4.136*eV};
  //G4double bgo_reflectivity[bgo_num] = {0.9, 0.9};
  G4double bgo_reflectivity[bgo_num] = {0.95, 0.95};
  G4double bgo_efficiency[bgo_num]   = {0.,0.};

  G4MaterialPropertiesTable* bgoST = new G4MaterialPropertiesTable();
  bgoST->AddProperty("REFLECTIVITY",bgo_ephoton,bgo_reflectivity,bgo_num);
  bgoST->AddProperty("EFFICIENCY",bgo_ephoton,bgo_efficiency,bgo_num);

  //G4cout << ">>> BGO Surface G4MaterialPropertiesTable <<<" << G4endl;
  //bgoST->DumpTable();

  opBGOSurface->SetMaterialPropertiesTable(bgoST);

  G4LogicalSkinSurface* bgoSurface = new G4LogicalSkinSurface("BGOSurface",bgo_crystal_log,opBGOSurface);

  //
  // PbF2
  //

  G4OpticalSurface* opPbF2Surface =  new G4OpticalSurface("PbF2Surface");
  opPbF2Surface->SetType(dielectric_metal);
  opPbF2Surface->SetModel(glisur);
  opPbF2Surface->SetFinish(polished);

  const G4int pbf2_num = 2;
  G4double pbf2_ephoton[pbf2_num] = {2.034*eV, 4.136*eV};
  G4double pbf2_reflectivity[pbf2_num] = {0.95, 0.95};
  G4double pbf2_efficiency[pbf2_num]   = {0.,0.};

  G4MaterialPropertiesTable* pbf2ST = new G4MaterialPropertiesTable();
  pbf2ST->AddProperty("REFLECTIVITY",pbf2_ephoton,pbf2_reflectivity,pbf2_num);
  pbf2ST->AddProperty("EFFICIENCY",pbf2_ephoton,pbf2_efficiency,pbf2_num);

  //G4cout << ">>> PbF2 Surface G4MaterialPropertiesTable <<<" << G4endl;
  //pbf2ST->DumpTable();

  opPbF2Surface->SetMaterialPropertiesTable(pbf2ST);

  G4LogicalSkinSurface* pbf2Surface = new G4LogicalSkinSurface("PbF2Surface",pbf2_crystal_log,opPbF2Surface);


  //always return the physical World
  return world_phys;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double OpNoviceDetectorConstruction::GetCrystalLength()
{
  if ( fDetectorMode == 0 ) return fBGOCrystal_z;
  return fPbF2Crystal_z;
}
