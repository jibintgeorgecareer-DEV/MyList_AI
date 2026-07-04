#ifndef APPSETTINGS_H
#define APPSETTINGS_H

#include <QDialog>
#include <QSettings>

#include "aimanager.h"

namespace Ui {
class AppSettings;
}

class AppSettings : public QDialog
{
    Q_OBJECT

public:
    explicit AppSettings(QWidget *parent = nullptr);
    ~AppSettings();

private slots:
    void on_btn_save_settings_clicked();

    void on_btn_browse_server_clicked();

    void on_pushButton_clicked();

private:
    Ui::AppSettings *ui;
    AIManager aiman;
    QSettings settings;
};

#endif // APPSETTINGS_H
