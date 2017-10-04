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
/// \file OpNovice/src/OpNovicePrimaryGeneratorAction.cc
/// \brief Implementation of the OpNovicePrimaryGeneratorAction class
//
//
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "OpNovicePrimaryGeneratorAction.hh"
#include "OpNovicePrimaryGeneratorMessenger.hh"

#include "Randomize.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

OpNovicePrimaryGeneratorAction::OpNovicePrimaryGeneratorAction()
 : G4VUserPrimaryGeneratorAction(), 
   fParticleGun(0)
{
  //create a messenger for this class
  fGunMessenger = new OpNovicePrimaryGeneratorMessenger(this);

  //default kinematic

  fGunMode = 0; // By default generate optical photons with BGO scintillating distributions

  // Shoot 10 optical photons from the center of the crystal
  fOptPhotonPos = G4ThreeVector(0.,0.,0.);
  fOptPhotonNr = 10;

  // Define a default gun for GunMode 1
  fParticleGun = new G4ParticleGun(1); // Single particle
  fParticleGun->SetParticleDefinition(G4ParticleTable::GetParticleTable()->FindParticle("gamma"));
  fParticleGun->SetParticleTime(0.*ns);
  fParticleGun->SetParticlePosition(G4ThreeVector(0.*mm,0.*mm,-200.*mm));
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
  fParticleGun->SetParticleEnergy(100.*MeV);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

OpNovicePrimaryGeneratorAction::~OpNovicePrimaryGeneratorAction()
{
  delete fParticleGun;
  delete fGunMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void OpNovicePrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{

  /*
  if ( fGunMode == 0 ) {

    // Choose a random direction
    G4double dz = -1.+2.*G4UniformRand();
    G4double a = sqrt(1.-dz*dz);
    G4double theta = 360.*G4UniformRand()*deg;
    G4double dx = a*cos(theta);
    G4double dy = a*sin(theta);
    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(dx,dy,dz).unit());

    // Choose optical photon energy from BGO scintillation spectrum
    fParticleGun->SetParticleEnergy(GetBGOPhotonEnergy());

  }
  fParticleGun->GeneratePrimaryVertex(anEvent);
  */

  if ( fGunMode == 0 ) {

    // Create primary vertex at desired position
    G4PrimaryVertex* vtx = new G4PrimaryVertex(fOptPhotonPos,0.*ns);

    G4ParticleDefinition* opticalphoton = G4ParticleTable::GetParticleTable()->FindParticle("opticalphoton");

    // Generate photons attached to this vertex
    for (G4int i=0; i<fOptPhotonNr; i++) {

      // Choose a random direction
      G4double dz = -1.+2.*G4UniformRand();
      G4double a = sqrt(1.-dz*dz);
      G4double theta = 360.*G4UniformRand()*deg;
      G4double dx = a*cos(theta);
      G4double dy = a*sin(theta);

      // Extract photon energy according to BGO scintillation spectrum
      G4double e = GetBGOPhotonEnergy();

      // Create photon with given direction and energy and attach it to vertex
      G4PrimaryParticle* opPhoton = new G4PrimaryParticle(opticalphoton,e*dx,e*dy,e*dz,e);
      vtx->SetPrimary(opPhoton);

    }

    // Insert this vertex into event
    anEvent->AddPrimaryVertex(vtx);

    printf("OpNovicePrimaryGeneratorAction - Generated vertex with %d optical photons\n",fOptPhotonNr);

  } else {

    fParticleGun->GeneratePrimaryVertex(anEvent);

  }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void OpNovicePrimaryGeneratorAction::SetOptPhotonPolar() {
  G4double angle = G4UniformRand() * 360.0*deg;
  SetOptPhotonPolar(angle);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void OpNovicePrimaryGeneratorAction::SetOptPhotonPolar(G4double angle)
{
 if (fParticleGun->GetParticleDefinition()->GetParticleName()!="opticalphoton")
   {
     G4cout << "--> warning from PrimaryGeneratorAction::SetOptPhotonPolar() :"
               "the particleGun is not an opticalphoton" << G4endl;
     return;
   }

 G4ThreeVector normal (1., 0., 0.);
 G4ThreeVector kphoton = fParticleGun->GetParticleMomentumDirection();
 G4ThreeVector product = normal.cross(kphoton);
 G4double modul2       = product*product;
 
 G4ThreeVector e_perpend (0., 0., 1.);
 if (modul2 > 0.) e_perpend = (1./std::sqrt(modul2))*product;
 G4ThreeVector e_paralle    = e_perpend.cross(kphoton);
 
 G4ThreeVector polar = std::cos(angle)*e_paralle + std::sin(angle)*e_perpend;
 fParticleGun->SetParticlePolarization(polar);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// Return random photon energy distributed according to BGO scintillation process
G4double OpNovicePrimaryGeneratorAction::GetBGOPhotonEnergy()
{
  static G4double photonEnergy[] = 
    { 1.94*eV, 1.95*eV, 1.96*eV, 1.97*eV, 1.98*eV, 1.99*eV, 2.00*eV, 2.01*eV, 2.02*eV, 2.03*eV,
      2.04*eV, 2.05*eV, 2.06*eV, 2.07*eV, 2.08*eV, 2.09*eV, 2.10*eV, 2.11*eV, 2.12*eV, 2.13*eV,
      2.14*eV, 2.15*eV, 2.16*eV, 2.17*eV, 2.18*eV, 2.19*eV, 2.20*eV, 2.21*eV, 2.22*eV, 2.23*eV,
      2.24*eV, 2.25*eV, 2.26*eV, 2.27*eV, 2.28*eV, 2.29*eV, 2.30*eV, 2.31*eV, 2.32*eV, 2.33*eV,
      2.34*eV, 2.35*eV, 2.36*eV, 2.37*eV, 2.38*eV, 2.39*eV, 2.40*eV, 2.41*eV, 2.42*eV, 2.43*eV,
      2.44*eV, 2.45*eV, 2.46*eV, 2.47*eV, 2.48*eV, 2.49*eV, 2.50*eV, 2.51*eV, 2.52*eV, 2.53*eV,
      2.54*eV, 2.55*eV, 2.56*eV, 2.57*eV, 2.58*eV, 2.59*eV, 2.60*eV, 2.61*eV, 2.62*eV, 2.63*eV,
      2.64*eV, 2.65*eV, 2.66*eV, 2.67*eV, 2.68*eV, 2.69*eV, 2.70*eV, 2.71*eV, 2.72*eV, 2.73*eV,
      2.74*eV, 2.75*eV, 2.76*eV, 2.77*eV, 2.78*eV, 2.79*eV, 2.80*eV, 2.81*eV, 2.82*eV, 2.83*eV,
      2.84*eV, 2.85*eV, 2.86*eV, 2.87*eV, 2.88*eV, 2.89*eV, 2.90*eV, 2.91*eV, 2.92*eV, 2.93*eV,
      2.94*eV, 2.95*eV, 2.96*eV, 2.97*eV, 2.98*eV, 2.99*eV, 3.00*eV, 3.01*eV, 3.02*eV, 3.03*eV,
      3.04*eV, 3.05*eV, 3.06*eV, 3.07*eV, 3.08*eV, 3.09*eV, 3.10*eV, 3.11*eV, 3.12*eV, 3.13*eV,
      3.14*eV, 3.15*eV, 3.16*eV, 3.17*eV, 3.18*eV, 3.19*eV, 3.20*eV, 3.21*eV, 3.22*eV, 3.23*eV,
      3.24*eV, 3.25*eV, 3.26*eV, 3.27*eV, 3.28*eV, 3.29*eV, 3.30*eV };

  const G4int nEntries = sizeof(photonEnergy)/sizeof(G4double);

  static G4double probability[] = 
    { 0.12, 0.12, 0.14, 0.15, 0.16, 0.18, 0.19, 0.20, 0.22, 0.23,
      0.24, 0.26, 0.27, 0.29, 0.30, 0.32, 0.33, 0.35, 0.36, 0.38,
      0.39, 0.41, 0.42, 0.44, 0.45, 0.47, 0.49, 0.50, 0.52, 0.54,
      0.56, 0.58, 0.60, 0.62, 0.64, 0.65, 0.67, 0.69, 0.71, 0.73,
      0.75, 0.77, 0.79, 0.81, 0.82, 0.84, 0.85, 0.86, 0.88, 0.89,
      0.91, 0.92, 0.93, 0.94, 0.95, 0.96, 0.97, 0.98, 0.98, 0.99,
      1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 0.99, 0.98, 0.98,
      0.97, 0.97, 0.97, 0.96, 0.95, 0.94, 0.93, 0.91, 0.90, 0.89,
      0.88, 0.86, 0.85, 0.83, 0.81, 0.79, 0.77, 0.75, 0.74, 0.72,
      0.70, 0.68, 0.66, 0.64, 0.62, 0.60, 0.58, 0.56, 0.54, 0.52,
      0.50, 0.48, 0.46, 0.44, 0.42, 0.40, 0.38, 0.36, 0.34, 0.33,
      0.31, 0.29, 0.28, 0.26, 0.24, 0.23, 0.22, 0.20, 0.19, 0.18,
      0.17, 0.16, 0.15, 0.14, 0.13, 0.12, 0.11, 0.10, 0.09, 0.08,
      0.08, 0.07, 0.06, 0.04, 0.03, 0.02, 0.02 };

  //static G4int first = 1;
  //if (first == 1) {
  //  first = 0;
  //  printf("BGO scintillation probability table\n");
  //  for(G4int i=0; i<nEntries; i++) printf("%f\t%f\n",photonEnergy[i]/eV,probability[i]);
  //}

  // Miss and hit
  while(true) {
    G4int bin = int(G4UniformRand()*nEntries);
    if (G4UniformRand()<probability[bin]) return photonEnergy[bin];
  }
  return 3.0*eV; // WTF?

}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
