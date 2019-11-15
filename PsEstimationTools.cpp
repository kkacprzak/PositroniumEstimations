#include "PsEstimationTools.h"
#include <TRandom.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TStyle.h>
#include <iostream>

std::map<std::string, TH1F*> PsEstimationTools::makeHistMap(double step)
{
  // Define histograms
  TH1F* ratioGoodHist = new TH1F("ratio_good", "  ",
    INTERVALS, ACT_MIN-step/2, ACT_MAX-step/2
  );
  ratioGoodHist->GetXaxis()->SetTitle("Source activity [MBq]");
  ratioGoodHist->SetLineColorAlpha(kGreen, 1);
  ratioGoodHist->SetLineWidth(3);

  TH1F* ratioBadHist = new TH1F("ratio_bad", "  ",
    INTERVALS, ACT_MIN-step/2, ACT_MAX-step/2
  );
  ratioBadHist->GetXaxis()->SetTitle("Source activity [MBq]");
  ratioBadHist->SetLineColorAlpha(kRed, 1);
  ratioBadHist->SetLineWidth(3);

  TH1F* goodHist = new TH1F("good", "  ",
    INTERVALS, ACT_MIN-step/2, ACT_MAX-step/2
  );
  goodHist->GetXaxis()->SetTitle("Source activity [MBq]");
  goodHist->SetLineColorAlpha(kGreen, 1);
  goodHist->SetLineWidth(3);

  TH1F* badHist = new TH1F("bad", "  ",
    INTERVALS, ACT_MIN-step/2, ACT_MAX-step/2
  );
  badHist->GetXaxis()->SetTitle("Source activity [MBq]");
  badHist->SetLineColorAlpha(kRed, 1);
  badHist->SetLineWidth(3);

  TH1F* deexHist = new TH1F("deex_times", "  ", 200, 0.0, 10.0);
  deexHist->GetXaxis()->SetTitle("Time diff * Act [#mus * MBq]");
  deexHist->SetLineColorAlpha(kBlack, 1);
  deexHist->SetLineWidth(2);

  TH1F* oPsHist = new TH1F("oPs_gen_times", "  ", 200, 0.0, 10.0*ORTHO_LIFETIME);
  oPsHist->GetXaxis()->SetTitle("lifetime [#mus]");
  oPsHist->SetLineColorAlpha(kBlue, 1);
  oPsHist->SetLineWidth(2);

  TH1F* pPsHist = new TH1F("pPs_gen_times", "  ", 200, 0.0, 10.0*PARA_LIFETIME);
  pPsHist->GetXaxis()->SetTitle("lifetime [#mus]");
  pPsHist->SetLineColorAlpha(kBlack, 1);
  pPsHist->SetLineWidth(2);

  std::map<std::string, TH1F*> histogramMap;
  histogramMap["ratio_good"] = ratioGoodHist;
  histogramMap["ratio_bad"] = ratioBadHist;
  histogramMap["good"] = goodHist;
  histogramMap["bad"] = badHist;
  histogramMap["deex_times"] = deexHist;
  histogramMap["oPs_gen_times"] = oPsHist;

  for(double activity=ACT_MIN; activity<ACT_MAX; activity+=step){
    TH1F* oPsObsHist = new TH1F(Form("oPs_obs_%f", activity), "  ", 200, 0.0, 10.0*ORTHO_LIFETIME);
    oPsObsHist->GetXaxis()->SetTitle("lifetime [#mus]");
    oPsObsHist->SetLineColorAlpha(kMagenta, 1);
    oPsObsHist->SetLineWidth(2);
    histogramMap[Form("oPs_obs_%f", activity)] = oPsObsHist;
  }

  return histogramMap;
}

