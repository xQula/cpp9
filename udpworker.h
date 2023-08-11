#ifndef UDPWORKER_H
#define UDPWORKER_H

#include <QUdpSocket>
#include <QNetworkDatagram>
#include <QDateTime>


#define BIND_PORT 12345

enum class SignalOption{
    One,
    Two,
    MaxSignal
};

class UDPworker : public QObject
{
    Q_OBJECT
public:
    explicit UDPworker(QObject *parent = nullptr);
    void InitSocket( void );
    void ReadDatagram( QNetworkDatagram datagram);
    void SendDatagram(QByteArray data, int signal_option );

private slots:
    void readPendingDatagrams(void);

private:
    QUdpSocket* serviceUdpSocket;
    int signal_option;

signals:
    void sig_sendTimeToGUI(QDateTime data);
    void sig_sendSizeByteToGUI(const qsizetype size,const QString str_send);

};

#endif // UDPWORKER_H
