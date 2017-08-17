#ifndef DATABASE_H
#define DATABASE_H
#include <QVector>
#include <QtDebug>


class database
{

public:


    struct car {
      int id;
      QMultiMap<int, double> trustMap; //Car ID, value
      QMultiMap<int, double> reputationMap; //Car ID, value
    } ;

    database(int size);

    //get function
    QList<double> getCarTrust(int CarBID, int CarXid) const;
    QList<double> getCarReputation(int CarAID, int CarBID) const;

    //write function
    void writeTrustX(int CarOwnID, int CarXID, double trust);
    void writeReputationB(int CarOwnID, int CarBID, double reputationTrust);

private:
    QVector<car> carVector;
};

#endif // DATABASE_H
