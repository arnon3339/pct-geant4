// ROOT macro file for plotting example B4 ntuple
//
// Can be run from ROOT session:
// root[0] .x plotNtuple.C

{
  gROOT->Reset();
  gROOT->SetStyle("Plain");

  // Draw histos filled by Geant4 simulation
  //

  // Open file filled by Geant4 simulation
  // TFile f("output/run_000.root");
  ROOT::RDataFrame df("pCT", "output/run_000.root");

  for (const auto &el: df.Take<int>("eventID"))
  {
   std::cout << "Element: " << el << "\n";
  }
  // Create a canvas and divide it into 2x2 pads
  // TCanvas* c1 = new TCanvas("c1", "", 20, 20, 1000, 1000);
  // c1->Divide(2,2);

  // Get ntuple
  // TNtuple* ntuple = (TNtuple*)f.Get("pCT");
  // TTree* t1 = (TTree*)f.Get("pCT");

  // Int_t re;
  // Double_t evt;

  // t1->SetBranchAddress("eventID", &evt);
  // t1->SetBranchAddress("RE", &re);

  // for (Long64_t entry = 0; entry < t1->GetEntries(); ++entry) 
  // {
  //   t1->GetEntry(entry);
  //   std::cout << "Entry: " << entry << ", RE: " << re << 
  //             " EventID:" << evt << std::endl;
  // }

  // Draw Eabs histogram in the pad 1
  // c1->cd(1);
  // ntuple->Draw("RE");

  // Draw Labs histogram in the pad 2
  // c1->cd(2);
  // ntuple->Draw("eventID");

  // Draw Egap histogram in the pad 3
  // with logaritmic scale for y  ?? how to do this?
  // c1->cd(3);
  // gPad->SetLogy(1);
  // ntuple->Draw("Egap");

  // Draw Lgap histogram in the pad 4
  // with logaritmic scale for y  ?? how to do this?
  // c1->cd(4);
  // gPad->SetLogy(1);
  // ntuple->Draw("Egap");
}
