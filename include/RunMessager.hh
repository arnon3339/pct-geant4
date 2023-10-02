#ifndef RUNMESSAGER_HH
#define RUNMESSAGER_HH 1

#include "G4UImessenger.hh"

class G4UIdirectory;
class G4UIcmdWithABool;
class G4UIcommand;
class G4UIcmdWithADoubleAndUnit;

namespace PCT
{

class RunAction;

class RunMessager: public G4UImessenger
{
private:
    RunAction* fRun;

    G4UIdirectory* fDir;
    G4UIdirectory* fSubDir;
    G4UIcmdWithABool* fCloseFileCmd;
    G4UIcmdWithABool* fOpenFileCmd;
    G4UIcmdWithADoubleAndUnit* fMaxLengthCmd;

public:
    RunMessager(RunAction*);
    ~RunMessager();

    virtual void SetNewValue(G4UIcommand*, G4String);
};

    
} // namespace PCT



#endif