std::vector<PsEvent> PsEstimationTools::generateEvents(
  double activity, std::map<std::string, TH1F*>& histogramMap
){
  std::vector<PsEvent> eventsVec;
  double deexAbsTime = 0.0;
  int eventsNum = (int) (activity * TIMESLOT);
  for(int i=0; i<eventsNum; i++){
    double deexRelTime = gRandom->Exp(1/activity);
    deexAbsTime += deexRelTime;
    histogramMap.at("deex_times")->Fill(deexRelTime*activity);

    // EC vs. e+ emmission
    if(gRandom->Uniform() < ELECAP) {
      // Electron capture - only deexcitation photon
      PsEvent ec(deexAbsTime, gRandom->Uniform(), PsEvent::ONE_GAMMA, PsEvent::EC);
      eventsVec.push_back(ec);
    } else {
      // e+ emission case
      if(gRandom->Uniform() < PS_FORMATION) {
        // e+ forms positronium in a material
        if(gRandom->Uniform() < OPS2PPS) {

          // generate times for deex and oPs
          double oPsRelTime = gRandom->Exp(ORTHO_LIFETIME);
          double oPsAbsTime = deexAbsTime + oPsRelTime;
          histogramMap.at("oPs_gen_times")->Fill(oPsRelTime);

          // Creating objects and setting as piared events
          PsEvent deex(deexAbsTime, gRandom->Uniform(), PsEvent::ONE_GAMMA, PsEvent::DEEX);
          PsEvent oPs(oPsAbsTime, gRandom->Uniform(), PsEvent::THREE_GAMMA, PsEvent::OPS);
          deex.setPair(oPs.getTime(), oPs.getRecoProb(), oPs.getMainType(), oPs.getSubType());
          oPs.setPair(deex.getTime(), deex.getRecoProb(), deex.getMainType(), deex.getSubType());

          // Adding to vector
          eventsVec.push_back(deex);
          eventsVec.push_back(oPs);
        } else {
          // generate times for deex and pPs
          double pPsRelTime = gRandom->Exp(PARA_LIFETIME);
          double pPsAbsTime = deexAbsTime + pPsRelTime;
          histogramMap.at("pPs_times")->Fill(pPsRelTime);

          // Creating objects and setting as piared events
          PsEvent deex(deexAbsTime, gRandom->Uniform(), PsEvent::ONE_GAMMA, PsEvent::DEEX);
          PsEvent pPs(pPsAbsTime, gRandom->Uniform(), PsEvent::TWO_GAMMA, PsEvent::PPS);
          deex.setPair(pPs.getTime(), pPs.getRecoProb(), pPs.getMainType(), pPs.getSubType());
          pPs.setPair(deex.getTime(), deex.getRecoProb(), deex.getMainType(), deex.getSubType());

          // Adding to vector
          eventsVec.push_back(deex);
          eventsVec.push_back(pPs);
        }
      } else {
        // deexcitation photon emmited, but no positronium formed
        PsEvent noPs(deexAbsTime, gRandom->Uniform(), PsEvent::ONE_GAMMA, PsEvent::NO_PS);
        eventsVec.push_back(noPs);
      }
      // TODO add other scenarios: pick-off and direct annihilaiton
    }
  }
  return eventsVec;
}

void PsEstimationTools::sortByTime(std::vector<PsEvent>& eventsVec)
{
  sort(eventsVec.begin(), eventsVec.end(),
    [](PsEvent evt1, PsEvent evt2) {
      return evt1.getTime() < evt2.getTime();
    }
  );
}

bool PsEstimationTools::wasReconstructed(PsEvent::MainType type, double recoProb)
{
  switch (type) {
    case PsEvent::ONE_GAMMA:
      if(recoProb < EFF_ONE_GAMMA){
        return true;
      } else {
        return false;
      }
      break;
    case PsEvent::TWO_GAMMA:
      if(recoProb < EFF_TWO_GAMMA){
        return true;
      } else {
        return false;
      }
      break;
    case PsEvent::THREE_GAMMA:
      if(recoProb < EFF_THREE_GAMMA){
        return true;
      } else {
        return false;
      }
      break;
  }
  // In  any other case
  return false;
}

