#include "client.h"
#include <QString>
#include <QTcpSocket>
#include <QDebug>
#include <QCoreApplication>

Client::~Client()
{
    delete m_sock;
    delete m_console;
}

Client::Client(): m_sock(new QTcpSocket()), m_console(new QTextStream(stdin))
{
    const QString port("9999");
    const QString add("localhost");

    connect(m_sock, &QTcpSocket::connected, this, &Client::socketConnect);
    connect(m_sock, &QTcpSocket::readyRead, this, &Client::readData);
    connect(m_sock, &QAbstractSocket::disconnected, this, &QCoreApplication::quit);

    m_sock->connectToHost(add, port.toInt());
}

void Client::socketConnect()
{
    qDebug() << "Connected to server!";

    QString buffer;
    while(buffer != "--q")
    {
        qDebug() << "Type your message:";
        buffer = m_console->readLine();

        if (buffer == "--q") {
            break;
        }

        if (m_sock->state() == QAbstractSocket::ConnectedState) {
            QString message = buffer + "\n";
            m_sock->write(message.toLocal8Bit());
            m_sock->waitForBytesWritten();
        } else {
            qDebug() << "Not connected to server!";
            break;
        }
    }

    m_sock->disconnectFromHost();
    if (m_sock->state() != QAbstractSocket::UnconnectedState) {
        m_sock->waitForDisconnected();
    }
    qDebug() << "Socket closed!";
}

void Client::readData()
{
    while (m_sock->canReadLine()) {
        QString response = QString::fromLocal8Bit(m_sock->readLine()).trimmed();
        qDebug() << "Server response:" << response;
    }
}
