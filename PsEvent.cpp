#include "PsEvent.h"

PsEvent::PsEvent(double time, double regProb, MainType mainType, SubType subType):
  fTime(time), fRecoProb(regProb), fMainType(mainType), fSubType(subType), fHasPair(false){}

double PsEvent::getTime()
{
  return fTime;
}

double PsEvent::getRecoProb()
{
  return fRecoProb;
}

PsEvent::MainType PsEvent::getMainType()
{
  return fMainType;
}

PsEvent::SubType PsEvent::getSubType()
{
  return fSubType;
}

bool PsEvent::hasPair()
{
  return fHasPair;
}

double PsEvent::getPairTime()
{
  return fPairTime;
}

double PsEvent::getPairRecoProb()
{
  return fPairRecoProb;
}

PsEvent::MainType PsEvent::getPairMainType()
{
  return fPairMainType;
}

PsEvent::SubType PsEvent::getPairSubType()
{
  return fPairSubType;
}

void PsEvent::setPair(double pairTime, double pairRecoProb, MainType pairMainType, SubType pairSubType)
{
  fHasPair = true;
  fPairTime = pairTime;
  fPairRecoProb = pairRecoProb;
  fPairMainType = pairMainType;
  fPairSubType = pairSubType;
}