void PsEstimationTools::checkForCoincidences(
  std::vector<PsEvent>& eventsVec, double activity, std::map<std::string, TH1F*>& histogramMap
){
  // Iteration starting from 2nd object and comparing with the previous one
  // Checking only oPs events to get lifetime spectra
  for(int i=1; i<eventsVec.size(); i++){
    auto evtLatter = eventsVec.at(i);
    if(!wasReconstructed(evtLatter.getMainType(), evtLatter.getRecoProb())) { continue; }

    if(evtLatter.getMainType()==PsEvent::THREE_GAMMA) {
      // There is a reconstructed 3 gamma event, looking for single photon
      int compareToIndex = i-1;
      while(compareToIndex>=0){
        auto evtFormer = eventsVec.at(compareToIndex);
        if(!wasReconstructed(evtFormer.getMainType(), evtFormer.getRecoProb())) {
          compareToIndex--;
          continue;
        }

        // The photon was a single photon reconstructed
        if(evtFormer.getMainType()==PsEvent::ONE_GAMMA){
          // There is 3gamma and 1gamma pair, checking if there is a match
          if(evtLatter.getPairTime()==evtFormer.getTime()){
            // True pair
            histogramMap.at(Form("oPs_obs_%f", activity))->Fill(evtLatter.getTime()-evtFormer.getTime());
            histogramMap.at("good")->Fill(activity);
          } else {
            if(evtFormer.getPairMainType()==PsEvent::TWO_GAMMA
            && wasReconstructed(PsEvent::TWO_GAMMA, evtFormer.getPairRecoProb())){
              // It was regonized as 1gamma - 2gamma pair, which we can cut out
              // Continue searching for another 1gamma
              compareToIndex--;
              continue;
            }
            // False pair
            histogramMap.at(Form("oPs_obs_%f", activity))->Fill(evtLatter.getTime()-evtFormer.getTime());
            histogramMap.at("bad")->Fill(activity);
          }
          break;
        } else {
          compareToIndex--;
          continue;
        }
      }
    }
  }
}

void PsEstimationTools::drawHistograms(std::map<std::string, TH1F*>& histogramMap, double step)
{
  gStyle->SetOptStat(0);

  // Calculations were repeted several times to make some average
  // Normalising histograms to 1.
  histogramMap.at("oPs_gen_times")->Scale(1.0/histogramMap.at("oPs_gen_times")->Integral(), "width");
  histogramMap.at("good")->Scale(1.0/histogramMap.at("good")->Integral(), "width");
  histogramMap.at("bad")->Scale(1.0/histogramMap.at("bad")->Integral(), "width");

  // Drawing and saving to a file
  for(double activity=ACT_MIN; activity<ACT_MAX; activity+=step){
    histogramMap.at(Form("oPs_obs_%f", activity))->Scale(1.0/histogramMap.at(Form("oPs_obs_%f", activity))->Integral(), "width");
    TCanvas* c1 = new TCanvas(Form("oPs_obs_%f", activity), Form("Lifetimes %f", activity), 800, 800);
    auto legend1 = new TLegend(0.5, 0.7, 0.9, 0.9);
    legend1->SetHeader("oPs lifetime", "C");
    legend1->AddEntry(histogramMap.at("oPs_gen_times"), "Generated", "l");
    legend1->AddEntry(histogramMap.at(Form("oPs_obs_%f", activity)), "Observed", "l");
    histogramMap.at(Form("oPs_obs_%f", activity))->Draw("hist");
    histogramMap.at("oPs_gen_times")->Draw("hist same");
    legend1->Draw();
    c1->SaveAs(Form("lifetimes_%f.png", activity));
  }

  TCanvas* c2 = new TCanvas("counts", "Events number", 800, 800);
  c2->SetLogy();
  auto legend2 = new TLegend(0.1, 0.8, 0.4, 0.9);
  legend2->SetHeader("Coincidences", "C");
  legend2->AddEntry(histogramMap.at("good"), "True", "l");
  legend2->AddEntry(histogramMap.at("bad"), "Accidental", "l");
  histogramMap.at("bad")->Draw("hist");
  histogramMap.at("good")->Draw("hist same");
  legend2->Draw();
  c2->SaveAs("accidentals.png");
}
