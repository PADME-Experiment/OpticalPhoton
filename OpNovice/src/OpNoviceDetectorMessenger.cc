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
/// \file OpNovice/src/OpNoviceDetectorMessenger.cc
/// \brief Implementation of the OpNoviceDetectorMessenger class
//
//
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "OpNoviceDetectorMessenger.hh"

#include "OpNoviceDetectorConstruction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

OpNoviceDetectorMessenger::
  OpNoviceDetectorMessenger(OpNoviceDetectorConstruction* OpNoviceDetector)
  : G4UImessenger(),
    fOpNoviceDetector(OpNoviceDetector)
{
  fDetectorDir = new G4UIdirectory("/OpNovice/detector/");
  fDetectorDir->SetGuidance("Detector control commands");

  fDetectorModeCmd = new G4UIcmdWithAnInteger("/OpNovice/detector/mode",this);
  fDetectorModeCmd->SetGuidance("Choose detector setup mode");
  fDetectorModeCmd->SetGuidance("0: BGO crystal");
  fDetectorModeCmd->SetGuidance("1: PBF2 crystal");
  fDetectorModeCmd->SetParameterName("mode",true);
  fDetectorModeCmd->SetDefaultValue(0);
  fDetectorModeCmd->SetRange("mode == 0 || mode == 1");
  fDetectorModeCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  fBGOCryAbsLenCmd = new G4UIcmdWithADouble("/OpNovice/detector/bgo_crystal_abslen_scale",this);
  fBGOCryAbsLenCmd->SetGuidance("Set scale factor for absorption length of BGO crystal");
  fBGOCryAbsLenCmd->SetParameterName("fac",true);
  fBGOCryAbsLenCmd->SetRange("fac > 0.");
  fBGOCryAbsLenCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  fPbF2CryLenCmd = new G4UIcmdWithADoubleAndUnit("/OpNovice/detector/pbf2_crystal_length",this);
  fPbF2CryLenCmd->SetGuidance("Set length of PbF2 crystal");
  fPbF2CryLenCmd->SetParameterName("len",true);
  fPbF2CryLenCmd->SetUnitCategory("Length");
  fPbF2CryLenCmd->SetDefaultUnit("mm");
  fPbF2CryLenCmd->SetDefaultValue(140.0);
  fPbF2CryLenCmd->SetRange("len > 0. && len < 300.");
  fPbF2CryLenCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  fPbF2CryAbsLenCmd = new G4UIcmdWithADouble("/OpNovice/detector/pbf2_crystal_abslen_scale",this);
  fPbF2CryAbsLenCmd->SetGuidance("Set scale factor for absorption length of PbF2 crystal");
  fPbF2CryAbsLenCmd->SetParameterName("fac",true);
  fPbF2CryAbsLenCmd->SetRange("fac > 0.");
  fPbF2CryAbsLenCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  fCrystalSurfaceCmd = new G4UIcmdWithAnInteger("/OpNovice/detector/crystal_surface",this);
  fCrystalSurfaceCmd->SetGuidance("Choose surface model for crystal");
  fCrystalSurfaceCmd->SetGuidance("0: Open (default)");
  fCrystalSurfaceCmd->SetGuidance("1: Specular");
  fCrystalSurfaceCmd->SetGuidance("2: Diffuse");
  fCrystalSurfaceCmd->SetGuidance("3: Black");
  fCrystalSurfaceCmd->SetGuidance("4: Tedlar");
  fCrystalSurfaceCmd->SetGuidance("5: Millipore");
  fCrystalSurfaceCmd->SetGuidance("6: Open and polished");
  fCrystalSurfaceCmd->SetParameterName("surf",true);
  fCrystalSurfaceCmd->SetDefaultValue(0);
  fCrystalSurfaceCmd->SetRange("surf >= 0 && surf <= 6");
  fCrystalSurfaceCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

OpNoviceDetectorMessenger::~OpNoviceDetectorMessenger()
{
  delete fDetectorModeCmd;
  delete fBGOCryAbsLenCmd;
  delete fPbF2CryLenCmd;
  delete fPbF2CryAbsLenCmd;
  delete fCrystalSurfaceCmd;
  delete fDetectorDir;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void OpNoviceDetectorMessenger::SetNewValue( G4UIcommand* command, G4String newValue )
{

  if ( command == fDetectorModeCmd )
    fOpNoviceDetector->SetDetectorMode(fDetectorModeCmd->GetNewIntValue(newValue));

  if ( command == fBGOCryAbsLenCmd )
    fOpNoviceDetector->SetBGOCrystalAbsLength(fBGOCryAbsLenCmd->GetNewDoubleValue(newValue));

  if ( command == fPbF2CryLenCmd )
    fOpNoviceDetector->SetPbF2CrystalLength(fPbF2CryLenCmd->GetNewDoubleValue(newValue));

  if ( command == fPbF2CryAbsLenCmd )
    fOpNoviceDetector->SetPbF2CrystalAbsLength(fPbF2CryAbsLenCmd->GetNewDoubleValue(newValue));

  if ( command == fCrystalSurfaceCmd )
    fOpNoviceDetector->SetCrystalSurfaceModel(fCrystalSurfaceCmd->GetNewIntValue(newValue));

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
