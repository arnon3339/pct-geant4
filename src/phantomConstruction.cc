#include "phantomConstruction.hh"

#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4NistManager.hh"
#include "G4VisAttributes.hh"
#include "G4VPhysicalVolume.hh"
#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"
#include "G4Element.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4RotationMatrix.hh"

#include "CADMesh.hh"

#include <map>
#include <string>

namespace PCT
{
  PhantomConstruction::~PhantomConstruction(){
    delete phLV;
  }

  PhantomConstruction::PhantomConstruction(G4String phName):
  fPhIndex(0), fPhName(phName){
    genMat(phanMat);

    G4bool checkOverlaps = true;

    auto nonVis = new G4VisAttributes();
    nonVis->SetVisibility(false);
    G4VisAttributes *C0vis = new G4VisAttributes();
    C0vis->SetVisibility(true);
    C0vis->SetColor(G4Color(0, 1.0, 0, 0.75));

    G4VisAttributes *C1vis = new G4VisAttributes();
    C1vis->SetVisibility(true);
    C1vis->SetColor(G4Color(1.0, 0, 0, 0.75));

    if (!phName.compare(refPhantoms.at(0))){
      fPhIndex = 0;
      auto mesh = CADMesh::TessellatedMesh::FromOBJ("./phantom/ctp404.obj");
      mesh->SetScale(1000);
      G4ThreeVector pos1_5 = G4ThreeVector(0, 0, 0);
      G4LogicalVolume *C0log = new G4LogicalVolume(
        mesh->GetSolid("C0"),
        phanMat["Water"],
        "C0log"
      );
      C0log->SetVisAttributes(C0vis);

      G4LogicalVolume *C1log = new G4LogicalVolume(
        mesh->GetSolid("C1"),
        phanMat["AcrylicLow"],
        "C1log"
      );
      C1log->SetVisAttributes(C1vis);
      new G4PVPlacement(nullptr,  // no rotation
        G4ThreeVector(0, 0, 0),                     // at position
        C1log,              // its logical volume
        "C1phys",                 // its name
        C0log,                 // its mother  volume
        false,                    // no boolean operation
        0,                        // copy number
        checkOverlaps);           // overlaps checking

      std::map<std::string, G4Material*> objMat = {
        {"C0", phanMat["Water"]},
        {"C1", phanMat["AcrylicLow"]},
        {"sen_acrylic", phanMat["Acrylic"]},
        {"sen_air", phanMat["Air"]},
        {"sen_delrin", phanMat["Delrin"]},
        {"sen_ldpe", phanMat["LDPE"]},
        {"sen_pmp", phanMat["PMP"]},
        {"sen_polystyrene", phanMat["Polystyrene"]},
        {"sen_teflon", phanMat["Teflon"]},
        {"sen_water", phanMat["Water"]},
        {"pix13", phanMat["Air"]},
        {"pix4", phanMat["Teflon"]},
        {"cen1", phanMat["Acrylic"]},
        {"cen2", phanMat["Acrylic"]},
        {"cen3", phanMat["Acrylic"]},
        {"cen4", phanMat["Acrylic"]},
        {"cen5", phanMat["Acrylic"]},
        {"ramp", phanMat["Ramp"]}
      };

      G4String logName = "";
      G4String physName = "";
      for (size_t i = 0; i < mesh->GetSolids().size(); i++)
      {
        if (!mesh->GetSolid(i)->GetName().compare("C0") || !mesh->GetSolid(i)->GetName().compare("C1"))
          continue;
        logName = mesh->GetSolid(i)->GetName()  + std::string("Log");
        auto logicShape1_5 = new G4LogicalVolume(mesh->GetSolid(i),  // its solid
          objMat[mesh->GetSolid(i)->GetName()],                                        // its material
          logName);                                         // its name

        physName = mesh->GetSolid(i)->GetName() + std::string("Phys");

        new G4PVPlacement(nullptr,  // no rotation
          G4ThreeVector(0, 0, 0),                     // at position
          logicShape1_5,              // its logical volume
          physName,                 // its name
          C1log,                 // its mother  volume
          false,                    // no boolean operation
          0,                        // copy number
          checkOverlaps);           // overlaps checking
      }
      phLV = C0log;
    }
    else if (!phName.compare(refPhantoms.at(1))){
      fPhIndex = 1;
      // heart phantom
      auto brainMesh = CADMesh::TessellatedMesh::FromOBJ("./phantom/brain.obj");
      brainMesh->SetScale(100);
      auto brainLV = new G4LogicalVolume(brainMesh->GetSolid(0), phanMat["brain"], "brainhLV");
      phLV = brainLV;
    }
    else if (!phName.compare(refPhantoms.at(2))){
      fPhIndex = 2;
      // heart phantom
      auto heartMesh = CADMesh::TessellatedMesh::FromOBJ("./phantom/heart.obj");
      heartMesh->SetScale(100);
      auto heartLV = new G4LogicalVolume(heartMesh->GetSolid(0), phanMat["heartwall"], "heartLV");
      phLV = heartLV;
    }
    else if (!phName.compare(refPhantoms.at(3))){
      fPhIndex = 3;
      // lung phantom
      auto lungMesh = CADMesh::TessellatedMesh::FromOBJ("./phantom/lung.obj");
      lungMesh->SetScale(100);
      auto lungLV = new G4LogicalVolume(lungMesh->GetSolid(0), phanMat["lung"], "lungLV");
      phLV = lungLV;
    }
    else if (!phName.compare(refPhantoms.at(4))){
      fPhIndex = 4;
      // lung phantom
      auto headMesh = CADMesh::TessellatedMesh::FromOBJ("./phantom/phantom_inner_center_male_head_2.obj");
      headMesh->SetScale(100);
      auto headLV = new G4LogicalVolume(headMesh->GetSolid(0), phanMat["lung"], "lungLV");
      phLV = headLV;
    }
    else{
      fPhIndex = 5;
      // small phantom
      auto smPhWrapS = new G4Box("smPhWrapS", 12 *cm, 12 *cm, 12 *cm);
      auto smPhWrapLV = new G4LogicalVolume(smPhWrapS, phanMat["Air"], "smPhWrapLV");
      smPhWrapLV->SetVisAttributes(nonVis);
      auto smPhS = new G4Tubs("smPhS", 0, 10.0 *cm, 5.0 *cm, 0 *deg, 360 *deg);
      auto smPhLV = new G4LogicalVolume(smPhS, phanMat["Water"], "smPhLV");
      smPhLV->SetVisAttributes(C0vis);
      // auto smPhInnerS = new G4Box("smPhInnerS", 8 *cm, 2 *cm, 2 *cm);
      // auto smPhInnerLV = new G4LogicalVolume(smPhInnerS, phanMat["Teflon"], "smPHInnerLV");
      // smPhInnerLV->SetVisAttributes(C1vis);

      auto rMatrix = new G4RotationMatrix();
      rMatrix->rotateX(90 *deg);

      new G4PVPlacement(
        rMatrix, 
        G4ThreeVector(0, 0, 0),
        smPhLV, 
        "smPhWrapPV",
        smPhWrapLV,
        false,
        0,
        checkOverlaps
      );

      // new G4PVPlacement(
      //   nullptr,
      //   G4ThreeVector(0, 0, 0),
      //   smPhInnerLV,
      //   "smPhPV",
      //   smPhLV,
      //   false,
      //   0,
      //   checkOverlaps 
      // );
    phLV = smPhWrapLV;
    }
  }

