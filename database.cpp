#include "database.h"

database::database(unsigned long size, QList<QPair<int, int>> PropDetects, QList<QPair<int, int>> PropHonest)
{
    carVector.clear();
    for(unsigned long int i=0; i<size; i++)
    {
        int temp1 = calcProp(i, PropDetects);
        int temp2 = calcProp(i, PropHonest);
        QMultiMap<unsigned long int, double> temp3;
        QMultiMap<unsigned long int, double> temp4;

        carVector.append({i, temp1, temp2, temp3, temp4});
        qDebug() << "car " << i << "detects truth with " << temp1;
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

QList<double> database::getCarTrust(unsigned long int CarBID,unsigned long int CarXid) const
{
    /// returns all trust values stored by B for the car X
    /** This function searches for all trust values stored by B towards Car X.
     * It returns a list which is sorted by the time with double values containing the trust score at that point  */


    if(CarBID < 0 || CarBID>=carVector.size() || CarXid < 0 || CarXid >= carVector.size()){qFatal("CarID out of scope!");}

    return carVector.at(CarBID).trustMap.values(CarXid); //long but efficent, as nothing is copied

}





QList<double> database::getCarReputation(unsigned long int CarAID,unsigned long int CarBID) const
{
    /// returns all reputation values stored by A for the car B
    /** This function searches for all reputation values stored by A (receiving trust recomondation) towards Car B (sending trust about X).
     * It returns a list which is sorted by the time with double values containing the trust score at that point  */

    if(CarAID < 0 || CarAID>=carVector.size() || CarBID < 0 || CarBID >= carVector.size()){qFatal("CarID out of scope!");}

    return carVector.at(CarAID).reputationMap.values(CarBID); //long but efficent, as nothing is copied

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

int database::getCarPropDetects(unsigned long int CarID)
{

    return carVector.at(CarID).PropDetects;
}




double database::getAverageTrust(unsigned long int CarBID, unsigned long int CarXid)
{
    QList<double> elements = getCarTrust(CarBID,CarXid);
    return average::averageMean(elements);
}



void database::saveToFileTrust()
{
    QString filename="C:\\Users\\Quirin\\Desktop\\DataTrust.txt";
    QFile file( filename );
    if ( file.open(QIODevice::WriteOnly) )
    {
        QTextStream stream( &file );

        for(int i=0; i < carVector.size(); i++)
        {
            stream << "Car" << i << ";" << "\r\n";

            for(int j=0; j < carVector.size(); j++)
            {
                stream << "TowardsCar" << j << ";";

                QList<double> tempList = carVector.at(i).trustMap.values(j);
                double trustValue = 0;
                for(int k=0; k < tempList.size(); k++)
                {
                    trustValue += tempList.at(k) / tempList.size();
                    stream << tempList.at(k) << ";";
                }
                stream << trustValue << ";"<< tempList.size() << ";";
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

        for(int i=0; i < carVector.size(); i++)
        {
            stream << "Car" << i << ";" << "\r\n";

            for(int j=0; j < carVector.size(); j++)
            {
                stream << "TowardsCar" << j << ";";

                QList<double> tempList = carVector.at(i).reputationMap.values(j);
                double reputationValue = 0;
                for(int k=0; k < tempList.size(); k++)
                {
                    reputationValue += tempList.at(k) / tempList.size();
                    stream << tempList.at(k) << ";";
                }
                stream << reputationValue << ";"<< tempList.size() << ";";
                stream << "\r\n";
            }
            stream << "\r\n";
        }
        stream << "Ende" << endl;
    }
}


