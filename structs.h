#ifndef STRUCTS_H
#define STRUCTS_H

struct randStruct {
  randomQ truthRand;
  randomQ detectRandX;
  randomQ honestRand;
  randomQ detectRandA;
  randomQ carSelectRand;
  randomQ carB2SelectRand;
} ;

struct settingsGUI {
  unsigned int numInteractions;
  unsigned int numTotalCars;
  unsigned int numCarsRecommending;
  int numRecomendingDepth;

  QList<QPair<int,int>> PropDetectsCarX;
  //unsigned int PropDetectsCarB;

  unsigned int PropHonestCarX;
  QList<QPair<int,int>> PropHonestCarB;
} ;




#endif // STRUCTS_H
