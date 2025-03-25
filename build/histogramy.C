#include <TFile.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TH3F.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TLegend.h>
#include <iostream>
#include <vector>
#include <cstring>
#include <cstdio>  

// Funkcja do ustawienia indywidualnej palety kolorów
void set_color_palette(const char* histName) {
    if (strcmp(histName, "NumberOfPEvsDistance") == 0 ||
        strcmp(histName, "NumberOfPEvsTheta") == 0 ||
        strcmp(histName, "NumberOfPEvsPhi") == 0 ||
        strcmp(histName, "NumberOfPEvsProtonEnergy") == 0 ||
        strcmp(histName, "NumberOfPEvsAngle") == 0 ||
        strcmp(histName, "NumberOfPEvsThetaPhi") == 0) {
        gStyle->SetPalette(kRainBow);
    } 
    else if (strcmp(histName, "hPhotonXZ") == 0) {
        gStyle->SetPalette(kViridis);
    }
}

// Funkcja do konfiguracji ustawień osi wykresów
void set_axis_style(TH1* hist) {
    hist->GetXaxis()->SetTitleSize(0.04);
    hist->GetYaxis()->SetTitleSize(0.04);
    hist->GetXaxis()->SetLabelSize(0.03);
    hist->GetYaxis()->SetLabelSize(0.02);
}

void save_canvas_with_override(TCanvas* c, const std::string& filename) {
    if (std::remove(filename.c_str()) == 0) {
        std::cout << "Existing file " << filename << " removed." << std::endl;
    }
    c->SaveAs(filename.c_str());
    std::cout << "Saved: " << filename << std::endl;
}

