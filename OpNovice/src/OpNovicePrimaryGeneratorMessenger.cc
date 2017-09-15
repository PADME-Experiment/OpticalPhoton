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
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

OpNovicePrimaryGeneratorMessenger::
  OpNovicePrimaryGeneratorMessenger(OpNovicePrimaryGeneratorAction* OpNoviceGun)
  : G4UImessenger(),
    fOpNoviceAction(OpNoviceGun)
{
  fGunDir = new G4UIdirectory("/OpNovice/gun/");
  fGunDir->SetGuidance("PrimaryGenerator control");

  fPolarCmd = new G4UIcmdWithADoubleAndUnit("/OpNovice/gun/optPhotonPolar",this);
  fPolarCmd->SetGuidance("Set linear polarization");
  fPolarCmd->SetGuidance("  angle w.r.t. (k,n) plane");
  fPolarCmd->SetParameterName("angle",true);
  fPolarCmd->SetUnitCategory("Angle");
  fPolarCmd->SetDefaultValue(-360.0);
  fPolarCmd->SetDefaultUnit("deg");
  fPolarCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  fPosXCmd = new G4UIcmdWithADoubleAndUnit("/OpNovice/gun/optPhotonPosX",this);
  fPosXCmd->SetGuidance("Set X coordinate of optical photons starting position");
  fPosXCmd->SetParameterName("posx",true);
  fPosXCmd->SetUnitCategory("Length");
  fPosXCmd->SetDefaultUnit("mm");
  fPosXCmd->SetDefaultValue(0.0);
  fPosXCmd->SetRange("posx > -10.5 && posx < 10.5");
  fPosXCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  fPosYCmd = new G4UIcmdWithADoubleAndUnit("/OpNovice/gun/optPhotonPosY",this);
  fPosYCmd->SetGuidance("Set Y coordinate of optical photons starting position");
  fPosYCmd->SetParameterName("posy",true);
  fPosYCmd->SetUnitCategory("Length");
  fPosYCmd->SetDefaultUnit("mm");
  fPosYCmd->SetDefaultValue(0.0);
  fPosYCmd->SetRange("posy > -10.5 && posy < 10.5");
  fPosYCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  fPosZCmd = new G4UIcmdWithADoubleAndUnit("/OpNovice/gun/optPhotonPosZ",this);
  fPosZCmd->SetGuidance("Set Z coordinate of optical photons starting position");
  fPosZCmd->SetParameterName("posz",true);
  fPosZCmd->SetUnitCategory("Length");
  fPosZCmd->SetDefaultUnit("mm");
  fPosZCmd->SetDefaultValue(0.0);
  fPosZCmd->SetRange("posz > -115. && posz < 115.");
  fPosZCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  fGunModeCmd = new G4UIcmdWithAnInteger("/OpNovice/gun/mode",this);
  fGunModeCmd->SetGuidance("Choose gun functioning mode");
  fGunModeCmd->SetGuidance("0: generate optical photon with BGO scintillation energy/angle distribution");
  fGunModeCmd->SetGuidance("1: generate particle with fixed initial kinematics");
  fGunModeCmd->SetParameterName("mode",true);
  fGunModeCmd->SetDefaultValue(0);
  fGunModeCmd->SetRange("mode == 0 || mode == 1");
  fGunModeCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

OpNovicePrimaryGeneratorMessenger::~OpNovicePrimaryGeneratorMessenger()
{
  delete fPolarCmd;
  delete fPosXCmd;
  delete fPosYCmd;
  delete fPosZCmd;
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

  if ( command == fPosXCmd ) fOpNoviceAction->SetOptPhotonPosX(fPosXCmd->GetNewDoubleValue(newValue));
  if ( command == fPosYCmd ) fOpNoviceAction->SetOptPhotonPosY(fPosYCmd->GetNewDoubleValue(newValue));
  if ( command == fPosZCmd ) fOpNoviceAction->SetOptPhotonPosZ(fPosZCmd->GetNewDoubleValue(newValue));

  if ( command == fGunModeCmd ) fOpNoviceAction->SetGunMode(fGunModeCmd->GetNewIntValue(newValue));

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
