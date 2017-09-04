#include "database.h"

database::database(unsigned long size, QList<QPair<int, int>> PropDetects, QList<QPair<int, int>> PropHonest)
{
    carVector.clear();
    for(unsigned long int i=0; i<size; i++)
    {
        car temp;
        temp.PropDetects = calcProp(i, PropDetects);
        temp.PropHonest = calcProp(i, PropHonest);
        //temp.reputationMap;
        //temp.trustMap;
        //temp.reputationSumMap;
        //temp.trustSumMap;
        temp.id = i;

        carVector.append(temp);
    }
    qDebug() << "Number of cars in database: " << carVector.size();
}

int database::calcProp(int CarID, QList<QPair<int, int>> prop)
{

    if(prop.size() == 0){qFatal("Something went wrong with the propabilities");}

    int propResult = 0;

    for(int i=0; i < prop.size() ; i++)
    {
        if(prop.at(i).first <= CarID)
        {
            propResult = prop.at(i).second;
        }
    }

    return propResult;
}

QList<QPair<bool, double>> database::getCarTrust(unsigned long int CarBID,unsigned long int CarXid) const
{
    /// returns all trust values stored by B for the car X
    /** This function searches for all trust values stored by B towards Car X.
     * It returns a list which is sorted by the time with double values containing the trust score at that point  */


    if(CarBID < 0 || CarBID>=carVector.size() || CarXid < 0 || CarXid >= carVector.size()){qFatal("CarID out of scope!");}

    return carVector.at(CarBID).trustMap.values(CarXid);

}





QList<QPair<bool, double> > database::getCarReputation(unsigned long int CarAID,unsigned long int CarBID) const
{
    /// returns all reputation values stored by A for the car B
    /** This function searches for all reputation values stored by A (receiving trust recomondation) towards Car B (sending trust about X).
     * It returns a list which is sorted by the time with double values containing the trust score at that point  */

    if(CarAID < 0 || CarAID>=carVector.size() || CarBID < 0 || CarBID >= carVector.size()){qFatal("CarID out of scope!");}

    return carVector.at(CarAID).reputationMap.values(CarBID);



}

void database::writeTrustX(unsigned long int CarOwnID,unsigned long int CarXID, QPair<bool, double> trust)
{
    /// inserts new trust pair for specified CarX in the database of CarA (me)
    /**   */

    if(CarOwnID < 0 || CarOwnID>=carVector.size() || CarXID < 0 || CarXID >= carVector.size()){qFatal("CarID out of scope!");}

    carVector[CarOwnID].trustMap.insert(CarXID, trust); //as it is a multi map it adds the value even, if the key already exists

    QList<QPair<bool,double>> tempList = carVector.at(CarOwnID).trustMap.values(CarXID); //getting list
    carVector[CarOwnID].trustSumMap.insert(CarXID, trustReputational::mergeTrustD(tempList)); //replaces existing values

    return;
}

void database::writeReputationB(unsigned long int CarOwnID, unsigned long int CarBID, QPair<bool, double> reputationTrust)
{

    if(CarOwnID < 0 || CarOwnID>=carVector.size() || CarBID < 0 || CarBID >= carVector.size()){qFatal("CarID out of scope!");}

    carVector[CarOwnID].reputationMap.insert(CarBID, reputationTrust);

    QList<QPair<bool,double>> tempList = carVector.at(CarOwnID).reputationMap.values(CarBID); //getting list
    carVector[CarOwnID].reputationSumMap.insert(CarBID, trustReputational::mergeReputationD(tempList)); //replaces existing values

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

    qDebug() << "Logged interaction";
}

int database::getCarPropDetects(unsigned long int CarID)
{

    return carVector.at(CarID).PropDetects;
}


double database::getAverageTrust(unsigned long int CarBID, unsigned long int CarXid)
{
    return carVector[CarBID].trustSumMap.value(CarXid);
}


QList<unsigned long> database::getCarIDsAscendingReputation(unsigned long CarB)
{
    QMultiMap<double, long unsigned> newMap;

    QMapIterator<long unsigned, double> it(carVector[CarB].reputationSumMap);
    while (it.hasNext())
    {
        it.next();
        newMap.insert(it.value(), it.key()); //swap value and key
    }
    return newMap.values();

}



void database::saveToFileTrust()
{
    QString filename="C:\\Users\\Quirin\\Desktop\\DataTrust.txt";
    QFile file( filename );
    if ( file.open(QIODevice::WriteOnly) )
    {
        QTextStream stream( &file );

        for(int i=0; i < carVector.size(); i++) // i = carID (own)
        {
            stream << "Car" << i << ";" << "\r\n";

            for(int j=0; j < carVector.size(); j++)  //j = carID (towards car)
            {
                stream << "TowardsCar" << j << ";";

                QList<QPair<bool, double>> tempList = carVector.at(i).trustMap.values(j);

                for(int k=0; k < tempList.size(); k++) // K = entries of i->j
                {
                    stream << tempList.at(k).second << ";";
                }
                double sum= carVector.at(i).trustSumMap.value(j);
                stream << sum << ";"<< tempList.size() << ";";
                stream << "\r\n";
            }
            stream << "\r\n";
        }
        stream << "Ende" << endl;
    }
}

void database::saveToFileReputation()
{
    QString filename="C:\\Users\\Quirin\\Desktop\\DataReputation.txt";
    QFile file( filename );
    if ( file.open(QIODevice::WriteOnly) )
    {
        QTextStream stream( &file );

        for(int i=0; i < carVector.size(); i++) // i = carID (own)
        {
            stream << "Car" << i << ";" << "\r\n";

            for(int j=0; j < carVector.size(); j++)  //j = carID (towards car)
            {
                stream << "TowardsCar" << j << ";";

                QList<QPair<bool, double>> tempList = carVector.at(i).reputationMap.values(j);

                for(int k=0; k < tempList.size(); k++) // K = entries of i->j
                {

                    stream << tempList.at(k).second << ";";
                }
                stream << carVector.at(i).reputationSumMap.value(j) << ";"<< tempList.size() << ";";
                stream << "\r\n";
            }
            stream << "\r\n";
        }
        stream << "Ende" << endl;
    }
}




