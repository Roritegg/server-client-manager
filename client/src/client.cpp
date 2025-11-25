#include "client.h"
#include <QString>
#include <QTcpSocket>

Client::~Client()
{
    delete m_sock;
}

Client::Client(): m_sock(new QTcpSocket())
{
    const QString port("9999");
    const QString add("localhost");

    m_sock->connectToHost(add, port.toInt());

    connect(m_sock, &QTcpSocket::connected, this, &Client::socketConnect);
}

void Client::socketConnect()
{
    QTextStream console(stdin);
    QString buffer;
    while(buffer != "--q")
    {
        qDebug() << "Type your message";
        buffer = console.readLine();
        m_sock->write(buffer.toStdString().c_str());
        m_sock->waitForBytesWritten();
    }
    m_sock->disconnectFromHost();
    m_sock->close();
    qDebug() << "Socket closed!";
}

