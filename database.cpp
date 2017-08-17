#include "database.h"

database::database(int size)
{
    carVector.clear();
    for(int i=0; i<size; i++)
    {

        QMultiMap<int, double> temp1;
        QMultiMap<int, double> temp2;
       // car temp3 = {i, <QMultiMap<int, double>> trust, <QMultiMap<int, double>> reputation};
        carVector.append({i, temp1, temp2});
    }
    qDebug() << "Number of cars: " << carVector.size();
}

QList<double> database::getCarTrust(int CarBID, int CarXid) const
{
    /// returns all trust values stored by B for the car X
    /** This function searches for all trust values stored by B towards Car X.
     * It returns a list which is sorted by the time with double values containing the trust score at that point  */

    if(CarBID < 0 || CarBID>=carVector.size() || CarXid < 0 || CarXid >= carVector.size()){qFatal("CarID out of scope!");}

    QList<double> TrustValueList = carVector.at(CarBID).trustMap.values(CarXid); //long but efficent, as nothing is copied

    return TrustValueList; //QList is a list of pointers

}





QList<double> database::getCarReputation(int CarAID, int CarBID) const
{
    /// returns all reputation values stored by A for the car B
    /** This function searches for all reputation values stored by A (receiving trust recomondation) towards Car B (sending trust about X).
     * It returns a list which is sorted by the time with double values containing the trust score at that point  */

    if(CarAID < 0 || CarAID>=carVector.size() || CarBID < 0 || CarBID >= carVector.size()){qFatal("CarID out of scope!");}

    QList<double> TrustValueList = carVector.at(CarAID).reputationMap.values(CarBID); //long but efficent, as nothing is copied

    return TrustValueList; //QList is a list of pointers

}

void database::writeTrustX(int CarOwnID, int CarXID, double trust)
{
    /// inserts new trust pair for specified CarX in the database of CarA (me)
    /**   */

    if(CarOwnID < 0 || CarOwnID>=carVector.size() || CarXID < 0 || CarXID >= carVector.size()){qFatal("CarID out of scope!");}


    carVector[CarOwnID].trustMap.insert(CarXID, trust);

    return;
}

void database::writeReputationB(int CarOwnID, int CarBID, double reputationTrust)
{

    if(CarOwnID < 0 || CarOwnID>=carVector.size() || CarBID < 0 || CarBID >= carVector.size()){qFatal("CarID out of scope!");}

    carVector[CarOwnID].reputationMap.insert(CarBID, reputationTrust);

    return;
}

