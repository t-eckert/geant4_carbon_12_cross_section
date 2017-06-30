//
#ifndef LTAC1EventAction_h
#define LTAC1EventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"
#include "LTAC1Analysis.hh"
#include "LTAC1RunAction.hh"
#include "Randomize.hh"
#include "CLHEP\Random\RandPoisson.h"


class HepRandom;
class Histogram;
class G4Event;


class LTAC1EventAction : public G4UserEventAction
{
  public:
    LTAC1EventAction();
		LTAC1EventAction(G4UserRunAction*);
		LTAC1EventAction(LTAC1RunAction*);
   ~LTAC1EventAction();
	
  public:
    void BeginOfEventAction(const G4Event*);
    void EndOfEventAction(const G4Event*);
	G4double LTAC1EventAction::GetEpmt(G4double);

		void AddAbs(G4double de, G4double dl) {EnergyAbs +=de;};
		G4double GetAbs() {return EnergyAbs;};
		void SetAbs(G4double EA) {EnergyAbs=EA;};
		G4int GetMaxTrackID() {return MaxTrackID;};
		void SetMaXTrackID(G4int TrckID) {MaxTrackID = TrckID;};
	private:
		G4int NHist;				// total number of histories to run
		G4int iDivide;				// Want to print Progress 10 times (so every 10th of NHist)
		LTAC1RunAction* runAct;		// run action
		G4double NaI1Edep;			// Energy deposited in LS from event
		G4double LSEpmt;			// Energy detected in PMT (approximate linear calibration from number of photoelectrons). Used for NaI1Hist.
		G4double LSLight;			// Light emitted in LS from event (Energy * Q * ...)
		G4double NaI2Edep;			// Energy deposited in NaI from event
		G4double EnergyAbs;			// Generic energy deposited in detector, running sum over all hits in an event.
		G4int MaxTrackID;
		G4int iChunk;
		G4int NaI1HitsCollectionIndex;	// index of hits collection for LS Hemisphere SD
		G4int NaI2HitsCollectionIndex;			// index of hits collection for NaI Sensitive Detector	
};

// Keep this at the end
#endif //
