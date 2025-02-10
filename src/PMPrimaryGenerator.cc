// PMPrimaryGenerator.cc
#include "PMPrimaryGenerator.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleTable.hh"
#include "G4SingleParticleSource.hh"

PMPrimaryGenerator::PMPrimaryGenerator()
{
    fParticleSource = new G4GeneralParticleSource();
    G4SingleParticleSource* spSource = fParticleSource->GetCurrentSource();
    
    // Set particle type to neutron
    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    G4ParticleDefinition* particle = particleTable->FindParticle("neutron");
    spSource->SetParticleDefinition(particle);
    
    // Set energy (2 MeV mono-energetic)
    spSource->GetEneDist()->SetEnergyDisType("Mono");
    spSource->GetEneDist()->SetMonoEnergy(2.0 * MeV);
    
    // Set position distribution (volume source in rock)
    spSource->GetPosDist()->SetPosDisType("Volume");
    spSource->GetPosDist()->SetPosDisShape("Para"); // Parallelepiped
    spSource->GetPosDist()->SetHalfX(4.0 * m);
    spSource->GetPosDist()->SetHalfY(4.0 * m);
    spSource->GetPosDist()->SetHalfZ(4.0 * m);
    
    // Set angular distribution (isotropic)
    spSource->GetAngDist()->SetAngDistType("iso");
}

PMPrimaryGenerator::~PMPrimaryGenerator()
{
    delete fParticleSource;
}

void PMPrimaryGenerator::GeneratePrimaries(G4Event* anEvent)
{
    fParticleSource->GeneratePrimaryVertex(anEvent);
}