#include "test_common.h"
#include <KVTranslator.h>

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
