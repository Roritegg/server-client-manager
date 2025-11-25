#include "server.h"
#include <QDebug>
#include <QTcpSocket>
#include <QMap>

Server::Server(){}

Server::~Server()
{

    for (auto it = mActiveClients.begin(); it != mActiveClients.end(); ++it)
    {
        
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

    qDebug() << "Server succsessfull start";

    return true;
}

void Server::newClientConnected()
{
    auto client = nextPendingConnection();
    if(client->state() == QAbstractSocket::SocketState::ConnectedState)
        qDebug() << "New connection!";
    else
        qDebug() << "Failed connection(((";

    mActiveClients.insert(client->socketDescriptor(), client);
    connect(client, &QTcpSocket::readyRead, this, &Server::recieveMessage);
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

void Server::recieveMessage()
{
    auto client = static_cast<QTcpSocket*>(QObject::sender());
    QString message;
    message = QString::fromLocal8Bit(client->readAll());
    qDebug() << "Recived message: " << message;
}
