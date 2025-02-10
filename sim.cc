#include <iostream>

#include "G4RunManager.hh"
#include "G4MTRunManager.hh"
#include "G4UImanager.hh"
#include "G4VisManager.hh"
#include "G4UIExecutive.hh"
#include "G4VisExecutive.hh"
#include "PMPhysicsList.hh"

#include "PMPhysicsList.hh"
#include "PMDetectorConstruction.hh"
#include "PMActioninitialization.hh"
#include "QGSP_BERT_HP.hh"
#include "FTFP_BERT_HP.hh"
#include "G4PhysListFactory.hh"


int main(int argc, char** argv)
{
    G4UIExecutive *ui;

    #ifdef G4MULTITHREADED
        G4MTRunManager *runManager = new G4MTRunManager;
    #else
        G4RunManager *runManager = new G4RunManager;  // Fixed the typo here
    #endif
    
    //Physics List
    runManager ->SetUserInitialization(new PMPhysicsList());
    G4PhysListFactory physListFactory;
    G4VModularPhysicsList* physicsList = physListFactory.GetReferencePhysList("QGSP_BERT_HPT");
    runManager->SetUserInitialization(physicsList);
    //runManager->SetUserInitialization(new QGSP_BERT_HP());
    //runManager->SetUserInitialization(new FTFP_BERT_HP());

    //Detector construction
    runManager ->SetUserInitialization(new PMDetectorConstruction());

    //Action Initialization
    runManager ->SetUserInitialization(new PMActionInitialization());

    if (argc == 1)
    {
        ui =new G4UIExecutive(argc, argv);
    }

    G4VisManager *visManager = new G4VisExecutive();
    visManager->Initialise();

    G4UImanager *UImanager = G4UImanager::GetUIpointer();

    if(ui)
    {
    UImanager->ApplyCommand("/control/execute vis.mac");
    ui->SessionStart();
    }
    else
    {
        G4String command = "/control/execute ";
        G4String fileName = argv[1];
        UImanager->ApplyCommand(command + fileName);
    }

    return 0;
}