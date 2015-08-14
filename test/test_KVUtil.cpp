#include "test_common.h"
#include <KVUtil.h>
#include <QMap>
#include <QList>

SCENARIO("extract() works")
{
	GIVEN("A map")
	{
		QMap<QString,QVariant> map {
			{ "a", "abc123" },
			{ "b", QMap<QString,QVariant> {
				{ "q", "qwerty" },
			}},
			{ "c", QList<QVariant> {
				"a", "b", "c",
			}},
		};
		
		THEN("Extracting root-level items should work")
		{
			REQUIRE(extract(map, "a").toString() == "abc123");
		}
		
		THEN("Extracting nested map keys should work")
		{
			REQUIRE(extract(map, "b.q").toString() == "qwerty");
		}
		
		THEN("Extracting array items should work")
		{
			CHECK(extract(map, "c.0").toString() == "a");
			CHECK(extract(map, "c.1").toString() == "b");
			CHECK(extract(map, "c.2").toString() == "c");
			CHECK(extract(map, "c.3").toString() == "");
		}
	}
}
