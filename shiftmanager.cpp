#include "shiftmanager.h"
#include "ui_shiftmanager.h"
#include "smtp.h"
#include <QPainter>
#include <QPrinter>
#include <QTextDocument>
#define getFromStorage StorageSingleton::GetInstance

ShiftManager::ShiftManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ShiftManager)
{
    currentDate = QDate::currentDate();
    shiftTime = EShiftTimeN::T8_20;
    ui->setupUi(this);

    ui->dateEdit->setDate(currentDate);

    ui->comboBoxShift->addItem("8:00 - 20:00");
    ui->comboBoxShift->addItem("20:00 - 8:00");

    ui->comboBoxShift->installEventFilter(&filter);

    for (int i = 0; i != 8; ++i) {
        ui->table->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Stretch);
    }

    GetData();
    CreateTable();
}

ShiftManager::~ShiftManager()
{
    delete ui;
    delete smtp;
}

void ShiftManager::ClearTable() // Сделать очищение всей памяти при изменении даты и времени, так как будут утечки памяти
{

}

void ShiftManager::GetData()
{
    shift = getFromStorage()->GetShift(currentDate, shiftTime);
    freeWorkers = getFromStorage()->GetFreeWorkers(shift);

    ui->labelFreeWorkers->setText(QString::number(freeWorkers.size()));
}

QWidget* ShiftManager::CreateWidget(QWidget* t) {
    QWidget* wgt = new QWidget;
    QBoxLayout* l = new QHBoxLayout;
    l->setContentsMargins(0, 0, 0, 0);
    l->addWidget(t);
    l->setStretch(0, QHeaderView::Stretch);
    wgt->setLayout(l);
    return wgt;
}

void ShiftManager::AddRowTapeInfo(int rowIndex, int tapeIndex)
{
    ProdTape tape = shift.GetTape(tapeIndex);

    RowTapeInfo[tapeIndex].tapeName = new QTableWidgetItem(tape.GetName());
    RowTapeInfo[tapeIndex].tapeAttendCount = new QTableWidgetItem();
    RowTapeInfo[tapeIndex].tapeAbsentCount = new QTableWidgetItem();
    RowTapeInfo[tapeIndex].tapePeopleCount = new QTableWidgetItem();

    size_t peopleCount = tape.CountWorkers();
    size_t peopleAttend = 0;
    size_t peopleAbsent = 0;
    for (size_t i = 0; i != peopleCount; ++i) {
        if (tape.GetWorkers()[i].GetStatus() == EStatus::ONPLACE) peopleAttend++;
        if (tape.GetWorkers()[i].GetStatus() == EStatus::NOTONPLACE) peopleAbsent++;
    }

    RowTapeInfo[tapeIndex].tapeAttendCount->setText(QString::number(peopleAttend));
    RowTapeInfo[tapeIndex].tapeAbsentCount->setText(QString::number(peopleAbsent));
    RowTapeInfo[tapeIndex].tapePeopleCount->setText(QString::number(peopleCount));

    ui->table->insertRow(rowIndex);

    ui->table->setItem(rowIndex, 0, RowTapeInfo[tapeIndex].tapeName);
    ui->table->setItem(rowIndex, 1, RowTapeInfo[tapeIndex].tapeAttendCount);
    ui->table->setItem(rowIndex, 2, RowTapeInfo[tapeIndex].tapeAbsentCount);
    ui->table->setItem(rowIndex, 3, RowTapeInfo[tapeIndex].tapePeopleCount);

    for (int i = 0; i != 4; ++i) {
        RowTapeInfo[tapeIndex].empty[i] = new QTableWidgetItem();
        RowTapeInfo[tapeIndex].empty[i]->setBackground(Qt::red); //choose nice color

        ui->table->setItem(rowIndex, 4 + i, RowTapeInfo[tapeIndex].empty[i]);
    }
}

