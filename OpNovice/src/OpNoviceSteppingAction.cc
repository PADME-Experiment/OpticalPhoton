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
// $Id: OpNoviceSteppingAction.cc 71007 2013-06-09 16:14:59Z maire $
//
/// \file OpNoviceSteppingAction.cc
/// \brief Implementation of the OpNoviceSteppingAction class

#include "OpNoviceSteppingAction.hh"

#include "OpNoviceEventAction.hh"
#include "OpNoviceDetectorConstruction.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4OpticalPhoton.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

OpNoviceSteppingAction::OpNoviceSteppingAction(OpNoviceEventAction* eventAction)
  : G4UserSteppingAction(),
    fEventAction(eventAction)
{ 

  fPMTPhotonCounter = 0;
  fEventNumber = -1;

  fDetectorConstruction = (OpNoviceDetectorConstruction*)G4RunManager::GetRunManager()->GetUserDetectorConstruction();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

OpNoviceSteppingAction::~OpNoviceSteppingAction()
{ ; }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void OpNoviceSteppingAction::UserSteppingAction(const G4Step* step)
{
  G4int eventNumber = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();

  if (eventNumber != fEventNumber) {
    fEventNumber = eventNumber;
    fPMTPhotonCounter = 0;
  }

  G4Track* track = step->GetTrack();

  G4String ParticleName = track->GetDynamicParticle()->GetParticleDefinition()->GetParticleName();

  if (ParticleName == "opticalphoton") {

    G4ThreeVector p = step->GetPostStepPoint()->GetPosition();

    // Check if step ended on the crystal's back surface
    if ( p.z() == 0.5*fDetectorConstruction->GetCrystalLength() ) {

      // Check if step ended inside PMT radius
      G4double g_r = sqrt(p.x()*p.x()+p.y()*p.y());
      if ( g_r < fDetectorConstruction->GetPMTRadius() ) {

	// This is the momentum after the boundary effect
	// If the Z component is >0 the photon was transmitted inside the Epoxy
	G4ThreeVector g_md = track->GetMomentumDirection();
	//printf("Momentum direction %.3f %.3f %.3f\n",g_md.x(),g_md.y(),g_md.z());
	if (g_md.z()>0.) {

	  // Count photon
	  fPMTPhotonCounter++;
	  fEventAction->CountPMTPhoton();

	  // Show properties of photon entering EPoxy
	  G4double g_x = p.x()/mm;
	  G4double g_y = p.y()/mm;
	  G4double g_z = p.z()/mm;

	  G4double g_t = step->GetPostStepPoint()->GetGlobalTime()/ns;
	  G4double g_e = track->GetTotalEnergy()/eV;

	  printf("*** Killing Photon Event %d N %d P = %.3f %.3f %.3f mm T = %.3f ns E = %.3f eV ***\n",
		 fEventNumber,fPMTPhotonCounter,g_x,g_y,g_z,g_t,g_e);

	  // Kill the photon
	  track->SetTrackStatus(fStopAndKill);

	}
      }
    }
    return;

  } else {

    // Compute total energy released inside the crystal
    if ( step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume() == fDetectorConstruction->GetCrystalVolume() ) {
      fEventAction->AddEdep(step->GetTotalEnergyDeposit());
      //if (step->GetNonIonizingEnergyDeposit() != 0.) printf("WARNING - Non ionizing energy deposit of %f keV\n",
      //							    step->GetNonIonizingEnergyDeposit()/keV);
    }

  }

  // Count total number of optical photons produced by scintillation and Cerenkov effect
  const std::vector<const G4Track*>* secondaries = step->GetSecondaryInCurrentStep();
  if (secondaries->size()>0) {
    for(unsigned int i=0; i<secondaries->size(); ++i) {
      if (secondaries->at(i)->GetParentID()>0) {
	if(secondaries->at(i)->GetDynamicParticle()->GetParticleDefinition() == G4OpticalPhoton::OpticalPhotonDefinition()){
	  if (secondaries->at(i)->GetCreatorProcess()->GetProcessName() == "Scintillation") fEventAction->CountScintillationPhoton();
	  if (secondaries->at(i)->GetCreatorProcess()->GetProcessName() == "Cerenkov") fEventAction->CountCerenkovPhoton();
	}
      }
    }
  }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
