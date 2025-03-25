#include "MyRunAction.hh"
#include "G4Run.hh"
#include "MyPrimaryGeneratorAction.hh"
#include "MyRun.hh"
#include "globals.hh"
#include "MyAnalysis.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include <cmath>

G4bool MyRunAction::useRandomPosition = false;
G4bool MyRunAction::useRandomAngle = false;
G4bool MyRunAction::useRandomEnergy = false;

MyRunAction::MyRunAction(MyPrimaryGeneratorAction* generator)
: G4UserRunAction()
,fPrimaryGenerator(generator)
{}

MyRunAction::~MyRunAction() 
{}
G4bool MyRunAction::GetUseRandomPosition() {
    return useRandomPosition;
}

G4bool MyRunAction::GetUseRandomAngle() {
    return useRandomAngle;
}
G4bool MyRunAction::GetUseRandomEnergy() {
    return useRandomEnergy;
}

void MyRunAction::BeginOfRunAction(const G4Run* aRun)
{
    G4cout << "MyLog:   Begin of run" << G4endl;
    // Create analysis manager
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    analysisManager->SetVerboseLevel(1);
  
    analysisManager->SetFirstNtupleId(1);
    //pobranie flag z generacji
    useRandomPosition = fPrimaryGenerator->GetRandomPosition();
    useRandomAngle = fPrimaryGenerator->GetRandomAngle();
    useRandomEnergy = fPrimaryGenerator->GetRandomEnergy();
    //tworzenie odpowiednich plikow i histogramow w zaleznosci od ustawien flag
    if (useRandomPosition && !useRandomAngle && !useRandomEnergy) {
        analysisManager->OpenFile("RnadomPosition.root");
        analysisManager->CreateH2("NumberOfPEvsDistance", "Number of PE vs Distance", 150, 0., 3*std::sqrt(2), 400, 0., 1600.);
        analysisManager->CreateH1("NumberOfPEvsDistance1D",  "Number of PE vs Distance (Averaged)", 150, 0., 3*std::sqrt(2));
    } else if (!useRandomPosition && useRandomAngle && !useRandomEnergy) {
        analysisManager->OpenFile("RandomAngle.root");
        analysisManager->CreateH2("NumberOfPEvsTheta", "Number of PE vs Theta", 150,  0., std::atan((3.0 - 0.0005) / 0.5), 400, 0., 1600.);
        analysisManager->CreateH2("NumberOfPEvsPhi", "Number of PE vs Phi", 150,  0., 2*CLHEP::pi, 400, 0., 1600.);
        analysisManager->CreateH2("NumberOfPEvsAngle", "Number of PE vs Angle", 50,  0., 2*CLHEP::pi, 50,  0., std::atan((3.0 - 0.0005) / 0.5));
        analysisManager->CreateH3("NumberOfPEvsThetaPhi", "Number of PE vs Theta vs Phi", 50, 0., 2*CLHEP::pi, 50, 0., std::atan((3.0 - 0.0005) / 0.5), 100, 0., 1600.);
    } else if (!useRandomPosition && !useRandomAngle) {
        if (useRandomEnergy) {
            analysisManager->OpenFile("RandomEnergy.root");
            analysisManager->CreateH2("NumberOfPEvsProtonEnergy", "Number of PE vs Proton Energy", 110,  0.1, 220., 700, 0., 1400.);
            analysisManager->CreateH1("NumberOfPEvsProtonEnergy1D","Number of PE vs Proton Energy (Averaged)", 110, 0.1, 220.);
        } else {
            analysisManager->OpenFile("Set.root");
            analysisManager->CreateH2("hPhotonXZ", "Photons on photocathode", 200, 10.4, 15.5, 200, -2.55, 2.55);
            analysisManager->CreateH1("hPhotonEnergy", "Photon energy on cathode", 100, 0.0, 10.0);
            analysisManager->CreateH1("hPhotoElectronsPerEvent", "Number of photoelectrons per event", 60, 800., 1400.);
            analysisManager->CreateH1("hPE_Time", "Photoelectrons time distribution", 60, 0.8, 1.4);
        }
    } else{
        analysisManager->OpenFile("OtherOption.root");
    }
    analysisManager->CreateNtuple("PhotonCollection", "Kolekcja fotonow");
    analysisManager->CreateNtupleDColumn("E");
    analysisManager->CreateNtupleDColumn("X");
    analysisManager->CreateNtupleDColumn("Y");
    analysisManager->CreateNtupleDColumn("Z");
    analysisManager->FinishNtuple();
}

