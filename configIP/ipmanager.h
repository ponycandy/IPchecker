#ifndef IPMANAGER_H
#define IPMANAGER_H

#include <QObject>

class IPmanager : public QObject
{
    Q_OBJECT
public:
    explicit IPmanager(QObject *parent = nullptr);
    bool pingIPof(QString IP) ;
    QString GetselfIP() ;
    QString Get_IP_of_Device(QString vehicleType, QString deciveType,QString &port);
    void change_ip_address(QString ip_address);

signals:

private:
    QString m_curVehicleType;
    QString rootPermission;
};

#endif // IPMANAGER_H
