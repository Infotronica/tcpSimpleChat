#ifndef WIDGETMAIN_H
#define WIDGETMAIN_H

#include <QWidget>
#include <QTcpSocket>
#include <QMessageBox>
#include <QImageReader>
#include <QImage>
#include <QBuffer>
#include <QPicture>
#include <QDataStream>
#include <QTcpServer>
#include <QHostAddress>
#include <QNetworkInterface>
#include <QFileDialog>
#include <QStandardPaths>
#include "../commonparams.h"

namespace Ui {
class WidgetMain;
}

class WidgetMain : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetMain(QWidget *parent = nullptr);
    ~WidgetMain();

    void listarDireccionesIP();

    QTcpServer *tcpServerChat;
    QTcpSocket *tcpClienteChat;

public slots:
    void chatConectado();
    void readChat();
    void sendText(QByteArray mensaje);

private slots:
    void on_btnEncenderServer_clicked();

    void on_lineEdit_returnPressed();

    void on_btnEnviarChat_clicked();

private:
    Ui::WidgetMain *ui;
};

#endif // WIDGETMAIN_H
