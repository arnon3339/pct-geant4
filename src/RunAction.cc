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
/// \file PCT/src/RunAction.cc
/// \brief Implementation of the PCT::RunAction class

#include "RunAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "DetectorConstruction.hh"
#include "ActionInitialization.hh"
#include "RunMessager.hh"
// #include "Run.hh"

#include "G4RunManager.hh"
#include "Randomize.hh"
#include "G4Run.hh"
#include "G4AccumulableManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4AnalysisManager.hh"
#include "G4VUserPhysicsList.hh"

#include <iostream>
#include <filesystem>
#include <string>
#include <sstream>
#include <fstream>

namespace PCT
{

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::RunAction(const G4int& numRun):numOfRun(numRun)
{
  _run_messager = new RunMessager(this);

  const G4double milligray = 1.e-3*gray;
  const G4double microgray = 1.e-6*gray;
  const G4double nanogray  = 1.e-9*gray;
  const G4double picogray  = 1.e-12*gray;

  new G4UnitDefinition("milligray", "milliGy" , "Dose", milligray);
  new G4UnitDefinition("microgray", "microGy" , "Dose", microgray);
  new G4UnitDefinition("nanogray" , "nanoGy"  , "Dose", nanogray);
  new G4UnitDefinition("picogray" , "picoGy"  , "Dose", picogray);

  G4int numAlpideX = 16;
  G4int numAlpideY = 16;
  G4double alpideSizeX = 3.0 *cm;
  G4double alpideSizeY = 1.38 *cm;
  G4double detectorSizeX = (numAlpideX*alpideSizeX);
  G4double detectorSizeY = (numAlpideY*alpideSizeY);

  auto analysisManager = G4AnalysisManager::Instance();

  analysisManager->CreateH1("PosX0", "X position hit in layer 0", 100, 0, detectorSizeX);
  analysisManager->CreateH1("PosY0", "Y position hit in layer 0", 100, 0, detectorSizeY);
  analysisManager->CreateH1("PosX1", "X position hit in layer 1", 100, 0, detectorSizeX);
  analysisManager->CreateH1("PosY1", "Y position hit in layer 1", 100, 0, detectorSizeY);
  analysisManager->CreateH1("PosX2", "X position hit in layer 2", 100, 0, detectorSizeX);
  analysisManager->CreateH1("PosY2", "Y position hit in layer 2", 100, 0, detectorSizeY);
  analysisManager->CreateH1("PosX3", "X position hit in layer 3", 100, 0, detectorSizeX);
  analysisManager->CreateH1("PosY3", "Y position hit in layer 3", 100, 0, detectorSizeY);
  analysisManager->CreateH1("PosX4", "X position hit in layer 4", 100, 0, detectorSizeX);
  analysisManager->CreateH1("PosY4", "Y position hit in layer 4", 100, 0, detectorSizeY);

  analysisManager->CreateH2("Posxy0", "XY position hit in layer 0", 
                            100, 0, detectorSizeX, 100, 0, detectorSizeY,
                            "cm", "cm");
  analysisManager->CreateH2("Posxy1", "XY position hit in layer 1", 
                            100, 0, detectorSizeX, 100, 0, detectorSizeY,
                            "cm", "cm");
  analysisManager->CreateH2("Posxy2", "XY position hit in layer 2", 
                            100, 0, detectorSizeX, 100, 0, detectorSizeY,
                            "cm", "cm");
  analysisManager->CreateH2("Posxy3", "XY position hit in layer 3", 
                            100, 0, detectorSizeX, 100, 0, detectorSizeY,
                            "cm", "cm");
  analysisManager->CreateH2("Posxy4", "XY position hit in layer 4", 
                            100, 0, detectorSizeX, 100, 0, detectorSizeY,
                            "cm", "cm");
  
  analysisManager->CreateNtuple("pCT", "Track and rasidual energy");
  analysisManager->CreateNtupleIColumn("eventID");
  analysisManager->CreateNtupleDColumn("posX");
  analysisManager->CreateNtupleDColumn("posY");
  analysisManager->CreateNtupleDColumn("layerID");
  analysisManager->CreateNtupleDColumn("angle");
  analysisManager->CreateNtupleDColumn("RE");
  analysisManager->FinishNtuple();

}

RunAction::~RunAction()
{
  delete _mutex;
  delete _run_messager;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::BeginOfRunAction(const G4Run*)
{
  auto det = (DetectorConstruction*)G4RunManager::GetRunManager()->GetUserDetectorConstruction();
  auto analysisManager = G4AnalysisManager::Instance();
  std::string output_root_dir = "./output";

  std::ostringstream oss;
  oss << std::setw(3) << std::setfill('0') << numOfRun;
  std::ostringstream oss2;
  oss2 << std::fixed << std::setprecision(2);
  oss2 << det->GetPHangle();

  analysisManager->OpenFile(output_root_dir + std::string("/run_") +
    oss.str() + "_project_" + oss2.str() + std::string(".root"));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOo {_close_file = close_file;}oo........oooOO0OOooo......

void RunAction::EndOfRunAction(const G4Run*)
{
  auto det = (DetectorConstruction*)G4RunManager::GetRunManager()->GetUserDetectorConstruction();
  G4cout << "...... Finished in projection of " << det->GetPHangle() << " degree ......" << G4endl;
  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->Write();
  analysisManager->CloseFile();
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

}
