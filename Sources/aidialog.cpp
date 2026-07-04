#include "aidialog.h"
#include "ui_aidialog.h"

#include <QFile>
#include <QDate>

aiDialog::aiDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::aiDialog)
{
    ui->setupUi(this);

    ui->chat_history->setReadOnly(true);

    //---------loading Qss files---------------------------------------------
    QFile file(":/styles/aidialog.qss");
    if(file.open(QFile::ReadOnly))
    {
        QString styles = QLatin1String(file.readAll());
        this->setStyleSheet(styles);
    }
    //-------------------------------------------------------------------------

    //---SET ai_submit (button) primary | When ENTER PRESS ai_submit works
    ui->ai_submit->setDefault(true);
    ui->ai_submit->setAutoDefault(true);
    //---------------------------------------------------------------------
}

aiDialog::~aiDialog()
{
    delete ui;
}

void aiDialog::setAuthObject(AuthManager *auth_obj) //Obj from mainwindow
{
    auth = auth_obj;
}

void aiDialog::setAIObject_AND_connectSlot(AIManager *ai_obj) //Obj from AIMANAGER
{
    ai = ai_obj;

    // If signal LLM_reply_generated wakesUp on_AI_reply_received
    connect(ai,&AIManager::LLM_reply_generated,this,&aiDialog::on_AI_reply_received);
}

QString aiDialog::make_prompt(QString question) //Structuring the PROMPT
{
    QStringList tasks =
        taskman.getTaskForAI(auth->get_current_user_id());

    QString context = tasks.join("\n");

    QString today =
        QDate::currentDate().toString("yyyy-MM-dd");

    QString systemMsg =
        "You are the built-in assistant for MYLIST, a to-do list app. "
        "You help the user understand and manage their tasks.\n"
        "Rules:\n"
        "- Only use the task data given to you. Never invent tasks, dates, or details that aren't listed.\n"
        "- If the task list is empty or doesn't contain what's asked, say so plainly.\n"
        "- When asked about deadlines or priorities, reason using the given 'Today' date.\n"
        "- If the question is ambiguous, make a reasonable assumption and state it briefly instead of asking for clarification.";

    QString userMsg =
        QString("Today's date: %1\n\n"
                "Current tasks:\n%2\n\n"
                "User question: %3")
            .arg(today, context.isEmpty() ? "(No tasks found.)" : context, question);

    QString prompt =
        QString("<|system|>\n%1<|end|>\n<|user|>\n%2<|end|>\n<|assistant|>\n")
            .arg(systemMsg, userMsg);

    return prompt;
}

void aiDialog::on_ai_submit_clicked()
{
    QString question =
        ui->user_question->text().trimmed();

    if(question.isEmpty())
        return;

    ui->chat_history->append("You: " + question);
    ui->chat_history->append("AI: Thinking...\n");

    QString prompt = make_prompt(question);

    ai->sentPrompt_to_LLM(prompt);
    ui->user_question->clear();
    ui->ai_submit->setEnabled(false);
}

void aiDialog::on_AI_reply_received(QString answer)
{
    ui->ai_submit->setEnabled(true);
    ui->chat_history->append(answer);
}

void aiDialog::on_btn_settings_clicked()
{
    AppSettings settings(this);
    settings.exec();
}