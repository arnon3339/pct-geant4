#include "RunMessager.hh"
#include "RunAction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcommand.hh"

namespace PCT
{

RunMessager::RunMessager(RunAction* run): fRun(run)
{
    fDir = new G4UIdirectory("/run/");
    fDir->SetGuidance("Seting for run action.");

    fSubDir = new G4UIdirectory("/run/file/");
    fSubDir->SetGuidance("Setting for file action in run action.");

    fCloseFileCmd = new G4UIcmdWithABool("/run/file/close", this);
    fCloseFileCmd->SetGuidance("Input boolean to close output file.");
    fCloseFileCmd->SetParameterName("Close(Boolean)", false);
    fCloseFileCmd->SetDefaultValue(false);
    fCloseFileCmd->AvailableForStates(G4State_PreInit,G4State_Idle);    

    fOpenFileCmd = new G4UIcmdWithABool("/run/file/open", this);
    fOpenFileCmd->SetGuidance("Input boolean to open output file.");
    fOpenFileCmd->SetParameterName("Close(Boolean)", false);
    fOpenFileCmd->SetDefaultValue(false);
    fOpenFileCmd->AvailableForStates(G4State_PreInit,G4State_Idle);    

    fMaxLengthCmd = new G4UIcmdWithADoubleAndUnit("/run/file/maxLength", this);
    fMaxLengthCmd->SetGuidance("Input maxlength step to close a file.");
    fMaxLengthCmd->SetParameterName("MaxLength(DoubleWithUnit)", false);
    fMaxLengthCmd->SetDefaultValue(240.0 *mm);
    fMaxLengthCmd->AvailableForStates(G4State_PreInit,G4State_Idle);    
}

RunMessager::~RunMessager()
{
    delete fDir;
    delete fSubDir;
    delete fCloseFileCmd;
    delete fOpenFileCmd;
    delete fMaxLengthCmd;
}

void RunMessager::SetNewValue(G4UIcommand* cmd, G4String newValue)
{
    if (cmd == fOpenFileCmd)
    {
        fRun->SetOpenFile(fOpenFileCmd->GetNewBoolValue(newValue));
    }
    if (cmd == fCloseFileCmd)
    {
        fRun->SetCloseFile(fCloseFileCmd->GetNewBoolValue(newValue));
    }
    if (cmd == fMaxLengthCmd)
    {
        fRun->SetMaxLength(fMaxLengthCmd->GetNewDoubleValue(newValue));
    }
}
    
} // namespace PCT
