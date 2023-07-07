#ifndef CONFIGIPCHECK_H
#define CONFIGIPCHECK_H

#include <QWidget>
#include <QPixmap>

#include <atomic>
#include "configIP/ipmanager.h"

namespace Ui {
class ConfigIpCheck;
}

class ConfigIpCheck : public QWidget
{
    Q_OBJECT

public:
    explicit ConfigIpCheck(QWidget *parent = nullptr);
    ~ConfigIpCheck();
    void updateUi();
private:

private slots:
    void on_resetLocalIpBtn_clicked();

    void on_carTypeComboBox_currentIndexChanged(int index);

    void on_refreshBtn_clicked();

    void on_cancelBtn_clicked();

private:
    Ui::ConfigIpCheck *ui;
    QString m_carAlias;
    QList<QString> m_carAliasList;
    QString m_connectSuccessStr;
    QString m_connectErrorStr;
    IPmanager * m_service;
    QString m_carIp;
    std::atomic<bool> m_isRun;
//    bool m_isRun;
};

#endif // CONFIGIPCHECK_H
