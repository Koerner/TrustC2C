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
      int PropDetects;
      int PropHonest;
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

    database(unsigned long size, QList<QPair<int, int>> PropDetects, QList<QPair<int, int>> PropHonest);

    //get function
    QList<QPair<bool, double>> getCarTrust(unsigned long int CarBID, unsigned long int CarXid) const;
    QList<QPair<bool, double>> getCarReputation(unsigned long int CarAID, unsigned long int CarBID) const;

    int getCarPropDetects(unsigned long CarID);

    QList<unsigned long> getCarIDsAscendingReputation(unsigned long CarB);

    //write function
    void writeTrustX(unsigned long int CarOwnID, unsigned long int CarXID, QPair<bool, double> trust);
    void writeReputationB(unsigned long int CarOwnID, unsigned long int CarBID, QPair<bool, double> reputationTrust);

    //Log functions
    double getAverageTrust(unsigned long int CarBID, unsigned long int CarXid);

    void writeInteractionLog (QVector<unsigned long int> carIDs, bool truth, bool xSays, bool success, QPair<bool, double> descissionResult);
    void saveToFileTrust();
    void saveToFileReputation();




private:
    QVector<car> carVector;

    int calcProp(int CarID, QList<QPair<int, int>> prop);
};

#endif // DATABASE_H
