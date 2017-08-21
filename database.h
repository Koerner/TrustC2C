#ifndef DATABASE_H
#define DATABASE_H
#include <QVector>
#include <QtDebug>
#include <QList>


class database
{

public:


    struct car {
      unsigned long int id;
      int PropDetects;
      int PropHonest;
      QMultiMap<unsigned long int, double> trustMap; //Car ID, value
      QMultiMap<unsigned long int, double> reputationMap; //Car ID, value
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
    QList<double> getCarTrust(unsigned long int CarBID, unsigned long int CarXid) const;
    QList<double> getCarReputation(unsigned long int CarAID, unsigned long int CarBID) const;

    //write function
    void writeTrustX(unsigned long int CarOwnID, unsigned long int CarXID, double trust);
    void writeReputationB(unsigned long int CarOwnID, unsigned long int CarBID, double reputationTrust);
    void writeInteractionLog (QVector<unsigned long int> carIDs, bool truth, bool xSays, bool success, QPair<bool, double> descissionResult);

    int getCarPropDetects(unsigned long CarID);
private:
    QVector<car> carVector;

    int calcProp(int CarID, QList<QPair<int, int>> prop);
};

#endif // DATABASE_H
