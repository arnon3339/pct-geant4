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
/// \file PCT/src/DetectorConstruction.cc
/// \brief Implementation of the PCT::DetectorConstruction class

#include "DetectorConstruction.hh"

#include "phantomConstruction.hh"
#include "TrackerSD.hh"
#include "DetectorMessager.hh"

#include "G4RotationMatrix.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4SDManager.hh"
#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4VSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"
#include "G4PVReplica.hh"
#include "G4Color.hh"
#include "G4Region.hh"

#include "CADMesh.hh"

#include <vector>
#include <string>
#include <fstream>
#include <filesystem>

namespace PCT
{
  DetectorConstruction::DetectorConstruction(G4String phName)
  :detMessager(0), phanLog(0), phAngle(0), worldLog(0), phPhys(0)
  {
    auto phantom = new PhantomConstruction(phName);
    phanLog = phantom->GetLogVolume();
    detMessager = new DetectorMessager(this);
  }

  DetectorConstruction::~DetectorConstruction()
  {
    delete phPhys;
    delete phanLog;
    delete detMessager;
  }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4VPhysicalVolume* DetectorConstruction::Construct()
{
  // Get nist material manager
  G4NistManager* nist = G4NistManager::Instance();

  // Envelope parameters
  //
  G4double env_sizeXY = 60*cm, env_sizeZ = 80*cm;
  // G4Material* env_mat = nist->FindOrBuildMaterial("G4_WATER");

  // Option to switch on/off checking of volumes overlaps
  //
  G4bool checkOverlaps = true;

  //
  // World
  //
  G4double world_sizeXY = 1.2*env_sizeXY;
  G4double world_sizeZ  = 1.2*env_sizeZ;
  G4Material* world_mat = nist->FindOrBuildMaterial("G4_AIR");
  G4Material* m_silicon = nist->FindOrBuildMaterial("G4_Si");
  // G4Material* m_vacuum = nist->FindOrBuildMaterial("G4_Galactic");

  auto solidWorld = new G4Box("World",                           // its name
    0.5 * world_sizeXY, 0.5 * world_sizeXY, 0.5 * world_sizeZ);  // its size

  worldLog = new G4LogicalVolume(solidWorld,  // its solid
    world_mat,                                       // its material
    "World");                                        // its name

  auto physWorld = new G4PVPlacement(nullptr,  // no rotation
    G4ThreeVector(),                           // at (0,0,0)
    worldLog,                                // its logical volume
    "World",                                   // its name
    nullptr,                                   // its mother  volume
    false,                                     // no boolean operation
    0,                                         // copy number
    checkOverlaps);                            // overlaps checking

  G4ThreeVector frontTrackerPos = G4ThreeVector(0, 0, -22.5*cm);
  G4ThreeVector rearTrackerPos = G4ThreeVector(0, 0, 22.5*cm);
  G4int numAlpideX = 16;
  G4int numAlpideY = 16;
  G4double alpideSizeX = 3.0 *cm;
  G4double alpideSizeY = 1.38 *cm;
  G4double alpideSizeZ = 100 *um;
  G4double detectorSizeX = (numAlpideX*alpideSizeX);
  G4double detectorSizeY = (numAlpideY*alpideSizeY);
  // G4double detectorSizeZ = 1*cm;

  auto frontTrackerSol = new G4Box("frontTrackSol", detectorSizeX/2, detectorSizeY/2, 2 *cm);
  auto rearTrackerSol = new G4Box("rearTrackSol", detectorSizeX/2, detectorSizeY/2, 2 *cm);

  auto frontTrackerLog = new G4LogicalVolume(
      frontTrackerSol,
      world_mat,
      "frontTrackerLog"
    );
  auto rearTrackerLog = new G4LogicalVolume(
      rearTrackerSol,
      world_mat,
      "rearTrackerLog"
    );
  
  new G4PVPlacement(nullptr,
    frontTrackerPos,
    frontTrackerLog,
    "frontTrackerPhys",
    worldLog,
    false,
    0,
    checkOverlaps
    );

  new G4PVPlacement(nullptr,
    rearTrackerPos,
    rearTrackerLog,
    "rearTrackerPhys",
    worldLog,
    false,
    0,
    checkOverlaps
    );

    auto trackerVis = new G4VisAttributes();
    trackerVis->SetVisibility(false);
    trackerVis->SetColor(0, 1.0, 1.0, 0.1);
    frontTrackerLog->SetVisAttributes(trackerVis);
    rearTrackerLog->SetVisAttributes(trackerVis);

    auto alpideSol = new G4Box("alpideSol", detectorSizeX/2, detectorSizeY/2, alpideSizeZ/2);

    auto alpideLog = new G4LogicalVolume(
      alpideSol,
      m_silicon,
      "alpideLog"
    );
    auto alpideVis = new G4VisAttributes();
    alpideVis->SetVisibility(true);
    alpideVis->SetColor(G4Color(0.8, 0.4, 0.4, 1.0));
    alpideLog->SetVisAttributes(alpideVis);

    for (int i = 0; i <4; i++)
    {
      if (static_cast<uint8_t>(i/2) == 0)
        new G4PVPlacement(
          nullptr,
          G4ThreeVector(0, 0, ((i%2)*2.5 - 1.25) *cm),
          alpideLog,
          "alpidePhys",
          frontTrackerLog,
          true,
          i,
          checkOverlaps
        );
      else
        new G4PVPlacement(
          nullptr,
          G4ThreeVector(0, 0, ((i%2)*2.5 - 1.25) *cm),
          alpideLog,
          "alpidePhys",
          rearTrackerLog,
          true,
          i,
          checkOverlaps
        );
    }

    new G4PVPlacement(
        nullptr,
        G4ThreeVector(0, 0, 30.0 *cm),
        alpideLog,
        "alpideCal",
        worldLog,
        true,
        4,
        checkOverlaps
      );

    // G4int numberOfpixelX = 1024;
    // G4int numberOfpixelY = 512;
    // G4double pixSizeX = alpideSizeX/numberOfpixelX;
    // G4double pixSizeY = alpideSizeY/numberOfpixelY;

    // auto pixSol = new G4Box("pixSol", pixSizeX/2, pixSizeY/2, alpideSizeZ/2);

    // auto pixXLog = new G4LogicalVolume(
    //   pixSol,
    //   m_silicon,
    //   "pixLog"
    //   );
    
    // auto pixXVis = new G4VisAttributes();
    // pixXVis->SetVisibility(true);
    // pixXVis->SetColor(G4Color(1.0, 0, 0, 1.0));
    // pixXLog->SetVisAttributes(pixXVis);

    // new G4PVReplica(
    //   "pixXPhys",
    //   pixXLog,
    //   alpideLog,
    //   kXAxis,
    //   numberOfpixelX*numAlpideX,
    //   pixSizeX,
    //   0
    // );

    // auto pixYLog = new G4LogicalVolume(
    //   pixSol,
    //   m_silicon,
    //   "pixLog"
    //   );
    
    // auto pixYVis = new G4VisAttributes();
    // pixYVis->SetVisibility(true);
    // pixYVis->SetColor(G4Color(1.0, 0, 0, 1.0));
    // pixYLog->SetVisAttributes(pixYVis);

    // new G4PVReplica(
    //   "pixYPhys",
    //   pixYLog,
    //   pixXLog,
    //   kYAxis,
    //   numberOfpixelY*numAlpideY,
    //   pixSizeY,
    //   0
    // );

    //
    // Shape 1
    //
    // G4Material* shape1_mat = nist->FindOrBuildMaterial("G4_A-150_TISSUE");
    // G4ThreeVector pos1 = G4ThreeVector(0, 2*cm, -7*cm);

    // // Conical section shape
    // G4double shape1_rmina =  0.*cm, shape1_rmaxa = 2.*cm;
    // G4double shape1_rminb =  0.*cm, shape1_rmaxb = 4.*cm;
    // G4double shape1_hz = 3.*cm;
    // G4double shape1_phimin = 0.*deg, shape1_phimax = 360.*deg;
    // auto solidShape1 = new G4Cons("Shape1", shape1_rmina, shape1_rmaxa, shape1_rminb, shape1_rmaxb,
    //   shape1_hz, shape1_phimin, shape1_phimax);

    // auto logicShape1 = new G4LogicalVolume(solidShape1,  // its solid
    //   shape1_mat,                                        // its material
    //   "Shape1");                                         // its name

    // new G4PVPlacement(nullptr,  // no rotation
    //   pos1,                     // at position
    //   logicShape1,              // its logical volume
    //   "Shape1",                 // its name
    //   logicEnv,                 // its mother  volume
    //   false,                    // no boolean operation
    //   0,                        // copy number
    //   checkOverlaps);           // overlaps checking



    //
    // Shape 2
    //
    // G4Material* shape2_mat = nist->FindOrBuildMaterial("G4_BONE_COMPACT_ICRU");
    // G4ThreeVector pos2 = G4ThreeVector(0, -1*cm, 7*cm);

    // // Trapezoid shape
    // G4double shape2_dxa = 12*cm, shape2_dxb = 12*cm;
    // G4double shape2_dya = 10*cm, shape2_dyb = 16*cm;
    // G4double shape2_dz  = 6*cm;
    // auto solidShape2 = new G4Trd("Shape2",  // its name
    //   0.5 * shape2_dxa, 0.5 * shape2_dxb, 0.5 * shape2_dya, 0.5 * shape2_dyb,
    //   0.5 * shape2_dz);  // its size

    // auto logicShape2 = new G4LogicalVolume(solidShape2,  // its solid
    //   shape2_mat,                                        // its material
    //   "Shape2");                                         // its name

    // new G4PVPlacement(nullptr,  // no rotation
    //   pos2,                     // at position
    //   logicShape2,              // its logical volume
    //   "Shape2",                 // its name
    //   logicEnv,                 // its mother  volume
    //   false,                    // no boolean operation
    //   0,                        // copy number
    //   checkOverlaps);           // overlaps checking

    // Set Shape2 as scoring volume
    //
    auto phReg = new G4Region("myPhantom");
    phReg->AddRootLogicalVolume(phanLog);

    // for aligned phantom
    phAngle = 90 *deg;
    auto rMatrix = new G4RotationMatrix();;
    rMatrix->rotateY(phAngle);

    phPhys = new G4PVPlacement(
      rMatrix,
      G4ThreeVector(0, 0, 0),
      phanLog,
      "phantomPhys",
      worldLog,
      false,
      0,
      checkOverlaps
    );

    //
    //always return the physical World
    //
    return physWorld;
  }

  void DetectorConstruction::RotatePhantom(G4double angle)
  {
    auto runManager = G4RunManager::GetRunManager();
    auto rMatrix = new G4RotationMatrix();;
    phAngle = angle;
    rMatrix->rotateY(phAngle); // Specify the rotation angle in radians

    phPhys->SetRotation(rMatrix);

    runManager->GeometryHasBeenModified();

    // auto angleFileLogs = new std::ofstream();
    // angleFileLogs->open("./anglelogs.txt", std::fstream::app);
    // (*angleFileLogs) << (int)(phAngle*180/3.14) << std::endl;
    // angleFileLogs->close();
  }

  void DetectorConstruction::ConstructSDandField()
  {
    // Sensitive detectors

    G4String trackerChamberSDname = "/TrackerChamberSD";
    TrackerSD* aTrackerSD = new TrackerSD(trackerChamberSDname,
                                              "TrackerHitsCollection");
    G4SDManager::GetSDMpointer()->AddNewDetector(aTrackerSD);
    // Setting aTrackerSD to all logical volumes with the same name
    // of "Chamber_LV".
    SetSensitiveDetector("alpideLog", aTrackerSD, true);

  }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

}