// Funkcja do rysowania histogramów z danego pliku ROOT
void draw_histograms_from_file(const char* filename, const std::vector<const char*>& histNames) {
    TFile* file = new TFile(filename, "READ");
    if (!file || file->IsZombie()) {
        std::cerr << "Error: Could not open file " << filename << "!" << std::endl;
        return;
    }

    std::cout << "Processing file: " << filename << std::endl;

    gStyle->SetOptStat(0);
    gStyle->SetHistLineWidth(2);
    //gStyle->SetOptStat(1111);
    gStyle->SetTitleSize(0.04, "XYZ");

    for (const char* histName : histNames) {
        TObject* obj = file->Get(histName);
        if (!obj) {
            std::cerr << "Warning: Histogram " << histName << " not found in file " << filename << "!" << std::endl;
            continue;
        }

        TCanvas* c = new TCanvas(histName, histName, 1000, 600);

        if (obj->InheritsFrom(TH2::Class())) {
            TH2F* hist2D = (TH2F*)obj;
            set_color_palette(histName);

            hist2D->GetXaxis()->SetTitleSize(0.04);
            hist2D->GetYaxis()->SetTitleSize(0.04);
            hist2D->GetXaxis()->SetLabelSize(0.03);
            hist2D->GetYaxis()->SetLabelSize(0.02);

            if (strcmp(histName, "hPhotonXZ") == 0) {
                hist2D->GetXaxis()->SetTitle("X [cm]");
                hist2D->GetYaxis()->SetTitle("Z [cm]");
            } 
            else if (strcmp(histName, "NumberOfPEvsDistance") == 0) {
                hist2D->GetXaxis()->SetTitle("R [cm]");
                hist2D->GetYaxis()->SetTitle("N_{photoelectrons}");
            } 
            else if (strcmp(histName, "NumberOfPEvsTheta") == 0) {
                hist2D->GetXaxis()->SetTitle("Theta [rad]");
                hist2D->GetYaxis()->SetTitle("N_{photoelectrons}");
            } 
            else if (strcmp(histName, "NumberOfPEvsPhi") == 0) {
                hist2D->GetXaxis()->SetTitle("Phi [rad]");
                hist2D->GetYaxis()->SetTitle("N_{photoelectrons}");
            }
            else if (strcmp(histName, "NumberOfPEvsProtonEnergy") == 0) {
                hist2D->GetXaxis()->SetTitle("E_{proton} [GeV]");
                hist2D->GetYaxis()->SetTitle("N_{photoelectrons}");
            }
            else if (strcmp(histName, "NumberOfPEvsAngle") == 0) {
                hist2D->GetXaxis()->SetTitle("Phi [rad]");
                hist2D->GetYaxis()->SetTitle("Theta [rad]");
            }

            hist2D->Draw("COLZ");
        } 
        else if (obj->InheritsFrom(TH3::Class())) {
            TH3F* hist3D = (TH3F*)obj;
            set_color_palette(histName);
            hist3D->GetXaxis()->SetTitleSize(0.03);
            hist3D->GetYaxis()->SetTitleSize(0.03);
            hist3D->GetXaxis()->SetLabelSize(0.02);
            hist3D->GetYaxis()->SetLabelSize(0.02);
            hist3D->GetZaxis()->SetTitleSize(0.03);
            hist3D->GetZaxis()->SetLabelSize(0.02);

            hist3D->GetXaxis()->SetTitle("Theta [rad]");
            hist3D->GetYaxis()->SetTitle("Phi [rad]");
            hist3D->GetZaxis()->SetTitle("N_{photoelectrons}");

            hist3D->Draw("BOX2");
        } 
        else if (obj->InheritsFrom(TH1::Class())) {
            TH1F* hist = (TH1F*)obj;
            set_axis_style(hist);
            gStyle->SetOptStat(1111);

            if (strcmp(histName, "hPhotonEnergy") == 0) {
                hist->SetFillColor(kBlue);
                hist->GetXaxis()->SetTitle("E_{photons} [eV]");
                hist->GetYaxis()->SetTitle("N_{photoelectrons}");
            } 
            else if (strcmp(histName, "hPhotoElectronsPerEvent") == 0) {
                hist->SetFillColor(kRed);
                hist->GetXaxis()->SetTitle("N_{photoelectrons}");
                hist->GetYaxis()->SetTitle("N_{events}");
            } 
            else if (strcmp(histName, "hPE_Time") == 0) {
                hist->SetFillColor(kGreen);
                hist->GetXaxis()->SetTitle("t [ns]");
                hist->GetYaxis()->SetTitle("N_{phototelectrons}");
            }
            else if (strcmp(histName, "NumberOfPEvsDistance1D") == 0) {
                hist->SetFillColor(kGreen);
                hist->GetXaxis()->SetTitle("R [cm]");
                hist->GetYaxis()->SetTitle("N_{photoelectrons}");
            } else if (strcmp(histName, "NumberOfPEvsProtonEnergy1D") == 0) {
                hist->SetFillColor(kGreen);
                hist->GetXaxis()->SetTitle("E_{proton} [GeV]");
                hist->GetYaxis()->SetTitle("N_{photoelectrons}");
            }

            hist->Draw("HIST");
        }

        std::string imgName = std::string(filename) + "_" + histName + ".png";
        save_canvas_with_override(c, imgName);

        c->Close();
        delete c;
    }

    file->Close();
}

void draw_all_histograms() {
    std::vector<std::pair<const char*, std::vector<const char*>>> files = {
        {"RnadomPosition.root", {"NumberOfPEvsDistance","NumberOfPEvsDistance1D"}},
        {"RandomAngle.root", {"NumberOfPEvsTheta", "NumberOfPEvsPhi", "NumberOfPEvsAngle", "NumberOfPEvsThetaPhi"}},
        {"RandomEnergy.root", {"NumberOfPEvsProtonEnergy","NumberOfPEvsProtonEnergy1D"}},
        {"Set.root", {"hPhotonXZ", "hPhotonEnergy", "hPhotoElectronsPerEvent", "hPE_Time"}}
    };

    for (const auto& file : files) {
        draw_histograms_from_file(file.first, file.second);
    }

    std::cout << "All histograms have been processed and saved." << std::endl;
}
