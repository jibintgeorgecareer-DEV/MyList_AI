#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "authmanager.h"
#include "taskmanager.h"
#include "taskdetailsdialog.h"
#include "aimanager.h"

#include <QMainWindow>
#include <QMessageBox>
#include <QListWidgetItem> //Access item in List
#include <QSystemTrayIcon> // Set desktop notifications
#include <QSettings>

QT_BEGIN_NAMESPACE
namespace Ui {    //When compile .ui , Qt UIC creates Ui::MainWindow
class MainWindow;
}
QT_END_NAMESPACE //Macros

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:  //Qt Meta-Object System can invoke private functions only inside mainwindow.cpp

    void on_btn_login_clicked();             //login management
    void on_btn_register_button_clicked();  //register user

    void on_btn_goto_register_clicked();    //from login.ui to register.ui
    void on_btn_goto_login_clicked();      //from register.ui to login.ui

    void on_btn_addtask_clicked();        //Add tasks to DB
    void load_all_tasks();               //Load all tasks from DB

    void on_btn_deletetask_2_clicked(); //Delete task from DB

    void taskCounter();   //Display task statitics

    void on_btn_comtask_clicked();     //Select & count completed tasks
    void on_btn_pentasks_clicked();   //Select & count Pending tasks
    void on_btn_alltasks_clicked();  //Select & count All tasks

    void on_task_list_itemClicked(QListWidgetItem *item);   //task Detailed dialog

    void on_btn_profile_clicked();  //Userprofile dialog opens

    void on_btn_searchpriority_clicked(); //Search tasks by priority

    void remainderAlert(); //MessageBox for tasks are today

    void overdueNotification(); //OS generated notification alert for overdue tasks

    void on_dash_searchtask_textChanged(const QString &keyword); //Live search for tasks

    void on_btn_ai_open_clicked(); //Open ai dialog

private:
    Ui::MainWindow *ui;
    int current_user_id;        //Store current user id
    AuthManager auth;           //Authentication object
    TaskManager taskman;        //Taskmanagement
    TaskDetailsDialog dialog;   //Task detailed
    AIManager ai;               //AI management

    QSystemTrayIcon *notification; //Desktop notification system obj
};
#endif // MAINWINDOW_H
