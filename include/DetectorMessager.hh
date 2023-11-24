#ifndef DETECTORMESSAGER_HH
#define DETECTORMESSAGER_HH 1

#include "G4UImessenger.hh"

class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWith3VectorAndUnit;
class G4UIcmdWithADoubleAndUnit;
class G4UIcommand;

namespace PCT
{
  class DetectorConstruction;
  class DetectorMessager: public G4UImessenger
    {
    private:
      DetectorConstruction* fdet;
      G4UIdirectory*             fDirectory;
      G4UIdirectory*             fSubDirectory;
      G4UIcmdWithADoubleAndUnit*        fSetAngleCmd;

    public:
      DetectorMessager(DetectorConstruction*);
      ~DetectorMessager();

      virtual void SetNewValue(G4UIcommand*, G4String);
    };

  
} // namespace PCT

#endif