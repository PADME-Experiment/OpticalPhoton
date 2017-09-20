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

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class OpNoviceDetectorConstruction : public G4VUserDetectorConstruction
{

public:

  OpNoviceDetectorConstruction();
  virtual ~OpNoviceDetectorConstruction();

public:

  virtual G4VPhysicalVolume* Construct();
  void SetDetectorMode(G4int m) { fDetectorMode = m; }

  void SetBGOCrystalAbsLength(G4double l) { fBGOCrystal_abslen = l; }

  void SetPbF2CrystalLength(G4double l) { fPbF2Crystal_z = l; }
  void SetPbF2CrystalAbsLength(G4double l) { fPbF2Crystal_abslen = l; }
  void SetPbF2CrystalReflectivity(G4double r) { fPbF2Crystal_reflectivity = r; }

  G4double GetCrystalLength();

private:

  OpNoviceDetectorMessenger* fDetectorMessenger;

  G4int fDetectorMode;

  G4double fWorld_x;
  G4double fWorld_y;
  G4double fWorld_z;

  G4double fBGOCrystal_x;
  G4double fBGOCrystal_y;
  G4double fBGOCrystal_z;

  G4double fBGOCrystal_abslen;

  G4double fPbF2Crystal_x;
  G4double fPbF2Crystal_y;
  G4double fPbF2Crystal_z;
  
  G4double fPbF2Crystal_abslen; // Scale factor for absorption length spectrum
  G4double fPbF2Crystal_reflectivity; // Reflectivity factor

  G4double fPaint;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif /*OpNoviceDetectorConstruction_h*/
