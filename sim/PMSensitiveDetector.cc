#include "PMSensitiveDetector.hh"
#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"
#include "G4AnalysisManager.hh"

PMSensitiveDetector::PMSensitiveDetector(G4String name) : G4VSensitiveDetector(name)
{
    fTotalNeutronEnergy = 0.;
    fNeutronCount = 0;
}

PMSensitiveDetector::~PMSensitiveDetector()
{
}

void PMSensitiveDetector::Initialize(G4HCofThisEvent*)
{
    fTotalNeutronEnergy = 0.;
    fNeutronCount = 0;
}

void PMSensitiveDetector::EndOfEvent(G4HCofThisEvent*)
{   
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

    // Fill histogram with total neutron energy
    analysisManager->FillH1(0, fTotalNeutronEnergy);

    if (fNeutronCount > 0) {
        G4cout << "Number of neutrons detected: " << fNeutronCount << G4endl;
        G4cout << "Average neutron energy: " << (fTotalNeutronEnergy / fNeutronCount) / MeV 
               << " MeV" << G4endl;
    }
}

G4bool PMSensitiveDetector::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
    G4Track* track = aStep->GetTrack();
    
    // Only process neutrons
    if (track->GetDefinition()->GetParticleName() != "neutron") return false;

    G4int eventID = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
    // Check if this event has already been processed
    if (fFirstInteractionProcessed[eventID]) return false;

    // Mark this event as processed
    fFirstInteractionProcessed[eventID] = true;

    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

    G4StepPoint* preStepPoint = aStep->GetPreStepPoint();
    G4ThreeVector position = preStepPoint->GetPosition();
    G4ThreeVector momentum = preStepPoint->GetMomentum();
    G4double time = preStepPoint->GetGlobalTime();
    G4double energy = track->GetKineticEnergy();

    // Fill histogram with the energy of first interaction
    analysisManager->FillH1(0, energy/MeV);

    // Fill ntuple with neutron data
    analysisManager->FillNtupleIColumn(0, 0, eventID);
    analysisManager->FillNtupleDColumn(0, 1, position.x());
    analysisManager->FillNtupleDColumn(0, 2, position.y());
    analysisManager->FillNtupleDColumn(0, 3, position.z());
    analysisManager->FillNtupleDColumn(0, 4, time);
    analysisManager->FillNtupleDColumn(0, 5, energy/MeV);
    analysisManager->FillNtupleDColumn(0, 6, momentum.mag()/MeV);
    analysisManager->AddNtupleRow(0);

    // Update neutron statistics
    fTotalNeutronEnergy += energy;
    fNeutronCount++;

    // Print information for first interaction only
    G4cout << "Event " << eventID << " - First neutron interaction:" << G4endl
           << "  Energy: " << energy/MeV << " MeV" << G4endl
           << "  Position (cm): (" 
           << position.x()/cm << ", " 
           << position.y()/cm << ", " 
           << position.z()/cm << ")" << G4endl
           << "  Time: " << time/ns << " ns" << G4endl;

    return true;
}