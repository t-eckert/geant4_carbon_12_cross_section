#ifndef LTAC1RunActionMessenger_h
#define LTAC1RunActionMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithABool.hh"

class LTAC1RunAction;
class G4UIdirectory;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithAString;


class LTAC1RunActionMessenger: public G4UImessenger
{
public:
	LTAC1RunActionMessenger(LTAC1RunAction*);
   ~LTAC1RunActionMessenger();

	void SetNewValue(G4UIcommand*, G4String);

private:
	LTAC1RunAction* myRun;

	G4UIdirectory* LTACDir;
	G4UIdirectory* runDir;


	G4UIcmdWithADouble* SetdetEffCmd;
	G4UIcmdWithADoubleAndUnit* SetNaIEmaxCmd;
	G4UIcmdWithoutParameter* GetNaIEmaxCmd;
	G4UIcmdWithAnInteger* SetNaIBinsCmd;
	G4UIcmdWithoutParameter* GetNaIBinsCmd;

	G4UIcmdWithADoubleAndUnit* SetGate1LowCmd;
	G4UIcmdWithADoubleAndUnit* SetGate1HighCmd;
	G4UIcmdWithADoubleAndUnit* SetGate2LowCmd;
	G4UIcmdWithADoubleAndUnit* SetGate2HighCmd;
	G4UIcmdWithADoubleAndUnit* SetGate3LowCmd;
	G4UIcmdWithADoubleAndUnit* SetGate3HighCmd;
	G4UIcmdWithoutParameter* GetGate1Cmd;
	G4UIcmdWithoutParameter* GetGate2Cmd;
	G4UIcmdWithoutParameter* GetGate3Cmd;

	G4UIcmdWithABool* SetOkToSave2DCmd;

	G4UIcmdWithADoubleAndUnit* SetXoffsetCmd;
	G4UIcmdWithADoubleAndUnit* SetYoffsetCmd;
	G4UIcmdWithADoubleAndUnit* SetZoffsetCmd;
	G4UIcmdWithADoubleAndUnit* SetDetectorGapCmd;
	G4UIcmdWithAString* SetMaterialCmd;
};

// KEEP THIS AT THE END OF FILE
#endif