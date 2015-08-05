#include <catch.hpp>
#include <KVTranslator.h>

SCENARIO("Strings can be translated")
{
	KVTranslator tl;
	
	GIVEN("Data for Naka, none for Maruyu")
	{
		tl.translation.insert(QString("124853853"), "Naka");		// 那珂 (Naka)
		tl.translation.insert(QString("3440185848"), QVariant());	// まるゆ (Maruyu)
		tl.state = KVTranslator::loaded;
		
		THEN("isLoaded() should be true")
		{
			REQUIRE(tl.isLoaded());
		}
		
		THEN("Naka's name should be translated")
		{
			REQUIRE(tl.translate("那珂") == QString("Naka"));
			REQUIRE(tl.translate("\\u90A3\\u73C2") == QString("Naka"));
		}
		
		THEN("Maruyu's name should be untouched")
		{
			REQUIRE(tl.translate("まるゆ") == QString("まるゆ"));
			REQUIRE(tl.translate("\\u307E\\u308B\\u3086") == QString("\\u307E\\u308B\\u3086"));
		}
		
		THEN("Unknwn strings should be untouched")
		{
			REQUIRE(tl.translate("テスト") == QString("テスト"));
			REQUIRE(tl.translate("\\u307E\\u308B\\u3086") == QString("\\u307E\\u308B\\u3086"));
		}
	}
}
