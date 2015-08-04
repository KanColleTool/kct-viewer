#include "KVTranslator.h"
#include "KVUtil.h"
#include "KVDefaults.h"

#include <QMutex>
#include <QUrl>
#include <QStandardPaths>
#include <QDir>
#include <QEventLoop>
#include <QByteArray>
#include <QStack>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QDateTime>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDebug>

KVTranslator& KVTranslator::instance()
{
	static KVTranslator _instance;
	return _instance;
}

KVTranslator::KVTranslator(QObject *parent):
	QObject(parent),
	reportUntranslated(kDefaultReportUntranslated),
	state(created)
{
	cacheFile.setFileName(QDir(QStandardPaths::writableLocation(QStandardPaths::CacheLocation)).filePath("translation.json"));
}

KVTranslator::~KVTranslator()
{

}

bool KVTranslator::isLoaded() { return state == loaded; }

void KVTranslator::loadTranslation(QString language)
{
	state = loading;
	blacklistState = loading;

	if(cacheFile.exists())
	{
		cacheFile.open(QIODevice::ReadOnly | QIODevice::Unbuffered);
		parseTranslationData(cacheFile.readAll());
		cacheFile.close();
	}

	//QNetworkReply *tlReply = manager.get(QNetworkRequest(QString("http://api.comeonandsl.am/translation/%1/").arg(language)));
	QNetworkReply *tlReply = manager.get(QNetworkRequest(QString("http://yukariin.github.io/%1.json").arg(language)));
	connect(tlReply, SIGNAL(finished()), this, SLOT(translationRequestFinished()));

	QNetworkReply *blReply = manager.get(QNetworkRequest(QString("http://kancolletool.github.io/report_blacklist.json")));
	connect(blReply, SIGNAL(finished()), this, SLOT(blacklistRequestFinished()));
}

void KVTranslator::translationRequestFinished()
{
	// Read the response body
	QNetworkReply *reply(qobject_cast<QNetworkReply*>(QObject::sender()));
	if(reply->error() != QNetworkReply::NoError)
	{
		state = failed;
		emit loadFailed(QString("Network Error: %1").arg(reply->errorString()));
		return;
	}
	QByteArray body(reply->readAll());

	if(parseTranslationData(body))
	{
		qDebug() << "Network translation loaded!";
		if(cacheFile.open(QIODevice::WriteOnly|QIODevice::Truncate)) {
			cacheFile.write(body);
			cacheFile.close();
		} else {
			qDebug() << "Couldn't write to cache file";
		}
	}
}

void KVTranslator::blacklistRequestFinished() {
	QNetworkReply *reply(qobject_cast<QNetworkReply*>(QObject::sender()));
	if(reply->error() != QNetworkReply::NoError)
	{
		blacklistState = failed;
		return;
	}

	QJsonParseError error;
	reportBlacklist = QJsonDocument::fromJson(reply->readAll(), &error).toVariant().toMap();
	if(error.error != QJsonParseError::NoError)
	{
		qDebug() << "Couldn't load Report Blacklist:" << error.errorString();
		blacklistState = failed;
	}
	else
	{
		qDebug() << "Blacklist Loaded";
		blacklistState = loaded;
	}
}

bool KVTranslator::parseTranslationData(const QByteArray &data) {
	// Parse the JSON
	QJsonParseError error;
	QJsonDocument doc(QJsonDocument::fromJson(data, &error));
	if(error.error != QJsonParseError::NoError) {
		state = failed;
		emit loadFailed(QString("JSON Error: %1").arg(error.errorString()));
		return false;
	}
	QJsonObject root(doc.object());

	// Check the response
	int success = (int) root.value("success").toDouble();
	if(success != 1) {
		state = failed;
		emit loadFailed(QString("API Error %1").arg(success));
		return false;
	}

	// Parse the translation data
	translation = root.value("translation").toObject().toVariantMap();

	state = loaded;
	emit loadFinished();
	return true;
}

