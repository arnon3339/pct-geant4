#include "RunMessager.hh"
#include "RunAction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcommand.hh"

namespace PCT
{

RunMessager::RunMessager(RunAction* run): _run(run)
{
    _dir = new G4UIdirectory("/run/");
    _dir->SetGuidance("Seting for run action.");

    _sub_dir = new G4UIdirectory("/run/file/");
    _sub_dir->SetGuidance("Setting for file action in run action.");

    _close_file_cmd = new G4UIcmdWithABool("/run/file/close", this);
    _close_file_cmd->SetGuidance("Input boolean to close output file.");
    _close_file_cmd->SetParameterName("Close(Boolean)", false);
    _close_file_cmd->SetDefaultValue(false);
    _close_file_cmd->AvailableForStates(G4State_PreInit,G4State_Idle);    
}

RunMessager::~RunMessager()
{
    delete _dir;
    delete _sub_dir;
    delete _close_file_cmd;
}

void RunMessager::SetNewValue(G4UIcommand* cmd, G4String newValue)
{

}
    
} // namespace PCT
