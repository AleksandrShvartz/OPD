#ifndef DATAPROVIDER_H
#define DATAPROVIDER_H

#include "WorkingModels/WorkingModels.h"

class DataStorageSingleton : StorageSingleton {
public:
    DataStorageSingleton(DataStorageSingleton& other) = delete;
    void operator= (const DataStorageSingleton &) = delete;
    static void Init(QString dbName, QString hostName, int port, QString userName, QString password);

    vector<Employee> GetFreeWorkers(QDate date, EShiftTimeN shiftNum);

    Shift GetShift(QDate date, EShiftTimeN shiftNum);

private:
    DataStorageSingleton(QString dbName, QString hostName, size_t port, QString userName, QString password) : StorageSingleton() {
        InitDB(dbName, hostName, port, userName, password);
    }

    void InitDB(QString dbName, QString hostName, int port, QString userName, QString password);
    QSqlQuery QueryFromStr(QString query);
    QSqlDatabase db;
};

void PrintDataShift(void);

#endif // DATAPROVIDER_H
