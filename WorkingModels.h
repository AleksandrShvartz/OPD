#ifndef WORKINGMODELS_H
#define WORKINGMODELS_H

#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <QDate>
#include <QString>
#include <QCoreApplication>
#include <QSqlDatabase>
#include <QSqlQuery>

#define TAPE_COUNT 6
using namespace std;

enum class  EShiftPeopleN {
    I = 1,
    II,
    III,
    IV
};

enum class  EShiftTimeN {
    T8_20,
    T20_8
};

enum class  EPost {
    WORKER = 1,
    SHIFT_LEADER,
    PRODUCTION_MANAGER
};

enum class  EStatus {
    ONPLACE = 1,
    NOTONPLACE,
    FREE
};

class Employee
{
public:
    Employee() {}

    Employee(size_t id, QString fullName, EPost post, EStatus status, QString comment)
        : id(id), fullName(fullName), post(post), status(status), comment(comment) {}

    Employee(size_t id, QString fullName, EPost post, EStatus status)
        : id(id), fullName(fullName), post(post), status(status), comment("") {}

    bool operator== (const Employee& wComp) {
        if (wComp.id == id && wComp.fullName == fullName && wComp.post == post)
            return true;

        return false;
    }

    size_t GetId() const {
        return id;
    }

    const QString & GetFullName() const {
        return fullName;
    }

    const QString & GetComment() const {
        return comment;
    }

    EPost GetPost() const {
        return post;
    }

    EStatus GetStatus() const {
        return status;
    }

    void SetStatus(EStatus status)
    {
        this->status = status;
    }

private:
    size_t id;
    QString fullName;
    EPost post;
    EStatus status;
    QString comment;
};

class ProdTape {
public:
    ProdTape(const size_t id, const QString name, const vector<Employee>& workers) : id(id), name(name), workers(workers) {}

    ProdTape(const size_t id, const QString name) :
        id(id), name(name) {}

    ProdTape() {
        id = 0;
        name = "empty";
    }

    void AddWorker(const Employee& worker) {
        workers.push_back(worker);
    }

    bool RemoveWorker(Employee& worker);

    size_t GetId() const {
        return id;
    }

    const QString & GetName() const {
        return name;
    }

    const vector<Employee> & GetWorkers() const {
        return workers;
    }

    Employee & GetWorker(QString fullName);

    size_t CountWorkers() const {
        return workers.size();
    }

    bool IsInTape(const Employee& worker) const;

private:
    size_t id;
    QString name;
    vector<Employee> workers;
};

class Shift {
public:
    Shift() {}

    Shift(QDate date, EShiftTimeN shiftTimeN, size_t idShiftLeader, EShiftPeopleN shiftPeopleN) :
        date(date), shiftTimeN(shiftTimeN), idShiftLeader(idShiftLeader), shiftPeopleN(shiftPeopleN) {}

    void SetTapes(const ProdTape* newTapes);

    void SetTape(const ProdTape& newTape);

    const ProdTape *GetTapes() const {
        return tapes;
    }

    const ProdTape & GetTape(const QString& tapeName) const;

    ProdTape & GetTape(const size_t index);

    bool IsInShift(const Employee& worker) const;

private:
    QDate date;
    EShiftTimeN shiftTimeN;
    size_t idShiftLeader;
    EShiftPeopleN shiftPeopleN;
    ProdTape tapes[TAPE_COUNT];
};

class StorageSingleton {
public:
    StorageSingleton(StorageSingleton& other) = delete;
    void operator= (const StorageSingleton &) = delete;

    virtual vector<Employee> GetFreeWorkers(const Shift& shift) = 0;

    virtual Shift GetShift(QDate date, EShiftTimeN shiftNum) = 0;

    virtual Employee Authorization(QString login, QString password) = 0;

    virtual void PushShift(const Shift& shift) = 0;

    static StorageSingleton *GetInstance() {
        return pStorageSingleton_s;
    }

    static void Free() {
        delete pStorageSingleton_s;
    }

    virtual ~StorageSingleton() {}

protected:
    StorageSingleton() {}

// Static objects will be deleted automatically in the end of the programm
    static StorageSingleton* pStorageSingleton_s;
};

#endif // WORKINGMODELS_H
