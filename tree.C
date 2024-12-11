#include <iostream>
#include <TMath.h>
#include "TTree.h"
#include "TFile.h"
#include <math.h>

double fitfunc(double *x, double *par){
    return par[0]*pow(x[0],par[1])*TMath::Exp(par[2]*x[0]);
}

void tree(){
    TFile file("tree.root", "read");
    TTree *tree =(TTree*)file.Get("h10");

    TFile filteredfile("filteredroot.root", "recreate");
    auto filteredtree = tree->CopyTree("Isrfilter == 1 && chi2_3p<30");
    filteredfile.Write();

    filteredtree->SetBranchStatus("*", 0);
    for (auto activeBranchName : {"nph", "eph", "thetaph", "phiph"}){
        filteredtree->SetBranchStatus(activeBranchName, 1);
    }
    TFile newfile("newroot.root", "recreate");
    auto newtree = filteredtree->CloneTree();
    newfile.Write();

    double max = newtree->GetMaximum("eph");
    double min = newtree->GetMinimum("eph");
    cout << max << " " << min <<endl;

    TF1 *energfit = new TF1("energfit","fitfunc", 0, 9, 3);
    newtree->Fit("energfit","eph");

    TCanvas *canvas = new TCanvas();
    newtree->Draw("eph");
    energfit->Draw("Same");
    newtree->GetHistogram()->GetXaxis()->SetTitle("energy, MeV");
    newtree->GetHistogram()->GetYaxis()->SetTitle("entries");
    newtree->GetHistogram()->SetTitle("photon enegy");
    gPad->SetLogy();
}