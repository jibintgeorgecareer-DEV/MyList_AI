#include "databasemanager.h"

#include <QSqlDatabase> //Sql DB connection
#include <QSqlQuery>    //Sql query execute
#include <QSqlError>    //Sql Error handling

#include <QDebug>       //Qt build in Debug handling
#include <QFileInfo> //To find a file path

DatabaseManager::DatabaseManager()
{
    //constructor
}

bool DatabaseManager::connectDatabase()
{
    // -------------Set Up SQLite Connection---------------------------------------

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");

    db.setDatabaseName("Mylist.db"); //Mylist_APP\build\Desktop_Qt\Mylist.db"

    if(db.open())
    {
        return true;
    }
    else
    {
        return false;
    }

    //-----------------End SQL Connection Setup-------------------------------------------
}

void DatabaseManager::createTables() //Creating table only for tasks
{
    QSqlQuery query; //object to write sql queries.

    query.exec(                             //user table
        "CREATE TABLE IF NOT EXISTS users("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "username TEXT UNIQUE,"
        "password TEXT"
        ")"
        );

    query.exec(                             //task table
        "CREATE TABLE IF NOT EXISTS tasks("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "user_id INTEGER,"
        "task TEXT,"
        "description TEXT,"
        "date_stored TEXT,"
        "date_end TEXT,"
        "priority TEXT,"
        "status TEXT"
        ")"
        );
}