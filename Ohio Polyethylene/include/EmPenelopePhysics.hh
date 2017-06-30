// RPF - modified G4EmPenelopePhysics to include G4RadioactiveDecay

#ifndef EmPenelopePhysics_h
#define EmPenelopePhysics_h 1

#include "G4VPhysicsConstructor.hh"
#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class EmPenelopePhysics : public G4VPhysicsConstructor
{
public:

  EmPenelopePhysics(G4int ver = 1);

  // obsolete
  EmPenelopePhysics(G4int ver, const G4String& name);

  virtual ~EmPenelopePhysics();

  virtual void ConstructParticle();
  virtual void ConstructProcess();

private:
  G4int  verbose;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif






