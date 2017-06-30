//
#ifndef LTAC1SD_h
#define LTAC1SD_h 1

#include "G4VSensitiveDetector.hh"
#include "LTAC1Hit.hh"


class G4Step;			// Will get Energy deposit per SD from the Step
class G4HCofThisEvent;	// Hit Collection for the present event. Add each SD to it.


class LTAC1SD : public G4VSensitiveDetector
{
	public:
		 LTAC1SD(G4String, G4String);
		~LTAC1SD();
		
		void Initialize(G4HCofThisEvent*);
		G4bool ProcessHits(G4Step*, G4TouchableHistory*);
		void EndOfEvent(G4HCofThisEvent*);
		
	private:
		LTAC1HitsCollection* LTAC1HitsCollection_use;
		G4int HCID;
};

#endif
