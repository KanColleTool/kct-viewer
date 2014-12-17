#ifndef KVCOOKIEJAR_H
#define KVCOOKIEJAR_H

#include <QNetworkCookieJar>
#include <QNetworkCookie>
#include <QList>

class KVCookieJar : public QNetworkCookieJar
{
    Q_OBJECT

public:
    explicit KVCookieJar(QObject *parent = 0);

    virtual QList<QNetworkCookie> cookiesForUrl(const QUrl &url) const;
};

#endif // KVCOOKIEJAR_H