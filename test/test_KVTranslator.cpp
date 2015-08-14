#include "test_common.h"
#include <KVTranslator.h>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

struct Untranslated {
	QString phrase;
	QString key;
	QUrl source;
	
	Untranslated(const QString &phrase, const QString &key, const QUrl &source):
		phrase(phrase), key(key), source(source) {}
};

SCENARIO("Strings can be translated")
{
	KVTranslator tl;
	QList<Untranslated> misses;
	
	QObject::connect(&tl, &KVTranslator::missingTranslation,
		[&](const QString &phrase, const QString &key, const QUrl &source) {
			misses.append({ phrase, key, source });
		}
	);
	
	GIVEN("A translated string")
	{
		tl.addTranslation("那珂", "Naka");
		
		THEN("Naka's name should be translated")
		{
			REQUIRE(tl.translate("那珂") == QString("Naka"));
			REQUIRE(tl.translate("\\u90A3\\u73C2") == QString("Naka"));
		}
		
		THEN("Unknwn strings should be untouched")
		{
			REQUIRE(tl.translate("テスト") == QString("テスト"));
			REQUIRE(tl.translate("\\u30C6\\u30B9\\u30C8") == QString("\\u30C6\\u30B9\\u30C8"));
			
			THEN("And they should be reported")
			{
				REQUIRE(misses.length() == 2);
				CHECK(misses[0].phrase == "テスト");
				CHECK(misses[1].phrase == "テスト");
			}
		}
	}
	
	GIVEN("An empty translation")
	{
		tl.addTranslation("まるゆ", "");
		
		THEN("It should be treated as unknown")
		{
			REQUIRE(tl.translate("まるゆ") == QString("まるゆ"));
			REQUIRE(tl.translate("\\u307E\\u308B\\u3086") == QString("\\u307E\\u308B\\u3086"));
		}
	}
	
	WHEN("Trying to translate something untranslatable")
	{
		tl.addTranslation("-", "fail");
		tl.addTranslation("123", "fail");
		tl.addTranslation("", "fail");
		
		THEN("Don't even try")
		{
			CHECK(tl.translate("-") == QString("-"));
			CHECK(tl.translate("123") == QString("123"));
			CHECK(tl.translate("") == QString(""));
			
			THEN("No misses should be reported either") {
				CHECK(misses.length() == 0);
			}
		}
	}
}

SCENARIO("JSON Documents can be translated")
{
	KVTranslator tl;
	QList<Untranslated> misses;
	
	QObject::connect(&tl, &KVTranslator::missingTranslation,
		[&](const QString &phrase, const QString &key, const QUrl &source) {
			misses.append({ phrase, key, source });
		}
	);
	
	tl.addTranslation("那珂", "Naka");
	tl.addTranslation("まるゆ", "Maruyu");
	
	QUrl url("http://google.com");
	
	GIVEN("An object")
	{
		QJsonDocument doc = QJsonDocument::fromJson("{\"a\": \"テスト\", \"b\": \"那珂\"}");
		QJsonObject obj = doc.object();
		
		THEN("The initial state should be correct")
		{
			REQUIRE(obj["a"].toString() == "テスト");
			REQUIRE(obj["b"].toString() == "那珂");
		}
		
		WHEN("It's translated")
		{
			QJsonDocument tldoc = tl.translate(doc, url);
			QJsonObject tlobj = tldoc.object();
			
			THEN("The original should be untouched")
			{
				REQUIRE(obj["a"].toString() == "テスト");
				REQUIRE(obj["b"].toString() == "那珂");
			}
			
			THEN("The result should be correct")
			{
				REQUIRE(tlobj["a"].toString() == "テスト");
				REQUIRE(tlobj["b"].toString() == "Naka");
			}
			
			THEN("The untranslated line should be reported")
			{
				REQUIRE(misses.length() == 1);
				CHECK(misses[0].phrase == "テスト");
				CHECK(misses[0].key == "a");
				CHECK(misses[0].source == url);
			}
		}
	}
	
	GIVEN("An array")
	{
		QJsonDocument doc = QJsonDocument::fromJson("[\"テスト\", \"那珂\"]");
		QJsonArray arr = doc.array();
		
		THEN("The initial state should be correct")
		{
			REQUIRE(arr[0].toString() == "テスト");
			REQUIRE(arr[1].toString() == "那珂");
		}
		
		WHEN("It's translated")
		{
			QJsonDocument tldoc = tl.translate(doc, url);
			QJsonArray tlarr = tldoc.array();
			
			THEN("The original should be untouched")
			{
				REQUIRE(arr[0].toString() == "テスト");
				REQUIRE(arr[1].toString() == "那珂");
			}
			
			THEN("The result should be correct")
			{
				REQUIRE(arr[0].toString() == "テスト");
				REQUIRE(arr[1].toString() == "那珂");
			}
			
			THEN("The untranslated line should be reported")
			{
				REQUIRE(misses.length() == 1);
				CHECK(misses[0].phrase == "テスト");
				CHECK(misses[0].key == "");
				CHECK(misses[0].source == url);
			}
		}
	}
	
	GIVEN("A nested object")
	{
		QJsonDocument doc = QJsonDocument::fromJson("{\"a\": {\"b\": \"那珂\", \"c\": {\"d\": \"まるゆ\", \"e\": \"テスト\"}}}");
		QJsonObject obj = doc.object();
		
		THEN("The initial state should be correct")
		{
			REQUIRE(obj["a"].toObject()["b"].toString() == "那珂");
			REQUIRE(obj["a"].toObject()["c"].toObject()["d"] == "まるゆ");
		}
		
		WHEN("It's translated")
		{
			QJsonDocument tldoc = tl.translate(doc, url);
			QJsonObject tlobj = tldoc.object();
			
			THEN("The original should be untouched")
			{
				REQUIRE(obj["a"].toObject()["b"].toString() == "那珂");
				REQUIRE(obj["a"].toObject()["c"].toObject()["d"] == "まるゆ");
				REQUIRE(obj["a"].toObject()["c"].toObject()["e"] == "テスト");
			}
			
			THEN("The result should be correct")
			{
				REQUIRE(tlobj["a"].toObject()["b"].toString() == "Naka");
				REQUIRE(tlobj["a"].toObject()["c"].toObject()["d"] == "Maruyu");
				REQUIRE(tlobj["a"].toObject()["c"].toObject()["e"] == "テスト");
			}
			
			THEN("The untranslated line should be reported")
			{
				REQUIRE(misses.length() == 1);
				CHECK(misses[0].phrase == "テスト");
				CHECK(misses[0].key == "e");
				CHECK(misses[0].source == url);
			}
		}
	}
}
