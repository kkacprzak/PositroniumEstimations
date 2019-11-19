#include "PsEstimationTools.h"
#include <iostream>
#include <map>

int main() {

  double step = (ACT_MAX-ACT_MIN)/INTERVALS;
  auto histogramMap = PsEstimationTools::makeHistMap(step);

  for(double activity=ACT_MIN; activity<ACT_MAX; activity+=step){
    std::cout << activity << std::endl;
    // Optimized number of repetitions to get smooth statistics
    for(int i=0;i<(1+REPEAT*(INTERVALS-i)/INTERVALS);i++){
      auto eventsVec = PsEstimationTools::generateEvents(activity, histogramMap);
      PsEstimationTools::sortByTime(eventsVec);
      PsEstimationTools::checkForCoincidences(eventsVec, activity, histogramMap);
    }
  }

  PsEstimationTools::drawHistograms(histogramMap, step);
  return 0;
}
