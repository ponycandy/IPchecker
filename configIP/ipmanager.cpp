#include "ipmanager.h"
#include <iostream>
#include <stdlib.h>
#include "QSettings"
#include "QDebug"
#include "QTextCodec"
#include "QProcess"
#include "QTime"
#include <QProcess>
#include <qprocess.h>
#include <qthread.h>
IPmanager::IPmanager(QObject *parent) : QObject(parent)
  , m_curVehicleType("F1")
  , rootPermission("echo whoami99 | sudo -S ")
{
}


bool IPmanager::pingIPof(QString IP)
{
//    system("echo whoami99 | sudo -v -S");
//    QString Qcommand="ping -c 1 -q "+IP ;
//    std::string command = Qcommand.toStdString(); // The command to run the script with the address as an argument
//    int result = system(command.c_str()); // Execute the command and get the result
//    if(result==0)
//    {result=1;}
//    else
//    {result=0;}
    QString result;

    QProcess *network_process = new QProcess();
    QString network_cmd = "ping " + IP;
    int iterate = 0;

    while(iterate < 3)
    {
        network_process->start(network_cmd);   //调用ping 指令
        network_process->waitForFinished(3000);    //等待指令执行完毕
        result = network_process->readAll();   //获取指令执行结果

        if(result.contains(QString("TTL=")) || result.contains(QString("ttl=")))   //若包含TTL=字符串则认为网络在线
            return 1;  //在线
        iterate++;
    }
    return 0; //离线
//        return result;
}

QString IPmanager::GetselfIP()
{
    std::string ifconfig = "ifconfig | grep 'inet ' | grep -v '127.0.0.1' | cut -d' ' -f10"; // The command to get the IPV4 address
    char buffer[128]; // A buffer to store the output of the command
    std::string ipv4 = ""; // A string to store the IPV4 address
    FILE* pipe = popen(ifconfig.c_str(), "r"); // Open a pipe to run the command and read the output
    if (pipe) // Check if the pipe is valid
    {
        while (fgets(buffer, sizeof buffer, pipe) != NULL) // Read from the pipe until it is empty
        {
            ipv4 += buffer; // Append the output to the ipv4 string
        }
        pclose(pipe); // Close the pipe
    }
    return QString::fromStdString(ipv4);
}

QString IPmanager::Get_IP_of_Device(QString vehicleType, QString deciveType,QString &port)
{
    m_curVehicleType = vehicleType;
    QString srcDir = "/home/enpht/atos/UnmannedVhicleTeamControlSoftware/build/debug";
    QString name= srcDir + "/authority/"+m_curVehicleType+"-"+"config.ini";

    QString strPath(name);
    QString routeip;
    QSettings *settings = new QSettings(strPath,QSettings::IniFormat);
    settings->setIniCodec(QTextCodec::codecForName("UTF-8"));
    routeip = settings->value("/"+deciveType+"ip").toString();
    port = settings->value("/"+deciveType+"port").toString();
    return  routeip;
}

void IPmanager::change_ip_address(QString ip_address)
{

    QString srcDir = "/home/enpht/atos/UnmannedVhicleTeamControlSoftware/build/debug";
    QString name= srcDir + QString("/authority/") + m_curVehicleType + QString("-") + QString("config.ini");

    QString strPath(name);
    QSettings *settings = new QSettings(strPath,QSettings::IniFormat);
    settings->setIniCodec(QTextCodec::codecForName("UTF-8"));
    QString ethernetname = settings->value("/ethernetname").toString();
    QString ethernetDown = rootPermission + QString(" ifconfig ")+ ethernetname +QString(" down");
    qDebug()<<ethernetDown;
    std::string commandDown = ethernetDown.toStdString();
    int resultDown = system(commandDown.c_str());

    QString ethernetChange = rootPermission + QString(" ifconfig ")+ ethernetname + " " + ip_address +  QString(" netmask 255.255.255.0");
    qDebug()<<ethernetChange;
    std::string commandChange = ethernetChange.toStdString();
    int resultChange  =system(commandChange.c_str());


    QString ethernetUp = rootPermission + QString(" ifconfig ")+ ethernetname +QString(" up");
    qDebug()<<ethernetUp;
    std::string commandUp = ethernetUp.toStdString();
    int resultUp =system(commandUp.c_str());
}

