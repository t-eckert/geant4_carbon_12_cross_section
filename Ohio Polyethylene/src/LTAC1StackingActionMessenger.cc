
#include "LTAC1StackingActionMessenger.hh"
#include "G4UnitsTable.hh"
#include "LTAC1StackingAction.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"

// Allow user to set a maximun track time, after the initial ion decays

LTAC1StackingActionMessenger::~LTAC1StackingActionMessenger(){
	delete stackDir;
	delete SetMaxTrackTimeCmd;
	delete GetMaxTrackTimeCmd;
}

LTAC1StackingActionMessenger::LTAC1StackingActionMessenger(LTAC1StackingAction* StackA)
:StackAction(StackA)
{
	stackDir = new G4UIdirectory("/LTAC/stack/"); // make directory in UI
	stackDir->SetGuidance("pass or kill particles at start of track"); // guidance for "help"

	SetMaxTrackTimeCmd = new G4UIcmdWithADoubleAndUnit("/LTAC/stack/SetMaxTrackTime", this);
	SetMaxTrackTimeCmd -> SetGuidance("Set Max time for track after initial decay");
	SetMaxTrackTimeCmd -> SetParameterName("SetMaxTrackTime",false);
	SetMaxTrackTimeCmd -> SetUnitCandidates("ns ms s");

	GetMaxTrackTimeCmd = new G4UIcmdWithoutParameter("/LTAC/stack/GetMaxTrackTime", this);
	GetMaxTrackTimeCmd -> SetGuidance("Get curret maximum time for a track creation post initial decay.");

}

void LTAC1StackingActionMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
		if ( command == SetMaxTrackTimeCmd ) // change maximum time after decay to allow track (limits delayed-states, daughters)
		{StackAction->SetMaxTrackTime(SetMaxTrackTimeCmd->GetNewDoubleValue(newValue));}

		else if ( command == GetMaxTrackTimeCmd ) // print Gate 1 settings to G4cout
		{
			G4cout << "Max Track Time: " << StackAction->GetMaxTrackTime() << " ns" << G4endl;}
}
