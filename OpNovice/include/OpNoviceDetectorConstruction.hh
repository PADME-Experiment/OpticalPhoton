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
/// \file OpNovice/include/OpNoviceDetectorConstruction.hh
/// \brief Definition of the OpNoviceDetectorConstruction class
//
//
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef OpNoviceDetectorConstruction_h
#define OpNoviceDetectorConstruction_h 1

#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"

class OpNoviceDetectorMessenger;
class G4OpticalSurface;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class OpNoviceDetectorConstruction : public G4VUserDetectorConstruction
{

public:

  OpNoviceDetectorConstruction();
  virtual ~OpNoviceDetectorConstruction();

public:

  virtual G4VPhysicalVolume* Construct();
  void SetDetectorMode(G4int m) { fDetectorMode = m; }

  void SetCrystalSurfaceModel(G4int m) { fCrystalSurfaceModel = m; }
  G4int GetCrystalSurfaceModel() { return fCrystalSurfaceModel; }

  void SetBGOCrystalAbsLength(G4double l) { fBGOCrystal_abslen = l; }

  void SetPbF2CrystalLength(G4double l) { fPbF2Crystal_z = l; }
  void SetPbF2CrystalAbsLength(G4double l) { fPbF2Crystal_abslen = l; }

  G4double GetCrystalLength();
  G4double GetPMTRadius();

  G4LogicalVolume* GetCrystalVolume() { return fCrystalVolume; }

  // Optical surfaces

  G4OpticalSurface* PolishedMetal();
  G4OpticalSurface* PolishedMetalReverse();
  G4OpticalSurface* Specular();
  G4OpticalSurface* SpecularReverse();
  G4OpticalSurface* Diffuse();
  G4OpticalSurface* DiffuseReverse();
  G4OpticalSurface* Black();
  G4OpticalSurface* BlackReverse();
  G4OpticalSurface* Tedlar();
  G4OpticalSurface* TedlarReverse();
  G4OpticalSurface* Millipore();
  G4OpticalSurface* MilliporeReverse();
  G4OpticalSurface* Open();
  G4OpticalSurface* OpenReverse();

private:

  OpNoviceDetectorMessenger* fDetectorMessenger;

  G4LogicalVolume* fCrystalVolume;

  G4int fDetectorMode;

  G4double fWorld_x;
  G4double fWorld_y;
  G4double fWorld_z;

  G4double fBGOCrystal_x;
  G4double fBGOCrystal_y;
  G4double fBGOCrystal_z;

  G4double fBGOCrystal_abslen; // Scale factor for absorption length spectrum

  G4double fBGOEpoxyRadius;
  G4double fBGOEpoxyThick;

  G4double fPbF2Crystal_x;
  G4double fPbF2Crystal_y;
  G4double fPbF2Crystal_z;
  
  G4double fPbF2Crystal_abslen; // Scale factor for absorption length spectrum

  G4double fPbF2EpoxyRadius;
  G4double fPbF2EpoxyThick;

  G4double fPaint;

  G4int fCrystalSurfaceModel; // Optical model to use for crystal surface
                              // 0: Open, 1: Specular, 2: Diffuse, 3: Black, 4: Tedlar, 5: Millipore

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif /*OpNoviceDetectorConstruction_h*/
