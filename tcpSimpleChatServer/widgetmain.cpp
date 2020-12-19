#include "widgetmain.h"
#include "ui_widgetmain.h"

WidgetMain::WidgetMain(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetMain)
{
    ui->setupUi(this);

    tcpClienteChat=nullptr;
    tcpServerChat = new QTcpServer(this);
    connect(tcpServerChat,SIGNAL(newConnection()),this,SLOT(chatConectado()));

    listarDireccionesIP();
}

WidgetMain::~WidgetMain()
{
    delete tcpServerChat;
    delete ui;
}

void WidgetMain::listarDireccionesIP()
{
    QList<QHostAddress> ipListAddress = QNetworkInterface::allAddresses();

    for(int i=0; i<ipListAddress.count(); i++)
    {
        if (ipListAddress[i].protocol()==QAbstractSocket::IPv4Protocol && !ipListAddress[i].isLoopback())
        {
            ui->cbxIPs->addItem(ipListAddress[i].toString());
        }
    }
    ui->cbxIPs->setCurrentIndex(0);
}

void WidgetMain::chatConectado()
{
    tcpClienteChat=tcpServerChat->nextPendingConnection();
    connect(tcpClienteChat,SIGNAL(readyRead()),this,SLOT(readChat()));
    sendText("Conectado");
}

void WidgetMain::readChat()
{
    QByteArray qByteArray;
    QString qString;

    qByteArray=tcpClienteChat->readAll();
    qString="<p style='color:#ff8000'><i>"+qByteArray+"</i></p>";
    ui->plainTextEditChat->appendHtml(qString);
}

void WidgetMain::sendText(QByteArray mensaje)
{
    tcpClienteChat->flush();
    tcpClienteChat->write(mensaje);
    tcpClienteChat->waitForBytesWritten(3000);
    tcpClienteChat->flush();
}

void WidgetMain::on_btnEncenderServer_clicked()
{
    QString ipServer,qString;
    int i;

    if (tcpServerChat->isListening())
    {
        tcpServerChat->close();
        ui->labelServerStatus->setText("Servidor Apagado");
        ui->btnEncenderServer->setText("Encender Servidor");
        return;
    }

    i=ui->cbxIPs->currentIndex();
    if (i==-1)
    {
        return;
    }
    ipServer=ui->cbxIPs->currentText();
    if (tcpServerChat->listen(QHostAddress(ipServer),__PUERTO_CHAT__))
    {
        qString="Servidor Activo en IP "+ipServer+" y Puerto "+QString::number(__PUERTO_CHAT__);
        ui->labelServerStatus->setText(qString);
        ui->btnEncenderServer->setText("Apagar Servidor");
    }
    else
    {
        qString="Error "+tcpServerChat->errorString();
        qString="<p style='color:red'><i>"+qString+"</i></p>";
        ui->plainTextEditChat->appendHtml(qString);
        tcpServerChat->close();
    }
}

void WidgetMain::on_btnEnviarChat_clicked()
{
    QByteArray qByteArray;
    QString qString;

    if (!tcpClienteChat)
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
