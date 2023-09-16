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
/// \file EventAction.cc
/// \brief Implementation of the PCT::EventAction class

#include "EventAction.hh"

#include "RunAction.hh"

#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4AnalysisManager.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4ios.hh"
#include "TrackerHit.hh"
#include "G4SystemOfUnits.hh"

namespace PCT
{

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::EventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::~EventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::BeginOfEventAction(const G4Event*)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::EndOfEventAction(const G4Event* event)
{
  // get number of stored trajectories
  currentLayer = -1;
  G4int numAlpideX = 16;
  G4int numAlpideY = 16;
  G4double alpideSizeX = 3.0 *cm;
  G4double alpideSizeY = 1.38 *cm;
  G4double detectorSizeX = (numAlpideX*alpideSizeX);
  G4double detectorSizeY = (numAlpideY*alpideSizeY);

  // periodic printing

  G4int eventID = event->GetEventID();
  auto runAction = (RunAction*)G4RunManager::GetRunManager()->GetUserRunAction();
  auto csv_out = runAction->get_csv_out();
  auto analysisManager = G4AnalysisManager::Instance();
    G4VHitsCollection* hc = event->GetHCofThisEvent()->GetHC(0);
    TrackerHit* hit;
    G4bool isCollected = false;
    if (hc->GetSize() > 0)
    {
      for (size_t i = 0; i < hc->GetSize(); i++)
      {
        hit = (TrackerHit*)hc->GetHit(i);
        if (hit->GetRE() > 0) isCollected = true;
      }

      if (!isCollected) return;

      G4int defEventID = -1;
      G4double defPoses[] = {-1, -1, -1, -1, -1, -1, -1, -1, -1};

      for (size_t i = 0; i < hc->GetSize(); i++)
      {
        hit = (TrackerHit*)hc->GetHit(i);
        switch (hit->GetChamberNb())
        {
        case 0:
          analysisManager->FillH1(0, hit->GetPos().getX() + detectorSizeX/2);
          analysisManager->FillH1(1, hit->GetPos().getY() + detectorSizeY/2);
          analysisManager->FillNtupleDColumn(1, hit->GetPos().getX() + detectorSizeX/2);
          analysisManager->FillNtupleDColumn(2, hit->GetPos().getY() + detectorSizeY/2);
          analysisManager->FillH2(0, hit->GetPos().getX() + detectorSizeX/2, hit->GetPos().getY() + detectorSizeY/2);
          defPoses[0] = hit->GetPos().getX() + detectorSizeX/2;
          defPoses[1] = hit->GetPos().getY() + detectorSizeY/2;
          break;

        case 1:
          analysisManager->FillH1(2, hit->GetPos().getX() + detectorSizeX/2);
          analysisManager->FillH1(3, hit->GetPos().getY() + detectorSizeY/2);
          analysisManager->FillNtupleDColumn(3, hit->GetPos().getX() + detectorSizeX/2);
          analysisManager->FillNtupleDColumn(4, hit->GetPos().getY() + detectorSizeY/2);
          analysisManager->FillH2(1, hit->GetPos().getX() + detectorSizeX/2, hit->GetPos().getY() + detectorSizeY/2);
          defPoses[2] = hit->GetPos().getX() + detectorSizeX/2;
          defPoses[3] = hit->GetPos().getY() + detectorSizeY/2;
          break;
        
        case 2:
          analysisManager->FillH1(4, hit->GetPos().getX() + detectorSizeX/2);
          analysisManager->FillH1(5, hit->GetPos().getY() + detectorSizeY/2);
          analysisManager->FillNtupleDColumn(5, hit->GetPos().getX() + detectorSizeX/2);
          analysisManager->FillNtupleDColumn(6, hit->GetPos().getY() + detectorSizeY/2);
          analysisManager->FillH2(2, hit->GetPos().getX() + detectorSizeX/2, hit->GetPos().getY() + detectorSizeY/2);
          defPoses[4] = hit->GetPos().getX() + detectorSizeX/2;
          defPoses[5] = hit->GetPos().getY() + detectorSizeY/2;
          break;

        case 3:
          analysisManager->FillH1(6, hit->GetPos().getX() + detectorSizeX/2);
          analysisManager->FillH1(7, hit->GetPos().getY() + detectorSizeY/2);
          analysisManager->FillNtupleDColumn(7, hit->GetPos().getX() + detectorSizeX/2);
          analysisManager->FillNtupleDColumn(8, hit->GetPos().getY() + detectorSizeY/2);
          analysisManager->FillH2(3, hit->GetPos().getX() + detectorSizeX/2, hit->GetPos().getY() + detectorSizeY/2);
          defPoses[6] = hit->GetPos().getX() + detectorSizeX/2;
          defPoses[7] = hit->GetPos().getY() + detectorSizeY/2;
          break;

        default:
          analysisManager->FillH1(8, hit->GetRE());
          analysisManager->FillNtupleDColumn(9, hit->GetRE());
          defPoses[8] = hit->GetRE();
          break;
        }
      }  
      defEventID = eventID;
      analysisManager->FillNtupleIColumn(0, eventID);
      analysisManager->AddNtupleRow();
      *csv_out << defEventID << ", " <<
                defPoses[0] << ", " << defPoses[1] << ", " << defPoses[2] << ", " <<
                defPoses[3] << ", " << defPoses[4] << ", " << defPoses[5] << ", " <<
                defPoses[6] << ", " << defPoses[7] << ", " << defPoses[8] << ", " <<
                std::endl;
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

}

