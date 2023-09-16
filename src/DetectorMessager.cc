#include "DetectorMessager.hh"

#include "DetectorConstruction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcommand.hh"
#include "G4SystemOfUnits.hh"

namespace PCT
{
  DetectorMessager::DetectorMessager(DetectorConstruction* det)
  :fdet(det), fDirectory(0), fSetAngleCmd(0)
  {
    fDirectory = new G4UIdirectory("/det/");
    fDirectory->SetGuidance("Dector geometry control");

    fSubDirectory = new G4UIdirectory("/det/rotate/");
    fSubDirectory->SetGuidance("Dector geometry rotation");
        
    fSetAngleCmd 
      = new G4UIcmdWithADouble("/det/rotate/angle",this);
    fSetAngleCmd->SetGuidance("Input the angle (degree) for phantom rotation in TOP axis.");
    fSetAngleCmd->SetParameterName("Angle(Degree)", false);
    fSetAngleCmd->AvailableForStates(G4State_PreInit,G4State_Idle);    
  }

  DetectorMessager::~DetectorMessager()
  {
    delete fDirectory;
    delete fSubDirectory;
    delete fSetAngleCmd;
  }

  void DetectorMessager::SetNewValue(G4UIcommand* command, G4String newValue)
  {
    if( command == fSetAngleCmd ) {
      fdet->RotatePhantom(std::stod(newValue));
    }
  }
} // namespace PCT
