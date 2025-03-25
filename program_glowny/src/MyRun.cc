#include "MyRun.hh"
#include "G4Event.hh"
#include "globals.hh"
#include "G4TrajectoryContainer.hh"
#include "G4VTrajectory.hh"
#include <iomanip>
#include "G4SDManager.hh"
#include "G4THitsMap.hh"
#include "G4THitsCollection.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "MyPhotonHit.hh"
#include "MyAnalysis.hh"
#include "MyRunAction.hh"
#include "Randomize.hh"
#include "MyPrimaryGeneratorAction.hh"


MyRun::MyRun()
{
  G4SDManager* manager = G4SDManager::GetSDMpointer();
  fMapId = manager->GetCollectionID("MyDetector/MyScorer");
  G4cout << "MyLog:  MyRun constructor: index of photon scorer map: " << fMapId << G4endl;
  fCollectionId =  manager->GetCollectionID("Photon/PhotonCollection");
  G4cout << "MyLog:  MyRun constructor: index of photon collection : " << fCollectionId << G4endl;
}

MyRun::~MyRun() {}

void MyRun::RecordEvent(const G4Event* evt)
{
 
  G4HCofThisEvent* hce = evt->GetHCofThisEvent();
 
  G4THitsCollection<MyPhotonHit>* photonCollection = dynamic_cast<G4THitsCollection<MyPhotonHit>*> (hce->GetHC(fCollectionId));
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

  //pobieranie informacji o protonie
  G4PrimaryVertex* primaryVertex = evt->GetPrimaryVertex(0);
  G4double protonY = primaryVertex->GetY0()/cm;
  G4double protonZ = primaryVertex->GetZ0()/cm;
  G4double protonTheta = MyPrimaryGeneratorAction::GetTheta();
  G4double protonPhi = MyPrimaryGeneratorAction::GetPhi();
  G4double protonEnergy = MyPrimaryGeneratorAction::GetEnergy()/MeV;
  //struktura z danymi o protonie
  EventData event;
    event.protonY = protonY;
    event.protonZ = protonZ;
    event.protonTheta = protonTheta;
    event.protonPhi = protonPhi;
    event.protonEnergy = protonEnergy*1e-3; //GeV
  
  nPE_thisEvent = 0;

  for (G4int i = 0; i < photonCollection->entries(); i++) {
    MyPhotonHit* hit = (*photonCollection)[i];
    float eMeV = (*photonCollection)[i]->GetEnergy()/MeV;
    float xcm = (*photonCollection)[i]->GetPosition().x()/cm;
    float ycm = (*photonCollection)[i]->GetPosition().y()/cm;
    float zcm = (*photonCollection)[i]->GetPosition().z()/cm;
    float tns = (*photonCollection)[i]->GetTime()/ns;

    analysisManager->FillNtupleDColumn(0, eMeV);
    analysisManager->FillNtupleDColumn(1, xcm);
    analysisManager->FillNtupleDColumn(2, ycm);
    analysisManager->FillNtupleDColumn(3, zcm);
    analysisManager->AddNtupleRow();
    //struktura z informacjami o fotonie
    PhotonData pd;
    pd.e = eMeV; 
    pd.x = xcm;
    pd.y = ycm;
    pd.z = zcm;
    pd.time = tns;
    //wektor struktur z informacjami o fotonach w evencie
    fPhotonData.push_back(pd);

    double eEV = eMeV * 1.0e6; 
    double prob = 0.0;
    //symulacja prawdopodobienstwa rejestracji na fotokatodzie - emisji fotoelektornu
    if (eEV <= 7.75) {
      if (G4UniformRand() < 0.75) {
       nPE_thisEvent += 1;
       fPE_Times.push_back(tns); 
      }
    }
  }
  
  event.nPE = nPE_thisEvent;
  //wektor struktur z informacjami o protonach w eventach
  fEventData.push_back(event);
  fPhotonsCountPerEvent.push_back(nPE_thisEvent);

}

