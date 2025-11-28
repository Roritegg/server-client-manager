#include <QObject>
#include <QTcpSocket>
#include <QTextStream>

class Client : public QObject
{
    Q_OBJECT
public:
    Client();
    ~Client();
public slots:
    void socketConnect();
    void readData();
private:
    QTcpSocket* m_sock;
    QTextStream* m_console;
};
