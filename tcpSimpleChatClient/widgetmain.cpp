#include "widgetmain.h"
#include "ui_widgetmain.h"

WidgetMain::WidgetMain(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetMain)
{
    ui->setupUi(this);

    tcpClienteChat = new QTcpSocket(this);
    connect(tcpClienteChat,SIGNAL(connected()),this,SLOT(chatConectado()));
    connect(tcpClienteChat,SIGNAL(disconnected()),this,SLOT(chatDesconectado()));
    connect(tcpClienteChat,SIGNAL(bytesWritten(qint64)),this,SLOT(bytesChatWriten(qint64)));
    connect(tcpClienteChat,SIGNAL(readyRead()),this,SLOT(readChat()));
    tcpClienteChat->setReadBufferSize(400000);
    //ui->lineEditServer->setText("192.168.1.68");
}

WidgetMain::~WidgetMain()
{
    delete tcpServerChat;
    delete ui;
}

void WidgetMain::sendText(QByteArray mensaje)
{
    tcpClienteChat->flush();
    tcpClienteChat->write(mensaje);
    tcpClienteChat->waitForBytesWritten(3000);
    tcpClienteChat->flush();
}

void WidgetMain::chatConectado()
{
    ui->btnConectarChat->setText("Desconectar");
}

void WidgetMain::chatDesconectado()
{
    ui->btnConectarChat->setText("Conectar");
}

void WidgetMain::bytesChatWriten(qint64 cuantos)
{
    ////////
}

void WidgetMain::readChat()
{
    QByteArray qByteArray;
    QString qString;

    qByteArray=tcpClienteChat->readAll();
    qString="<p style='color:#ff8000'><i>"+qByteArray+"</i></p>";
    ui->plainTextEditChat->appendHtml(qString);
}

void WidgetMain::on_btnEnviarChat_clicked()
{
    QByteArray qByteArray;
    QString qString;

    if (!tcpClienteChat->isOpen())
    {
        ui->plainTextEditChat->appendPlainText("Desconectado");
        return;
    }

    qByteArray=ui->lineEdit->text().toUtf8();
    sendText(qByteArray);
    qString="<p style='color:blue'><i>"+qByteArray+"</i></p>";
    ui->plainTextEditChat->appendHtml(qString);
    ui->lineEdit->setText("");
}

void WidgetMain::on_lineEdit_returnPressed()
{
    on_btnEnviarChat_clicked();
}

void WidgetMain::on_btnConectarChat_clicked()
{
    QString remoteServer,qString;

    if (tcpClienteChat->isOpen())
    {
        tcpClienteChat->close();
        ui->btnConectarChat->setText("Conectar");
    }
    else
    {
        remoteServer=ui->lineEditServer->text();
        tcpClienteChat->connectToHost(remoteServer,__PUERTO_CHAT__);
        if (tcpClienteChat->waitForConnected(3000))
        {
            ui->btnConectarChat->setText("Desconectar");
        }
        else
        {
            qString="Error "+tcpClienteChat->errorString();
            qString="<p style='color:red'><i>"+qString+"</i></p>";
            ui->plainTextEditChat->appendHtml(qString);
            tcpClienteChat->close();
        }
    }
}
