#include "test_common.h"
#include <KVTranslator.h>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

SCENARIO("Strings can be translated")
{
	KVTranslator tl;
	
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
			REQUIRE(tl.translate("\\u307E\\u308B\\u3086") == QString("\\u307E\\u308B\\u3086"));
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
		}
	}
}

SCENARIO("JSON Documents can be translated")
{
	KVTranslator tl;
	tl.addTranslation("那珂", "Naka");
	tl.addTranslation("まるゆ", "Maruyu");
	
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
			QJsonDocument tldoc = tl.translate(doc);
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
			QJsonDocument tldoc = tl.translate(doc);
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
		}
	}
	
	GIVEN("A nested object")
	{
		QJsonDocument doc = QJsonDocument::fromJson("{\"a\": {\"b\": \"那珂\", \"c\": {\"d\": \"まるゆ\"}}}");
		QJsonObject obj = doc.object();
		
		THEN("The initial state should be correct")
		{
			REQUIRE(obj["a"].toObject()["b"].toString() == "那珂");
			REQUIRE(obj["a"].toObject()["c"].toObject()["d"] == "まるゆ");
		}
		
		WHEN("It's translated")
		{
			QJsonDocument tldoc = tl.translate(doc);
			QJsonObject tlobj = tldoc.object();
			
			THEN("The original should be untouched")
			{
				REQUIRE(obj["a"].toObject()["b"].toString() == "那珂");
				REQUIRE(obj["a"].toObject()["c"].toObject()["d"] == "まるゆ");
			}
			
			THEN("The result should be correct")
			{
				REQUIRE(tlobj["a"].toObject()["b"].toString() == "Naka");
				REQUIRE(tlobj["a"].toObject()["c"].toObject()["d"] == "Maruyu");
			}
		}
	}
}
