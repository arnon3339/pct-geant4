//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
/// \file PCT/include/DetectorConstruction.hh
/// \brief Definition of the PCT::DetectorConstruction class

#ifndef PCTDetectorConstruction_h
#define PCTDetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "G4RotationMatrix.hh"
#include "G4SystemOfUnits.hh"
#include "globals.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;

/// Detector construction class to define materials and geometry.

namespace PCT
{
class DetectorMessager;
class PhantomConstruction;

class DetectorConstruction : public G4VUserDetectorConstruction
{
  private:
    DetectorMessager* detMessager;
    G4LogicalVolume* phanLog;
    G4double phAngle;
    G4LogicalVolume* worldLog;
    G4VPhysicalVolume* phPhys;

  public:
    DetectorConstruction(G4String);
    ~DetectorConstruction();

    G4VPhysicalVolume* Construct() override;
    void ConstructSDandField() override;

    void RotatePhantom(G4double);
    G4double GetPHangle() {return phAngle;};
};

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
