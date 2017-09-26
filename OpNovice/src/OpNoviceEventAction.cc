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
// $Id: OpNoviceEventAction.cc 93886 2015-11-03 08:28:26Z gcosmo $
//
/// \file OpNoviceEventAction.cc
/// \brief Implementation of the OpNoviceEventAction class

#include "OpNoviceEventAction.hh"

#include "OpNoviceRunAction.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

OpNoviceEventAction::OpNoviceEventAction() : G4UserEventAction()
{

  fEdep = 0.;
  fPMTPhotonCounter = 0;
  fScinitillationCounter = 0;
  fCerenkovCounter = 0;

  fRunAction = (OpNoviceRunAction*)G4RunManager::GetRunManager()->GetUserRunAction();

} 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

OpNoviceEventAction::~OpNoviceEventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void OpNoviceEventAction::BeginOfEventAction(const G4Event* event)
{    
  fEdep = 0.;
  fPMTPhotonCounter = 0;
  fScinitillationCounter = 0;
  fCerenkovCounter = 0;
  printf("OpNoviceEventAction - Event %d Begin\n",event->GetEventID());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void OpNoviceEventAction::EndOfEventAction(const G4Event* event)
{   
  printf("OpNoviceEventAction - Event %d End\n",event->GetEventID());
  printf("OpNoviceEventAction - Edep %f MeV NPMT %d NScint %d NCerenkov %d\n",
	 fEdep/MeV,fPMTPhotonCounter,fScinitillationCounter,fCerenkovCounter);
  // accumulate statistics in run action
  //fRunAction->AddEdep(fEdep);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
