#ifndef StackingActionMessenger_h
#define StackingActionMessenger_h 1

// Allow user to set a maximun track time, after the initial ion decays

#include "globals.hh"
#include "G4UImessenger.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "LTAC1StackingAction.hh"

class LTAC1StackingAction;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithADouble;
class G4UIcmdWihtoutParameter;
class G4UIdirectory;

class LTAC1StackingActionMessenger: public G4UImessenger
{
  public:
    LTAC1StackingActionMessenger(LTAC1StackingAction*);
   ~LTAC1StackingActionMessenger();
    
    void SetNewValue(G4UIcommand*, G4String);
    
  private:

    LTAC1StackingAction* StackAction;
	G4UIdirectory* stackDir;
    G4UIcmdWithADoubleAndUnit* SetMaxTrackTimeCmd;
	G4UIcmdWithoutParameter* GetMaxTrackTimeCmd;

};



#endif