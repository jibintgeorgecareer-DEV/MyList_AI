#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QSqlDatabase> //Sql DB connection

class DatabaseManager
{
public:
    DatabaseManager();
    bool connectDatabase(); //Connection Qt - DB
    void createTables();   //Create table for only tasks
};

#endif
