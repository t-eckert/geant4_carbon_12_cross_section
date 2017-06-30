//
//
#include "LTAC1RunActionMessenger.hh"

#include "LTAC1RunAction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithABool.hh"
#include "globals.hh"

//
//----------- Communicate with user about current run. -------------- //
// Allow user to set NaI gates, and get settings of those gates
// -------------------------------------------------------------------//
LTAC1RunActionMessenger::LTAC1RunActionMessenger(LTAC1RunAction* myRn)
	:myRun(myRn)
{
	LTACDir = new G4UIdirectory("/LTAC/"); // make directory in UI
	LTACDir->SetGuidance("LTAC settings: gates, particle"); // guidance for "help"

	runDir = new G4UIdirectory("/LTAC/run/"); // make directory for run items.
	runDir-> SetGuidance("run control: gates");


	SetNaIEmaxCmd = new G4UIcmdWithADoubleAndUnit("/LTAC/run/SetNaIEmax", this);
	SetNaIEmaxCmd -> SetGuidance("Set Maximum Energy for NaI spectrum");
	SetNaIEmaxCmd -> SetParameterName("SetNaIEmax",false);
	SetNaIEmaxCmd -> SetUnitCandidates("eV keV MeV GeV TeV");

	SetNaIBinsCmd = new G4UIcmdWithAnInteger("/LTAC/run/SetNaIBins", this);
	SetNaIBinsCmd -> SetGuidance("Set Number of Bins for LS Spectra");
	SetNaIBinsCmd -> SetParameterName("SetNaIBins",false);


	SetGate1LowCmd = new G4UIcmdWithADoubleAndUnit("/LTAC/run/SetGate1Low", this);
	SetGate1LowCmd -> SetGuidance("Set Gate 1 Lower Limit (value unit)");
	SetGate1LowCmd -> SetParameterName("SetG1Low",false);
	SetGate1LowCmd -> SetUnitCandidates("eV keV MeV GeV TeV");

	SetGate1HighCmd = new G4UIcmdWithADoubleAndUnit("/LTAC/run/SetGate1High", this);
	SetGate1HighCmd -> SetGuidance("Set Gate 1 Upper Limit (value unit)");
	SetGate1HighCmd -> SetParameterName("SetG1High",false);
	SetGate1HighCmd -> SetUnitCandidates("eV keV MeV GeV TeV");

	SetGate2LowCmd = new G4UIcmdWithADoubleAndUnit("/LTAC/run/SetGate2Low", this);
	SetGate2LowCmd -> SetGuidance("Set Gate 2 Lower Limit (value unit)");
	SetGate2LowCmd -> SetParameterName("SetG2Low",false);
	SetGate2LowCmd -> SetUnitCandidates("eV keV MeV GeV TeV");

	SetGate2HighCmd = new G4UIcmdWithADoubleAndUnit("/LTAC/run/SetGate2High", this);
	SetGate2HighCmd -> SetGuidance("Set Gate 2 Upper Limit (value unit)");
	SetGate2HighCmd -> SetParameterName("SetG2High",false);
	SetGate2HighCmd -> SetUnitCandidates("eV keV MeV GeV TeV");

	SetGate3LowCmd = new G4UIcmdWithADoubleAndUnit("/LTAC/run/SetGate3Low", this);
	SetGate3LowCmd -> SetGuidance("Set Gate 3 Lower Limit (value unit)");
	SetGate3LowCmd -> SetParameterName("SetG3Low",false);
	SetGate3LowCmd -> SetUnitCandidates("eV keV MeV GeV TeV");

	SetGate3HighCmd = new G4UIcmdWithADoubleAndUnit("/LTAC/run/SetGate3High", this);
	SetGate3HighCmd -> SetGuidance("Set Gate 3 Upper Limit (value unit)");
	SetGate3HighCmd -> SetParameterName("SetG3High",false);
	SetGate3HighCmd -> SetUnitCandidates("eV keV MeV GeV TeV");

	GetGate1Cmd = new G4UIcmdWithoutParameter("/LTAC/run/GetGate1", this);
	GetGate1Cmd -> SetGuidance("Get curret Gate1 setting");
	GetGate2Cmd = new G4UIcmdWithoutParameter("/LTAC/run/GetGate2", this);
	GetGate2Cmd -> SetGuidance("Get curret Gate2 setting");
	GetGate3Cmd = new G4UIcmdWithoutParameter("/LTAC/run/GetGate3", this);
	GetGate3Cmd -> SetGuidance("Get curret Gate3 setting");

	SetOkToSave2DCmd = new G4UIcmdWithABool("/LTAC/run/SetOkToSave2D",this);
	SetOkToSave2DCmd -> SetGuidance("True to save 2D spectrum");
	SetOkToSave2DCmd -> SetDefaultValue(true);
	SetOkToSave2DCmd -> SetParameterName("SetOktoSave2D",true);

	SetXoffsetCmd = new G4UIcmdWithADoubleAndUnit("/LTAC/run/SetXoffset",this);
	SetXoffsetCmd->SetParameterName("SetXoffset",false);
	SetXoffsetCmd->SetGuidance("Set x offset of center of source");
	SetXoffsetCmd -> SetUnitCandidates("mm cm m");

	SetYoffsetCmd = new G4UIcmdWithADoubleAndUnit("/LTAC/run/SetYoffset",this);
	SetYoffsetCmd->SetParameterName("SetYoffset",false);
	SetYoffsetCmd->SetGuidance("Set Y offset of center of source");
	SetYoffsetCmd -> SetUnitCandidates("mm cm m");

	SetZoffsetCmd = new G4UIcmdWithADoubleAndUnit("/LTAC/run/SetZoffset",this);
	SetZoffsetCmd->SetParameterName("SetZoffset",false);
	SetZoffsetCmd->SetGuidance("Set Z offset of center of source");
	SetZoffsetCmd -> SetUnitCandidates("mm cm m");

	SetDetectorGapCmd = new G4UIcmdWithADoubleAndUnit("/LTAC/run/SetDetectorGap",this);
	SetDetectorGapCmd->SetParameterName("SetDetectorGap",false);
	SetDetectorGapCmd->SetGuidance("Set gap between detectors");
	SetDetectorGapCmd -> SetUnitCandidates("mm cm m");

    SetMaterialCmd = new G4UIcmdWithAString("/LTAC/run/SetSourceMaterial",this);
    SetMaterialCmd->SetGuidance("Select source material");
    SetMaterialCmd->SetParameterName("SourceMaterial", false);
    SetMaterialCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
}

