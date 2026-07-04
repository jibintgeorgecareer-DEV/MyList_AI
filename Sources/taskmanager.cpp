#include "taskmanager.h"

#include <QString>
#include <QStringList> //List of Strings like a array
#include <QSqlQuery>
#include <QVariant>
#include <QSqlError>   // sql errors
#include <QDate>

TaskManager::TaskManager()
{
    //contructor
}

bool TaskManager::addTask(int current_user_id, QString task, QString description, QString date_stored, QString date_end, QString priority, QString status)
{
    QSqlQuery query;

    query.prepare(
        "INSERT INTO tasks("
        "user_id,"
        "task,"
        "description,"
        "date_stored,"
        "date_end,"
        "priority,"
        "status"
        ")"
        "VALUES("
        ":current_user_id,"
        ":task,"
        ":description,"
        ":date_stored,"
        ":date_end,"
        ":priority,"
        ":status)"
        );
    query.bindValue(":current_user_id",current_user_id);
    query.bindValue(":task",task);
    query.bindValue(":description",description);
    query.bindValue(":date_stored",date_stored);
    query.bindValue(":date_end",date_end);
    query.bindValue(":priority",priority);
    query.bindValue(":status",status);

    return query.exec();
}

QSqlQuery TaskManager::loadTask(int current_user_id) //Load tasks from DB
{
    QStringList tasks_list;
    QSqlQuery query;

    query.prepare("SELECT id,task FROM tasks WHERE user_id=:current_user_id");
    query.bindValue(":current_user_id",current_user_id);
    query.exec();

    return query;
}

bool TaskManager::deleteTask(int task_id) //Delete task from DB
{
    QSqlQuery query;

    query.prepare("DELETE FROM tasks WHERE id=:task_id;");
    query.bindValue(":task_id",task_id);

    if(query.exec())
    {
        return true;
    }
    return false;
}

QSqlQuery TaskManager::searchTask(int current_user_id, QString keyword) //Serach tasks from DB
{
    QStringList tasks_list;
    QSqlQuery query;

    query.prepare("SELECT id,task FROM tasks WHERE user_id=:current_user_id AND task LIKE :keyword OR description LIKE :keyword");
    query.bindValue(":current_user_id",current_user_id);
    query.bindValue(":keyword", "%" + keyword.trimmed() + "%"); //makes %keyword%
                                                                //by removing whitespace trimmed()
    query.exec();
    return query;
}

int TaskManager::totalTasks(int current_user_id) //Total count of tasks
{
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM tasks WHERE user_id=:current_user_id;");
    query.bindValue(":current_user_id",current_user_id);
    query.exec();

    if(query.next())
    {
        return query.value(0).toInt();
    }

    return 0;
}

int TaskManager::completedTasks(int current_user_id) //Total count of completed tasks
{
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM tasks WHERE user_id=:current_user_id AND status='Completed';");
    query.bindValue(":current_user_id",current_user_id);
    query.exec();

    if(query.next())
    {
        return query.value(0).toInt();
    }

    return 0;
}

int TaskManager::pendingTasks(int current_user_id) //Total count of pending tasks
{
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM tasks WHERE user_id=:current_user_id AND status='Pending';");
    query.bindValue(":current_user_id",current_user_id);
    query.exec();

    if(query.next())
    {
        return query.value(0).toInt();
    }

    return 0;
}

QSqlQuery TaskManager::taskByStatus(int current_user_id, QString status) //Returns tasks by status
{
    QSqlQuery query;
    QStringList tasks_list;

    query.prepare("SELECT id,task FROM tasks WHERE user_id=:current_user_id AND status=:status;");
    query.bindValue(":current_user_id",current_user_id);
    query.bindValue(":status",status);
    query.exec();

    return query;
}

QSqlQuery TaskManager::searchByPriority(int current_user_id, QString keyword) //Serch by task priority
{
    QSqlQuery query;
    //QStringList tasks_list;

    query.prepare("SELECT task FROM tasks WHERE user_id=:current_user_id AND priority=:keyword");
    query.bindValue(":current_user_id",current_user_id);
    query.bindValue(":keyword",keyword);
    query.exec();

    return query;
}

QStringList TaskManager::remainder(int current_user_id) //Reminder system for tasks are today
{
    QSqlQuery query;
    QStringList task_lists;
    QStringList task_lists_tommarrow;

    query.prepare("SELECT task,date_end FROM tasks WHERE user_id=:current_user_id AND status=:Pending;");
    query.bindValue(":current_user_id",current_user_id);
    query.bindValue(":Pending","Pending");
    query.exec();

    QDate today = QDate::currentDate(); //sets current date
    QDate tommarrow = today.addDays(1);

    while(query.next())
    {
        QString task =  query.value("task").toString();

        QString end_date_string = query.value("date_end").toString();

        QDate date_end =QDate::fromString(end_date_string, "dd-MM-yyyy");

        if(today == date_end)
        {
            task_lists.append(task);
        }
        else if(tommarrow == date_end)
        {
            task_lists_tommarrow.append(task);
        }
    }
    return task_lists;
}

QStringList TaskManager::overdueTask(int current_user_id) //System notifications
{
    QStringList overdueasks;
    QDate today=QDate::currentDate();
    QSqlQuery query;

    query.prepare("SELECT task,date_end FROM tasks WHERE user_id=:current_user_id AND status=:Pending;");
    query.bindValue(":current_user_id",current_user_id);
    query.bindValue(":Pending","Pending");
    query.exec();

    while(query.next())
    {
        QString task=query.value("task").toString();
        QString end_date_string=query.value("date_end").toString();

        QDate date_end = QDate::fromString(end_date_string,"dd-MM-yyyy");

        if(today > date_end)
        {
            overdueasks.append(task);
        }
    }
    return overdueasks;
}

QStringList TaskManager::getTaskForAI(int current_user_id) //Gets all tasks for LLM
{
    QSqlQuery query;
    QStringList tasks;

    query.prepare("SELECT task,status,date_end FROM tasks WHERE user_id = :current_user_id");
    query.bindValue(":current_user_id",current_user_id);
    query.exec();

    while(query.next())
    {
        QString taskInfo =
            "Task: " + query.value("task").toString()
            + ", Status: " + query.value("status").toString()
            + ", Due: " + query.value("date_end").toString();

        tasks.append(taskInfo);
    }

    return tasks;

}