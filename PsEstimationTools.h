#ifndef ESTIMATIONTOOLS
#define ESTIMATIONTOOLS 1

#include "PsConstants.h"
#include "PsEvent.h"
#include <TH1F.h>
#include <vector>
#include <map>

class PsEstimationTools
{
public:
  static std::map<std::string, TH1F*> makeHistMap(double step);
  static std::vector<PsEvent> generateEvents(
    double activity, std::map<std::string, TH1F*>& histogramMap
  );
  static void sortByTime(std::vector<PsEvent>& eventVec);
  static void checkForCoincidences(
    std::vector<PsEvent>& eventVec, double activity, std::map<std::string, TH1F*>& histogramMap
  );
  static bool wasReconstructed(PsEvent::MainType type, double recoProb);
  static void drawHistograms(std::map<std::string, TH1F*>& histogramMap, double step);
};

#endif /* !ESTIMATIONTOOLS */
