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
  G4LogicalVolume *PhantomConstruction::getLogVolume()
  {
    genMat(phanMat);
    G4bool checkOverlaps = true;
    auto mesh = CADMesh::TessellatedMesh::FromOBJ("./phantom/ctp404.obj");
    mesh->SetScale(1000);

    G4ThreeVector pos1_5 = G4ThreeVector(0, 0, 0);

    auto nonVis = new G4VisAttributes();
    nonVis->SetVisibility(false);

    G4VisAttributes *C0vis = new G4VisAttributes();
    C0vis->SetVisibility(true);
    C0vis->SetColor(G4Color(0, 1.0, 0, 0.75));
    G4LogicalVolume *C0log = new G4LogicalVolume(
      mesh->GetSolid("C0"),
      phanMat["Water"],
      "C0log"
    );
    C0log->SetVisAttributes(C0vis);

    G4VisAttributes *C1vis = new G4VisAttributes();
    C1vis->SetVisibility(true);
    C1vis->SetColor(G4Color(1.0, 0, 0, 0.75));
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

    auto boxSol = new G4Box("boxSol", 20*cm, 10*cm, 10*cm);
    auto boxLog = new G4LogicalVolume(
      boxSol,
      phanMat["Teflon"],
      "boxLog"
    );


    // small phantom
    auto smPhWrapS = new G4Box("smPhWrapS", 10 *cm, 10 *cm, 10 *cm);
    auto smPhWrapLV = new G4LogicalVolume(smPhWrapS, phanMat["Air"], "smPhWrapLV");
    smPhWrapLV->SetVisAttributes(nonVis);
    auto smPhS = new G4Tubs("smPhS", 0, 10.0 *cm, 10.0 *cm, 0 *deg, 360 *deg);
    auto smPhLV = new G4LogicalVolume(smPhS, phanMat["Water"], "smPhLV");
    smPhLV->SetVisAttributes(C0vis);
    auto smPhInnerS = new G4Box("smPhInnerS", 10 *cm, 0.5 *cm, 5 *cm);
    auto smPhInnerLV = new G4LogicalVolume(smPhInnerS, phanMat["Teflon"], "smPHInnerLV");
    smPhInnerLV->SetVisAttributes(C1vis);

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

    new G4PVPlacement(
      nullptr,
      G4ThreeVector(0, 0, 0),
      smPhInnerLV,
      "smPhPV",
      smPhLV,
      false,
      0,
      checkOverlaps 
    );

    // return C0log;
    return smPhWrapLV;
    // return boxLog;
  }

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

  }
} // namespace PCT
