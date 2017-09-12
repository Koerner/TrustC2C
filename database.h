#ifndef DATABASE_H
#define DATABASE_H
#include <QVector>
#include <QtDebug>
#include <QList>
#include "average.h"
#include "trustreputational.h"


class database
{

public:


    struct car {
      unsigned long int id;
      QPair <double, double> PropDetectsPrediction;
      QPair <double, double> PropDetectsObservation;
      QPair <double, double> PropHonest;
      QMultiMap<unsigned long int, QPair<bool, double>> trustMap; //Car ID, value
      QMap<unsigned long int, double > trustSumMap; //Sum of trust values for each CarID
      QMultiMap<unsigned long int, QPair<bool, double>> reputationMap; //Car ID, value
      QMap<unsigned long int, double> reputationSumMap; //Sum of reputation values for each CarID
    } ;

    struct interactionLogStruct {
        QVector<unsigned long int> carIDs;
        bool truth;
        bool xSays;
        bool success;
        QPair<bool, double> descissionResult;
    } ;

    QVector<interactionLogStruct> interactionLogList;

    database(unsigned long size, QList<QPair<int, QPair<double, double>> > PropDetectsPrediction, QList<QPair<int, QPair<double, double> > > PropDetectsObservation, QList<QPair<int, QPair<double, double>> > PropHonest);

    //get function
    QList<QPair<bool, double>> getCarTrust(unsigned long int CarBID, unsigned long int CarXid) const;
    QList<QPair<bool, double>> getCarReputation(unsigned long int CarAID, unsigned long int CarBID) const;

    double getCarPropDetectsObservation(unsigned long int CarID);
    double getCarPropDetectsObservationStdDist(unsigned long int CarID);
    double getCarPropDetectsPrediction(unsigned long int CarID);
    double getCarPropDetectsPredictionStdDist(unsigned long int CarID);
    double getCarPropHonest(unsigned long int CarID);
    double getCarPropHonestStdDist(unsigned long CarID);

    QList<unsigned long> getCarIDsAscendingReputation(unsigned long CarB);

    //write function
    void writeTrustX(unsigned long int CarOwnID, unsigned long int CarXID, QPair<bool, double> trust);
    void writeReputationB(unsigned long int CarOwnID, unsigned long int CarBID, QPair<bool, double> reputationTrust);

    //Log functions
    double getAverageTrust(unsigned long int CarBID, unsigned long int CarXid);

    void writeInteractionLog (QVector<unsigned long int> carIDs, bool truth, bool xSays, bool success, QPair<bool, double> descissionResult);
    void saveToFileTrust();
    void saveToFileReputation();




    void addCar(QPair<double, double> PropDetectsPrediction, QPair<double, double> PropDetectsObservation, QPair<double, double> PropHonest);
    int sizeCarsVector();

private:
    QVector<car> carVector;

    QPair <double, double> calcProp(unsigned long CarID, QList<QPair<int, QPair<double, double>>> prop);
};

#endif // DATABASE_H
