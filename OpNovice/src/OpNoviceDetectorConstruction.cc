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
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

OpNoviceDetectorConstruction::OpNoviceDetectorConstruction()
 : G4VUserDetectorConstruction()
{

  fDetectorMessenger = new OpNoviceDetectorMessenger(this);

  // Default: PbF2 crystal with Epoxy on readout face
  fDetectorMode = 1;

  fWorld_x = fWorld_y = fWorld_z = 1.0*m;

  fBGOCrystal_x  = fBGOCrystal_y  = 21.*mm; fBGOCrystal_z  = 23.*cm;
  fBGOCrystal_abslen = 3.*m;

  fPbF2Crystal_x = fPbF2Crystal_y = 30.*mm; fPbF2Crystal_z = 14.*cm;
  fPbF2Crystal_abslen = 1.; // Default scale factor
  fPbF2Crystal_reflectivity = 0.95; // Default reflectivity factor

  fEpoxyRadius = 1.27*cm; // 1/2 inch
  fEpoxyThick = 1.*mm;

  fPaint = 100.*um; // Thickness of paint coating around crystal

  fCrystalVolume = 0;

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

  //Epoxy resin (CMS IN1999/026)
  G4Material * Epoxy = new G4Material("Epoxy",1.3*g/cm3,3);
  Epoxy->AddElement(G4Element::GetElement("C"),15);
  Epoxy->AddElement(G4Element::GetElement("H"),44);
  Epoxy->AddElement(G4Element::GetElement("O"),7);

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

  G4double bgoPhotonEnergy[] = {
      1.000*eV, 1.030*eV, 1.060*eV, 1.090*eV, 1.120*eV, 1.150*eV, 1.180*eV, 1.210*eV,
      1.240*eV, 1.270*eV, 1.300*eV, 1.330*eV, 1.360*eV, 1.390*eV, 1.420*eV, 1.450*eV,
      1.480*eV, 1.510*eV, 1.540*eV, 1.570*eV, 1.600*eV, 1.630*eV, 1.660*eV, 1.690*eV,
      1.720*eV, 1.750*eV, 1.780*eV, 1.810*eV, 1.840*eV, 1.870*eV, 1.900*eV, 1.930*eV,
      1.960*eV, 1.990*eV, 2.020*eV, 2.050*eV, 2.080*eV, 2.110*eV, 2.140*eV, 2.170*eV,
      2.200*eV, 2.230*eV, 2.260*eV, 2.290*eV, 2.320*eV, 2.350*eV, 2.380*eV, 2.410*eV,
      2.440*eV, 2.470*eV, 2.500*eV, 2.530*eV, 2.560*eV, 2.590*eV, 2.620*eV, 2.650*eV,
      2.680*eV, 2.710*eV, 2.740*eV, 2.770*eV, 2.800*eV, 2.830*eV, 2.860*eV, 2.890*eV,
      2.920*eV, 2.950*eV, 2.980*eV, 3.010*eV, 3.040*eV, 3.070*eV, 3.100*eV, 3.130*eV,
      3.160*eV, 3.190*eV, 3.220*eV, 3.250*eV, 3.280*eV, 3.310*eV, 3.340*eV, 3.370*eV,
      3.400*eV, 3.430*eV, 3.460*eV, 3.490*eV, 3.520*eV, 3.550*eV, 3.580*eV, 3.610*eV,
      3.640*eV, 3.670*eV, 3.700*eV, 3.730*eV, 3.760*eV, 3.790*eV, 3.820*eV, 3.850*eV,
      3.880*eV, 3.910*eV, 3.940*eV, 3.970*eV, 4.000*eV };

  const G4int bgoNEntries = sizeof(bgoPhotonEnergy)/sizeof(G4double);

  G4double bgoRefractiveIndex[] = {
      2.0468, 2.0479, 2.0489, 2.0500, 2.0512, 2.0523, 2.0535, 2.0548,
      2.0560, 2.0573, 2.0587, 2.0600, 2.0614, 2.0629, 2.0643, 2.0659,
      2.0674, 2.0690, 2.0706, 2.0723, 2.0740, 2.0757, 2.0775, 2.0793,
      2.0811, 2.0830, 2.0850, 2.0869, 2.0890, 2.0910, 2.0931, 2.0952,
      2.0974, 2.0997, 2.1019, 2.1042, 2.1066, 2.1090, 2.1115, 2.1140,
      2.1165, 2.1191, 2.1217, 2.1244, 2.1272, 2.1300, 2.1328, 2.1357,
      2.1386, 2.1416, 2.1447, 2.1478, 2.1510, 2.1542, 2.1575, 2.1608,
      2.1642, 2.1677, 2.1712, 2.1748, 2.1784, 2.1821, 2.1859, 2.1898,
      2.1937, 2.1977, 2.2017, 2.2059, 2.2101, 2.2143, 2.2187, 2.2231,
      2.2276, 2.2322, 2.2369, 2.2416, 2.2465, 2.2514, 2.2564, 2.2615,
      2.2667, 2.2720, 2.2774, 2.2828, 2.2884, 2.2941, 2.2999, 2.3058,
      2.3118, 2.3179, 2.3242, 2.3305, 2.3370, 2.3436, 2.3503, 2.3572,
      2.3642, 2.3713, 2.3786, 2.3860, 2.3935 };
  assert(sizeof(bgoRefractiveIndex) == sizeof(bgoPhotonEnergy));

  G4double bgoScintFastComp[] = {
      0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000,
      0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000,
      0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000,
      0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0976,
      0.3321, 0.4049, 0.4808, 0.5513, 0.6248, 0.6891, 0.7542, 0.8179,
      0.8839, 0.9535, 1.0208, 1.0841, 1.1543, 1.2160, 1.2513, 1.2762,
      1.3035, 1.3180, 1.3223, 1.3156, 1.2913, 1.2537, 1.2076, 1.1622,
      1.1081, 1.0399, 0.9665, 0.8878, 0.8098, 0.7325, 0.6579, 0.5816,
      0.5112, 0.4406, 0.3761, 0.3156, 0.2633, 0.2163, 0.1783, 0.1463,
      0.1163, 0.0915, 0.0680, 0.0415, 0.0171, 0.0004, 0.0000, 0.0000,
      0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000,
      0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000,
      0.0000, 0.0000, 0.0000, 0.0000, 0.0000 };
  assert(sizeof(bgoScintFastComp) == sizeof(bgoPhotonEnergy));

  // Need to find the right table
  G4double bgoAbsorption[] = {
      fBGOCrystal_abslen, fBGOCrystal_abslen, fBGOCrystal_abslen, fBGOCrystal_abslen,
      fBGOCrystal_abslen, fBGOCrystal_abslen, fBGOCrystal_abslen, fBGOCrystal_abslen,
      fBGOCrystal_abslen, fBGOCrystal_abslen, fBGOCrystal_abslen, fBGOCrystal_abslen,
      fBGOCrystal_abslen, fBGOCrystal_abslen, fBGOCrystal_abslen, fBGOCrystal_abslen,
      fBGOCrystal_abslen, fBGOCrystal_abslen, fBGOCrystal_abslen, fBGOCrystal_abslen,
      fBGOCrystal_abslen, fBGOCrystal_abslen, fBGOCrystal_abslen, fBGOCrystal_abslen,
      fBGOCrystal_abslen, fBGOCrystal_abslen, fBGOCrystal_abslen, fBGOCrystal_abslen,
      fBGOCrystal_abslen, fBGOCrystal_abslen, fBGOCrystal_abslen, fBGOCrystal_abslen,
      fBGOCrystal_abslen, fBGOCrystal_abslen, fBGOCrystal_abslen, fBGOCrystal_abslen,
      fBGOCrystal_abslen, fBGOCrystal_abslen, fBGOCrystal_abslen, fBGOCrystal_abslen,
      fBGOCrystal_abslen, fBGOCrystal_abslen, fBGOCrystal_abslen, fBGOCrystal_abslen,
      fBGOCrystal_abslen, fBGOCrystal_abslen, fBGOCrystal_abslen, fBGOCrystal_abslen,
      fBGOCrystal_abslen, fBGOCrystal_abslen, fBGOCrystal_abslen, fBGOCrystal_abslen,
      fBGOCrystal_abslen, fBGOCrystal_abslen, fBGOCrystal_abslen, fBGOCrystal_abslen,
      fBGOCrystal_abslen, fBGOCrystal_abslen, fBGOCrystal_abslen, fBGOCrystal_abslen,
      fBGOCrystal_abslen, fBGOCrystal_abslen, fBGOCrystal_abslen, fBGOCrystal_abslen,
      fBGOCrystal_abslen, fBGOCrystal_abslen, fBGOCrystal_abslen, fBGOCrystal_abslen,
      fBGOCrystal_abslen, fBGOCrystal_abslen, fBGOCrystal_abslen, fBGOCrystal_abslen,
      fBGOCrystal_abslen, fBGOCrystal_abslen, fBGOCrystal_abslen, fBGOCrystal_abslen,
      fBGOCrystal_abslen, fBGOCrystal_abslen, fBGOCrystal_abslen, fBGOCrystal_abslen,
      fBGOCrystal_abslen, fBGOCrystal_abslen, fBGOCrystal_abslen, fBGOCrystal_abslen,
      fBGOCrystal_abslen, fBGOCrystal_abslen, fBGOCrystal_abslen, fBGOCrystal_abslen,
      fBGOCrystal_abslen, fBGOCrystal_abslen, fBGOCrystal_abslen, fBGOCrystal_abslen,
      fBGOCrystal_abslen, fBGOCrystal_abslen, fBGOCrystal_abslen, fBGOCrystal_abslen,
      fBGOCrystal_abslen, fBGOCrystal_abslen, fBGOCrystal_abslen, fBGOCrystal_abslen,
      fBGOCrystal_abslen };
  assert(sizeof(bgoAbsorption) == sizeof(bgoPhotonEnergy));
  
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

  G4double pbf2PhotonEnergy[] = {
      1.60*eV, 1.62*eV, 1.64*eV, 1.66*eV, 1.68*eV, 1.70*eV, 1.72*eV, 1.74*eV, 1.76*eV, 1.78*eV,
      1.80*eV, 1.82*eV, 1.84*eV, 1.86*eV, 1.88*eV, 1.90*eV, 1.92*eV, 1.94*eV, 1.96*eV, 1.98*eV,
      2.00*eV, 2.02*eV, 2.04*eV, 2.06*eV, 2.08*eV, 2.10*eV, 2.12*eV, 2.14*eV, 2.16*eV, 2.18*eV,
      2.20*eV, 2.22*eV, 2.24*eV, 2.26*eV, 2.28*eV, 2.30*eV, 2.32*eV, 2.34*eV, 2.36*eV, 2.38*eV,
      2.40*eV, 2.42*eV, 2.44*eV, 2.46*eV, 2.48*eV, 2.50*eV, 2.52*eV, 2.54*eV, 2.56*eV, 2.58*eV,
      2.60*eV, 2.62*eV, 2.64*eV, 2.66*eV, 2.68*eV, 2.70*eV, 2.72*eV, 2.74*eV, 2.76*eV, 2.78*eV,
      2.80*eV, 2.82*eV, 2.84*eV, 2.86*eV, 2.88*eV, 2.90*eV, 2.92*eV, 2.94*eV, 2.96*eV, 2.98*eV,
      3.00*eV, 3.02*eV, 3.04*eV, 3.06*eV, 3.08*eV, 3.10*eV, 3.12*eV, 3.14*eV, 3.16*eV, 3.18*eV,
      3.20*eV, 3.22*eV, 3.24*eV, 3.26*eV, 3.28*eV, 3.30*eV, 3.32*eV, 3.34*eV, 3.36*eV, 3.38*eV,
      3.40*eV, 3.42*eV, 3.44*eV, 3.46*eV, 3.48*eV, 3.50*eV, 3.52*eV, 3.54*eV, 3.56*eV, 3.58*eV,
      3.60*eV, 3.62*eV, 3.64*eV, 3.66*eV, 3.68*eV, 3.70*eV, 3.72*eV, 3.74*eV, 3.76*eV, 3.78*eV,
      3.80*eV, 3.82*eV, 3.84*eV, 3.86*eV, 3.88*eV, 3.90*eV, 3.92*eV, 3.94*eV, 3.96*eV, 3.98*eV,
      4.00*eV, 4.02*eV, 4.04*eV, 4.06*eV, 4.08*eV, 4.10*eV, 4.12*eV, 4.14*eV, 4.16*eV, 4.18*eV,
      4.20*eV, 4.22*eV, 4.24*eV, 4.26*eV, 4.28*eV, 4.30*eV, 4.32*eV, 4.34*eV, 4.36*eV, 4.38*eV,
      4.40*eV, 4.42*eV, 4.44*eV, 4.46*eV, 4.48*eV, 4.50*eV, 4.52*eV, 4.54*eV, 4.56*eV, 4.58*eV,
      4.60*eV, 4.62*eV, 4.64*eV, 4.66*eV, 4.68*eV, 4.70*eV, 4.72*eV, 4.74*eV, 4.76*eV, 4.78*eV,
      4.80*eV, 4.82*eV, 4.84*eV, 4.86*eV, 4.88*eV, 4.90*eV, 4.92*eV, 4.94*eV, 4.96*eV, 4.98*eV,
      5.00*eV };
  const G4int pbf2NEntries = sizeof(pbf2PhotonEnergy)/sizeof(G4double);

  G4double pbf2RefractiveIndex[] = {
      1.7501, 1.7507, 1.7512, 1.7518, 1.7523, 1.7529, 1.7535, 1.7541, 1.7547, 1.7553,
      1.7559, 1.7565, 1.7572, 1.7578, 1.7584, 1.7591, 1.7598, 1.7604, 1.7611, 1.7618,
      1.7625, 1.7632, 1.7639, 1.7647, 1.7654, 1.7661, 1.7669, 1.7677, 1.7684, 1.7692,
      1.7700, 1.7708, 1.7716, 1.7724, 1.7733, 1.7741, 1.7750, 1.7758, 1.7767, 1.7776,
      1.7785, 1.7794, 1.7803, 1.7813, 1.7822, 1.7832, 1.7841, 1.7851, 1.7861, 1.7871,
      1.7881, 1.7891, 1.7902, 1.7912, 1.7923, 1.7934, 1.7945, 1.7956, 1.7967, 1.7978,
      1.7990, 1.8002, 1.8013, 1.8025, 1.8037, 1.8050, 1.8062, 1.8074, 1.8087, 1.8100,
      1.8113, 1.8126, 1.8140, 1.8153, 1.8167, 1.8181, 1.8195, 1.8209, 1.8223, 1.8238,
      1.8253, 1.8268, 1.8283, 1.8299, 1.8314, 1.8330, 1.8346, 1.8363, 1.8379, 1.8396,
      1.8413, 1.8430, 1.8448, 1.8466, 1.8484, 1.8502, 1.8521, 1.8540, 1.8559, 1.8579,
      1.8599, 1.8619, 1.8640, 1.8661, 1.8682, 1.8704, 1.8726, 1.8749, 1.8772, 1.8796,
      1.8820, 1.8845, 1.8871, 1.8897, 1.8924, 1.8951, 1.8979, 1.9009, 1.9039, 1.9071,
      1.9103, 2.0510, 2.0520, 2.0530, 2.0540, 2.0550, 2.0560, 2.0570, 2.0580, 2.0590,
      2.0600, 2.0620, 2.0640, 2.0660, 2.0680, 2.0700, 2.0720, 2.0740, 2.0760, 2.0780,
      2.0800, 2.0820, 2.0840, 2.0860, 2.0880, 2.0900, 2.0920, 2.0940, 2.0960, 2.0980,
      2.1000, 2.1030, 2.1060, 2.1090, 2.1120, 2.1150, 2.1180, 2.1210, 2.1240, 2.1270,
      2.1300, 2.1340, 2.1380, 2.1420, 2.1460, 2.1500, 2.1540, 2.1580, 2.1620, 2.1660,
      2.1700 };
  assert(sizeof(pbf2RefractiveIndex) == sizeof(pbf2PhotonEnergy));

  G4double pbf2Absorption[] = {
      12.98*cm, 12.98*cm, 12.98*cm, 12.89*cm, 12.89*cm, 12.89*cm, 12.71*cm, 12.71*cm, 12.53*cm, 12.53*cm,
      12.45*cm, 12.45*cm, 12.28*cm, 12.28*cm, 12.11*cm, 12.11*cm, 12.03*cm, 11.95*cm, 11.87*cm, 11.72*cm,
      11.71*cm, 11.63*cm, 11.63*cm, 11.48*cm, 11.48*cm, 11.41*cm, 11.26*cm, 11.26*cm, 11.12*cm, 11.05*cm,
      11.05*cm, 10.91*cm, 10.77*cm, 10.77*cm, 10.70*cm, 10.57*cm, 10.57*cm, 10.44*cm, 10.44*cm, 10.38*cm,
      10.26*cm, 10.13*cm, 10.07*cm, 10.07*cm,  9.96*cm,  9.84*cm,  9.78*cm,  9.96*cm,  9.90*cm,  9.84*cm,
       9.72*cm,  9.72*cm,  9.61*cm,  9.56*cm,  9.45*cm,  9.40*cm,  9.29*cm,  9.19*cm,  9.19*cm,  9.07*cm,
       9.03*cm,  8.94*cm,  8.89*cm,  8.79*cm,  8.70*cm,  8.65*cm,  8.56*cm,  8.47*cm,  8.42*cm,  8.34*cm,
       8.28*cm,  8.21*cm,  8.12*cm,  8.04*cm,  8.00*cm,  7.92*cm,  7.84*cm,  7.77*cm,  7.68*cm,  7.63*cm,
       7.57*cm,  7.47*cm,  7.43*cm,  7.35*cm,  7.26*cm,  7.21*cm,  7.14*cm,  7.04*cm,  6.98*cm,  6.92*cm,
       6.85*cm,  6.78*cm,  6.69*cm,  6.64*cm,  6.59*cm,  6.52*cm,  6.43*cm,  6.39*cm,  6.30*cm,  6.24*cm,
       6.15*cm,  6.12*cm,  6.05*cm,  5.99*cm,  5.92*cm,  5.84*cm,  5.77*cm,  5.71*cm,  5.66*cm,  5.60*cm,
       5.54*cm,  5.47*cm,  5.39*cm,  5.34*cm,  5.28*cm,  5.23*cm,  5.15*cm,  5.08*cm,  5.03*cm,  5.01*cm,
       4.91*cm,  4.87*cm,  4.81*cm,  4.74*cm,  4.68*cm,  4.62*cm,  4.56*cm,  4.49*cm,  4.42*cm,  4.37*cm,
       4.29*cm,  4.26*cm,  4.18*cm,  4.10*cm,  4.04*cm,  3.97*cm,  3.91*cm,  3.84*cm,  3.74*cm,  3.68*cm,
       3.61*cm,  3.52*cm,  3.44*cm,  3.37*cm,  3.30*cm,  3.19*cm,  3.09*cm,  3.02*cm,  2.93*cm,  2.81*cm,
       2.75*cm,  2.60*cm,  2.47*cm,  2.38*cm,  2.26*cm,  2.13*cm,  2.04*cm,  1.83*cm,  1.69*cm,  1.60*cm,
       1.42*cm,  1.27*cm,  1.18*cm,  1.02*cm,  0.88*cm,  0.81*cm,  0.74*cm,  0.64*cm,  0.54*cm,  0.44*cm,
       0.34*cm };
  assert(sizeof(pbf2Absorption) == sizeof(pbf2PhotonEnergy));

  // Apply overall scale to absorption length spectrum
  for(G4int i=0; i<pbf2NEntries; i++) {
    pbf2Absorption[i] *= fPbF2Crystal_abslen;
  }

  G4MaterialPropertiesTable* pbf2MPT = new G4MaterialPropertiesTable();
  pbf2MPT->AddProperty("RINDEX",pbf2PhotonEnergy,pbf2RefractiveIndex,pbf2NEntries)->SetSpline(true);
  pbf2MPT->AddProperty("ABSLENGTH",pbf2PhotonEnergy,pbf2Absorption,pbf2NEntries)->SetSpline(true);

  //G4cout << ">>> PbF2 G4MaterialPropertiesTable <<<" << G4endl;
  //pbf2MPT->DumpTable();

  PbF2->SetMaterialPropertiesTable(pbf2MPT);

  //
  // Epoxy EJ-500
  //
  const G4int epoxyNEntries = 2;
  G4double epoxyPhotonEnergy[] = { 1.60*eV, 5.00*eV };
  G4double epoxyRefractiveIndex[] = { 1.57, 1.57 };
  G4MaterialPropertiesTable* epoxyMPT = new G4MaterialPropertiesTable();
  epoxyMPT->AddProperty("RINDEX",epoxyPhotonEnergy,epoxyRefractiveIndex,epoxyNEntries)->SetSpline(true);

  //G4cout << ">>> Epoxy G4MaterialPropertiesTable <<<" << G4endl;
  //epoxyMPT->DumpTable();

  Epoxy->SetMaterialPropertiesTable(epoxyMPT);

  //
  // Air
  //
  G4double airPhotonEnergy[] = { 1.0*eV, 5.0*eV };
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

  // The Epoxy resin cylinder
  G4Tubs* resin_tubs = new G4Tubs("Epoxy_Resin",0.,fEpoxyRadius,0.5*fEpoxyThick,0.*deg,360.*deg);
  G4LogicalVolume* resin_log = new G4LogicalVolume(resin_tubs,Epoxy,"Epoxy_Resin",0,0,0);

  if ( fDetectorMode == 0 ) {

    G4cout << G4endl << ">>> Using BGO crystal with size " << fBGOCrystal_x/mm << "x" << fBGOCrystal_y/mm << "x" << fBGOCrystal_z/mm << " mm <<<" << G4endl << G4endl;
    //G4VPhysicalVolume* crystal_phys = 
    new G4PVPlacement(0,G4ThreeVector(),bgo_crystal_log,"BGO_Crystal",world_log,false,0);
    fCrystalVolume = bgo_crystal_log;

  } else {

    G4cout << G4endl << ">>> Using PbF2 crystal with size " << fPbF2Crystal_x/mm << "x" << fPbF2Crystal_y/mm << "x" << fPbF2Crystal_z/mm << " mm3 <<<" << G4endl << G4endl;
    //G4VPhysicalVolume* crystal_phys =
    new G4PVPlacement(0,G4ThreeVector(),pbf2_crystal_log,"PbF2_Crystal",world_log,false,0);
    //G4VPhysicalVolume* epoxy_phys =
    new G4PVPlacement(0,G4ThreeVector(0.,0.,0.5*(fPbF2Crystal_z+fEpoxyThick)),resin_log,"Epoxy_Resin",world_log,false,0);
    fCrystalVolume = pbf2_crystal_log;

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
  G4double bgo_ephoton[bgo_num] = {1.*eV, 4.*eV};
  //G4double bgo_reflectivity[bgo_num] = {0.9, 0.9};
  G4double bgo_reflectivity[bgo_num] = {0.95, 0.95};
  G4double bgo_efficiency[bgo_num]   = {0.,0.};

  G4MaterialPropertiesTable* bgoST = new G4MaterialPropertiesTable();
  bgoST->AddProperty("REFLECTIVITY",bgo_ephoton,bgo_reflectivity,bgo_num);
  bgoST->AddProperty("EFFICIENCY",bgo_ephoton,bgo_efficiency,bgo_num);

  //G4cout << ">>> BGO Surface G4MaterialPropertiesTable <<<" << G4endl;
  //bgoST->DumpTable();

  opBGOSurface->SetMaterialPropertiesTable(bgoST);

  //G4LogicalSkinSurface* bgoSurface = 
  new G4LogicalSkinSurface("BGOSurface",bgo_crystal_log,opBGOSurface);

  //
  // PbF2
  //
  /*
  G4OpticalSurface* opPbF2Surface =  new G4OpticalSurface("PbF2Surface");
  //opPbF2Surface->SetType(dielectric_metal);
  opPbF2Surface->SetType(dielectric_dielectric);
  opPbF2Surface->SetModel(glisur);
  opPbF2Surface->SetFinish(polished);

  const G4int pbf2_num = 2;
  G4double pbf2_ephoton[pbf2_num] = {1.6*eV, 4.*eV};
  G4double pbf2_reflectivity[pbf2_num] = {fPbF2Crystal_reflectivity,fPbF2Crystal_reflectivity};
  G4double pbf2_efficiency[pbf2_num]   = {0.,0.};

  G4MaterialPropertiesTable* pbf2ST = new G4MaterialPropertiesTable();
  pbf2ST->AddProperty("REFLECTIVITY",pbf2_ephoton,pbf2_reflectivity,pbf2_num);
  pbf2ST->AddProperty("EFFICIENCY",pbf2_ephoton,pbf2_efficiency,pbf2_num);

  //G4cout << ">>> PbF2 Surface G4MaterialPropertiesTable <<<" << G4endl;
  //pbf2ST->DumpTable();

  opPbF2Surface->SetMaterialPropertiesTable(pbf2ST);

  G4LogicalSkinSurface* pbf2Surface = new G4LogicalSkinSurface("PbF2Surface",pbf2_crystal_log,opPbF2Surface);
  */

  //always return the physical World
  return world_phys;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double OpNoviceDetectorConstruction::GetCrystalLength()
{
  if ( fDetectorMode == 0 ) return fBGOCrystal_z;
  return fPbF2Crystal_z;
}