QString KVTranslator::translate(const QString &line, QString lastPathComponent, QString key)
{
	// Block until translation is loaded
	QEventLoop loop;
	switch(state)
	{
	case created:
	case failed:
		return line;
	case loading:
		loop.connect(this, SIGNAL(loadFinished()), SLOT(quit()));
		loop.connect(this, SIGNAL(loadFailed(QString)), SLOT(quit()));
		loop.exec();
	case loaded:
		break;
	}

	QString realLine = unescape(line);

	// Check if the string is empty
	if(realLine.isEmpty() || realLine == "-")
		return line;

	// Check if the line is numeric
	bool isNumber = false;
	realLine.toFloat(&isNumber);
	if(isNumber)
		return line;

	// Check if the line is blacklisted
	QVariantList endpointBlacklist = reportBlacklist.value(lastPathComponent).toList();
	if(endpointBlacklist.contains(key))
	{
		//qDebug() << "Blacklisted Line:" << lastPathComponent << "::" << key << "=" << realLine;
		return line;
	}

	// Translate it if it's translatable
	QByteArray utf8 = realLine.toUtf8();
	quint32 crc = crc32(0, utf8.constData(), utf8.size());
	QString crcString = QString::number(crc);

	if(translation.contains(crcString))
	{
		QVariant value = translation.value(crcString);

		if(!value.isNull())
		{
			//qDebug() << "TL:" << lastPathComponent << "::" << key << "=" << realLine << "->" << value.toString();
			return value.toString();
		}
		else
		{
			//qDebug() << "UnTL'd:" << lastPathComponent << "::" << key <<"=" << realLine;
			return line;
		}
	}
	else
	{
		//qDebug() << "No TL:" << lastPathComponent << "::" << key << "=" << realLine;

		// The !reportBlacklist.isEmpty() part is to prevent failures to load the blacklist
		// from spamming the translation servers (not that they should ever happen)
		if(reportUntranslated && !lastPathComponent.isEmpty())
		{
			qDebug() << "Reporting untranslated line" << lastPathComponent << "::" << key << "=" << realLine;
			report(realLine, lastPathComponent, key);
		}

		translation.insert(crcString, QVariant::Invalid);

		return line;
	}
}

