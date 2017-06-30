// HCID = 0 is LS detector
// HCID = 1 is NaI detector
// HCE is "Hits collection of this event"

#include "LTAC1SD.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "G4EventManager.hh"
#include "LTAC1EventAction.hh"
#include "G4RunManager.hh"
LTAC1SD::LTAC1SD(G4String name, G4String HCName)	// Sensitive Detector (SD) class
:G4VSensitiveDetector(name),
LTAC1HitsCollection_use(NULL), HCID(-1)
{
  collectionName.insert(HCName);
}

LTAC1SD::~LTAC1SD(){ }

void LTAC1SD::Initialize(G4HCofThisEvent* HCE)		// Make hits collection for the SD and add it to HCE
{
	LTAC1HitsCollection_use = new LTAC1HitsCollection(SensitiveDetectorName, collectionName[0]);

	HCID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
	HCE->AddHitsCollection( HCID, LTAC1HitsCollection_use);
	
}

G4bool LTAC1SD::ProcessHits(G4Step* aStep, G4TouchableHistory*) // This is essential.
{
	G4double edep = aStep->GetTotalEnergyDeposit(); // Get energy deposited in the SD during the step.

	if(edep==0.) return false; // TO DO: Do I want this? Move to END? no energy deposited in SD during step
	
	LTAC1Hit* newHit = new LTAC1Hit(); // If energy was deposited, make a new hit.
    newHit->SetEdep (edep);  // Set hit energy.
	newHit->SetParentID(aStep->GetTrack()->GetParentID());
	newHit->SetTrackID(aStep->GetTrack()->GetTrackID());
	newHit->SetTrackDef(aStep->GetTrack()->GetDefinition());

	LTAC1HitsCollection_use->insert( newHit ); // Insert hit into collection.
  
	return true;
}

void LTAC1SD::EndOfEvent (G4HCofThisEvent* HCE)
{}
