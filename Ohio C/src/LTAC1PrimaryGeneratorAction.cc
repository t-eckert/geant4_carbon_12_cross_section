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

G4double polyRadius(G4double rmin, G4double rmax) {
		float coef[7] = {-0.01558319, 0.1498466, -0.53306553, 0.85961186, -0.61887564, 0.1567185, 0.99544092};  // order 6 poly fit to r dist of #/vol

		G4double val;
		G4double polySet = 0.0;
		int i = 0;

		do{
		val = G4RandFlat::shoot(0.,1);
		for( i = 0; i<100; i++ ) if (val > ((coef[0]/8.*pow(i/100.*4.,8)
	  			+coef[1]/7.*pow(i/100.*4.,7)
	  			+coef[2]/6.*pow(i/100.*4.,6)
	  			+coef[3]/5.*pow(i/100.*4.,5)
	  			+coef[4]/4.*pow(i/100.*4.,4)
	  			+coef[5]/3.*pow(i/100.*4.,3)
				+coef[6]/2.*pow(i/100.*4.,2)) / 6.91466716 )
								&& val < (coef[0]/8.*pow((i+1) /100.*4.,8)
	  			+coef[1]/7.*pow((i+1) /100.*4.,7)
	  			+coef[2]/6.*pow((i+1) /100.*4.,6)
	  			+coef[3]/5.*pow((i+1) /100.*4.,5)
	  			+coef[4]/4.*pow((i+1) /100.*4.,4)
	  			+coef[5]/3.*pow((i+1) /100.*4.,3)
				+coef[6]/2.*pow((i+1) /100.*4.,2)) / 6.91466716 ) break;  
		
		polySet = 10*(i+0.5)/100.*4.;
		//cout << polySet << endl;
		}while(polySet < rmin);
		return polySet;
	}

	G4double sum[81] = {0,0.000180997,0.000723691,0.001627683,0.002892573,0.004517965,0.00650346,0.008848658,0.011553162,0.014616573,0.018038494,0.021818524,0.025956267,0.030451324,0.035303295,0.040511784,0.046076391,0.051996718,0.058272366,0.064902937,0.071888034,0.079227256,0.086920207,0.094966487,0.103365698,0.112117442,0.12122132,0.130676934,0.140483885,0.150641776,0.161150207,0.17200878,0.183217097,0.194774759,0.206681368,0.218936526,0.231539834,0.244490893,0.257789306,0.271434674,0.285426598,0.29976468,0.314448521,0.329477723,0.344851888,0.360570615,0.376633507,0.393040162,0.409790181,0.426883162,0.444318699,0.462096382,0.480215797,0.498676512,0.51747808,0.536620014,0.556101773,0.575922717,0.596082041,0.616578669,0.637411067,0.658576937,0.680072704,0.701892659,0.724027548,0.746462214,0.769171748,0.792115217,0.815225684,0.838394789,0.86145028,0.884126467,0.906033096,0.926639879,0.945309498,0.96141277,0.974514077,0.984531903,0.991757241,0.996718643,1};
	G4double radius[81] = {0,0.05,0.1,0.15,0.2,0.25,0.3,0.35,0.4,0.45,0.5,0.55,0.6,0.65,0.7,0.75,0.8,0.85,0.9,0.95,1,1.05,1.1,1.15,1.2,1.25,1.3,1.35,1.4,1.45,1.5,1.55,1.6,1.65,1.7,1.75,1.8,1.85,1.9,1.95,2,2.05,2.1,2.15,2.2,2.25,2.3,2.35,2.4,2.45,2.5,2.55,2.6,2.65,2.7,2.75,2.8,2.85,2.9,2.95,3,3.05,3.1,3.15,3.2,3.25,3.3,3.35,3.4,3.45,3.5,3.55,3.6,3.65,3.7,3.75,3.8,3.85,3.9,3.95,4};


G4double fermiRadius( G4double rmin, G4double rmax ){
	G4double val; 	//Create a value that will later be set to random.
	G4double fermiSet = 0.0;	//Original fermiSet value
	int i = 0;

	do{
	//cout << "In Do" << endl;
	val = G4RandFlat::shoot(0.,1);//((G4double) rand()) / (G4double) RAND_MAX;
	for ( i=0; i<81; i++) {
		//cout << i << ", " << val << ", " << sum[i] << ", " << sum[i+1] << ", " << radius[i] << ", " << radius[i+1] << endl;
		if (val > sum[i] && val < sum[i+1])break;}
	fermiSet = 5.*(radius[i] + radius[i+1]); 
	}while(fermiSet < rmin);
	//cout << "after while" << endl;

	return fermiSet;
}

G4double stepwise( G4double rmin, G4double rmax ){
	G4double val;
	G4double rand;
	G4double stepSet;

	G4double sum[11] = {0, 3.17E-03, 2.86E-02, 7.95E-02, 1.55E-01, 2.55E-01, 3.79E-01, 5.27E-01, 6.97E-01, 8.90E-01, 1.00E+00};
	G4double radius[11] = {0.0, 0.2, 0.6, 1.0, 1.4, 1.8, 2.2, 2.6, 3.0, 3.4, 3.8};

	//cout << "in stepwise" << endl;

	do{
	val = G4RandFlat::shoot(0.,1);
	rand = G4RandFlat::shoot();
	for (int i=0; i<11; i++) if (val > sum[i] && val < sum[i+1] )
	(stepSet = 10*(radius[i] + (radius[i+1]-radius[i])*rand));
	//cout << stepSet << endl;
	}while(stepSet < rmin);
	//cout << "outside loop" << endl;
	return stepSet;
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
	
	//More likely that positron is emitted from one side of graphite. 100 to 77 percent on either side. Linear distrib.
	G4double zmin;
	G4double zmax = myDetector->GetSourceZoffset() +myDetector->GetSourceHalfHeight(); 
  
	G4double pSelect = G4RandFlat::shoot(0.0, 100.0);
	
	if( pSelect <= 77 ){ zmin = myDetector->GetSourceZoffset() -myDetector->GetSourceHalfHeight(); }
	else { zmin = 0.0869565*zmax*pSelect -7.695652*zmax; }
	
	// Linear distribution of positrons with higher likelihood in center of face.

	//Polynomial Fit. Written by Dr. Yuly. Added 2-8-16

	G4double rmin = myDetector->GetSourceInnerRadius();
	G4double rmax = myDetector->GetSourceRadius();
	G4double poly = polyRadius(rmin, rmax);
	//G4double fermi = fermiRadius(rmin, rmax);
	//G4double step = stepwise(rmin, rmax);

	G4double xmid = myDetector->GetSourceXoffset();
	G4double ymid = myDetector->GetSourceYoffset();

	//=======Side of emission has been set==============================================

    G4double zStart = G4RandFlat::shoot(zmin, zmax); //random z value
		//zvalue; // z from source_side
	G4double phiStart = G4RandFlat::shoot(0.,2.*3.1415926); // random phi
	G4double rStart = 
		poly;/*//*fermi;*//*step;//*/
		//sqrt(G4RandFlat::shoot(pow(rmin,2.),pow(rmax,2.))); // random r^2 value
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

	cout << rStart << ", " << xStart << ", " << yStart << ", " << zStart << "\n";

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