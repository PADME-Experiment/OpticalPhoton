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
// $Id: OpNoviceEventAction.hh 93886 2015-11-03 08:28:26Z gcosmo $
//
/// \file OpNoviceEventAction.hh
/// \brief Definition of the OpNoviceEventAction class

#ifndef OpNoviceEventAction_h
#define OpNoviceEventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"

class OpNoviceRunAction;

/// Event action class
///

class OpNoviceEventAction : public G4UserEventAction
{
public:

  OpNoviceEventAction();
  virtual ~OpNoviceEventAction();

  virtual void BeginOfEventAction(const G4Event*);
  virtual void EndOfEventAction(const G4Event*);

  void AddEdep(G4double edep) { fEdep += edep; }

  void CountPMTPhoton() { fPMTPhotonCounter++; }
  void CountScintillationPhoton() { fScinitillationCounter++; }
  void CountCerenkovPhoton() { fCerenkovCounter++; }

private:

  OpNoviceRunAction* fRunAction;

  G4double fEdep;
  G4int fPMTPhotonCounter;
  G4int fScinitillationCounter;
  G4int fCerenkovCounter;
  
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

    
