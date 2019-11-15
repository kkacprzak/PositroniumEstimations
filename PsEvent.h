#ifndef PSEVENT
#define PSEVENT 1

class PsEvent
{
public:
  enum MainType { ONE_GAMMA, TWO_GAMMA, THREE_GAMMA, WHTEVR };
  enum SubType { DEEX, NO_PS, EC, PPS, DIRECT, PICKOFF, OPS, SCATT3G, SUBWHTEVR };

  PsEvent(double time, double regProb, MainType mainType, SubType subType);
  double getTime();
  double getRecoProb();
  MainType getMainType();
  SubType getSubType();
  bool hasPair();
  double getPairTime();
  double getPairRecoProb();
  MainType getPairMainType();
  SubType getPairSubType();
  void setPair(double pairTime, double pairRecoProb, MainType pairMainType, SubType pairSubType);

private:
  double fTime = -1.0;
  double fRecoProb = -1.0;
  MainType fMainType = MainType::WHTEVR;
  SubType fSubType = SubType::SUBWHTEVR;
  bool fHasPair = false;
  double fPairTime = -1.0;
  double fPairRecoProb = -1.0;
  MainType fPairMainType = MainType::WHTEVR;
  SubType fPairSubType = SubType::SUBWHTEVR;
};

#endif /* !PSEVENT */
