#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    TestStorageSingleton::Init();
    ui->setupUi(this);

    connect(&autorizationWindow, SIGNAL(LogInSuccess()), this, SLOT(AuthorizeUser()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Display()
{
    autorizationWindow.show();
}

void MainWindow::AuthorizeUser()
{
    if (autorizationWindow.getUser().GetPost() == EPost::SHIFT_LEADER)
    {
        shiftSuperWindow = new ShiftManager();
        shiftSuperWindow->show();
        autorizationWindow.close();
    }
    else if (autorizationWindow.getUser().GetPost() == EPost::PRODUCTION_MANAGER)
    {
        prodManaherWindow = new ProductionManager();
        prodManaherWindow->show();
        autorizationWindow.close();
    }
}

