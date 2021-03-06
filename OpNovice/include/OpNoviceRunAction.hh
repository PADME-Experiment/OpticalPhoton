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
/// \file OpNovice/include/OpNoviceRunAction.hh
/// \brief Definition of the OpNoviceRunAction class
//
//
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef OpNoviceRunAction_h
#define OpNoviceRunAction_h 1

#include "globals.hh"
#include "G4UserRunAction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class G4Timer;
class G4Run;

class OpNoviceRunAction : public G4UserRunAction
{
public:

  OpNoviceRunAction();
  virtual ~OpNoviceRunAction();

public:

  virtual void BeginOfRunAction(const G4Run* aRun);
  virtual void EndOfRunAction(const G4Run* aRun);

  void AddEdep(G4double e)   { fSumEdep += e; fSumEdep2 += e*e;      }
  void AddNPMT(G4int n)      { fSumNPMT += (G4double)n; fSumNPMT2 += (G4double)n*(G4double)n;      }
  void AddNScint(G4int n)    { fSumNSci += (G4double)n; fSumNSci2 += (G4double)n*(G4double)n;    }
  void AddNCerenkov(G4int n) { fSumNCer += (G4double)n; fSumNCer2 += (G4double)n*(G4double)n; }

private:

  G4double fSumEdep;
  G4double fSumEdep2;

  G4double fSumNPMT;
  G4double fSumNPMT2;

  G4double fSumNSci;
  G4double fSumNSci2;

  G4double fSumNCer;
  G4double fSumNCer2;

  G4Timer* fTimer;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif /*OpNoviceRunAction_h*/
