#include <QObject>
#include <QTcpSocket>

class Client : public QObject
{
    Q_OBJECT
public:
    Client();
    ~Client();
public slots:
    void socketConnect();
private:
    QTcpSocket* m_sock;
};
