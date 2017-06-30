/// \brief Definition of the LTAC1StackingAction class

#ifndef LTAC1StackingAction_h
#define LTAC1StackingAction_h 1

#include "G4UserStackingAction.hh"
#include "globals.hh"
#include "LTAC1StackingActionMessenger.hh"
#include "LTAC1RunActionMessenger.hh"
/// Stacking action class : manage the newly generated particles
///
/// One wishes do not track secondary neutrino.Therefore one kills it 
/// immediately, before created particles will  put in a stack.
class LTAC1StackingActionMessenger;

class LTAC1StackingAction : public G4UserStackingAction
{
private: 
	 G4double EventStartGlobalTime; // time when first track from event starts. That is, after decay of ion if the ion is radioactive.
	 G4double EventGlobalTime;		// time since first track started.

	 G4double MaxTrackTime;			// max time allowed for a track to start since first decay.

	 LTAC1StackingActionMessenger* StackingActionMessenger; // messenger used to set MaxTrackTime


  public:
    LTAC1StackingAction();
    virtual ~LTAC1StackingAction();
	void SetMaxTrackTime(G4double Time){MaxTrackTime = Time;};
	G4double GetMaxTrackTime(void){return MaxTrackTime;};


    virtual G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track*);        
};


#endif