void ShiftManager::AddRowTapeEnd(int rowIndex, int tapeIndex)
{
    RowTapeEnd[tapeIndex].workerAdd = new QComboBox;
    RowTapeEnd[tapeIndex].workerAdd->addItem("");
    RowTapeEnd[tapeIndex].workerAdd->setEditable(true);
    RowTapeEnd[tapeIndex].workerAdd->installEventFilter(&filter);
    RowTapeEnd[tapeIndex].workerAdd->setProperty("row", rowIndex);
    RowTapeEnd[tapeIndex].workerAdd->setProperty("col", 4);
    RowTapeEnd[tapeIndex].workerAdd->setProperty("tapeIndex", tapeIndex);
    connect(RowTapeEnd[tapeIndex].workerAdd, SIGNAL(activated(int)), this, SLOT(AddEmployeerInTape(int)));

    for (const auto& worker : freeWorkers) {
        RowTapeEnd[tapeIndex].workerAdd->addItem(worker.GetFullName());
    }

    QWidget* widget = CreateWidget(RowTapeEnd[tapeIndex].workerAdd);
    widgets.push_back(widget);


    ui->table->insertRow(rowIndex);
    ui->table->setIndexWidget(ui->table->model()->index(rowIndex, 4), widget);

    for (int i = 0; i != 7; ++i) {
        RowTapeEnd[tapeIndex].empty[i] = new QTableWidgetItem();
        RowTapeEnd[tapeIndex].empty[i]->setBackground(Qt::red); //choose nice color
        if (i < 4) {
            ui->table->setItem(rowIndex, i, RowTapeEnd[tapeIndex].empty[i]);
        }
        else {
            ui->table->setItem(rowIndex, i + 1, RowTapeEnd[tapeIndex].empty[i]);
        }
    }
}

int ShiftManager::AddRowsWorker(int rowIndex, int tapeIndex)
{
    int peopleCount = 0;
    for (const auto& worker : shift.GetTape(tapeIndex).GetWorkers()) {
        AddRowWorker(rowIndex + peopleCount, tapeIndex, worker);
        peopleCount++;
    }
    return peopleCount;
}

void ShiftManager::AddRowWorker(int rowIndex, int tapeIndex, const Employee& worker)
{
    RowWorker rowWorker;

    ui->table->insertRow(rowIndex);

    for (int i = 0; i != 4; ++i) {
        rowWorker.empty[i] = new QTableWidgetItem();
        rowWorker.empty[i]->setBackground(Qt::red); //choose nice color

        ui->table->setItem(rowIndex, i, rowWorker.empty[i]);
    }

    rowWorker.workerName = new QTableWidgetItem();
    rowWorker.workerName->setText(worker.GetFullName());
    ui->table->setItem(rowIndex, 4, rowWorker.workerName);


    rowWorker.workerStatus = new QComboBox();
    rowWorker.workerStatus->addItem("на месте");
    rowWorker.workerStatus->addItem("не на месте");
    rowWorker.workerStatus->installEventFilter(&filter);
    rowWorker.workerStatus->setProperty("row", rowIndex);
    rowWorker.workerStatus->setProperty("col", 5);
    rowWorker.workerStatus->setProperty("tapeIndex", tapeIndex);
    rowWorker.workerStatus->setProperty("workerName", worker.GetFullName());
    if (worker.GetStatus() == EStatus::NOTONPLACE) {
        rowWorker.workerStatus->setCurrentIndex(1);
    }


    rowWorker.workerComment = new QComboBox;
    rowWorker.workerComment->addItem("");
    rowWorker.workerComment->addItem("больничный");
    rowWorker.workerComment->addItem("отпуск");
    rowWorker.workerComment->addItem("карантин");
    rowWorker.workerComment->addItem("донорские дни");
    rowWorker.workerComment->addItem("разрешено законом");
    rowWorker.workerComment->installEventFilter(&filter);
    rowWorker.workerComment->setProperty("row", rowIndex);
    rowWorker.workerComment->setProperty("col", 6);
    rowWorker.workerComment->setProperty("tapeIndex", tapeIndex);
    rowWorker.workerComment->setProperty("workerName", worker.GetFullName());
    if (worker.GetStatus() == EStatus::NOTONPLACE) {
        // dodeltatb i sprositb Lva pro EnumComment
    }

    rowWorker.workerButtonDelete = new QPushButton;
    rowWorker.workerButtonDelete->setProperty("row", rowIndex);
    rowWorker.workerButtonDelete->setProperty("col", 7);
    rowWorker.workerButtonDelete->setProperty("tapeIndex", tapeIndex);
    rowWorker.workerButtonDelete->setProperty("workerName", worker.GetFullName());
    rowWorker.workerButtonDelete->setText("Удалить");


    QWidget* comboBoxStatus = CreateWidget(rowWorker.workerStatus);
    QWidget* comboBoxComment = CreateWidget(rowWorker.workerComment);
    QWidget* pushButtonDelete = CreateWidget(rowWorker.workerButtonDelete);

    ui->table->setIndexWidget(ui->table->model()->index(rowIndex, 5), comboBoxStatus);
    ui->table->setIndexWidget(ui->table->model()->index(rowIndex, 6), comboBoxComment);
    ui->table->setIndexWidget(ui->table->model()->index(rowIndex, 7), pushButtonDelete);

    widgets.push_back(comboBoxStatus);
    widgets.push_back(comboBoxComment);
    widgets.push_back(pushButtonDelete);


    RowsWorker.push_back(rowWorker);
}