LTAC1RunActionMessenger::~LTAC1RunActionMessenger()
{

	delete SetGate1LowCmd;
	delete SetGate1HighCmd;
	delete SetGate2LowCmd;
	delete SetGate2HighCmd;
	delete SetGate3LowCmd;
	delete SetGate3HighCmd;

	delete GetGate1Cmd;
	delete GetGate2Cmd;
	delete GetGate3Cmd;

	delete SetOkToSave2DCmd;

	delete runDir;
	delete LTACDir;

	delete SetXoffsetCmd;
	delete SetYoffsetCmd;
	delete SetZoffsetCmd;
	delete SetDetectorGapCmd;
	delete SetMaterialCmd;
}

void LTAC1RunActionMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
	
	if ( command == SetNaIEmaxCmd ) // change Max Energy for LS Spectra
	{myRun->SetNaIEmax(SetNaIEmaxCmd->GetNewDoubleValue(newValue));}
	if ( command == SetNaIEmaxCmd ) // change Max Energy for NaI Spectra
	{myRun->SetNaIEmax(SetNaIEmaxCmd->GetNewDoubleValue(newValue));}
	if ( command == SetNaIBinsCmd ) // Change number of bins for LS spectra
	{myRun->SetNaIBins(SetNaIBinsCmd->GetNewIntValue(newValue));}
	if ( command == SetNaIBinsCmd ) // Change number of bins for NaI spectra
	{myRun->SetNaIBins(SetNaIBinsCmd->GetNewIntValue(newValue));}
	if ( command == SetOkToSave2DCmd ) // Set whether or not to save 2D spectra
	{myRun->SetOkToSave2D( SetOkToSave2DCmd->GetNewBoolValue(newValue));}


	if ( command == SetGate1LowCmd ) // change Gate 1 lower limit
		{myRun->SetG1(SetGate1LowCmd->GetNewDoubleValue(newValue),myRun->GetG1High());}
	if ( command == SetGate1HighCmd ) // change Gate 1 upper limit
		{myRun->SetG1(myRun->GetG1Low(),SetGate1HighCmd->GetNewDoubleValue(newValue));}

	if ( command == SetGate2LowCmd ) 
		{myRun->SetG2(SetGate2LowCmd->GetNewDoubleValue(newValue),myRun->GetG2High());}
	if ( command == SetGate2HighCmd ) 
		{myRun->SetG2(myRun->GetG2Low(),SetGate2HighCmd->GetNewDoubleValue(newValue));}

	if ( command == SetGate3LowCmd ) // change Gate 3 lower limit
		{myRun->SetG3(SetGate3LowCmd->GetNewDoubleValue(newValue),myRun->GetG3High());}
	if ( command == SetGate3HighCmd ) // change Gate 3 upper limit
		{myRun->SetG3(myRun->GetG3Low(),SetGate3HighCmd->GetNewDoubleValue(newValue));}

	if ( command == GetGate1Cmd ) // print Gate 1 settings to G4cout
		{G4cout << "Gate 1: (MeV) " << myRun->GetG1Low() << ", "<<myRun->GetG1High() << G4endl;}
	if ( command == GetGate2Cmd )
		{G4cout << "Gate 2: (MeV) " << myRun->GetG2Low() << ", "<<myRun->GetG2High() << G4endl;}
	if ( command == GetGate3Cmd )
		{G4cout << "Gate 3: (MeV) " << myRun->GetG3Low() << ", "<<myRun->GetG3High() << G4endl;}

	if ( command == SetXoffsetCmd ) // change source position from center
	{myRun->SetXoffset(SetXoffsetCmd->GetNewDoubleValue(newValue));}

	if ( command == SetYoffsetCmd ) // change source position from center
	{myRun->SetYoffset(SetYoffsetCmd->GetNewDoubleValue(newValue));}

	if ( command == SetZoffsetCmd ) // change source position from center
	{myRun->SetZoffset(SetZoffsetCmd->GetNewDoubleValue(newValue));}

	if ( command == SetDetectorGapCmd ) // change gap between detectors. Centered on zero.
	{myRun->SetDetectorGap(SetDetectorGapCmd->GetNewDoubleValue(newValue));}

	if ( command == SetMaterialCmd ) // change material name
	{myRun->SetMaterial(newValue);}
}
