#include "aimanager.h"

#include <QNetworkReply> //Manage Reply from llma-server
#include <QNetworkRequest> //Manage Request to llama-server
#include <QJsonDocument> //hmmm
#include <QJsonObject>
#include <QProcess> //To Open separate program (llama-server.exe)
#include <QTimer>
#include <QSettings>
#include <QMessageBox>
#include <QDebug>

AIManager::AIManager(QObject *parent) : QObject(parent)
{
    network = new QNetworkAccessManager(this); //Network obj for llama-server
    connect(network, &QNetworkAccessManager::finished,this,&AIManager::LLM_reply_ready);

    server_process = new QProcess(this); //QProcess obj
}

void AIManager::start_llama_server() //Settting llama-server.exe using QProcess
{
    if(server_process->state() != QProcess::NotRunning)
    {
        qDebug()<<"Server is Running | No worries!";
        return;
    }

    //Accessing QSetting to get server & model path-------------------------
    QSettings settings("MyList","MyList");
    QString server_path = settings.value("llama-server").toString(); //lama-server.exe
    QString model_path = settings.value("ai-model").toString(); //model.gguf

    qDebug()<<"AIMANAGER->model path by user:"<<model_path;
    qDebug()<<"AIMANAGER->server path by user:"<<server_path;

    if(server_path.isEmpty() || model_path.isEmpty())
    {
        qDebug()<<"Select llama-server & AI-Model";
    }
    //----------------------------------------------------------------------

    QStringList args;

    args << "-m"
         << model_path
         << "-n"
         << "100"
         << "--port"
         << "8080";

    //server_process->start("D:/llama.cpp/llama-server.exe",args);//Starting Server
    server_process->start(server_path,args);//Starting Server

    QTimer::singleShot(5000,this,&AIManager::check_server_health);//Waits 5 secs and go-to check_server_health()

}

bool AIManager::is_server_ready() //True if server ready
{
    return server_ready;
}

void AIManager::check_server_health() //Returns json (status:OK) if server set But not used
{
    QNetworkRequest req;

    req.setUrl(QUrl("http://127.0.0.1:8080/health"));

    network->get(req);
}

void AIManager::sentPrompt_to_LLM(QString prompt) //Sent Prompt to LLM (server)
{

    //Sending an Http request
    QNetworkRequest request;
    request.setUrl(QUrl("http://127.0.0.1:8080/completion")); //Request Line (schema=http host port path=completion)
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/json"); //Header (how data sent)

    //--QSetting for model token--------------------------------------------
    QSettings settings("MyList","MyList");
    int model_token = settings.value("model_token",100).toInt();
    qDebug()<<"TOKEN NUMBER:"<<model_token;
    //----------------------------------------------------------------------


    QJsonObject json; //Json container for key-value pair

    json["prompt"] = prompt; //Adding data to json container
    json["n_predict"] = model_token;
    json["cache_prompt"] = false;

    QJsonDocument doc(json); //QJsonDocument used to convert json obj into raw text or bytes

    network->post(request,doc.toJson()); //Sending request.post to server. as json
}

void AIManager::LLM_reply_ready(QNetworkReply *reply) //slot function
{

    QString path = reply->url().path(); //get path from llama-server reply

    if(path == "/health") //server is ready
    {
        bool ready = (reply->error() == QNetworkReply::NoError);
        server_ready = ready;

        reply->deleteLater();
        return;
    }

    QByteArray data = reply->readAll();

    QJsonDocument doc = QJsonDocument::fromJson(data); //ByteArray to Json
    QJsonObject obj = doc.object();

    QString answer = obj["content"].toString();

    emit LLM_reply_generated(QString(answer)); //Is a slot function from aidialog.cpp
    reply->deleteLater();

    qDebug() << "Reply Received";

}