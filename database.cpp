#include "database.h"

database::database(unsigned long size, QList<QPair<int,  QPair<double, double>>> PropDetectsPrediction, QList<QPair<int, QPair<double, double>> > PropDetectsObservation, QList<QPair<int,  QPair<double, double>>> PropHonest)
{
    carVector.clear();
    for(unsigned long int i=0; i<size; i++)
    {
        car temp;
        temp.PropDetectsPrediction = calcProp(i, PropDetectsPrediction);
        temp.PropDetectsObservation = calcProp(i, PropDetectsObservation);
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

int database::sizeCarsVector()
{
    return carVector.size();

}


void database::addCar(QPair <double, double> PropDetectsPrediction, QPair <double, double> PropDetectsObservation, QPair <double, double> PropHonest)
{
    car temp;
    temp.id = carVector.size();
    temp.PropDetectsObservation = PropDetectsObservation;
    temp.PropDetectsPrediction = PropDetectsPrediction;
    temp.PropHonest = PropHonest;

    carVector.append(temp);
    qDebug() <<"last id" << temp.id <<"size database" << carVector.size();

}

QPair <double, double> database::calcProp(unsigned long int CarID, QList<QPair<int, QPair<double, double>>> prop)
{

    if(prop.size() == 0){qFatal("Something went wrong with the propabilities");}

    QPair <double, double> propResult;

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

    QList<QPair<bool, double>> returnList  = carVector.at(CarBID).trustMap.values(CarXid);
    /*if(returnList.isEmpty())
    {
        qDebug() << "No trust values found";
        returnList.append(qMakePair(false, 0.0));
    }*/
    //qDebug() << "Following Trust values returned: " << returnList;
    return returnList;

}





QList<QPair<bool, double> > database::getCarReputation(unsigned long int CarAID,unsigned long int CarBID) const
{
    /// returns all reputation values stored by A for the car B
    /** This function searches for all reputation values stored by A (receiving trust recomondation) towards Car B (sending trust about X).
     * It returns a list which is sorted by the time with double values containing the trust score at that point  */

    if(CarAID < 0 || CarAID>=carVector.size() || CarBID < 0 || CarBID >= carVector.size()){qFatal("CarID out of scope!");}

    QList<QPair<bool, double>> returnList  = carVector.at(CarAID).reputationMap.values(CarBID);
    /*if(returnList.isEmpty())
    {
        qDebug() << "No reputation values found";
        returnList.append(qMakePair(false, 0.0));
    }
    */
    //qDebug() << "Following Reputation values returned: " << returnList;
    return returnList;




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

double database::getCarPropDetectsObservation(unsigned long int CarID)
{

    return carVector.at(CarID).PropDetectsObservation.first;
}

double database::getCarPropDetectsObservationStdDist(unsigned long CarID)
{
    return carVector.at(CarID).PropDetectsObservation.second;
}

double database::getCarPropDetectsPrediction(unsigned long int CarID)
{

    return carVector.at(CarID).PropDetectsPrediction.first;
}

double database::getCarPropDetectsPredictionStdDist(unsigned long CarID)
{
    return carVector.at(CarID).PropDetectsPrediction.second;
}

double database::getCarPropHonest(unsigned long CarID)
{
    return carVector.at(CarID).PropHonest.first;
}

double database::getCarPropHonestStdDist(unsigned long CarID)
{
    return carVector.at(CarID).PropHonest.second;
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

                    stream << "(" << tempList.at(k).first <<","<< tempList.at(k).second << ");";
                }

                stream << "SUM(" << carVector.at(i).trustSumMap.value(j) << ","<< tempList.size() << ");";

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

                    stream << "(" << tempList.at(k).first <<","<< tempList.at(k).second << ");";
                }
                stream << "SUM(" << carVector.at(i).reputationSumMap.value(j) << ","<< tempList.size() << ");";
                stream << "\r\n";
            }
            stream << "\r\n";
        }
        stream << "Ende" << endl;
    }
}




