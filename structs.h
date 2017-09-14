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
  randomQ carSelectRandAX;
  randomQ carSelectRandB;
  randomQ carB2SelectRand;
  randomQ Poison;
} ;

struct settingsGUI {
  unsigned int numInteractionsFirst;
  unsigned int numInteractionsSecond;
  unsigned int numTotalCarsFirst;
  unsigned int numTotalCarsSecond;
  unsigned int numCarsRecommending;
  int maxRecomendingDepth;
  int maxLevelWidth;


  bool certaintyXon;

  QList<QPair<unsigned long int,QPair<double, double>>> PropDetectsPrediction;
  QList<QPair<unsigned long int,QPair<double, double>>> PropDetectsObservation;
  QList<QPair<unsigned long int,QPair<double, double>>> PropDetectsX;
  QList<QPair<unsigned long int,QPair<double, double>>> PropHonestCarX;
} ;

struct interactionLog {
    bool truth;
    QPair<bool,double> carXKnowsTruth;
    bool carXthinks;
    bool carXHonest;
    QPair<bool,double> carXsays;
    //bool carAKnowsTruth;
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
