#ifndef TRAYICON_H
#define TRAYICON_H

#include <QObject>
#include <QMenu>
#include <QSystemTrayIcon>
#include <QNetworkAccessManager>
#include <QTimer>

class TrayIcon : public QSystemTrayIcon
{
    Q_OBJECT
public:
    static const int ICON_WHITE = 0;
    static const int ICON_BLUE = 1;
    static const int ICON_GREEN = 2;
    static const int ICON_YELLOW = 3;
    static const int ICON_RED = 4;

public:
    explicit TrayIcon(QObject *parent = 0);
    virtual ~TrayIcon();

signals:

protected slots:
    void changeIcon(int iconType);
    void activate(QSystemTrayIcon::ActivationReason reason);
    void reload();
    void reload(bool);
    void info(bool);
    void quit(bool);

    void replyFinished(QNetworkReply * reply);

protected:
    void setMenu();

private:
    QMenu * m_menu;
    QNetworkAccessManager * m_network;

    QTimer * m_timer;
};

#endif // TRAYICON_H
