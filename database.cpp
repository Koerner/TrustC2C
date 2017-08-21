#include "database.h"

database::database(unsigned long int size)
{
    carVector.clear();
    for(unsigned long int i=0; i<size; i++)
    {

        QMultiMap<unsigned long int, double> temp1;
        QMultiMap<unsigned long int, double> temp2;
       // car temp3 = {i, <QMultiMap<int, double>> trust, <QMultiMap<int, double>> reputation};
        carVector.append({i, temp1, temp2});
    }
    qDebug() << "Number of cars in database: " << carVector.size();
}

QList<double> database::getCarTrust(unsigned long int CarBID,unsigned long int CarXid) const
{
    /// returns all trust values stored by B for the car X
    /** This function searches for all trust values stored by B towards Car X.
     * It returns a list which is sorted by the time with double values containing the trust score at that point  */


    if(CarBID < 0 || CarBID>=carVector.size() || CarXid < 0 || CarXid >= carVector.size()){qFatal("CarID out of scope!");}

    QList<double> TrustValueList = carVector.at(CarBID).trustMap.values(CarXid); //long but efficent, as nothing is copied

    return TrustValueList; //QList is a list of pointers

}





QList<double> database::getCarReputation(unsigned long int CarAID,unsigned long int CarBID) const
{
    /// returns all reputation values stored by A for the car B
    /** This function searches for all reputation values stored by A (receiving trust recomondation) towards Car B (sending trust about X).
     * It returns a list which is sorted by the time with double values containing the trust score at that point  */

    if(CarAID < 0 || CarAID>=carVector.size() || CarBID < 0 || CarBID >= carVector.size()){qFatal("CarID out of scope!");}

    QList<double> TrustValueList = carVector.at(CarAID).reputationMap.values(CarBID); //long but efficent, as nothing is copied

    return TrustValueList; //QList is a list of pointers

}

void database::writeTrustX(unsigned long int CarOwnID,unsigned long int CarXID, double trust)
{
    /// inserts new trust pair for specified CarX in the database of CarA (me)
    /**   */

    if(CarOwnID < 0 || CarOwnID>=carVector.size() || CarXID < 0 || CarXID >= carVector.size()){qFatal("CarID out of scope!");}


    carVector[CarOwnID].trustMap.insert(CarXID, trust);

    return;
}

void database::writeReputationB(unsigned long int CarOwnID,unsigned long int CarBID, double reputationTrust)
{

    if(CarOwnID < 0 || CarOwnID>=carVector.size() || CarBID < 0 || CarBID >= carVector.size()){qFatal("CarID out of scope!");}

    carVector[CarOwnID].reputationMap.insert(CarBID, reputationTrust);

    return;
}

void database::writeInteractionLog(QVector<unsigned long> carIDs, bool truth, bool xSays, bool success, QPair<bool, double> descissionResult)
{
    //prepare
    interactionLogStruct log;
    log.carIDs = carIDs;
    log.truth = truth;
    log.xSays = xSays;
    log.descissionResult = descissionResult;
    log.success = success;

    //write
    interactionLogList.append(log);

    qDebug() << "Interaction Done. Truth:" << log.truth << "Descission: " << log.descissionResult.first;
}