  // G4LogicalVolume* PhantomConstruction::GetLogVolume()
  // {
  //   G4bool checkOverlaps = true;
  //   auto mesh = CADMesh::TessellatedMesh::FromOBJ("./phantom/ctp404.obj");
  //   auto brainMesh = CADMesh::TessellatedMesh::FromOBJ("./phantom/phantom_male_brain_center.obj");
  //   auto heartMesh = CADMesh::TessellatedMesh::FromOBJ("./phantom/phantom_male_heart_center.obj");
  //   auto lungMesh = CADMesh::TessellatedMesh::FromOBJ("./phantom/phantom_male_lung_center.obj");
  //   G4ThreeVector pos1_5 = G4ThreeVector(0, 0, 0);

  //   auto nonVis = new G4VisAttributes();
  //   nonVis->SetVisibility(false);

  //   G4VisAttributes *C0vis = new G4VisAttributes();
  //   C0vis->SetVisibility(true);
  //   C0vis->SetColor(G4Color(0, 1.0, 0, 0.75));
  //   G4LogicalVolume *C0log = new G4LogicalVolume(
  //     mesh->GetSolid("C0"),
  //     phanMat["Water"],
  //     "C0log"
  //   );
  //   C0log->SetVisAttributes(C0vis);

  //   G4VisAttributes *C1vis = new G4VisAttributes();
  //   C1vis->SetVisibility(true);
  //   C1vis->SetColor(G4Color(1.0, 0, 0, 0.75));
  //   G4LogicalVolume *C1log = new G4LogicalVolume(
  //     mesh->GetSolid("C1"),
  //     phanMat["AcrylicLow"],
  //     "C1log"
  //   );
  //   C1log->SetVisAttributes(C1vis);

