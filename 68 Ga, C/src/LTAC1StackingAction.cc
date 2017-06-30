
/// \brief Implementation of the LTAC1StackingAction class
// 8-10-2012 Added cutoff on EventGlobalTime, so that Tc-99 will not decay following Tc-99m.
// Not the nicest way to do it, but they are both same A,Z so not blocked by SetNucleusLimits
#include "LTAC1StackingAction.hh"
#include "LTAC1RunAction.hh"
#include "G4RunManager.hh"
#include "G4Run.hh"
#include "G4Track.hh"
#include "G4NeutrinoE.hh"
#include "G4AntiNeutrinoE.hh"
#include "G4Positron.hh"
#include "G4Gamma.hh"
#include "G4RadioactiveDecay.hh"
#include "G4BetaDecayCorrections.hh"
#include "G4UnitsTable.hh"

LTAC1StackingAction::LTAC1StackingAction()
{
	EventStartGlobalTime = 0.;
	StackingActionMessenger = new LTAC1StackingActionMessenger(this); // Define user commands}
	MaxTrackTime = 1.0E100; // accept all decays
}

LTAC1StackingAction::~LTAC1StackingAction()
{ 
	delete StackingActionMessenger;
;}


G4ClassificationOfNewTrack
LTAC1StackingAction::ClassifyNewTrack(const G4Track* track)
{  

	if (track->GetParentID() == 1)							// start clock when primary ion decays (assuming primary ion is radioactive)
	{EventStartGlobalTime = track->GetGlobalTime();}
	EventGlobalTime = track->GetGlobalTime()-EventStartGlobalTime; // define time since primary decay


  // now decide how to rank, or kill, particles.
  //keep primary decay particles
  if (track->GetParentID() == 0) return fUrgent;



  if (track->GetDefinition() == G4NeutrinoE::NeutrinoE()) return fKill;   //kill secondary neutrino or anti-neutrino
 // if(track->GetDefinition() == G4Positron::Positron()) return fKill; // kill positrons
  else if(track->GetDefinition() == G4AntiNeutrinoE::AntiNeutrinoE()) return fKill;
//  else if ((track->GetKineticEnergy()<1.*GeV) &&(track->GetDefinition() == G4Positron::Positron())) return fKill;
 // else if (EventGlobalTime > MaxTrackTime) return fKill; //  kill delayed-states, short-lived daughters etc.

  else return fUrgent;
}

