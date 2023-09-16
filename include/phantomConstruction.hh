#ifndef PHANTOMCONSTRUCTION_HH
#define PHANTOMCONSTRUCTION_HH 1

#include <map>
#include <string>

class G4LogicalVolume;
class G4Material;

namespace PCT
{
  class PhantomConstruction
  {
  private:
    void genMat(std::map<std::string, G4Material*> &mat);
    std::map<std::string, G4Material*> phanMat;
    /* data */
  public:
    PhantomConstruction(/* args */) = default;
    ~PhantomConstruction() = default;

    G4LogicalVolume *getLogVolume();
  };
} // namespace PCT

#endif