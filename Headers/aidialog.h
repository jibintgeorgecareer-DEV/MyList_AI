#ifndef AIDIALOG_H
#define AIDIALOG_H

#include <QDialog>

#include "authmanager.h"
#include "taskmanager.h"
#include "aimanager.h"
#include "appsettings.h"

namespace Ui {
class aiDialog;
}

class aiDialog : public QDialog
{
    Q_OBJECT

public:
    explicit aiDialog(QWidget *parent = nullptr);
    ~aiDialog();

    void setAuthObject(AuthManager *auth_obj);  //Gets actual auth obj from mainwindow
    void setAIObject_AND_connectSlot(AIManager *ai_obj); //Get AIMANAGER obj and connect slot

    QString make_prompt(QString question); //Makes awonderfull prompt

private slots:
    void on_ai_submit_clicked();

    void on_AI_reply_received(QString answer);

    void on_btn_settings_clicked();

private:
    Ui::aiDialog *ui;

    TaskManager taskman;
    AuthManager *auth = nullptr;
    AIManager *ai = nullptr;

};

#endif // AIDIALOG_H