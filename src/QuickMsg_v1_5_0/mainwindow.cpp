
#include <windows.h>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QPushButton>
#include <QListWidget>
#include <QLineEdit>
#include <QTimer>
#include <QBoxLayout>
#include <QSettings>
#include <QComboBox>
#include "tshifr.h"
using namespace std;

QVector<QString> opentabs;
QVector<QWidget*> openwidget;
QVector <Tshifr*> shifrs;

QString Myname="Cnonim";

QNetworkAccessManager qaccess;
QNetworkReply *rep;

//Форма для смены ника
QWidget *form_nick;
QLineEdit * text_nick;
QPushButton * button_nick;

//форма для просмотра пользователей он-лайн и выбора шифрования
QWidget * Form_online;
QListWidget * list_online;
QComboBox *box;
QTextEdit *key;


QVBoxLayout *vbox;
QHBoxLayout *hbox1;

QTimer * timer;

//функция деления строки на подстроки с разделителем
QVector<QString> split(const QString &s, const QString by = "\n") {
    QVector<QString> res;
    int i, j=by.size(), n = s.size();
    QString tmp="";
    for (i = 0; i < n-j+1; i++) {
        bool flag=true;
        for (int k=0;(k<j)&&(flag);k++)
        {
            if (s[i+k]==by[k])
                flag=true;
            else
                flag= false;
        }
        if (!flag)
            tmp+=s[i];
        else
        {
            res.push_back(tmp);
            tmp="";
            i=i+j-1;
        }
    }
    return res;
}

//Функция отправки POST запроса результат которого можно будет обработать
//через 1 сек в on_timer_reply()
void MainWindow::SendPost(QUrl url,QString host,QString param)
{
    QNetworkRequest req;
    req.setUrl(url);
    QByteArray temp;
    temp.append(host);
    req.setRawHeader("Host", temp);
    req.setRawHeader("Cache-Control", "no-cache");
    req.setRawHeader("Accept","*/*");
    req.setRawHeader("content-type","application/x-www-form-urlencoded");
    QByteArray temp1;
    temp1.append(param);
    rep=qaccess.post(req,temp1);
    connect(rep,SIGNAL(finished()),this,SLOT(on_timer_reply()));
}


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    opentabs.push_back("how to");
    openwidget.push_back(ui->plainTextEdit);
    Tshifr *s=new none();
    shifrs.push_back(s);

    QSettings set("HKEY_CURRENT_USER\\Software\\AndreusAmadeus\\QuickMsg",
                        QSettings::NativeFormat);
    Myname = set.value("nick").toString();

    if (Myname=="")
    {
        Myname="Cnonim";
        set.setValue("nick","Cnonim");
    }

    setWindowTitle("QuickMsg-<"+Myname+">");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    if (ui->tabWidget->currentIndex()!=0)
    {
        QString message,to,from,parametrs;
        message = ui->textEdit->document()->toPlainText();
        if(message!="" && message!="\n")
        {
            QPlainTextEdit *t=(QPlainTextEdit*)openwidget[ui->tabWidget->currentIndex()];
            t->document()->setHtml(t->document()->toHtml()+Myname+":\t"+message+"\n");
            QTextCursor cursor =t->textCursor();
            cursor.movePosition(QTextCursor::End);
            t->setTextCursor(cursor);
            from=Myname;
            to=ui->tabWidget->tabText(ui->tabWidget->currentIndex());
            message=shifrs[ui->tabWidget->currentIndex()]->Encode(message);
            parametrs = "to="+to+"&from="+from+"&msg="+message;
            SendPost(QUrl("http://ejudge.litvinov.in.ua/qmsg/send_message.php"),
                          "ejudge.litvinov.in.ua",
                          parametrs);
        }
    }
    else
        MessageBox(0,
                   L"You can not send messages to this oponent.Please choose another one!",
                   L"error send message",
                   MB_OK);
    ui->textEdit->document()->clear();
}

void MainWindow::on_timer_reply()
{
    if (rep->isFinished())
        ui->plainTextEdit->document()->setHtml(QString(rep->readAll()));
}



int my_search(QVector<QString> a,QString what)
{
    for (int i=0;i<a.size();i++)
        if (a[i]==what)
            return i;
    return -1;
}


void MainWindow::on_plainTextEdit_textChanged()
{
    QString request=ui->plainTextEdit->document()->toPlainText();
    QVector<QString> out=split(request,"\\n");
    if (out.size()<1)
        return;
    if (out[0]=="Online")
    {
        for (int i=1;i<out.size();i++)
        {
            list_online->addItem(out[i]);
        }
    }
    else if (out[0]=="Message")
    {
        for (int i=0;i<(out.size()/2);i++)
        {


            int number=my_search(opentabs,out[i*2+1]);

            if (number==-1)
            {
                QPlainTextEdit *text= new QPlainTextEdit("\n"+out[i*2+1]+":\n\t"+out[i*2+2]+"\n");
                opentabs.push_back(out[i*2+1]);
                Tshifr *s=new none();
                shifrs.push_back(s);
                text->setFocusPolicy(Qt::NoFocus);
                text->setReadOnly(true);
                openwidget.push_back(text);
                ui->tabWidget->addTab(text,out[i*2+1]);
                number=opentabs.length()-1;
                PlaySound(L"w1.wav",NULL,SND_ASYNC);
            }
            else
            {

                QPlainTextEdit *text= (QPlainTextEdit*)openwidget[number];
                if (ui->actionShow_encoded_message->isChecked())
                {
                    text->document()->setHtml(text->document()->toHtml()+out[i*2+1]+" [encoded]:\t"+out[i*2+2]+"\n");
                }
                text->document()->setHtml(text->document()->toHtml()+out[i*2+1]+":\t"+shifrs[number]->Decode(out[i*2+2])+"\n");
                text->setFocusPolicy(Qt::NoFocus);
                PlaySound(L"w2.wav",NULL,SND_ASYNC);
                QTextCursor cursor =text->textCursor();
                cursor.movePosition(QTextCursor::End);
                text->setTextCursor(cursor);
            }
        }
    }
}

