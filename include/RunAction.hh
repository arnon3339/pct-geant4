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
/// \file PCT/include/RunAction.hh
/// \brief Definition of the PCT::RunAction class

#ifndef PCTRunAction_h
#define PCTRunAction_h 1

#include "G4UserRunAction.hh"
#include "G4Accumulable.hh"
#include "globals.hh"
#include "G4SystemOfUnits.hh"

#include <iostream>
#include <fstream>
#include <mutex>

class G4Run;
class RunMessager;

/// Run action class
///
/// In EndOfRunAction(), it calculates the dose in the selected volume
/// from the energy deposit accumulated via stepping and event actions.
/// The computed dose is then printed on the screen.

namespace PCT
{

class RunMessager;

class RunAction : public G4UserRunAction
{
  private:
    const G4int& numOfRun;
    G4bool  fCloseFile{false};
    G4bool  fOpenFile{false};
    G4double fMaxLength{0.0 *mm};
    RunMessager* fRunMessager;

  public:
    RunAction() = default;
    RunAction(const G4int&);
    ~RunAction();

    void BeginOfRunAction(const G4Run*) override;
    void   EndOfRunAction(const G4Run*) override;

    void SetCloseFile(G4bool closeFile) {fCloseFile = closeFile;};
    void SetOpenFile(G4bool openFile) {fOpenFile = openFile;};
    void SetMaxLength(G4double maxLength) {fMaxLength = maxLength;};

};

}

#endif

