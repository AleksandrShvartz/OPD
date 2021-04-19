#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "authorization.h"
#include "productionmanager.h"
#include "shiftmanager.h"
#include "TestProvider.h"
#include "WorkingModels.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void Display();

private slots:
    void AuthorizeUser();

private:
    Ui::MainWindow *ui;
    Authorization autorizationWindow;
    ShiftManager* shiftSuperWindow = nullptr;
    ProductionManager* prodManaherWindow = nullptr;
};
#endif // MAINWINDOW_H
