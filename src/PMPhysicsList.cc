#include "PMPhysicsList.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Material.hh"
#include "G4VModularPhysicsList.hh"
#include "G4EmStandardPhysics.hh"
#include "G4HadronPhysicsQGSP_BIC_HP.hh"
#include "G4DecayPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"
#include "Shielding.hh" 

PMPhysicsList::PMPhysicsList() : G4VModularPhysicsList()
{
    // EM Physics
    RegisterPhysics(new G4EmStandardPhysics());

    // Hadronic Physics
    RegisterPhysics(new G4HadronPhysicsQGSP_BIC_HP());
}

PMPhysicsList::~PMPhysicsList()
{
    // Destructor implementation if needed
}