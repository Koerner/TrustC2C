#ifndef STRUCTS_H
#define STRUCTS_H

struct randStruct {
  randomQ truthRand;
  randomQ detectRandX;
  randomQ honestRand;
  randomQ detectRandA;
  randomQ carSelectRand;
} ;

struct settingsGUI {
  unsigned int numInteractions;
  unsigned int numTotalCars;
  unsigned int numCarsRecommending;

  unsigned int PropDetectsCarA;
  QList<QPair<int,int>> PropDetectsCarX;
  //unsigned int PropDetectsCarB;

  unsigned int PropHonestCarX;
  QList<QPair<int,int>> PropHonestCarB;
} ;




#endif // STRUCTS_H
