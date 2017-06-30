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
  G4double source_side; 
  G4int select_side = rand() % 2; //Creates a random number, 0 or 1.
  G4double source_half_height = myDetector->GetSourceHalfHeight(); //Pull in the instance of the half_height of the source. 
  
  if(select_side == 0) {source_side = -source_half_height;} // Uses random number to select which side the particle is released from, slightly within scintillation source. 
  else {source_side = source_half_height;}

 
    //G4double zmin = myDetector->GetSourceZoffset() - myDetector->GetSourceHalfHeight();
	//G4double zmax = myDetector->GetSourceZoffset() + myDetector->GetSourceHalfHeight(); 
	G4double zvalue = myDetector->GetSourceZoffset() + source_side;
    G4double rmin = myDetector->GetSourceInnerRadius();
	G4double rmax = myDetector->GetSourceRadius();
	G4double xmid = myDetector->GetSourceXoffset();
	G4double ymid = myDetector->GetSourceYoffset();
	
	//=======Side of emission has been set==============================================
	//====== Setting Positron Energy with Spread =======================================

	/* The energy spread for emitted positrons peaks at 178 keV and drops off toward 
	   0 keV and 540 keV in an almost linear slope. Here, I am simulating that slope
       as a line. There is a function for the rising slope and the falling slope
			rising:		energy = 178keV *(random y value from 0 to 1) 
			falling:	energy = 362keV *[(random y value from 0 to 1) -1.491727]
	   A random number called halfSelect (0 or 1) selects which function is used to 
	   set the energy.
	*/

	G4int halfSelect = rand() %2;
	G4double energySelect = ((G4double)rand()/(G4double)RAND_MAX);
	G4double pEnergy;
	//G4double energyMultiplier = energySelect -1.491727;

	if( halfSelect = 1 ){ pEnergy = 178.0 *energySelect; }
				   else { pEnergy = -362.0 *(energySelect -1.491727); }

    //cout << energySelect << ", " << pEnergy << "\n";

	//====== Positron energy has been set ==============================================

    G4double zStart = zvalue; // z from source_side
	G4double phiStart = G4RandFlat::shoot(0.,2.*3.1415926); // random phi
	G4double rStart = sqrt(G4RandFlat::shoot(pow(rmin,2.),pow(rmax,2.))); // random r^2 value
	G4double xStart = xmid + rStart*cos(phiStart);
	G4double yStart = ymid + rStart*sin(phiStart); // y value using random phi
	particleGun->SetParticleEnergy(pEnergy*keV);

	
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
	
	if(select_side == 0) {c =  abs(c);} 
					else {c = -abs(c);}

G4ThreeVector direction(a,b,c);
	particleGun->SetParticleMomentumDirection(direction);

	particleGun->SetParticlePosition(G4ThreeVector(xStart,yStart,zStart));

	//cout << a << ", " << b << ", " << c << ", "<< select_side << "\n";
	//cout << xStart << ", " << yStart << ", " << zStart << "\n";
	//cout << rStart << ", " << phiStart << "\n";
	//cout << pEnergy << "\n";

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