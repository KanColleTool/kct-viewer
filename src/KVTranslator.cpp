#include "KVTranslator.h"
#include "KVUtil.h"
#include <QJsonObject>
#include <QJsonArray>

KVTranslator::KVTranslator(QObject *parent):
	QObject(parent)
{
	
}

KVTranslator::~KVTranslator()
{
	
}

void KVTranslator::addTranslation(const QString &from, const QString &to)
{
	QByteArray fromBytes = from.toUtf8();
	this->addTranslation(crc32(0, fromBytes.data(), fromBytes.size()), to);
}

void KVTranslator::addTranslation(quint32 from, const QString &to)
{
	dictionary.insert(from, to);
}

QString KVTranslator::translate(const QString &phrase) const
{
	if (!this->isTranslatable(phrase)) {
		return phrase;
	}
	
	QString real = unescape(phrase);
	QByteArray realBytes = real.toUtf8();
	quint32 crc = crc32(0, realBytes.data(), realBytes.size());
	
	const QString &translation = dictionary.value(crc);
	return !translation.isEmpty() ? translation : phrase;
}

QJsonDocument KVTranslator::translate(const QJsonDocument &doc) const
{
	if (doc.isArray()) {
		QJsonArray val = doc.array();
		for(auto it = val.begin(); it != val.end(); ++it) {
			this->walk(*it);
		}
		return QJsonDocument(val);
	} else {
		QJsonObject val = doc.object();
		for(auto it = val.begin(); it != val.end(); ++it) {
			this->walk(*it, it.key());
		}
		return QJsonDocument(val);
	}
}

bool KVTranslator::isTranslatable(const QString &phrase) const
{
	// Some special exceptions...
	static QList<QString> exceptions { "", "-" };
	if(exceptions.contains(phrase)) {
		return false;
	}
	
	// Numeric strings aren't translatable
	bool isNumeric;
	phrase.toFloat(&isNumeric);
	if(isNumeric) {
		return false;
	}
	
	return true;
}

void KVTranslator::walk(QJsonValueRef value, const QString &key) const
{
	QJsonObject obj = value.toObject();
	QJsonArray arr = value.toArray();
	
	switch(value.type()) {
		case QJsonValue::Object:
			for(auto it = obj.begin(); it != obj.end(); ++it) {
				this->walk(*it, it.key());
			}
			break;
		case QJsonValue::Array:
			for(auto it = arr.begin(); it != arr.end(); ++it) {
				this->walk(*it, key);
			}
			break;
		case QJsonValue::String:
			value = this->translate(value.toString());
			break;
		default:
			break;
	}
}
