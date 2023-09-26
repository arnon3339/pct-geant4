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

  G4int activeLayers[5]{0};

  G4int eventID = event->GetEventID();
  auto analysisManager = G4AnalysisManager::Instance();
  G4VHitsCollection* hc = event->GetHCofThisEvent()->GetHC(0);
  TrackerHit* hit;
  G4bool found0 = false;
  G4bool found1 = false;
  if (hc->GetSize() > 1)
  {
    for (size_t i = 0; i < hc->GetSize(); i++)
    {
      hit = (TrackerHit*)hc->GetHit(i);
      if (hit->GetChamberNb() == 0) found0 = true;
      if (hit->GetChamberNb() == 1) found1 = true;
    }

    if (!found0 || !found1) return;

    for (size_t i = 0; i < hc->GetSize(); i++)
    {
      hit = (TrackerHit*)hc->GetHit(i);

      analysisManager->FillH1(2*(hit->GetChamberNb()), hit->GetPos().getX() + detectorSizeX/2);
      analysisManager->FillH1(2*(hit->GetChamberNb()) + 1, hit->GetPos().getY() + detectorSizeY/2);
      analysisManager->FillH2(hit->GetChamberNb(), hit->GetPos().getX() + detectorSizeX/2, hit->GetPos().getY() + detectorSizeY/2);

      analysisManager->FillNtupleIColumn(0, eventID);
      analysisManager->FillNtupleDColumn(1, hit->GetPos().getX() + detectorSizeX/2);
      analysisManager->FillNtupleDColumn(2, hit->GetPos().getY() + detectorSizeY/2);
      analysisManager->FillNtupleDColumn(3, hit->GetChamberNb());
      analysisManager->FillNtupleDColumn(4, hit->GetAngle());
      analysisManager->FillNtupleDColumn(5, hit->GetRE());
      analysisManager->AddNtupleRow();
    }  
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

}

