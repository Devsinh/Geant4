#include "PMRunAction.hh"

PMRunAction::PMRunAction()
{
    G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();

    // Create histogram for neutron energy
    analysisManager->CreateH1("NeutronEnergy", "Neutron Energy", 100, 0., 2. *MeV);
    
    // Create ntuple
    analysisManager->CreateNtuple("NeutronData", "Neutron Detection Data");
    analysisManager->CreateNtupleIColumn("EventID");
    analysisManager->CreateNtupleDColumn("PosX");
    analysisManager->CreateNtupleDColumn("PosY");
    analysisManager->CreateNtupleDColumn("PosZ");
    analysisManager->CreateNtupleDColumn("Time");
    analysisManager->CreateNtupleDColumn("Energy");
    analysisManager->CreateNtupleDColumn("Momentum");
    analysisManager->FinishNtuple();
}

PMRunAction::~PMRunAction()
{
}

void PMRunAction::BeginOfRunAction(const G4Run *run)
{
    G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();

    G4int runID = run->GetRunID();

    std::stringstream strRunID;
    strRunID << runID;

    analysisManager->OpenFile("output" + strRunID.str() + ".root");

}

void PMRunAction::EndOfRunAction(const G4Run *run)
{
    G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();

    analysisManager->Write();

    analysisManager->CloseFile();

    G4int runID = run->GetRunID();

    G4cout << "Finishing run" << runID << G4endl;
}