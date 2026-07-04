#ifndef EXPORTINTOFILE_H
#define EXPORTINTOFILE_H
//Only does Take data from DB and write into a txt file
#include <QString>

class ExportIntoFile
{
public:
    ExportIntoFile();

    bool exportTxt(int current_user_id);

};

#endif // EXPORTINTOFILE_H
