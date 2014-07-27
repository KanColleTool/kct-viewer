#include "KVScreenshoter.h"

#include <QDebug>

KVScreenshoter::KVScreenshoter(QObject *parent) :
    QObject(parent)
{
    manager = new QNetworkAccessManager(this);
    clipboard = QApplication::clipboard();
}

QByteArray KVScreenshoter::capture(QWebView* view)
{
    QImage image(view->size(), QImage::Format_ARGB32);
    image.fill(Qt::transparent);
    QPainter painter(&image);

    view->render(&painter);
    painter.end();

    QByteArray rawData;
    QBuffer buffer(&rawData);
    buffer.open(QBuffer::WriteOnly);

    image.save(&buffer, "PNG");
    buffer.close();

    return rawData.toBase64();
}

void KVScreenshoter::upload(QByteArray base64Data)
{
    QNetworkRequest request(QUrl("https://api.imgur.com/3/image.json"));
    request.setRawHeader("Authorization", "Client-ID ef6bd901726e8b7");

    QByteArray requestBody;
    requestBody.append(QString("image=").toUtf8());
    requestBody.append(QUrl::toPercentEncoding(base64Data));

    manager->post(request, requestBody);
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onFinished(QNetworkReply*)));
}

void KVScreenshoter::onFinished(QNetworkReply *reply)
{
    if (reply->error()) {
        qDebug() << "Error:" << reply->errorString();
        return;
    }

    QString textData(reply->readAll());

    QJsonDocument jsonDocument = QJsonDocument::fromJson(textData.toUtf8());
    QJsonObject jsonObject = jsonDocument.object();

    QString link = jsonObject["data"].toObject()["link"].toString();

    clipboard->setText(link);

    reply->deleteLater();
}

void KVScreenshoter::takeScreenshot(QWebView *view)
{
    QByteArray base64Data = KVScreenshoter::capture(view);
    KVScreenshoter::upload(base64Data);
}