  //   new G4PVPlacement(nullptr,  // no rotation
  //     G4ThreeVector(0, 0, 0),                     // at position
  //     C1log,              // its logical volume
  //     "C1phys",                 // its name
  //     C0log,                 // its mother  volume
  //     false,                    // no boolean operation
  //     0,                        // copy number
  //     checkOverlaps);           // overlaps checking

  //   std::map<std::string, G4Material*> objMat = {
  //     {"C0", phanMat["Water"]},
  //     {"C1", phanMat["AcrylicLow"]},
  //     {"sen_acrylic", phanMat["Acrylic"]},
  //     {"sen_air", phanMat["Air"]},
  //     {"sen_delrin", phanMat["Delrin"]},
  //     {"sen_ldpe", phanMat["LDPE"]},
  //     {"sen_pmp", phanMat["PMP"]},
  //     {"sen_polystyrene", phanMat["Polystyrene"]},
  //     {"sen_teflon", phanMat["Teflon"]},
  //     {"sen_water", phanMat["Water"]},
  //     {"pix13", phanMat["Air"]},
  //     {"pix4", phanMat["Teflon"]},
  //     {"cen1", phanMat["Acrylic"]},
  //     {"cen2", phanMat["Acrylic"]},
  //     {"cen3", phanMat["Acrylic"]},
  //     {"cen4", phanMat["Acrylic"]},
  //     {"cen5", phanMat["Acrylic"]},
  //     {"ramp", phanMat["Ramp"]}
  //   };

  //   G4String logName = "";
  //   G4String physName = "";
  //   for (size_t i = 0; i < mesh->GetSolids().size(); i++)
  //   {
  //     if (!mesh->GetSolid(i)->GetName().compare("C0") || !mesh->GetSolid(i)->GetName().compare("C1"))
  //       continue;
  //     logName = mesh->GetSolid(i)->GetName()  + std::string("Log");
  //     auto logicShape1_5 = new G4LogicalVolume(mesh->GetSolid(i),  // its solid
  //       objMat[mesh->GetSolid(i)->GetName()],                                        // its material
  //       logName);                                         // its name

  //     physName = mesh->GetSolid(i)->GetName() + std::string("Phys");

  //     new G4PVPlacement(nullptr,  // no rotation
  //       G4ThreeVector(0, 0, 0),                     // at position
  //       logicShape1_5,              // its logical volume
  //       physName,                 // its name
  //       C1log,                 // its mother  volume
  //       false,                    // no boolean operation
  //       0,                        // copy number
  //       checkOverlaps);           // overlaps checking
  //   }

  //   // small phantom
  //   auto smPhWrapS = new G4Box("smPhWrapS", 12 *cm, 12 *cm, 12 *cm);
  //   auto smPhWrapLV = new G4LogicalVolume(smPhWrapS, phanMat["Air"], "smPhWrapLV");
  //   smPhWrapLV->SetVisAttributes(nonVis);
  //   auto smPhS = new G4Tubs("smPhS", 0, 10.0 *cm, 5.0 *cm, 0 *deg, 360 *deg);
  //   auto smPhLV = new G4LogicalVolume(smPhS, phanMat["Water"], "smPhLV");
  //   smPhLV->SetVisAttributes(C0vis);
  //   auto smPhInnerS = new G4Box("smPhInnerS", 8 *cm, 2 *cm, 2 *cm);
  //   auto smPhInnerLV = new G4LogicalVolume(smPhInnerS, phanMat["Teflon"], "smPHInnerLV");
  //   smPhInnerLV->SetVisAttributes(C1vis);

  //   auto rMatrix = new G4RotationMatrix();
  //   rMatrix->rotateX(90 *deg);

  //   new G4PVPlacement(
  //     rMatrix, 
  //     G4ThreeVector(0, 0, 0),
  //     smPhLV, 
  //     "smPhWrapPV",
  //     smPhWrapLV,
  //     false,
  //     0,
  //     checkOverlaps
  //   );

  //   new G4PVPlacement(
  //     nullptr,
  //     G4ThreeVector(0, 0, 0),
  //     smPhInnerLV,
  //     "smPhPV",
  //     smPhLV,
  //     false,
  //     0,
  //     checkOverlaps 
  //   );

