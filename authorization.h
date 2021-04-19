#ifndef AUTHORIZATION_H
#define AUTHORIZATION_H

#include <QWidget>
#include "WorkingModels.h"

namespace Ui {
class Authorization;
}

class Authorization : public QWidget
{
    Q_OBJECT

public:
    explicit Authorization(QWidget *parent = nullptr);
    ~Authorization();
    const Employee& getUser() { return user; }
protected:
    void keyPressEvent(QKeyEvent *event);

signals:
    void LogInSuccess();

private slots:
    void on_pushButtonEnter_clicked();

private:
    Ui::Authorization *ui;
    Employee user;
};

#endif // AUTHORIZATION_H
