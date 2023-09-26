#ifndef RUNMESSAGER_HH
#define RUNMESSAGER_HH 1

#include "G4UImessenger.hh"

class G4UIdirectory;
class G4UIcmdWithABool;
class G4UIcommand;

namespace PCT
{

class RunAction;

class RunMessager: public G4UImessenger
{
private:
    RunAction* _run;

    G4UIdirectory* _dir;
    G4UIdirectory* _sub_dir;
    G4UIcmdWithABool* _close_file_cmd;

public:
    RunMessager(RunAction*);
    ~RunMessager();

    virtual void SetNewValue(G4UIcommand*, G4String);
};

    
} // namespace PCT



#endif