#include <iostream>
#include <QApplication>
#include <QDesktopServices>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDomDocument>
#include <QUrl>

#include "trayicon.h"

TrayIcon::TrayIcon(QObject *parent):
    QSystemTrayIcon(parent),
    m_menu(new QMenu),
    m_network(new QNetworkAccessManager(this)),
    m_timer(new QTimer(this))
{
    this->setIcon(QIcon(":/coffee/white"));
    setMenu();

    reload();
    m_timer->setInterval(5000);
    m_timer->start();

    QObject::connect(this, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(activate(QSystemTrayIcon::ActivationReason)));
    QObject::connect(m_network, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
    QObject::connect(m_timer, SIGNAL(timeout()), this, SLOT(reload()));
}

TrayIcon::~TrayIcon()
{
    delete m_menu;
}

void TrayIcon::changeIcon(int iconType)
{
    switch(iconType)
    {
    case ICON_BLUE:
        this->setIcon(QIcon(":/coffee/blue"));
        break;
    case ICON_GREEN:
        this->setIcon(QIcon(":/coffee/green"));
        break;
    case ICON_YELLOW:
        this->setIcon(QIcon(":/coffee/yellow"));
        break;
    case ICON_RED:
        this->setIcon(QIcon(":/coffee/red"));
        break;
    default:
        this->setIcon(QIcon(":/coffee/white"));
    }
}

void TrayIcon::activate(QSystemTrayIcon::ActivationReason reason)
{
    if(reason == QSystemTrayIcon::Trigger)
        reload(true);
}

void TrayIcon::reload()
{
    changeIcon(ICON_WHITE);
    m_network->get(QNetworkRequest(QUrl("http://file-cafet.irisa.fr/")));
}

void TrayIcon::reload(bool)
{
    reload();
}

void TrayIcon::info(bool)
{
    QDesktopServices::openUrl(QUrl("http://file-cafet.irisa.fr/"));
}

void TrayIcon::quit(bool)
{
    qApp->quit();
}

void TrayIcon::replyFinished(QNetworkReply *reply)
{
    const int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if(statusCode >= 200 && statusCode < 300)
    {
        QString document(reply->readAll());

        if(document.contains("faviconblue"))
            changeIcon(ICON_BLUE);
        else if(document.contains("favicongreen"))
            changeIcon(ICON_GREEN);
        else if(document.contains("faviconyellow"))
            changeIcon(ICON_YELLOW);
        else if(document.contains("faviconred"))
            changeIcon(ICON_RED);
        else
            changeIcon(ICON_WHITE);
    }
    else
    {
        changeIcon(ICON_WHITE);
    }
}

void TrayIcon::setMenu()
{
    QAction * tmp_action;
    tmp_action = m_menu->addAction("reload");
    QObject::connect(tmp_action, SIGNAL(triggered(bool)), this, SLOT(reload(bool)));

    m_menu->addSeparator();

    tmp_action = m_menu->addAction("info");
    QObject::connect(tmp_action, SIGNAL(triggered(bool)), this, SLOT(info(bool)));

    m_menu->addSeparator();

    tmp_action = m_menu->addAction("quit");
    QObject::connect(tmp_action, SIGNAL(triggered(bool)), this, SLOT(quit(bool)));

    this->setContextMenu(m_menu);
}
