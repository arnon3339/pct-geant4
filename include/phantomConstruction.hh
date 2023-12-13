#ifndef PHANTOMCONSTRUCTION_HH
#define PHANTOMCONSTRUCTION_HH 1

#include <map>
#include <string>
#include "CADMesh.hh"
#include "G4SystemOfUnits.hh"

class G4LogicalVolume;
class G4Material;

namespace PCT
{
  class PhantomConstruction
  {
  private:
    void genMat(std::map<std::string, G4Material*> &mat);
    std::map<std::string, G4Material*> phanMat;
    G4LogicalVolume* phLV;
    const std::array<const std::string, 5> refPhantoms = {
    "catphan404",
    "brain",
    "heart",
    "lung",
    "test"
};
    /* data */
  public:
    PhantomConstruction(G4String);
    ~PhantomConstruction();

    inline G4LogicalVolume *GetLogVolume(){
      return phLV;
    }
  };
} // namespace PCT

#endif