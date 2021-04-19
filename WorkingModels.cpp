#include "WorkingModels.h"

StorageSingleton * StorageSingleton::pStorageSingleton_s = nullptr;

bool ProdTape::RemoveWorker(Employee& worker) {
    auto it = find(workers.begin(), workers.end(), worker);

    if (it != workers.end()) {
        workers.erase(it);
        return true;
    }

    return false;
}

bool ProdTape::IsInTape(const Employee& worker) const {
    for (const auto& w : workers)
        if (w.GetFullName() == worker.GetFullName())
            return true;

    return false;
}

Employee & ProdTape::GetWorker(QString fullName) {
    for (auto& employee : workers)
        if (employee.GetFullName() == fullName)
            return employee;

    throw "No worker with this name";
}

void Shift::SetTapes(const ProdTape* newTapes) {
    for (size_t i = 0; i != TAPE_COUNT; ++i)
        tapes[i] = newTapes[i];
}

void Shift::SetTape(const ProdTape& newTape) {
    for (size_t i = 0; i < TAPE_COUNT; i++)
        if (tapes[i].GetName() == newTape.GetName()) {
            tapes[i] = newTape;
            break;
        }
}

const ProdTape & Shift::GetTape(const QString& tapeName) const {
    for (size_t i = 0; i < TAPE_COUNT; i++)
        if (tapes[i].GetName() == tapeName)
            return tapes[i];

    throw "No tape with this name";
}

ProdTape & Shift::GetTape(const size_t index) {
    return tapes[index];
}

bool Shift::IsInShift(const Employee& worker) const {
    for (size_t i = 0; i != TAPE_COUNT; i++)
        if (tapes[i].IsInTape(worker))
            return true;

    return false;
}
