#ifndef STRUCTS_H
#define STRUCTS_H

#include <QPair>
#include <QVector>
#include <QList>
#include "randomQ.h"

struct randStruct {
  randomQ truthRand;
  randomQ detectRandX;
  randomQ honestRand;
  randomQ detectRandA;
  randomQ carSelectRand;
  randomQ carB2SelectRand;
  randomQ Poison;
} ;

struct settingsGUI {
  unsigned int numInteractions;
  unsigned int numTotalCars;
  unsigned int numCarsRecommending;
  int maxRecomendingDepth;
  int minRecomendingWidthDirect;
  int minRecomendingWidth;

  bool certaintyXon;

  QList<QPair<int,double>> PropDetectsPrediction;
  QList<QPair<int,double>> PropDetectsObservation;

  unsigned int PropHonestCarX;
  QList<QPair<int,double>> PropHonestCarB;
} ;

struct interactionLog {
    bool truth;
    QPair<bool,double> carXKnowsTruth;
    bool carXthinks;
    bool carXHonest;
    QPair<bool,double> carXsays;
    bool carAKnowsTruth;
    QPair<bool,double> carAprediction;
    QPair<bool,double> carAobservation;
    QVector<unsigned long int> carIDs;
    QList<QPair<double,int>> reputations;
    QList<QPair<double,int>> reputationABXs;
    QPair<bool, double> descissionResult;
    bool correctDecission;
};

#define REP_DEBUG false




#endif // STRUCTS_H
