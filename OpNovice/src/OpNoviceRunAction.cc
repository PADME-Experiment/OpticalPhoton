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
/// \file OpNovice/src/OpNoviceRunAction.cc
/// \brief Implementation of the OpNoviceRunAction class
//
//
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// Make this appear first!
#include "G4Timer.hh"

#include "OpNoviceRunAction.hh"

#include "G4Run.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

OpNoviceRunAction::OpNoviceRunAction()
 : G4UserRunAction(),
   fTimer(0)
{
  fTimer = new G4Timer;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

OpNoviceRunAction::~OpNoviceRunAction()
{
  delete fTimer;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void OpNoviceRunAction::BeginOfRunAction(const G4Run* aRun)
{
  G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;
  fTimer->Start();

  fSumEdep  = 0.;
  fSumEdep2 = 0.;
  fSumNPMT  = 0;
  fSumNPMT2 = 0;
  fSumNSci  = 0;
  fSumNSci2 = 0;
  fSumNCer  = 0;
  fSumNCer2 = 0;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void OpNoviceRunAction::EndOfRunAction(const G4Run* aRun)
{
  fTimer->Stop();
  G4double nevt = (G4double)aRun->GetNumberOfEvent();
  G4cout << "number of event = " << (G4int)nevt << " " << *fTimer << G4endl;

  G4double avgEdep = 0.;
  G4double avgNPMT = 0.;
  G4double avgNSci = 0.;
  G4double avgNCer = 0.;
  G4double rmsEdep = 0.;
  G4double rmsNPMT = 0.;
  G4double rmsNSci = 0.;
  G4double rmsNCer = 0.;
  if (nevt>0) {
    avgEdep = fSumEdep/nevt;
    avgNPMT = fSumNPMT/nevt;
    avgNSci = fSumNSci/nevt;
    avgNCer = fSumNCer/nevt;
    if (nevt>1) {
      rmsEdep = sqrt((fSumEdep2-nevt*avgEdep*avgEdep)/(nevt-1.));
      rmsNPMT = sqrt((fSumNPMT2-nevt*avgNPMT*avgNPMT)/(nevt-1.));
      rmsNSci = sqrt((fSumNSci2-nevt*avgNSci*avgNSci)/(nevt-1.));
      rmsNCer = sqrt((fSumNCer2-nevt*avgNCer*avgNCer)/(nevt-1.));
    }
  }

  printf("Edep = %.3f (%.3f) MeV\n",avgEdep/MeV,rmsEdep/MeV);
  printf("NPMT = %.3f (%.3f)\n",avgNPMT,rmsNPMT);
  printf("NSci = %.3f (%.3f)\n",avgNSci,rmsNSci);
  printf("NCer = %.3f (%.3f)\n",avgNCer,rmsNCer);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