QByteArray KVTranslator::translateJson(QByteArray json, QString lastPathComponent)
{
	// Skip BOM if present
	if(json.length() >= 3 && (unsigned char)json.at(0) == 0xEF && (unsigned char)json.at(1) == 0xBB && (unsigned char)json.at(2) == 0xBF)
		json = json.mid(3);

	bool hasPrefix = json.startsWith("svdata=");

	QJsonDocument doc = QJsonDocument::fromJson(json.mid(hasPrefix ? 7 : 0));
	QJsonValue val = this->_walk(QJsonValue(doc.object()), lastPathComponent);
	//qDebug() << val;
	doc = QJsonDocument(val.toObject());

#if QT_VERSION >= 0x050100
	QByteArray ret = doc.toJson(QJsonDocument::Compact);
#else
	QByteArray ret = doc.toJson();
#endif
	return (hasPrefix ? "svdata=" + ret : ret);

	/*Reader r(json);
	QStack<JsonState> state;
	state.reserve(5);
	state.push(JsonState(End, 0));
	state.push(JsonState(Start, 0));

	QByteArray ret;
	ret.reserve(json.size()*2);
	for(int i = 0; i < json.size(); i++) {
		if(state.top().context != Key && state.top().context != String
		   && isspace(json.at(i))) continue;

		switch(state.top().context) {
		case Start:
			if(json.at(i) == '{')
				state.top() = JsonState(Object, i);
			continue;
		case End:
			qDebug() << "Unmatched '[' or '{' in JSON";
			break;
		case Object:
			switch(json.at(i)) {
			case '"':
				state.push(JsonState(Key, i));
				continue;
			case '}':
				state.top() = JsonState(AfterValue, i);
				continue;
			default:
				qDebug() << "Unexpected character" << json.at(i) << "in object";
				break;
			}
			break;
		case Array:
			switch(json.at(i)) {
			case '"':
				ret.append(r.readTo(i+1));
				state.push(JsonState(String, i));
				break;
			case '{':
				state.push(JsonState(Object, i));
				break;
			case '[':
				state.push(JsonState(Array, i));
				break;
			case ']':
				state.top() = JsonState(AfterValue, i);
				break;
			case ',':
				break;
			default:
				state.push(JsonState(NonString, i));
				break;
			}
			continue;
		case Key:
			switch(json.at(i)) {
			case '\\':
				i++;
				break;
			case '"':
				state.top() = JsonState(AfterKey, i);
				break;
			}
			continue;
		case AfterKey:
			if(json.at(i) == ':') {
				state.top() = JsonState(Value, i);
				continue;
			}
			qDebug() << "Character after key not ':'";
			break;
		case Value:
			switch(json.at(i)) {
			case '"':
				ret.append(r.readTo(i+1));
				state.top() = JsonState(String, i);
				continue;
			case '{':
				state.top() = JsonState(Object, i);
				continue;
			case '[':
				state.top() = JsonState(Array, i);
				continue;
			case ',':
				qDebug() << "Empty value";
				break;
			default:
				state.top() = JsonState(NonString, i);
				continue;
			}
			break;
		case NonString:
			switch(json.at(i)) {
			case ']':
			case '}':
				i--;
			case ',':
				state.pop();
				break;
			}
			continue;
		case String:
			switch(json.at(i)) {
			case '\\':
				i++;
				break;
			case '"':
				ret.append(translate(r.readTo(i)));
				state.top() = JsonState(AfterValue, i);
				break;
			}
			continue;
		case AfterValue:
			switch(json.at(i)) {
			case ']':
			case '}':
				i--;
			case ',':
				state.pop();
				continue;
			}
			qDebug() << "No ',', ']', or '}' after string";
			break;
		case Invalid:
			qDebug() << "Invalid state!";
			break;
		}

		qDebug() << "Failed to parse json at index" << i;
		int ds = state.isEmpty() ? i-50 : state.top().start;
		qDebug() << "Context:" << json.mid(ds, i+50-ds);
		qDebug() << "Near:" << json.mid(i-5, 10);
		//qDebug() << QByteArray(i-ds-10, ' ').constData() << "^";
		qDebug() << "State:" << state << "\n";
		return json;
	}

	if(state.top().context == AfterValue) state.pop();
	if(state.top().context != End) {
		qDebug() << "Unexpected end of input reached while parsing JSON";
		return json;
	}

	ret.append(r.readAll());
	return ret;*/
}

QString KVTranslator::fixTime(const QString &time) {
	QDateTime realTime = QDateTime::fromString(time, "yyyy-MM-dd hh:mm:ss");
	if(!realTime.isValid()) return time;
	realTime = realTime.addSecs(-32400).toLocalTime();
	//qDebug() << "fix time" << time << "to" << realTime.toString("yyyy-MM-dd hh:mm:ss");
	return realTime.toString("yyyy-MM-dd hh:mm:ss");
}

QJsonValue KVTranslator::_walk(QJsonValue value, QString lastPathComponent, QString key) {
	switch(value.type()) {
		case QJsonValue::Object:
		{
			QJsonObject obj = value.toObject();
			for(QJsonObject::iterator it = obj.begin(); it != obj.end(); it++)
				*it = this->_walk(*it, lastPathComponent, it.key());
			return obj;
		}
		case QJsonValue::Array:
		{
			QJsonArray arr = value.toArray();
			for(QJsonArray::iterator it = arr.begin(); it != arr.end(); it++)
				*it = this->_walk(*it, lastPathComponent, key);
			return arr;
		}
		case QJsonValue::String:
			if(key == "api_complete_time_str")
				return this->fixTime(value.toString());
			return this->translate(value.toString(), lastPathComponent, key);
		default:
			return value;
	}
}

void KVTranslator::report(const QString &line, const QString &lastPathComponent, const QString &key)
{
	if(blacklistState == loading)
		reportQueue.append({line, lastPathComponent, key});
	else if(blacklistState == loaded)
	{
		QNetworkRequest req(QString("http://api.comeonandsl.am/report/%1").arg(lastPathComponent));
		req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
		QByteArray body = QString("value=%1").arg(line).toUtf8();
		manager.post(req, body);
	}
}
