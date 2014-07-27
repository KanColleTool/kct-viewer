#ifndef KVDEFAULTS_H
#define KVDEFAULTS_H

#include <QNetworkProxy>

static const bool kDefaultTranslation = false;
static const bool kDefaultReportUntranslated = true;
static const bool kDefaultTaskbarProgress = true;
static const bool kDefaultProxy = false;
static const QString kDefaultProxyServer = "localhost";
static const int kDefaultProxyPort = 8080;
static const int kDefaultProxyType = QNetworkProxy::Socks5Proxy;
static const QString kDefaultProxyUser = "";
static const QString kDefaultProxyPass = "";
static const bool kDefaultUploadScreenshots = false;

#endif // KVDEFAULTS_H
