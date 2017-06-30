//
//

#ifndef LTAC1PrimaryGeneratorAction_h
#define LTAC1PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"
#include "G4ParticleGun.hh"
#include "LTAC1DetectorConstruction.hh"

class G4Event;
class LTAC1DetectorConstruction;


class LTAC1PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  private:
    G4ParticleGun*  particleGun;	// To create vertex for given particle	
	G4int Z;
	G4int A; 
	G4double Elevel;
  public:
    LTAC1PrimaryGeneratorAction(LTAC1DetectorConstruction*);   // Use Detector to get position of source 
   ~LTAC1PrimaryGeneratorAction();

  public:
    void GeneratePrimaries(G4Event*);
    G4ParticleGun* GetParticleGun() { return particleGun;} ;
	LTAC1DetectorConstruction* myDetector; // Use detector to get position of source

	void SetZ(G4int Zset){Z = Zset;};
	void SetA(G4int Aset){A = Aset;};
	void SetElevel(G4double E){Elevel=E;};
	G4int GetZ(){return Z;};
	G4int GetA(){return A;};  
	G4double GetElevel(){return Elevel;};


};



#endif
