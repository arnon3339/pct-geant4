#include "Analysis.hh"

#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include "TBranch.h"

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <filesystem>

namespace PCT
{
    Analysis* Analysis::singleton_= nullptr;;


Analysis *Analysis::GetInstance(const std::string& tree, const std::string& fpath)
{

    if(singleton_==nullptr){
        singleton_ = new Analysis(tree, fpath);
    }
    return singleton_;
}

void Analysis::GenOutFile(const std::string& fpath)
{
    TChain* chain = new TChain("pCT;1");

   try {
        for (const auto& entry : std::filesystem::directory_iterator(fpath_)) {
            if (entry.path().string().find("run_000_t") != std::string::npos)
            {
                chain->Add(entry.path().c_str());
            }
        }
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    TFile* outputFile = TFile::Open(fpath.c_str(), "RECREATE");
    TTree* outputTree = chain->CloneTree(0);
    Long64_t nEntries = chain->GetEntries();
    for (Long64_t i = 0; i < nEntries; ++i) {
        chain->GetEntry(i);
        outputTree->Fill();
    }
    outputTree->Write();
    outputFile->Close();
    std::cout << "I have a root output!" << std::endl;

}

} // namespace PCT
