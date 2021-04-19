#ifndef TESTPROVIDER_H
#define TESTPROVIDER_H

#include "WorkingModels.h"

using namespace std;

class TestStorageSingleton : StorageSingleton {
public:
    TestStorageSingleton(TestStorageSingleton& other) = delete;
    void operator= (const TestStorageSingleton &) = delete;
    static void Init();

    virtual vector<Employee> GetFreeWorkers(const Shift& shift) override;

    virtual Shift GetShift(QDate date, EShiftTimeN shiftTimeNum) override;

    virtual Employee Authorization(QString login, QString password) override;

    virtual void PushShift(const Shift& shift) override;
private:
    TestStorageSingleton() : StorageSingleton() {}
};

void PrintTestShift(void);
#endif // TESTPROVIDER_H
