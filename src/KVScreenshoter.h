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

class KVScreenshoter : public QObject
{
    Q_OBJECT
public:
    explicit KVScreenshoter(QObject *parent = 0);
    void takeScreenshot(QWidget *widget);

private:
    QNetworkAccessManager *manager;
    QClipboard *clipboard;
    QByteArray capture(QWidget *widget);
    void upload(QByteArray base64Data);

signals:

public slots:

private slots:
    void onFinished(QNetworkReply* reply);

};

#endif // UPLOADER_H
