#include "mainwindow.h"
#include "databasemanager.h"
#include "./ui_mainwindow.h" //Auto Generates UIC from mainwindow.ui

#include "taskdetailsdialog.h"
#include "authmanager.h"
#include "taskmanager.h"
#include "userprofile.h"
#include "aidialog.h"
#include "aimanager.h"

#include <QListWidgetItem>   //Represent an item inside QListWidget(row)
#include <QSqlQuery>        //Sql query execute
#include <QSqlError>       //Sql Error handling
#include <QSqlDatabase>  //DB connection
#include <QDebug>       //Qt build in Debug handling
#include <QDate>        //To get system date
#include <QFile> //Import QSS files

MainWindow::MainWindow(QWidget *parent) //Constructore defined
    : QMainWindow(parent)  //CAlling parent constructor  , Set QMainwindow top level
    , ui(new Ui::MainWindow) //ui = new Ui::MainWindow;
{
    ui->setupUi(this);   //Helper class fun managing widgets of ui_mainwindow.h (mainwindow.ui)
    setWindowTitle("MyList - AI Powered To Do Manager");
    this->setMinimumWidth(1300); //Ui window width

    //------------Button AI image setting---------------------------------------------------
    ui->btn_ai_open->setIcon(QIcon(":/icons/AI_image.ico"));
    ui->btn_ai_open->setIconSize(QSize(160, 160));   // Adjust to fit nicely

    // Make button round and styled
    ui->btn_ai_open->setFixedSize(170, 170);  // Button size (slightly bigger than icon)
    ui->btn_ai_open->setStyleSheet(
        "QPushButton {"
        "   border-radius: 65px;"              // Half of width/height → circle
        "   padding: 5px;"
        "}"
        );
    //--------------------------------------------------------------------------------------

    //---------loading Qss files---------------------------------------------
    QFile file(":/styles/mainwindow.qss");
    if(file.open(QFile::ReadOnly))
    {
        QString styles = QLatin1String(file.readAll());
        this->setStyleSheet(styles);
    }
    //-------------------------------------------------------------------------

    ui->stackedWidget->setCurrentIndex(0); //To start app on login page

//------Set DataBase-----------------------------------------------------------
    DatabaseManager db; //From Database module (SQL-Qt connection)
    db.connectDatabase();
    db.createTables();
//------------------------------------------------------------------------------

    //-----------------Notification Setting-----------------------------------

    notification = new QSystemTrayIcon(this); // 'this' make the QSystemTrayIcon child of mainwindow
    notification->setIcon(QIcon(":/icons/logo.ico"));
    notification->show();

    //------------------------------------------------------------------------------


    //-------------------AI Manager, llama starting---------------------------------------
    ai.start_llama_server(); //Starting llama-server when logged in
    //------------------------------------------------------------------------------------
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btn_login_clicked() //Login User (Authentication)
{
    QString username = ui -> login_name -> text();
    QString password = ui -> login_password -> text();

    QSqlQuery query;

    if (auth.loginUser(username,password))
    {
        QString USER = auth.get_current_user_name();

        QMessageBox msgBox(QMessageBox::Information, "USER LOGIN", "Login Successful\nThis is my personal project", QMessageBox::Ok, this);
        msgBox.setMinimumSize(860, 740);
        msgBox.exec();

        load_all_tasks(); //load task in dash
        taskCounter(); //load Task ststi in dash

        ui->dash_username->setText(auth.get_current_user_name());

        ui->stackedWidget->setCurrentIndex(2);//Go to Dash
        remainderAlert();//Remainder System awake
        overdueNotification();//Notification system awake.
    }
    else
    {
        QMessageBox::information(this,"AUTHENTICATION FAILED","Name or Password is invalid");
    }
}

void MainWindow::on_btn_register_button_clicked() //Register User
{
    //auth declared in mainwindow.h

    QString username = ui->register_name->text();
    QString password = ui->register_password->text();
    QString email = ui->register_email->text();

    if(username.isEmpty() || password.isEmpty())
    {
        QMessageBox::warning(this,"REGISTER","field is EMPTY!");
        return;
    }

            if(auth.registerUser(username,password,email))
            {
            QMessageBox::information(this,"USER REGISTRATION","Registration Completed");
            ui->stackedWidget->setCurrentIndex(0);
            }
            else
            {
            QMessageBox::information(this,"REGISTRATION FAILED","Something Wrong!");
            }

}

void MainWindow::on_btn_goto_register_clicked() //From login to register
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_btn_goto_login_clicked() //From Register to login
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::load_all_tasks() //Fetch all tasks from DB (TaskManager)
{
    ui->task_list->clear();

    QSqlQuery query = taskman.loadTask(auth.get_current_user_id());

    while(query.next())
    {
        QString task= query.value("task").toString();
        int task_id= query.value("id").toInt();

        QListWidgetItem *item = new QListWidgetItem(task);

        item->setData(Qt::UserRole,task_id); //UserRole is a custom hidden data for hide task_id

        ui->task_list->addItem(item);
    }
}

void MainWindow::on_btn_addtask_clicked() //ADD Task to DB
{
    QString task = ui->dash_task->text();
    QString description = ui->task_description->text();
    QString date_end =ui->task_end->text();
    QString priority = ui->task_priority->currentText();
    QString status = "Pending"; //New task always in "Pending"
    QString date_stored = QDate::currentDate().toString("yyyy-MM-dd");

    if(task.isEmpty())
    {
        QMessageBox::warning(this,"Empty Input","Enter a task");
        return;
    }
    if(priority == "SELECT PRIORITY")
    {
        priority="";  //Not sets a priority by user
    }
    //taskman declared in mainwindow.h to manage tasks

    bool okey=taskman.addTask(auth.get_current_user_id(),task,description,date_stored,date_end,priority,status);

    if(okey)
    {
        //QMessageBox::information(this,"Succesfull","Task Added");
        load_all_tasks();
        taskCounter();

        ui->dash_task->clear();
        ui->task_description->clear();
    }
    else
    {
        QMessageBox::information(this,"Failed","Task Not Added!");
    }

}

void MainWindow::on_btn_deletetask_2_clicked() //Delete a task from DB
{
    QListWidgetItem *item = ui->task_list->currentItem(); //Get selected task to delete

    if (item==nullptr)
    {
        QMessageBox::warning(this, "SELECT TASK", "Select task from list");
        return;
    }

    int task_id = item->data(Qt::UserRole).toInt();

    if(taskman.deleteTask(task_id))
    {
        load_all_tasks(); //After Delete load all tasks again
        taskCounter();    //Load task stati again

        //QMessageBox::information(this,"SUCCESFULL","Task Deleted");
    }
    else
    {
        QMessageBox::warning(this,"FAILED","Something went wrong");
    }

}

void MainWindow::taskCounter() //Quick view of tast statistics
{
    int total_tasks = taskman.totalTasks(auth.get_current_user_id());
    int com_tasks = taskman.completedTasks(auth.get_current_user_id());
    int pen_tasks = taskman.pendingTasks(auth.get_current_user_id());

    ui->total_tasks->setText(QString::number(total_tasks));
    ui->com_tasks->setText(QString::number(com_tasks));
    ui->pen_tasks->setText(QString::number(pen_tasks));
}

void MainWindow::on_btn_comtask_clicked() //Gets all completed tasks from DB
{
    ui->task_list->clear();

    QSqlQuery query = taskman.taskByStatus(auth.get_current_user_id(),"Completed");
    bool found=false;

    while(query.next())
    {
        found = true;

        QString task = query.value("task").toString();
        int task_id  = query.value("id").toInt();

        QListWidgetItem *item = new QListWidgetItem(task);

        item->setData(Qt::UserRole,task_id);
        ui->task_list->addItem(item);
    }

    if(!found)
    {
        ui->task_list->addItem("No Completed Tasks!");
    }
}

void MainWindow::on_btn_pentasks_clicked() //Gets all Pending Tasks from DB
{
    ui->task_list->clear();

    QSqlQuery query = taskman.taskByStatus(auth.get_current_user_id(),"Pending");
    bool found=false;

    while(query.next())
    {
        found = true;

        QString task = query.value("task").toString();
        int task_id  = query.value("id").toInt();

        QListWidgetItem *item = new QListWidgetItem(task);

        item->setData(Qt::UserRole,task_id);
        ui->task_list->addItem(item);
    }

    if(!found)
    {
        ui->task_list->addItem("No Pending Tasks");
    }
}

void MainWindow::on_btn_alltasks_clicked() //call load_all_tasks()
{
    ui->task_list->clear();
    load_all_tasks();
}

void MainWindow::on_task_list_itemClicked(QListWidgetItem *item) //Detailed view of single task
{
    QString selected_task = item->text();

    int task_id = item->data(Qt::UserRole).toInt();

    dialog.loadTaskDetails(task_id);
    dialog.exec();

    taskCounter();    //To update the task counter after update.
    load_all_tasks(); //Update tasks
}

void MainWindow::on_btn_profile_clicked() //UserProfile dialog
{
    UserProfile profile;
    profile.receiveAuth(&auth); //sharing actual auth to userprofile (object)

    profile.loadProfile(auth.get_current_user_id());
    profile.exec();

    if(auth.get_current_user_id() == -1) //if logout clicked go to login
    {
        ui->stackedWidget->setCurrentIndex(0);
    }
}

void MainWindow::on_btn_searchpriority_clicked() //Search tasks by priority
{
    ui->task_list->clear(); //Clear widget list from ui

    QString keyword = ui->search_priority->currentText(); //Get selected text from dropdown
    QSqlQuery query = taskman.searchByPriority(auth.get_current_user_id(),keyword);
    bool found = false;

    while(query.next())
    {
        found = true;
        QString task = query.value("task").toString();
        int task_id = query.value("id").toInt();

        QListWidgetItem *item = new QListWidgetItem(task);
        item->setData(Qt::UserRole,task_id);
        ui->task_list->addItem(task);
    }

    if(!found)
    {
        ui->task_list->addItem("No Tasks Found");
    }
}

void MainWindow::remainderAlert() //Alert for tasks are today
{
    QStringList tasks_remainders = taskman.remainder(auth.get_current_user_id());

    if(tasks_remainders.isEmpty())
    {
        return;
    }

    QString all_tasks_remainders; //To append all tasks into one string

    for(QString &i : tasks_remainders)
    {
        all_tasks_remainders= all_tasks_remainders + " " + i + " " + "\n" ;
    }
    QMessageBox::about(this,"REMAINDER","Taks due today!\n\n" + all_tasks_remainders);
}

void MainWindow::overdueNotification() //System nitification for overdue tasks
{
    QStringList overdue_tasks = taskman.overdueTask(auth.get_current_user_id());

    if(overdue_tasks.isEmpty())
    {
        return;
    }

    QString all_tasks;

    for(QString &i : overdue_tasks)
    {
        all_tasks += "- " + i + "\n";
    }

    notification->showMessage("OVERDUE TASKS",all_tasks,notification->icon(),3000);
}

void MainWindow::on_dash_searchtask_textChanged(const QString &keyword) //Live search for tasks
{
    ui->task_list->clear();

    if(keyword.isEmpty())
    {
        load_all_tasks();
        return;
    }

    QSqlQuery query = taskman.searchTask(auth.get_current_user_id(),keyword);

    while(query.next())
    {
        QString task = query.value("task").toString();
        int task_id = query.value("id").toInt();

        QListWidgetItem *item = new QListWidgetItem(task);
        item->setData(Qt::UserRole, task_id);

        ui->task_list->addItem(task);
    }
}

void MainWindow::on_btn_ai_open_clicked() //AI Dialog
{
    aiDialog aichat;
    aichat.setAuthObject(&auth); //pass actual obj to AIDialog
    aichat.setAIObject_AND_connectSlot(&ai); //pass actual obj and setting a connect func
    aichat.exec(); //Dialog executed
}
