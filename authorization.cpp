#include "authorization.h"
#include "ui_authorization.h"
#include "QTextStream"
#include <QKeyEvent>
#include <QSslSocket>
#include <regex>
#include "TestProvider.h"
Authorization::Authorization(QWidget *parent) : QWidget(parent), ui(new Ui::Authorization) {
    ui->setupUi(this);
    ui->lineEditLogin->setFocus();
    QFile logins("data.txt");
     qDebug( "Create authorization");
    if(logins.exists()){
        if(!logins.open(QIODevice::ReadOnly))
        {
            qDebug( "Ошибка открытия для чтения");
        }else{
            qDebug( "Setting existing logins..");
          if(!(logins.readLine().compare("true\n"))){
             ui->checkBox->setChecked(true);
             QString s=logins.readLine(),ss="";
             for (int i=0;i!=s.length()-1 ;++i ) {
                 ss+=s[i];
             }
          ui->lineEditLogin->setText(ss);
           s=logins.readLine(),ss="";
          for (int i=0;i!=s.length()-1 ;++i ) {
              ss+=s[i];
          }
          ui->lineEditPassword->setText(ss);
          }
          logins.close();
        }


    }else{
        qDebug("no saved logins found");
    }
}

Authorization::~Authorization() {
    delete ui;
}

void Authorization::on_pushButtonEnter_clicked() {
    QString login = ui->lineEditLogin->text();
    QString password = ui->lineEditPassword->text();
qDebug() << "Device supports OpenSSL: " << QSslSocket::supportsSsl();
    const auto r = std::regex(R"(^[A-Za-z0-9_]+$)");
       if(regex_match(login.toStdString(),r) && regex_match(password.toStdString(),r)){
           try {
                user = StorageSingleton::GetInstance()->Authorization(login, password);
                emit LogInSuccess();
                if(ui->checkBox->isChecked()){
                    QFile logins("data.txt");

                    if(!logins.open(QIODevice::WriteOnly))
                    {
                        qDebug( "Ошибка открытия для записи");
                    }else{
                        QTextStream writeStream(&logins);
                         writeStream<<"true\n";
                         writeStream<<login<<'\n'<<password<<'\n';

                        logins.close();
                    }
                }else{
                    QFile logins("data.txt");
                    if(!logins.open(QIODevice::WriteOnly))
                    {

                        qDebug( "Ошибка открытия для записи");
                    }else{
                        QTextStream writeStream(&logins);
                         writeStream<<"false";

                        logins.close();
                    }
                }
           }  catch (char const* exp) {
               ui->labelErrorLogIn->setText("Неверный логин или пароль");
           }
       }else
          ui->labelErrorLogIn->setText("Некорректный формат ввода");
}

void Authorization::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Return:
        on_pushButtonEnter_clicked();
        break;
    }
}
