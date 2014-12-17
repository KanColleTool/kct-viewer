#include "KVCookieJar.h"

#include <QDebug>
#include <QDateTime>
#include <ksba.h>

KVCookieJar::KVCookieJar(QObject *parent) : QNetworkCookieJar(parent)
{
}

static inline bool isParentPath(QString path, QString reference)
{
    if (!path.endsWith(QLatin1Char('/')))
        path += QLatin1Char('/');
    if (!reference.endsWith(QLatin1Char('/')))
        reference += QLatin1Char('/');
    return path.startsWith(reference);
}

static inline bool isParentDomain(QString domain, QString reference)
{
    if (!reference.startsWith(QLatin1Char('.')))
        return domain == reference;
    return domain.endsWith(reference) || domain == reference.mid(1);
}

QList<QNetworkCookie> KVCookieJar::cookiesForUrl(const QUrl &url) const
{
    QDateTime now = QDateTime::currentDateTime();
    QList<QNetworkCookie> result;
    bool isEncrypted = url.scheme().toLower() == QLatin1String("https");

    // Create dmm cookies
    if(url.host().contains(".dmm.com"))
    {
        QNetworkCookie lang;
        lang.setDomain(".dmm.com");
        lang.setPath("/");
        lang.setName("cklg");
        lang.setValue("ja");
        lang.setExpirationDate(now.addYears(1));

        QNetworkCookie loc;
        loc.setDomain(".dmm.com");
        loc.setPath("/");
        loc.setName("ckcy");
        loc.setValue("1");
        loc.setExpirationDate(now.addYears(1));

        result.append(lang);
        result.append(loc);
    }

    // scan our cookies for something that matches
    QList<QNetworkCookie>::ConstIterator it = allCookies().constBegin(),
                                         end = allCookies().constEnd();
    for ( ; it != end; ++it) {
        if (!isParentDomain(url.host(), it->domain()))
            continue;
        if (!isParentPath(url.path(), it->path()))
            continue;
        if (!(*it).isSessionCookie() && (*it).expirationDate() < now)
            continue;
        if ((*it).isSecure() && !isEncrypted)
            continue;

        // insert this cookie into result, sorted by path
        QList<QNetworkCookie>::Iterator insertIt = result.begin();
        while (insertIt != result.end()) {
            if (insertIt->path().length() < it->path().length()) {
                // insert here
                insertIt = result.insert(insertIt, *it);
                break;
            } else {
                ++insertIt;
            }
        }
        // this is the shortest path yet, just append
        if (insertIt == result.end())
            result += *it;
    }
    return result;
}