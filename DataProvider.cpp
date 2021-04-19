#include "DataProvider.h"

static QString DateToString(QDate date) {
    int year  = date.year();
    int month = date.month();
    int day = date.day();
    QString dateStr("'" + QString::number(year) + "-" + QString::number(month) + "-" + QString::number(day) + "'");
    return dateStr;
}

static QString ShiftToString(EShiftTimeN shiftTimeNum) {
    QString shiftTime("");

    if (shiftTimeNum == EShiftTimeN::T8_20)
        shiftTime = "'T8_20'";
    else if (shiftTimeNum == EShiftTimeN::T20_8)
        shiftTime = "'T20_8'";
    else {
        // error
        std::cerr << "ERROR: Incorrect time";
    }

    return shiftTime;
}

void DataStorageSingleton::InitDB(QString dbName, QString hostName, int port, QString userName, QString password) {
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setDatabaseName(dbName);
    db.setHostName(hostName);
    db.setPort(port);
    db.setUserName(userName);
    db.setPassword(password);

    if (!db.open()) {
        qDebug() << "ERROR";
    }
}

QSqlQuery DataStorageSingleton::QueryFromStr(QString query) {
    QSqlQuery q(db);
    q.prepare(query);
    q.exec();
    q.first();
    return q;
}

void DataStorageSingleton::Init(QString dbName, QString hostName, int port, QString userName, QString password) {
    if (pStorageSingleton_s == nullptr) {
        pStorageSingleton_s = new DataStorageSingleton(dbName, hostName, port, userName, password);
    } else {
        // ASSERT
        std::cerr << "Double Init error, last Init was for DataStorageSingleton";
    }
}

vector<Employee> DataStorageSingleton::GetFreeWorkers(QDate date, EShiftTimeN shiftTimeN) {
    vector<Employee> freeWorkers;

    for (int j = 0; j != 10; ++j)
        freeWorkers.push_back(Employee(j, QString::number(j) + "_worker", EPost::WORKER, EStatus::FREE));

    return freeWorkers;
}

Shift DataStorageSingleton::GetShift(QDate date, EShiftTimeN shiftTimeN) {
    if (date.isValid() == false) {
        // error
        std::cerr << "ERROR: Incorrect date";
    }

    QString dateStr = DateToString(date);
    QString shiftTime = ShiftToString(shiftTimeN);

    QString query = "SELECT `work_shift_num`, `id_leader` FROM `leader_work_shift` WHERE `date` = " +
     dateStr + " AND `time_shift` = " + shiftTime + ";";

    QSqlQuery q  = QueryFromStr(query);
    QVariant val = q.value(0);
    size_t shiftPeopleN = (size_t)val.toUInt();
    val = q.value(1);
    size_t idShiftLeader = (size_t)val.toUInt(); // TODO: seek for better solution

    Shift shift(date, shiftTimeN, idShiftLeader, (EShiftPeopleN)shiftPeopleN);

    return shift;
}

void PrintDataShift(void) {
    DataStorageSingleton::Init("essity", "127.0.0.1", 3306, "root", "root");

    QDate date(2021, 4, 1);

    Shift shift = StorageSingleton::GetInstance()->GetShift(date, EShiftTimeN::T8_20);

    for (size_t i = 0; i != TAPE_COUNT; i++) {
        qDebug() << "Tape " << shift.GetTape(i).GetName();

        for (size_t j = 0; j < shift.GetTape(i).CountWorkers(); j++)
            qDebug() << "\t" << (shift.GetTape(i).GetWorkers()[j]).GetFullName();
    }

    vector<Employee> freeWorkers = StorageSingleton::GetInstance()->GetFreeWorkers(date, EShiftTimeN::T8_20);
    qDebug() << "\n" << "Free workers";

    for (Employee freeWorker : freeWorkers)
        qDebug() << "\t" << freeWorker.GetFullName();

    StorageSingleton::GetInstance()->Free();
}
