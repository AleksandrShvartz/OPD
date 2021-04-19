#ifndef PRODUCTIONMANAGER_H
#define PRODUCTIONMANAGER_H

#include "TestProvider.h"
#include "WorkingModels.h"
#include <QWidget>
#include <QTableWidgetItem>

namespace Ui {
class ProductionManager;
}

class ProductionManager : public QWidget
{
    Q_OBJECT

public:
    explicit ProductionManager(QWidget *parent = nullptr);
    ~ProductionManager();

private slots:
    void on_pushButton_clicked();
    void mailSent(QString);
private:
    void GetData();
    void FillTables();

private:
    Ui::ProductionManager *ui;
    EShiftTimeN shiftTime;
    QDate currentDate;
    Shift shift;

    QTableWidgetItem* tapesName;
    vector<QTableWidgetItem*> workersNames;
};

#endif // PRODUCTIONMANAGER_H
