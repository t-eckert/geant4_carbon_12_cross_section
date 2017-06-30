//

#include "LTAC1PrimaryGeneratorAction.hh"

#include "G4Event.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "G4Positron.hh"
#include "G4Geantino.hh"
#include "Randomize.hh"
#include "LTAC1DetectorConstruction.hh"
#include <cmath>
#include <iostream>

using namespace std;

LTAC1PrimaryGeneratorAction::LTAC1PrimaryGeneratorAction(LTAC1DetectorConstruction* det)
	:myDetector(det)
{
  G4int n_particle = 1;
  particleGun = new G4ParticleGun(n_particle); 
  // by default, makes a Geantino. keep it that way, for conditional in GeneratePrimaries. This can be altered in batch.mac.
  particleGun->SetParticleEnergy(0*eV);
  particleGun->SetParticlePosition(G4ThreeVector(0., 0., 0.));
  particleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.)); 
}

LTAC1PrimaryGeneratorAction::~LTAC1PrimaryGeneratorAction()
{
  delete particleGun;
}

void LTAC1PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{ // first time around, change default Geantino to particle of interest
  if (particleGun->GetParticleDefinition() == G4Geantino::Geantino()) {  
// DECAY TABLE: ENSDF data in c/Geant4/geant4.9.5-install/share/Geant4-9.5.0/data/RadioactiveDecay3.4
// "custom" shape factor added by RPF. Use ShapeParameters from Behren's Equations. Include all 11 parameters in one line. Eg. 99Tc:
// BetaMinus       0.0000   1.0000e+02     293.5000  custom 0. 0. 0. 1.0 0.54 0. 0. 0. 0. 0. 0. 0. 0.
// C(W) = 1 + a*W + b/W + c*W^2 + d*(q^2 + e*p^2) + f*q^4 + g*q^2*p^4 + i*q^4*p^2 + j*q^2*p^4 + k*p^6
  }  


	//--------------------------------------------------------------------------------------------------
	// INITIAL POSITION TAKEN AS RANDOM POINT WITHIN SOURCE CYLINDER AS DEFINED IN LTAC1DetectorConstruction
	//--------------------------------------------------------------------------------------------------
	
		
  //=======Set which side of the scintillator the particle is emitted from============
  //G4double source_side; 
  //G4int select_side = rand() % 2; //Creates a random number, 0 or 1.
  //G4double source_half_height = myDetector->GetSourceHalfHeight(); //Pull in the instance of the half_height of the source. 
  
  //if(select_side == 0) {source_side = -source_half_height;} // Uses random number to select which side the particle is released from, slightly within scintillation source. 
  //else {source_side = source_half_height;}

 
    G4double zmin = myDetector->GetSourceZoffset() - myDetector->GetSourceHalfHeight();
	G4double zmax = myDetector->GetSourceZoffset() + myDetector->GetSourceHalfHeight(); 
	//G4double zvalue = myDetector->GetSourceZoffset() + source_side;
    G4double rmin = 0.0; //myDetector->GetSourceInnerRadius();
	G4double rmax = 3.0*mm/2; //myDetector->GetSourceRadius();
	G4double xmid = myDetector->GetSourceXoffset();
	G4double ymid = myDetector->GetSourceYoffset();
	

  
  

	//=======Side of emission has been set==============================================

    G4double zStart = 0.0*mm;//G4RandFlat::shoot(zmin, zmax); //random z value
		//zvalue; // z from source_side
	G4double phiStart = G4RandFlat::shoot(0.,2.*3.1415926); // random phi
	G4double rStart = sqrt(G4RandFlat::shoot(pow(rmin,2.),pow(rmax,2.))); // random r^2 value
	G4double xStart = xmid + rStart*cos(phiStart);	
	G4double yStart = ymid + rStart*sin(phiStart); // y value using random phi
//	particleGun->SetParticleEnergy(0*eV);

	
// RANDOM DIRECTION, FROM EXAMPLE. CAREFUL WITH NORMALIZATION.
G4double a,b,c;


G4double n;
do {
	a=(G4UniformRand()-0.5)/.5;
	b=(G4UniformRand()-0.5)/.5;
	c=(G4UniformRand()-0.5)/.5;
n = a*a+b*b+c*c;
} while (n > 1 || n == 0.0);
n = std::sqrt(n);
a /= n;
b /= n;
c /= n;


//	a = 0;
//	b = 0;
	
//	if(select_side == 0) {c =  1.0;} 
//					else {c = -1.0;}

G4ThreeVector direction(a,b,c);
	particleGun->SetParticleMomentumDirection(direction);

	particleGun->SetParticlePosition(G4ThreeVector(xStart,yStart,zStart));

	/*cout << phiStart /*<< ", " << yStart << ", " << zStart << "\n";*/

	// TEMPORARY POSITRONS
	//G4ParticleDefinition* pos = G4Positron::PositronDefinition();
	//particleGun->SetParticleDefinition(pos);
	//particleGun->SetParticleEnergy(100.*keV);


	//G4ParticleDefinition* gam = G4Gamma::GammaDefinition();
	//particleGun->SetParticleDefinition(gam);
	//particleGun->SetParticleEnergy(511.*keV);


	//particleGun->SetParticlePosition(G4ThreeVector(0,0,0));

	//particleGun->SetParticleMomentumDirection(G4ThreeVector(vx,vy,vz));

	particleGun->GeneratePrimaryVertex(anEvent);	// create vertex

	// CREATE MIRROR PHOTON



//	particleGun->SetParticleMomentumDirection(-direction);
//	particleGun->GeneratePrimaryVertex(anEvent);	// create vertex
}