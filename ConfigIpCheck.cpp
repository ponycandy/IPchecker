#include "ConfigIpCheck.h"
#include "ui_ConfigIpCheck.h"

#include <QtConcurrent/QtConcurrent>
#include <QMessageBox>
ConfigIpCheck::ConfigIpCheck(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConfigIpCheck)
  , m_isRun(false)
{
    ui->setupUi(this);

    m_carAliasList = {"F1", "F2", "F3", "F4", "F5"};
    m_carAlias = m_carAliasList.at(ui->carTypeComboBox->currentIndex());
    m_connectSuccessStr = "QCheckBox::indicator{image:url(:/img/connect_success);}";
    m_connectErrorStr = "QCheckBox::indicator{image:url(:/img/connect_error);}";
    setWindowFlags(Qt::FramelessWindowHint);

    m_service = new IPmanager;

    updateUi();
}

ConfigIpCheck::~ConfigIpCheck()
{
    delete m_service;
    delete ui;
}

void ConfigIpCheck::updateUi()
{
    ui->remoteipLive->setStyleSheet("");
    ui->vcstreamipLive->setStyleSheet("");
    ui->zhstreamipLive->setStyleSheet("");
    ui->localipLive->setStyleSheet("");

    m_carAlias = m_carAliasList.at(ui->carTypeComboBox->currentIndex());
    QString localPcIp = m_service->GetselfIP();
    int findIndex = localPcIp.indexOf("\n");
    if(findIndex >= 0 && findIndex < localPcIp.size())
        ui->localip->setText(localPcIp.left(findIndex));
    else
        ui->localip->setText(localPcIp);

    QString carIpPort;
    m_carIp = m_service->Get_IP_of_Device(m_carAlias, "local", carIpPort);
    ui->carIpLb->setText(m_carIp + ":" + carIpPort);

    QString remoteipPort;
    QString remoteip = m_service->Get_IP_of_Device(m_carAlias, "remote", remoteipPort);
    ui->remoteip->setText(remoteip + ":" + remoteipPort);

    QString vcstreamipPort;
    QString vcstreamip = m_service->Get_IP_of_Device(m_carAlias, "vcstream", vcstreamipPort);
    ui->vcstreamip->setText(vcstreamip + ":" + vcstreamipPort);

    QString zhstreamipPort;
    QString zhstreamip = m_service->Get_IP_of_Device(m_carAlias, "zhstream", zhstreamipPort);
    ui->zhstreamip->setText(zhstreamip + ":" + zhstreamipPort);


    QtConcurrent::run([=](){
        //        if(m_isRun.load())
        //        {
        //            QMessageBox::critical(this, "warn", "waiting 5s");
        ////            return ;
        //        }
        //        else
        {
            m_isRun = true;
            static qint64 lastTime;
            lastTime = QDateTime::currentMSecsSinceEpoch();
            bool localipLive = m_service->pingIPof(localPcIp);
            qDebug()<<"localPcIp "<<QDateTime::currentMSecsSinceEpoch() - lastTime;
            ui->localipLive->setStyleSheet(localipLive ? m_connectSuccessStr: m_connectErrorStr);

            lastTime = QDateTime::currentMSecsSinceEpoch();
            bool remoteipLive = m_service->pingIPof(remoteip);
            qDebug()<<"remoteip "<<QDateTime::currentMSecsSinceEpoch() - lastTime;
            ui->remoteipLive->setStyleSheet(remoteipLive ? m_connectSuccessStr: m_connectErrorStr);

            lastTime = QDateTime::currentMSecsSinceEpoch();
            bool vcstreamipLive = m_service->pingIPof(vcstreamip);
            qDebug()<<"vcstreamip "<<QDateTime::currentMSecsSinceEpoch() - lastTime;
            ui->vcstreamipLive->setStyleSheet(vcstreamipLive ? m_connectSuccessStr: m_connectErrorStr);

            lastTime = QDateTime::currentMSecsSinceEpoch();
            bool zhstreamipLive = m_service->pingIPof(zhstreamip);
            qDebug()<<"zhstreamip "<<QDateTime::currentMSecsSinceEpoch() - lastTime;
            ui->zhstreamipLive->setStyleSheet(zhstreamipLive ? m_connectSuccessStr: m_connectErrorStr);
            m_isRun = false;
        }
    });
}

void ConfigIpCheck::on_resetLocalIpBtn_clicked()
{
    m_service->change_ip_address(m_carIp);
}

void ConfigIpCheck::on_carTypeComboBox_currentIndexChanged(int index)
{
    updateUi();
}

void ConfigIpCheck::on_refreshBtn_clicked()
{
    updateUi();
}

void ConfigIpCheck::on_cancelBtn_clicked()
{
    close();
}
