#include "server.h"
#include <QDebug>
#include <QTcpSocket>
#include <QMap>

Server::Server(){}

Server::~Server()
{
    for (auto it = mActiveClients.begin(); it != mActiveClients.end(); ++it)
    {
        if (it.value()) {
            it.value()->close();
            delete it.value();
        }
    }
    close();
    mActiveClients.clear();
}

bool Server::open(const QString& port)
{
    if(!listen(QHostAddress::Any, port.toInt()))
    {
        qDebug() << "Unable to start server";
        return false;
    }
    connect(this, &QTcpServer::newConnection, this, &Server::newClientConnected);

    qDebug() << "Server successfully started on port" << port;

    return true;
}

void Server::newClientConnected()
{
    auto client = nextPendingConnection();
    if(client->state() == QAbstractSocket::SocketState::ConnectedState)
    {
        qDebug() << "New connection from" << client->peerAddress().toString()
        << "with descriptor" << client->socketDescriptor();
    }
    else
    {
        qDebug() << "Failed connection";
        return;
    }

    mActiveClients.insert(client->socketDescriptor(), client);
    connect(client, &QTcpSocket::readyRead, this, &Server::receiveMessage);
    connect(client, &QAbstractSocket::disconnected, this, &Server::clientStateChanged);
}

void Server::clientStateChanged()
{
    QObject* signalSender = QObject::sender();
    QTcpSocket* disconnectedSocket = static_cast<QTcpSocket*>(signalSender);

    int socketDescriptor = disconnectedSocket->socketDescriptor();
    mActiveClients.remove(socketDescriptor);

    qDebug() << "Client with descriptor" << socketDescriptor << "disconnected";
}

void Server::receiveMessage()
{
    auto client = static_cast<QTcpSocket*>(QObject::sender());
    QString message = QString::fromLocal8Bit(client->readAll()).trimmed();

    qDebug() << "Received message from client" << client->socketDescriptor() << ":" << message;

    // Переворачиваем строку без использования algorithm
    QString reversed;
    QString cleanMessage = message.remove("\r\n").remove("\n");

    for (int i = cleanMessage.length() - 1; i >= 0; --i) {
        reversed.append(cleanMessage[i]);
    }

    qDebug() << "Reversed message:" << reversed;

    // Отправляем обратно клиенту
    if (client->state() == QAbstractSocket::ConnectedState) {
        QString response = reversed + "\n";
        client->write(response.toLocal8Bit());
        client->waitForBytesWritten();
    }
}
