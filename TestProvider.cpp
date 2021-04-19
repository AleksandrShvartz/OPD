#include "TestProvider.h"
#include <QDebug>

void TestStorageSingleton::Init() {
    if (pStorageSingleton_s == nullptr) {
        pStorageSingleton_s = new TestStorageSingleton();
    } else {
        // ASSERT
        qDebug() << "Double Init error, last Init was for TestStorageSingleton";
    }
}

vector<Employee> TestStorageSingleton::GetFreeWorkers(const Shift& shift) {
    vector<Employee> freeWorkers;

    for (int j = 0; j != 10; ++j)
        freeWorkers.push_back(Employee(j, QString::number(j) + "_worker", EPost::WORKER, EStatus::FREE));

    return freeWorkers;
}

Shift TestStorageSingleton::GetShift(QDate date, EShiftTimeN shiftTimeN) {
    Employee leader(239, "Leader Surname", EPost::SHIFT_LEADER, EStatus::ONPLACE);
    EShiftPeopleN shiftPeopleN = EShiftPeopleN::I;
    Shift shift(date, shiftTimeN, leader.GetId(), shiftPeopleN);

    QString tapeNames[TAPE_COUNT] = { "Zewa-3", "Zewa-6", "Zewa-5", "ZZ3-ZZ4", "Rodumat", "Gilzokleyshiki" };
    ProdTape tapes[TAPE_COUNT];

    for (size_t i = 0; i != TAPE_COUNT; ++i) {
        tapes[i] = ProdTape(i, tapeNames[i]);

        for (size_t j = 0; j != 6; ++j)
            tapes[i].AddWorker(Employee(j, QString::number(j) + "_worker", EPost::WORKER, EStatus::ONPLACE));
    }

    shift.SetTapes(tapes);
    return shift;
}

Employee TestStorageSingleton::Authorization(QString login, QString password) {
    if (login == "SHIFT_LEADER" && password == "SHIFT_LEADER")
        return Employee(1, "Test Shift Leader", EPost::SHIFT_LEADER, EStatus::ONPLACE);
    else if (login == "PRODUCTION_MANAGER" && password == "PRODUCTION_MANAGER")
        return Employee(2, "Test Production Manager", EPost::PRODUCTION_MANAGER, EStatus::ONPLACE);
    else
        throw "Incorrect login or password";
}

void TestStorageSingleton::PushShift(const Shift& shift) {
    // Method should push shift in database
}

void PrintTestShift(void) {
    TestStorageSingleton::Init();
    // DataStorageSingleton::Init();
    QDate date;
    Shift shift = StorageSingleton::GetInstance()->GetShift(date, EShiftTimeN::T8_20);

    for (size_t i = 0; i != TAPE_COUNT; i++) {
        qDebug() << "Tape " << shift.GetTape(i).GetName();

        for (size_t j = 0; j < shift.GetTape(i).CountWorkers(); j++)
            qDebug() << "\t" << (shift.GetTape(i).GetWorkers()[j]).GetFullName();
    }

    vector<Employee> freeWorkers = StorageSingleton::GetInstance()->GetFreeWorkers(shift);
    qDebug() << "\n" << "Free workers";

    for (Employee freeWorker : freeWorkers)
        qDebug() << "\t" << freeWorker.GetFullName();

    StorageSingleton::GetInstance()->Free();
}
