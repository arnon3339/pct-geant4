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

#include <iostream>
#include <filesystem>
#include <string>
#include <sstream>
#include <fstream>

namespace fs = std::filesystem;

namespace PCT
{

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::RunAction(): csvOut(0), numOfRun(0)
{
  // G4Random::setTheSeed(1);
  G4cout << "ooooooooooooooooooooooooooooo" << G4endl;
  G4cout << "ooooooooooooooooooooooooooooo" << G4endl;
  G4cout << "ooooooooooooooooooooooooooooo" << G4endl;
  // add new units for dose
  //
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

  analysisManager->CreateH1("RE", "Rasidual energy", 100, 0, 200);

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
  
  analysisManager->CreateNtuple("pCT", "Track and rasidual energy");
  analysisManager->CreateNtupleIColumn("eventID");
  analysisManager->CreateNtupleDColumn("posX0");
  analysisManager->CreateNtupleDColumn("posY0");
  analysisManager->CreateNtupleDColumn("posX1");
  analysisManager->CreateNtupleDColumn("posY1");
  analysisManager->CreateNtupleDColumn("posX2");
  analysisManager->CreateNtupleDColumn("posY2");
  analysisManager->CreateNtupleDColumn("posX3");
  analysisManager->CreateNtupleDColumn("posY3");
  analysisManager->CreateNtupleDColumn("RE");
  analysisManager->FinishNtuple();

  csvOut = new std::ofstream();
  // analysisManager->CreateNtupleDColumn("parentID");
}

RunAction::~RunAction(){
  delete csvOut;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::BeginOfRunAction(const G4Run*)
{
  auto runAction = (ActionInitialization*)G4RunManager::GetRunManager()->GetUserActionInitialization();
  G4Random::setTheSeed(runAction->GetRunNum() + 1);
  std::ostringstream oss;
  oss << std::setw(5) << std::setfill('0') << runAction->GetRunNum();
  std::string output_root_dir = "./output/root/run_" + oss.str();
  std::string output_csv_dir = "./output/csv/run_" + oss.str();

  if (!fs::exists(output_root_dir))
  {
    try
    {
      fs::create_directory(output_root_dir);
      G4cout << output_root_dir << " output directory was created." << G4endl;
    }
    catch(const std::exception& e)
    {
      std::cerr << "Error to create " << output_root_dir << " directory!: " << e.what() << '\n';
    }
  }
  else G4cout << output_root_dir << " directory already exist." << G4endl;

  if (!fs::exists(output_csv_dir))
  {
    try
    {
      fs::create_directory(output_csv_dir);
      G4cout << output_csv_dir << " output directory was created." << G4endl;
    }
    catch(const std::exception& e)
    {
      std::cerr << "Error to create " << output_csv_dir << " directory!: " << e.what() << '\n';
    }
  }
  else G4cout << output_csv_dir << " directory already exist." << G4endl;

  // inform the runManager to save random number seed
  G4RunManager::GetRunManager()->SetRandomNumberStore(false);
  auto analysisManager = G4AnalysisManager::Instance();

  std::ostringstream oss2;
  oss2 << std::setw(3) << std::setfill('0') << static_cast<int>(numOfRun);

  analysisManager->OpenFile(output_root_dir + std::string("/projection_") +
    oss2.str()  + std::string(".root"));

  csvOut->open(output_csv_dir + std::string("/projection_") +
    oss2.str()  + std::string(".csv"), std::ios::out);

  *csvOut << "eventID, posX0, posY0, posX1, posY1, posX2, posY2, posX3, posY3, resE" << std::endl;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::EndOfRunAction(const G4Run*)
{

  auto analysisManager = G4AnalysisManager::Instance();

  // G4cout << "XXXXXXXXXXXXXX" << G4endl;
  // G4cout << analysisManager->GetNofNtuples() << G4endl;

  // for (int i = 0; i < 1000; i++)
  // {
  //   analysisManager->FillNtupleDColumn(0, i);
  //   analysisManager->AddNtupleRow();
  // }

     
  analysisManager->Write();
  analysisManager->CloseFile();

  csvOut->close();

  numOfRun++;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

}