  //   // heart phantom
  //   auto heartLV = new G4LogicalVolume(heartMesh->GetSolid(0), phanMat["heartwall"], "heartLV");
  //   // heart phantom
  //   auto brainLV = new G4LogicalVolume(brainMesh->GetSolid(0), phanMat["brain"], "brainhLV");
  //   // heart phantom
  //   auto lungLV = new G4LogicalVolume(lungMesh->GetSolid(0), phanMat["lung"], "lungLV");

  //   // return C0log;
  //   // return smPhWrapLV;
  //   // return boxLog;
  //   // return heartLV;
  //   // return brainLV;
  //   return lungLV;
  // }

  void PhantomConstruction::genMat(std::map<std::string, G4Material*> &mat)
  {
    auto nist = G4NistManager::Instance();
    mat.insert({std::string("Air"), nist->FindOrBuildMaterial("G4_AIR")});
    mat.insert({std::string("Water"), nist->FindOrBuildMaterial("G4_WATER")});
    
    G4Material *m_acrylic_low = new G4Material("myAcrylicLow", 1.14*g/cm3, 3);
    G4Material *m_acrylic = new G4Material("myAcrylic", 1.18*g/cm3, 3);
    m_acrylic_low->AddElement(G4Element::GetElement("C"), 5);
    m_acrylic->AddElement(G4Element::GetElement("C"), 5);
    m_acrylic_low->AddElement(G4Element::GetElement("H"), 8);
    m_acrylic->AddElement(G4Element::GetElement("H"), 8);
    m_acrylic_low->AddElement(G4Element::GetElement("O"), 2);
    m_acrylic->AddElement(G4Element::GetElement("O"), 2);
    mat.insert({std::string("AcrylicLow"), m_acrylic_low});
    mat.insert({std::string("Acrylic"), m_acrylic});

    G4Material *m_pmp = new G4Material("myPMP", 0.853*g/cm3, 2);
    m_pmp->AddElement(G4Element::GetElement("C"), 7);
    m_pmp->AddElement(G4Element::GetElement("H"), 14);
    mat.insert({std::string("PMP"), m_pmp});

    G4Material *m_ldpe = new G4Material("myLDPE", 0.945*g/cm3, 2);
    m_ldpe->AddElement(G4Element::GetElement("C"), 2);
    m_ldpe->AddElement(G4Element::GetElement("H"), 4);
    mat.insert({std::string("LDPE"), m_ldpe});

    G4Material *m_polystr = new G4Material("myPolystr", 0.998*g/cm3, 2);
    m_polystr->AddElement(G4Element::GetElement("C"), 8);
    m_polystr->AddElement(G4Element::GetElement("H"), 8);
    mat.insert({std::string("Polystyrene"), m_polystr});

    G4Material *m_delrin = new G4Material("myDelrin", 1.363*g/cm3, 3);
    m_delrin->AddElement(G4Element::GetElement("C"), 1);
    m_delrin->AddElement(G4Element::GetElement("H"), 2);
    m_delrin->AddElement(G4Element::GetElement("O"), 1);
    mat.insert({std::string("Delrin"), m_delrin});
    
    G4Material *m_teflon = new G4Material("myTeflon", 1.868*g/cm3, 2);
    m_teflon->AddElement(nist->FindOrBuildElement("C"), 1);
    m_teflon->AddElement(nist->FindOrBuildElement("F"), 2);
    mat.insert({std::string("Teflon"), m_teflon});

    G4Material *m_ramp = new G4Material("myRamp", 1.252*g/cm3, 11);
    m_ramp->AddElementByMassFraction(nist->FindOrBuildElement("H"), 7.9/100);
    m_ramp->AddElementByMassFraction(nist->FindOrBuildElement("C"), 34.5/100);
    m_ramp->AddElementByMassFraction(nist->FindOrBuildElement("N"), 2.9/100);
    m_ramp->AddElementByMassFraction(nist->FindOrBuildElement("O"), 41.3/100);
    m_ramp->AddElementByMassFraction(nist->FindOrBuildElement("Na"), 0.2/100);
    m_ramp->AddElementByMassFraction(nist->FindOrBuildElement("Mg"), 0.1/100);
    m_ramp->AddElementByMassFraction(nist->FindOrBuildElement("F"), 3.9/100);
    m_ramp->AddElementByMassFraction(nist->FindOrBuildElement("S"), 0.2/100);
    m_ramp->AddElementByMassFraction(nist->FindOrBuildElement("Cl"), 0.1/100);
    m_ramp->AddElementByMassFraction(nist->FindOrBuildElement("K"), 0.1/100);
    m_ramp->AddElementByMassFraction(nist->FindOrBuildElement("Ca"), 8.8/100);
    mat.insert({std::string("Ramp"), m_ramp});

    G4Material *m_heartwall = new G4Material("heartWall", 1.051*g/cm3, 9);
    m_heartwall->AddElementByMassFraction(nist->FindOrBuildElement("H"), 10.4/100);
    m_heartwall->AddElementByMassFraction(nist->FindOrBuildElement("C"), 13.5/100);
    m_heartwall->AddElementByMassFraction(nist->FindOrBuildElement("N"), 2.9/100);
    m_heartwall->AddElementByMassFraction(nist->FindOrBuildElement("O"), 72.3/100);
    m_heartwall->AddElementByMassFraction(nist->FindOrBuildElement("Na"), 0.1/100);
    m_heartwall->AddElementByMassFraction(nist->FindOrBuildElement("P"), 0.2/100);
    m_heartwall->AddElementByMassFraction(nist->FindOrBuildElement("S"), 0.2/100);
    m_heartwall->AddElementByMassFraction(nist->FindOrBuildElement("Cl"), 0.2/100);
    m_heartwall->AddElementByMassFraction(nist->FindOrBuildElement("K"), 0.3/100);
    mat.insert({std::string("heartwall"), m_heartwall});

    G4Material *m_brain = new G4Material("brain", 1.041*g/cm3, 9);
    m_brain->AddElementByMassFraction(nist->FindOrBuildElement("H"), 10.7/100);
    m_brain->AddElementByMassFraction(nist->FindOrBuildElement("C"), 14.3/100);
    m_brain->AddElementByMassFraction(nist->FindOrBuildElement("N"), 2.3/100);
    m_brain->AddElementByMassFraction(nist->FindOrBuildElement("O"), 71.3/100);
    m_brain->AddElementByMassFraction(nist->FindOrBuildElement("Na"), 0.2/100);
    m_brain->AddElementByMassFraction(nist->FindOrBuildElement("P"), 0.4/100);
    m_brain->AddElementByMassFraction(nist->FindOrBuildElement("S"), 0.2/100);
    m_brain->AddElementByMassFraction(nist->FindOrBuildElement("Cl"), 0.3/100);
    m_brain->AddElementByMassFraction(nist->FindOrBuildElement("K"), 0.3/100);
    mat.insert({std::string("brain"), m_brain});

    G4Material *m_lung = new G4Material("lung", 0.415*g/cm3, 10);
    m_lung->AddElementByMassFraction(nist->FindOrBuildElement("H"), 10.2/100);
    m_lung->AddElementByMassFraction(nist->FindOrBuildElement("C"), 10.8/100);
    m_lung->AddElementByMassFraction(nist->FindOrBuildElement("N"), 3.2/100);
    m_lung->AddElementByMassFraction(nist->FindOrBuildElement("O"), 74.8/100);
    m_lung->AddElementByMassFraction(nist->FindOrBuildElement("Na"), 0.1/100);
    m_lung->AddElementByMassFraction(nist->FindOrBuildElement("P"), 0.1/100);
    m_lung->AddElementByMassFraction(nist->FindOrBuildElement("S"), 0.2/100);
    m_lung->AddElementByMassFraction(nist->FindOrBuildElement("Cl"), 0.3/100);
    m_lung->AddElementByMassFraction(nist->FindOrBuildElement("K"), 0.2/100);
    m_lung->AddElementByMassFraction(nist->FindOrBuildElement("Fe"), 0.1/100);
    mat.insert({std::string("lung"), m_lung});

    G4Material *m_ET = new G4Material("ET", 1.031*g/cm3, 9);
    mat.insert({std::string("ET"), m_ET});
    G4Material *m_aqueous = new G4Material("Aqueous", 1.025*g/cm3, 4);
    mat.insert({std::string("aqueous"), m_aqueous});
    G4Material *m_cornea = new G4Material("Cornea", 1.100*g/cm3, 8);
    mat.insert({std::string("cornea"), m_cornea});
    G4Material *m_cranium = new G4Material("Cranium", 1.165*g/cm3, 11);
    mat.insert({std::string("cranium"), m_cranium});
    G4Material *m_eye = new G4Material("Eye", 1.060*g/cm3, 8);
    mat.insert({std::string("eye"), m_eye});
    G4Material *m_pituitary = new G4Material("Pituitary", 1.031*g/cm3, 9);
    mat.insert({std::string("pituitary"), m_pituitary});
    G4Material *m_vitreous = new G4Material("Vitreous", 1.031*g/cm3, 4);
    mat.insert({std::string("vitreous"), m_vitreous});
  }
} // namespace PCT
