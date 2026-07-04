#ifndef AIMANAGER_H
#define AIMANAGER_H

#include <QObject>
#include <QtNetwork/QNetworkAccessManager> //Manahes all network things
#include <QtNetwork/QNetworkReply>
#include <QProcess>
#include <QSettings>

class AIManager : public QObject //fundamental class in QT(Signals,slots,Event,memory, Object hierarchy)
{
    Q_OBJECT //macros for enabling signals ,slots (without it no signal,slot emits)

public:
    explicit AIManager(QObject *parent = nullptr);

    QString server_path_name;
    QString model_path_name;

    void start_llama_server(); //Start llama-server.exe
    void check_server_health(); //To check llama-server returns {"status":"OK"}
    bool is_server_ready(); //Return true if llama-server is Ready
    void sentPrompt_to_LLM(QString prompt); //Send prompt to server

signals:
    void LLM_reply_generated(QString answer);

private slots:
    void LLM_reply_ready(QNetworkReply *reply); //LLM response sent-to aiDialog

private:
    QNetworkAccessManager *network;
    QProcess *server_process;
    bool server_ready = false; //Not used (To check server & model is loaded)
};

#endif // AIMANAGER_H