void ShiftManager::UpdateProperty(int tapeIndex)
{
    for (int i = tapeIndex; i != TAPE_COUNT; ++i) {
        RowTapeEnd[i].workerAdd->setProperty("row", RowTapeEnd[i].workerAdd->property("row").toInt() + 1);
    }
}

void ShiftManager::CreateTable()
{
    int rowIndex = 0;
    for (int i = 0; i != TAPE_COUNT; ++i) {
        AddRowTapeInfo(rowIndex, i);
        rowIndex++;
        rowIndex += AddRowsWorker(rowIndex, i);
        AddRowTapeEnd(rowIndex, i);
        rowIndex++;
    }
}

void ShiftManager::AddEmployeerInTape(int index)
{
    QComboBox* senderBox = qobject_cast<QComboBox*>(sender());

    int rowIndex = senderBox->property("row").toInt();
    int tapeIndex = senderBox->property("tapeIndex").toInt();

    size_t i = 0;
    for (const auto& worker : freeWorkers) {
        if (worker.GetFullName() == senderBox->itemText(index)) {
            shift.GetTape(tapeIndex).AddWorker(worker);
            shift.GetTape(tapeIndex).GetWorker(worker.GetFullName()).SetStatus(EStatus::NOTONPLACE); //dlya testa
            AddRowWorker(rowIndex, tapeIndex, shift.GetTape(tapeIndex).GetWorker(worker.GetFullName()));
            freeWorkers.erase(freeWorkers.begin() + i);
            break;
        }
        i++;
        if (i == freeWorkers.size())
            return;
    }

    for (int i = 0; i != TAPE_COUNT; ++i) {
        RowTapeEnd[i].workerAdd->removeItem(index);
    }
    senderBox->setCurrentIndex(0);
//    senderBox->setProperty("row", rowIndex + 1);

    // plus 1 ko vsem property nije dannogo
    UpdateProperty(tapeIndex);


}

void ShiftManager::UpdateComboBoxFreeWorkers()
{

}

void ShiftManager::on_labelFreeWorkers_linkActivated(const QString &link)
{
    ui->labelFreeWorkers->setText(link);
}

void ShiftManager::mailSent(QString status)
{
    if(status == "Message sent")
       qDebug("Message sent!\n\n");// QMessageBox::warning( 0, tr( "Essity" ), tr( "Message sent!\n\n" ) );
}
QString ShiftManager::doPdf(){
    QString name(ui->dateEdit->date().toString()+".pdf");
    QPrinter printer;
   QFile file(name);
   if(file.exists()){
        printer.setOutputFormat(QPrinter::PdfFormat);
        qDebug()<<"creating/updating "+name;

        printer.setOutputFileName(name);
        QPainter painter;
        if (!painter.begin(&printer))
               qWarning("failed to open file, is it writable?");
        painter.save();
        QPen pen = painter.pen();
            pen.setWidth(2);
            painter.setPen(pen);
            QFont f = painter.font();
                f.setPixelSize(15);
                painter.setFont(f);
                painter.setBrush(palette().light());

                QStringList columns = {"ФИО", "Статус", "Комментарий"};
                const int maxIntLenght = painter.fontMetrics().horizontalAdvance(QString::number(UINT64_MAX)) + 20;
                const int textHeight = painter.fontMetrics().height() + 10;
                const int leftOffset = 20;
                const int topOffset = 40;
                int curheight=textHeight + topOffset;
            //    for(int j=0;j!=6;++j)
                {

                for (int i = 0; i < columns.size(); i++)
                    {
                        QRect drawArea{leftOffset + maxIntLenght*i, topOffset, maxIntLenght, textHeight};
                        painter.drawRect(drawArea);
                        painter.drawText(drawArea, Qt::AlignCenter, columns.at(i));
                    }
              // for(auto &worker:)
                }
                painter.restore();
   }else
       qDebug()<<"Can not create/update "<<name;
                return name;
}
void ShiftManager::on_pushButton_2_clicked()
{
    qDebug("start sending emails...");
    this->smtp = new Smtp("essitymailing@gmail.com", "Essitymailing239", "smtp.gmail.com",465);
    connect(smtp, SIGNAL(status(QString)), this, SLOT(mailSent(QString)));
    QStringList list;
    QString pdf=doPdf();
             //    list.push_back("data.txt");
    list.push_back(pdf);
   // list.push_back("data.txt");
    smtp->sendMail("essitymailing@gmail.com", "aleksandr5xz@gmail.com" , "Essity mailing","Testing",list);
   // delete smtp;
}
