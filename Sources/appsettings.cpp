#include "appsettings.h"
#include "ui_appsettings.h"

#include <QSettings>
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>

AppSettings::AppSettings(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AppSettings),settings("MyList","MyList") //QSetting obj initilized
{
    ui->setupUi(this);
    this->setWindowTitle("MyList - Settings");

    //----show whatever in QSetting---------------------------------------
    ui->model_path->setText(settings.value("ai-model").toString());
    ui->server_path->setText(settings.value("llama-server").toString());
    ui->tokens_number->setText(settings.value("model_token",100).toString());
    //--------------------------------------------------------------------

    //---------loading Qss files---------------------------------------------
    QFile file(":/styles/appsettings.qss");
    if(file.open(QFile::ReadOnly))
    {
        QString styles = QLatin1String(file.readAll());
        this->setStyleSheet(styles);
    }
    //-------------------------------------------------------------------------
}

AppSettings::~AppSettings()
{
    delete ui;
}

void AppSettings::on_btn_save_settings_clicked() //When clicked store server & model paths in QSettings
{
    settings.setValue("llama-server",ui->server_path->text()); //Setting model and server paths
    settings.setValue("ai-model",ui->model_path->text());

    if(ui->tokens_number->text().isEmpty()) //Setting model token default = 100
    {
        settings.setValue("model_token",100); //Default is 100
    }
    else
    {
        settings.setValue("model_token",ui->tokens_number->text().toInt()); //No of tokens to be generated
    }

    QMessageBox::information(this,"Succesfull","Settings Saved");
    return;
}

void AppSettings::on_btn_browse_server_clicked() //Enable file browsing for user (server)
{
    QString server_path = QFileDialog::getOpenFileName(this,"Select llama-server.exe","","Executable (*.exe)");

    if(!server_path.isEmpty())
    {
        ui->server_path->setText(server_path);
    }
}

void AppSettings::on_pushButton_clicked() //Enable file browsing for user (AI model)
{
    QString model_path = QFileDialog::getOpenFileName(this,"Select model.exe","","ModelFile (*.gguf)");

    if(!model_path.isEmpty())
    {
        ui->model_path->setText(model_path);
    }
}