void MainWindow::on_actionChange_nickname_triggered()
{

    form_nick =new QWidget();
    hbox1 = new QHBoxLayout(form_nick);
    text_nick= new QLineEdit(Myname);
    button_nick = new QPushButton("OK",form_nick);

    hbox1->addWidget(text_nick);
    hbox1->addWidget(button_nick);

    form_nick->setAttribute(Qt::WA_DeleteOnClose, true);

    form_nick->show();

    button_nick->connect(button_nick,SIGNAL(clicked()),this,SLOT(on_change_nickname()));

}

void MainWindow::on_actionOnline_triggered()
{

    Form_online = new QWidget;
    vbox = new QVBoxLayout(Form_online);
    list_online = new QListWidget(Form_online);
    key = new QTextEdit(Form_online);
    box = new QComboBox(Form_online);

    key->setMaximumHeight(30);

    box->addItem("NONE");
    box->addItem("symbol_to_number");

    vbox->addWidget(list_online);
    vbox->addWidget(box);
    vbox->addWidget(key);

    Form_online->setAttribute(Qt::WA_DeleteOnClose, true);
    Form_online->show();
    Form_online->setGeometry(geometry().x()-230,geometry().y(),200,300);

    connect(list_online,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(on_select_online_item(QModelIndex)));

    SendPost(QUrl("http://ejudge.litvinov.in.ua/qmsg/read_online.php"),
                 "ejudge.litvinov.in.ua",
                 "");
}

void MainWindow::on_select_online_item(const QModelIndex &index)
{
    QString op=list_online->currentItem()->text();
    int number=my_search(opentabs,op);

    if (number==-1)
    {
        Tshifr *s;
        switch (box->currentIndex())
        {
        case 0:
            s=new none();
            break;
        case 1:
            if (key->document()->toPlainText()=="")
            {
                MessageBox(0,
                           L"Can not use this method without KEY!\nPlease enter the key or choose another method!",
                           L"ERROR in crypto-algorythm!!!",MB_OK);
                return;
            }
            s=new Symbol_to_Number();
            break;
        case 2:
            break;
        }
        s->setKey(key->document()->toPlainText());
        shifrs.push_back(s);


        QPlainTextEdit *text= new QPlainTextEdit("");
        QBoxLayout *lay= new QBoxLayout(QBoxLayout::LeftToRight);
        text->setFocusPolicy(Qt::NoFocus);
        text->setReadOnly(true);
        opentabs.push_back(op);
        openwidget.push_back(text);


        ui->tabWidget->addTab(text,op);
        ui->tabWidget->setLayout(lay);
        number=opentabs.length()-1;
    }
    else
    {
        ui->tabWidget->widget(number)->setFocus();
    }
}

void MainWindow::on_change_nickname()
{
    Myname=text_nick->text();
    form_nick->close();
    delete(text_nick);
    delete(button_nick);
    delete(hbox1);
    setWindowTitle("QuickMsg-<"+Myname+">");

    QSettings set("HKEY_CURRENT_USER\\Software\\AndreusAmadeus\\QuickMsg",
                        QSettings::NativeFormat);
    set.setValue("nick",Myname);

}

void MainWindow::on_tabWidget_tabCloseRequested(int index)
{
    if (index!=0)
    {   ui->tabWidget->removeTab(index);
        opentabs.erase(opentabs.begin()+index);
        openwidget.erase(openwidget.begin()+index);
        shifrs.erase(shifrs.begin()+index);
    }
}

void MainWindow::on_actionExit_triggered()
{

    SendPost(QUrl("http://ejudge.litvinov.in.ua/qmsg/offline.php"),
             "ejudge.litvinov.in.ua",
             "name="+Myname);
    QTimer::singleShot( 1000, this, SLOT(close()));
}

void MainWindow::on_actionEnter_triggered()
{
    SendPost(QUrl("http://ejudge.litvinov.in.ua/qmsg/online.php"),
             "ejudge.litvinov.in.ua",
             "name="+Myname);
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(is_income_message()));
    timer->start(1000);
}

void MainWindow::is_income_message()
{
    SendPost(QUrl("http://ejudge.litvinov.in.ua/qmsg/read_message.php"),
             "ejudge.litvinov.in.ua",
             "name="+Myname);
}

void MainWindow::on_textEdit_textChanged()
{
    if (ui->textEdit->document()->toPlainText()[ui->textEdit->document()->toPlainText().size()-1]=='\n')
        on_pushButton_clicked();
}

void MainWindow::on_actionAbout_program_triggered()
{

}

void MainWindow::on_actionAbout_authors_triggered()
{
    MessageBox(0,
               L"Program author and CEO - Kolgatin Andrey\nProviding server - Dmitri Litvinov\nIn case of errors, bugs, or just on improving the program to write to the post office 'andreusamadeus@yandex.ru'\nor in a group 'http://vk.com/andreusamadeus'!\n\n© 2013 AndreusAmadeus Company,2013. All rights reserved.",
               L"About Author",
               MB_OK);
}
