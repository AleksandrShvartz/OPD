#include "productionmanager.h"
#include "productionmanager.h"
#include "ui_productionmanager.h"
#include "smtp.h"
#define getFromStorage StorageSingleton::GetInstance

ProductionManager::ProductionManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProductionManager)
{
    ui->setupUi(this);

    GetData();

    ui->Tape1->setText(shift.GetTapes()[0].GetName());
    ui->Tape2->setText(shift.GetTapes()[1].GetName());
    ui->Tape3->setText(shift.GetTapes()[2].GetName());
    ui->Tape4->setText(shift.GetTapes()[3].GetName());
    ui->Tape5->setText(shift.GetTapes()[4].GetName());
    ui->Tape6->setText(shift.GetTapes()[5].GetName());
    for (int i = 0; i < 3; i++) {
         ui->tableTape1->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Stretch);
         ui->tableTape2->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Stretch);
         ui->tableTape3->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Stretch);
         ui->tableTape4->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Stretch);
         ui->tableTape5->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Stretch);
         ui->tableTape6->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Stretch);
    }

    FillTables();
}

ProductionManager::~ProductionManager()
{
    delete ui;
}

void ProductionManager::GetData()
{
    shift = getFromStorage()->GetShift(currentDate, shiftTime);
}

void ProductionManager::FillTables()
{
    for (int i  = 0; i < TAPE_COUNT; i++)
    {
        for (const auto& employee : shift.GetTapes()[i].GetWorkers())
        {
            if (ui->Tape1->text() == shift.GetTapes()[i].GetName())
            {
                QTableWidgetItem* newItem = new QTableWidgetItem(employee.GetFullName());
                workersNames.push_back(newItem);
                ui->tableTape1->insertRow(ui->tableTape1->rowCount() + 1);
                ui->tableTape1->setItem(ui->tableTape1->rowCount(), 0, workersNames.at(workersNames.size() - 1));
            }
        }
    }
}


