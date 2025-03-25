#include "MyDetectorConstruction.hh"
#include "MyPhysicsList.hh"
#include "MyPrimaryGeneratorAction.hh"
#include "MyRunAction.hh"

#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4UIExecutive.hh"
#include "G4VisExecutive.hh"

int main(int argc, char** argv) 
{
    G4RunManager* runManager = new G4RunManager;

    runManager->SetUserInitialization(new MyDetectorConstruction());
    runManager->SetUserInitialization(new MyPhysicsList());

    MyPrimaryGeneratorAction* primaryGenerator = new MyPrimaryGeneratorAction();
    runManager->SetUserAction(primaryGenerator);
    runManager->SetUserAction(new MyRunAction(primaryGenerator));

    runManager->Initialize();

    G4VisManager* visManager = new G4VisExecutive;
    visManager->Initialize();

    G4UIExecutive* ui = new G4UIExecutive(argc, argv);
    G4UImanager* UI = G4UImanager::GetUIpointer();

    UI->ApplyCommand("/control/execute vis.mac");

    ui->SessionStart();

    delete ui;
    delete visManager;
    delete runManager;

    return 0;
}
