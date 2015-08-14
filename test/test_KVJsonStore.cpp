#include "test_common.h"
#include <KVJsonStore.h>

SCENARIO("Items can be inserted")
{
	KVJsonStore store;
	
	WHEN("An item is inserted")
	{
		store.add(QMap<QString,QVariant> { {"a", 1}, {"b", 2} });
		
		THEN("It should be added to the data")
		{
			REQUIRE(store.data().length() == 1);
		}
	}
	
	GIVEN("An ID key")
	{
		store.setIdKey("id");
		
		WHEN("An item is inserted")
		{
			store.add(QMap<QString,QVariant> { { "id", "1" }, { "data", "abc123" } });
			
			THEN("Its ID should be remembered")
			{
				REQUIRE(store.ids().contains("1"));
			}
			
			WHEN("A duplicate insert is attempted")
			{
				store.add(QMap<QString,QVariant> { { "id", "1" }, { "data", "wrong" } });
				
				THEN("It should be ignored")
				{
					REQUIRE(store.data().length() == 1);
					REQUIRE(store.data()[0].toMap()["data"].toString() == "abc123");
				}
			}
		}
	}
}