void MyRunAction::EndOfRunAction(const G4Run* aRun)
{
G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    MyRun* theRun = (MyRun*) aRun;

    if (useRandomPosition && !useRandomAngle && !useRandomEnergy) {
        G4int idNumberOfPEvsDistance = analysisManager->GetH2Id("NumberOfPEvsDistance");
        G4int idNumberOfPEvsDistance1D = analysisManager->GetH1Id("NumberOfPEvsDistance1D");
        //obliczanie srednich dla binow jeeli jest wiecej niz 1 event dla jednego
        std::map<G4int, std::pair<G4double, G4int>> distanceBinContent; 

        auto histDistance = analysisManager->GetH2(idNumberOfPEvsDistance);
        G4double distanceMin = histDistance->get_axis(0).lower_edge();
        G4double distanceMax = histDistance->get_axis(0).upper_edge();
        G4int nBinsDistance = histDistance->get_axis(0).bins();
        G4double binWidthDistance = (distanceMax - distanceMin) / nBinsDistance;


        for (auto& pd : theRun->fEventData) {
            G4double R = std::sqrt(pd.protonY * pd.protonY + pd.protonZ * pd.protonZ);
            analysisManager->FillH2(idNumberOfPEvsDistance, R, pd.nPE);
                
            if (R < distanceMin || R >= distanceMax) continue;

            G4int binX = static_cast<G4int>((R - distanceMin) / binWidthDistance);
    
            if (distanceBinContent.find(binX) != distanceBinContent.end()) {
                distanceBinContent[binX].first += pd.nPE;
                distanceBinContent[binX].second += 1;
            } else {
                distanceBinContent[binX] = std::make_pair(pd.nPE, 1);
            }
        }

        for (const auto& entry : distanceBinContent) {
        G4int binX = entry.first;
        G4double avgPE = entry.second.first / static_cast<G4double>(entry.second.second);
        G4double distanceValue = distanceMin + (binX + 0.5) * binWidthDistance;

        analysisManager->FillH1(idNumberOfPEvsDistance1D, distanceValue, avgPE);

        }
    } else if (!useRandomPosition && useRandomAngle && !useRandomEnergy) {
        G4int idNumberOfPEvsTheta = analysisManager->GetH2Id("NumberOfPEvsTheta");
        G4int idNumberOfPEvsPhi = analysisManager->GetH2Id("NumberOfPEvsPhi");
        G4int idNumberOfPEvsAngle = analysisManager->GetH2Id("NumberOfPEvsAngle");
        G4int idNumberOfPEvsThetaPhi = analysisManager->GetH3Id("NumberOfPEvsThetaPhi");
        //obliczanie sredniej jezeli wiecej niz 1 event w binie
        std::map<std::pair<G4int, G4int>, std::pair<G4double, G4int>> binContent;  

        auto histAngle = analysisManager->GetH2(idNumberOfPEvsAngle);

        G4double phiMin = histAngle->get_axis(0).lower_edge();
        G4double phiMax = histAngle->get_axis(0).upper_edge();
        G4double thetaMin = histAngle->get_axis(1).lower_edge();
        G4double thetaMax = histAngle->get_axis(1).upper_edge();

        G4int nBinsPhi = histAngle->get_axis(0).bins();
        G4int nBinsTheta = histAngle->get_axis(1).bins();

        G4double binWidthPhi = (phiMax - phiMin) / nBinsPhi;
        G4double binWidthTheta = (thetaMax - thetaMin) / nBinsTheta;

        for (auto& pd : theRun->fEventData) {
            analysisManager->FillH2(idNumberOfPEvsTheta, pd.protonTheta, pd.nPE);
            analysisManager->FillH2(idNumberOfPEvsPhi, pd.protonPhi, pd.nPE);
            analysisManager->FillH3(idNumberOfPEvsThetaPhi, pd.protonTheta, pd.protonPhi, pd.nPE);

            G4int binX = static_cast<G4int>((pd.protonPhi - phiMin) / binWidthPhi);
            G4int binY = static_cast<G4int>((pd.protonTheta - thetaMin) / binWidthTheta);

            std::pair<G4int, G4int> binIndex = std::make_pair(binX, binY);

            if (binContent.find(binIndex) != binContent.end()) {
                binContent[binIndex].first += pd.nPE;
                binContent[binIndex].second += 1;
            } else {
                binContent[binIndex] = std::make_pair(pd.nPE, 1);
            }
        }

        for (const auto& entry : binContent) {
            G4int binX = entry.first.first;
            G4int binY = entry.first.second;
            G4double avgPE = entry.second.first / entry.second.second;

            G4double phiValue = phiMin + (binX + 0.5) * binWidthPhi;
            G4double thetaValue = thetaMin + (binY + 0.5) * binWidthTheta;

            analysisManager->FillH2(idNumberOfPEvsAngle, phiValue, thetaValue, avgPE);
        }

    } else if (!useRandomPosition && !useRandomAngle) {
        if (useRandomEnergy) {
            G4int idNumberOfPEvsProtonEnergy = analysisManager->GetH2Id("NumberOfPEvsProtonEnergy");
             G4int idNumberOfPEvsProtonEnergy1D = analysisManager->GetH1Id("NumberOfPEvsProtonEnergy1D");
            //obliczanie sredniej jezeli wiecej niz 1 event w binie
            std::map<G4int, std::pair<G4double, G4int>> energyBinContent;

            auto histEnergy = analysisManager->GetH2(idNumberOfPEvsProtonEnergy);
            G4double energyMin = histEnergy->get_axis(0).lower_edge();
            G4double energyMax = histEnergy->get_axis(0).upper_edge();
            G4int nBinsEnergy = histEnergy->get_axis(0).bins();
            G4double binWidthEnergy = (energyMax - energyMin) / nBinsEnergy;

            for (auto& pd : theRun->fEventData) {
                G4int binX = static_cast<G4int>((pd.protonEnergy - energyMin) / binWidthEnergy);
                analysisManager->FillH2(idNumberOfPEvsProtonEnergy, pd.protonEnergy, pd.nPE);

                if (energyBinContent.find(binX) != energyBinContent.end()) {
                    energyBinContent[binX].first += pd.nPE;
                    energyBinContent[binX].second += 1;
                } else {
                    energyBinContent[binX] = std::make_pair(pd.nPE, 1);
                }
            }

            for (const auto& entry : energyBinContent) {
                G4int binX = entry.first;
                G4double avgPE = entry.second.first / entry.second.second;
                G4double energyValue = energyMin + (binX + 0.5) * binWidthEnergy;
                analysisManager->FillH1(idNumberOfPEvsProtonEnergy1D, energyValue, avgPE);
            }
        } else {
            G4int idPhotonXZ = analysisManager->GetH2Id("hPhotonXZ");
            G4int idPhotonEnergy = analysisManager->GetH1Id("hPhotonEnergy");
            G4int idPhotoElectrons = analysisManager->GetH1Id("hPhotoElectronsPerEvent");
            G4int idPE_Time = analysisManager->GetH1Id("hPE_Time");
            for (auto& pd : theRun->fPhotonData) {
                analysisManager->FillH2(idPhotonXZ, pd.x, pd.z);
                analysisManager->FillH1(idPhotonEnergy, pd.e * 1.0e6); 
            }
            for (G4int nPE : theRun->fPhotonsCountPerEvent) {
                analysisManager->FillH1(idPhotoElectrons, nPE);
            }
            for (G4double t : theRun->fPE_Times) {
                analysisManager->FillH1(idPE_Time, t, 1);
            }
        }
    }

G4cout << "MyLog:   End of run" << G4endl;
  
  analysisManager->Write();
  analysisManager->CloseFile();
  
}

G4Run* MyRunAction::GenerateRun()
{
 return new MyRun();
}
