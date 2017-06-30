//
#include "LTAC1EventAction.hh"
#include "LTAC1Hit.hh"
#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4ios.hh"
#include "G4SDManager.hh"
#include "LTAC1Analysis.hh"
#include <iostream>
#include "G4UserRunAction.hh"
#include "G4RunManager.hh"
#include "G4Run.hh"
#include "G4Event.hh"
#include "G4ParticleDefinition.hh"
#include "Randomize.hh"
#include "CLHEP\Random\RandPoisson.h"
#include "CLHEP\Random\RandGauss.h"
#include "G4UnitsTable.hh"
#include <exception>

using namespace std;
//
// TO DO: Add chunking option to run messenger
//

////==..==..==..==..==..==..==..==..==..==..==..==..==..==..==..==..==..==..==..==..==..==..//
////   EVENT ACTION: An event is a history, what follows from a Primary Generator Action.   //
///		             There may be many (or zero) hits in each detector for a given event.	//
////==..==..==..==..==..==..==..==..==..==..==..==..==..==..==..==..==..==..==..==..==..==..//
LTAC1EventAction::LTAC1EventAction(LTAC1RunAction* run) :
runAct(run), EnergyAbs(0.0), MaxTrackID(0),iChunk(0),
NaI1HitsCollectionIndex(-1),NaI2HitsCollectionIndex(-1)
{}
 
LTAC1EventAction::~LTAC1EventAction()
{}

////==..==..==..==..==..==..==..==..//
////    BEGIN OF EVENT ACTION       //
////==..==..==..==..==..==..==..==..//
void LTAC1EventAction::BeginOfEventAction(const G4Event* evt)
{
	if(NaI1HitsCollectionIndex<0) // For the first event
	{  // Retrieve the ID of the hits collection for each Sensitive Detector
	  NaI1HitsCollectionIndex = G4SDManager::GetSDMpointer() ->GetCollectionID("NaI1Collection");
	  NaI2HitsCollectionIndex = G4SDManager::GetSDMpointer() ->GetCollectionID("NaI2Collection");	

	  NHist = runAct->GetNhistories();  // total number of histories to run
	  iDivide=NHist/10;					// Want to print Progress 10 times (so every 10th of NHist)
	  if (iDivide < 1) {iDivide = 1;}	// Force iDivide not to be zero, so we can divide by it.
	}
}

