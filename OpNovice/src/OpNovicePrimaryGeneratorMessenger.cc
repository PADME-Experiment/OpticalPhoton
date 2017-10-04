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
/// \file OpNovice/src/OpNovicePrimaryGeneratorMessenger.cc
/// \brief Implementation of the OpNovicePrimaryGeneratorMessenger class
//
//
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "OpNovicePrimaryGeneratorMessenger.hh"

#include "OpNovicePrimaryGeneratorAction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

OpNovicePrimaryGeneratorMessenger::
  OpNovicePrimaryGeneratorMessenger(OpNovicePrimaryGeneratorAction* OpNoviceGun)
  : G4UImessenger(),
    fOpNoviceAction(OpNoviceGun)
{
  fGunDir = new G4UIdirectory("/OpNovice/gun/");
  fGunDir->SetGuidance("PrimaryGenerator control");

  fGunModeCmd = new G4UIcmdWithAnInteger("/OpNovice/gun/mode",this);
  fGunModeCmd->SetGuidance("Choose gun functioning mode");
  fGunModeCmd->SetGuidance("0: generate optical photons with BGO scintillation energy/angle distribution");
  fGunModeCmd->SetGuidance("1: generate particle with fixed initial kinematics");
  fGunModeCmd->SetParameterName("mode",true);
  fGunModeCmd->SetDefaultValue(0);
  fGunModeCmd->SetRange("mode == 0 || mode == 1");
  fGunModeCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  fPolarCmd = new G4UIcmdWithADoubleAndUnit("/OpNovice/gun/optPhotonPolar",this);
  fPolarCmd->SetGuidance("Set linear polarization");
  fPolarCmd->SetGuidance("  angle w.r.t. (k,n) plane");
  fPolarCmd->SetParameterName("angle",true);
  fPolarCmd->SetUnitCategory("Angle");
  fPolarCmd->SetDefaultValue(-360.0);
  fPolarCmd->SetDefaultUnit("deg");
  fPolarCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  fOpPhotonPosCmd = new G4UIcmdWith3VectorAndUnit("/OpNovice/gun/optPhotonPos",this);
  fOpPhotonPosCmd->SetGuidance("Set coordinates of optical photons starting position");
  fOpPhotonPosCmd->SetParameterName("X","Y","Z",true);
  fOpPhotonPosCmd->SetUnitCategory("Length");
  fOpPhotonPosCmd->SetDefaultUnit("mm");
  fOpPhotonPosCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  fOpPhotonNrCmd = new G4UIcmdWithAnInteger("/OpNovice/gun/optPhotonNr",this);
  fOpPhotonNrCmd->SetGuidance("Define number of optical photons to generate at each event");
  fOpPhotonNrCmd->SetParameterName("n",true);
  fOpPhotonNrCmd->SetDefaultValue(100);
  fOpPhotonNrCmd->SetRange("n>0");
  fOpPhotonNrCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

OpNovicePrimaryGeneratorMessenger::~OpNovicePrimaryGeneratorMessenger()
{
  delete fPolarCmd;
  delete fOpPhotonPosCmd;
  delete fOpPhotonNrCmd;
  delete fGunModeCmd;
  delete fGunDir;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void OpNovicePrimaryGeneratorMessenger::SetNewValue(
                                        G4UIcommand* command, G4String newValue)
{

  if( command == fPolarCmd ) {
      G4double angle = fPolarCmd->GetNewDoubleValue(newValue);
      if ( angle == -360.0*deg ) {
         fOpNoviceAction->SetOptPhotonPolar();
      } else {
         fOpNoviceAction->SetOptPhotonPolar(angle);
      }
  }

  if ( command == fGunModeCmd )
    fOpNoviceAction->SetGunMode(fGunModeCmd->GetNewIntValue(newValue));

  if ( command == fOpPhotonPosCmd )
    fOpNoviceAction->SetOptPhotonPos(fOpPhotonPosCmd->GetNew3VectorValue(newValue));

  if ( command == fOpPhotonNrCmd )
    fOpNoviceAction->SetOptPhotonNr(fOpPhotonNrCmd->GetNewIntValue(newValue));

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
