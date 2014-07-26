#ifndef UPLOADER_H
#define UPLOADER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QClipboard>
#include <QApplication>
#include <QBuffer>
#include <QWebView>

class KVScreenshot : public QObject
{
    Q_OBJECT
public:
    explicit KVScreenshot(QObject *parent = 0);
    void takeScreenshot(QWebView *view);

private:
    QNetworkAccessManager *manager;
    QClipboard *clipboard;
    QByteArray capture(QWebView *view);
    void upload(QByteArray base64Data);

signals:

public slots:

private slots:
    void onFinished(QNetworkReply* reply);

};

#endif // UPLOADER_H