////==..==..==..==..==..==..==..==..//
////    END OF EVENT ACTION		    //
////==..==..==..==..==..==..==..==..//
void LTAC1EventAction::EndOfEventAction(const G4Event* evt)
{
static G4EventManager* em = G4EventManager::GetEventManager();
G4int Verbosity = em->GetVerboseLevel(); // get event action verbose setting
time_t rawtime;
struct tm  timeinfo;
time (&rawtime);
char buffer [80];
timeinfo = *localtime ( &rawtime) ;
strftime (buffer,80,"%Y-%m-%d-%H:%M:%S",&timeinfo);	
	
  G4int event_id = evt->GetEventID(); // event_id is the number of the event (1, 2, 3...)

  G4double pcnt = event_id*1./NHist*100.;								// Percent progress
  if (event_id < 4 || event_id%iDivide == 0||event_id == NHist-1) {		// Progress for 1st 3 events and decades.
	  // TO DO: put Timer in here //
    G4cout << ">>> Event: " << evt->GetEventID() << " out of: "  << NHist; 
    G4cout << ": (" << pcnt <<"% complete)"<< "   " << buffer <<G4endl; 

	if (event_id == 1)
	  { // Clunky, but just once go and get the particle name (e.g. Co60) and pass it to the run for writing to output files.
	    static G4RunManager* rm = G4RunManager::GetRunManager();
	    const G4String PN =  rm->GetCurrentEvent()->GetPrimaryVertex()->GetPrimary()->GetParticleDefinition()->GetParticleName();
	    runAct->SetParticleName(PN);
	  }
  }
	// If no valid hits collection exists, something is messed up, so don't process data.
	if(NaI1HitsCollectionIndex < 0||NaI2HitsCollectionIndex < 0) return;
	
	G4HCofThisEvent* HCE = evt -> GetHCofThisEvent();	// Container for hits collections.
	LTAC1HitsCollection* NaI1HC=0;	// One hits collection per detector
	LTAC1HitsCollection* NaI2HC=0;	// One hits collection per detector
	
	if(HCE) // If HCofThisEvent exists, retreive the hits collection for each detector, using indexes from above.
	{ 
		NaI1HC = (LTAC1HitsCollection*)(HCE-> GetHC(NaI1HitsCollectionIndex));
	    // If you want different data from each det, you can define separate HitsCollection classes.
		NaI2HC = (LTAC1HitsCollection*)(HCE-> GetHC(NaI2HitsCollectionIndex));
	  }

//********************************************************************************************************************************//
// Process NaI1 Detector hits collection 
//********************************************************************************************************************************//

		if(NaI1HC) 
	{
		G4int numberHits = NaI1HC -> entries();  // number of hits in hits collection
		NaI2Edep = 0;							// initialize energy to zero
		SetAbs(0.0);
		for (int i = 0; i < numberHits ; i++) 
		  {
			LTAC1Hit* hit = (*NaI1HC)[i];				// retrieve the i-th hit from the collection.			
			G4double energyDeposit = hit -> GetEdep();  // energy deposited by hit.
			AddAbs(energyDeposit, 0.0);					// sum energy from all hits within detector within event.
		   }
		G4double E0 = GetAbs();

		G4double Resolution =  sqrt( E0/MeV*3.78*3.78 + E0/MeV*E0/MeV*4.2*4.2 ) / 2.35 / 100.; // Experimental Gaussian stdev
		NaI1Edep = CLHEP::RandGauss::shoot(GetAbs(),Resolution*MeV); // make Gaussian energy jitter
		LSEpmt = NaI1Edep;
		if (NaI1Edep <= 0.0) NaI1Edep = 0.0;				// don't let energy go negative
//	    NaI1Edep = GetAbs(); // ignore resolution
	}

//********************************************************************************************************************************//
// Process NaI2 Detector hits collection 
//********************************************************************************************************************************//
	if(NaI2HC) 
	{
		G4int numberHits = NaI2HC -> entries();  // number of hits in hits collection
		NaI2Edep = 0;							// initialize energy to zero
		SetAbs(0.0);
		for (int i = 0; i < numberHits ; i++) 
		  {
			LTAC1Hit* hit = (*NaI2HC)[i];				// retrieve the i-th hit from the collection.			
			G4double energyDeposit = hit -> GetEdep();  // energy deposited by hit.
			AddAbs(energyDeposit, 0.0);					// sum energy from all hits within detector within event.
		   }
		G4double E0 = GetAbs();

		G4double Resolution =  sqrt( E0/MeV*3.78*3.78 + E0/MeV*E0/MeV*4.2*4.2 ) / 2.35 / 100.; // Experimental Gaussian stdev
		NaI2Edep = CLHEP::RandGauss::shoot(GetAbs(),Resolution*MeV); // make Gaussian energy jitter

		if (NaI2Edep <= 0.0) NaI2Edep = 0.0;				// don't let energy go negative
//	    NaI2Edep = GetAbs(); // ignore resolution
	}

//********************************************************************************************************************************//
// Fill Histograms 
//********************************************************************************************************************************//
	
	// ==.==.== FILL HISTOGRAMS ==.==.==//
	runAct->NaI1Histogram.Fill(NaI1Edep);								
	runAct->NaI2Histogram.Fill(NaI2Edep);

	runAct->NaI1Histogram.FillWindows(NaI1Edep);								
	runAct->NaI2Histogram.FillWindows(NaI2Edep);
	runAct->TheHistogram2D.FillWindows(NaI1Edep,NaI2Edep);	
		
	runAct->TheHistogram2D.Fill(NaI1Edep,NaI2Edep);	

	if (Verbosity > 0) // If event verbosity called for, print energy of each event
	{
		printf("NaI1 energy = %-5.3f MeV \n",NaI1Edep/MeV);
		printf("NaI2 energy = %-5.3f MeV \n",NaI2Edep/MeV);

	}
}

