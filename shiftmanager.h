#ifndef SHIFTMANAGER_H
#define SHIFTMANAGER_H

#include <QWidget>
#include "TestProvider.h"
#include "WorkingModels.h"
#include <QTableWidgetItem>
#include <QPushButton>
#include <QComboBox>

class Filter : public QObject
{
public:
    bool eventFilter(QObject * o, QEvent * e)
    {
        if(e->type() == QEvent::Wheel)
        {
             return true;
        }
        return QObject::eventFilter(o, e);
    }
};

namespace Ui {
class ShiftManager;
}

class ShiftManager : public QWidget
{
    Q_OBJECT

public:
    explicit ShiftManager(QWidget *parent = nullptr);
    ~ShiftManager();
    void ClearTable();

private:
    void GetData();
    QWidget* CreateWidget(QWidget* t);
    void CreateTable();
    void AddRowTapeInfo(int rowIndex, int tapeIndex);
    void AddRowTapeEnd(int rowIndex, int tapeIndex);
    int AddRowsWorker(int rowIndex, int tapeIndex);
    void AddRowWorker(int rowIndex, int tapeIndex, const Employee& worker);
    void UpdateProperty(int tapeIndex);
    void UpdateComboBoxFreeWorkers();

private slots:
    void AddEmployeerInTape(int index);

    void on_labelFreeWorkers_linkActivated(const QString &link);

private:
    Filter filter;

    Ui::ShiftManager *ui;
    EShiftTimeN shiftTime;
    size_t collection;
    QDate currentDate;
    Shift shift;
    std::vector<Employee> freeWorkers;

    struct RowWorker {
        QTableWidgetItem* empty[4];
        QTableWidgetItem* workerName;
        QComboBox* workerStatus;
        QComboBox* workerComment;
        QPushButton* workerButtonDelete;
    };

    std::vector<RowWorker> RowsWorker;

    struct RowTapeInfo {
        QTableWidgetItem* tapeName;
        QTableWidgetItem* tapeAttendCount;
        QTableWidgetItem* tapeAbsentCount;
        QTableWidgetItem* tapePeopleCount;
        QTableWidgetItem* empty[4];
    } RowTapeInfo[TAPE_COUNT];

    struct RowTapeEnd {
        QTableWidgetItem* empty[7];
        QComboBox* workerAdd;
    } RowTapeEnd[TAPE_COUNT];

    std::vector<QWidget*> widgets;
};

#endif // SHIFTMANAGER_H
