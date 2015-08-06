#include "test_common.h"
#include <KVGameWrapper.h>

SCENARIO("API Links can be handled")
{
	KVGameWrapper game;
	
	GIVEN("A set of credentials")
	{
		game.setServer("192.168.0.1");
		game.setApiToken("abc123");
		
		THEN("The credentials should be stored")
		{
			REQUIRE(game.server() == "192.168.0.1");
			REQUIRE(game.apiToken() == "abc123");
		}
		
		THEN("The API link should be correct")
		{
			REQUIRE(game.apiLink() == QUrl("http://192.168.0.1/kcs/mainD2.swf?api_token=abc123"));
		}
	}
	
	GIVEN("An API link")
	{
		game.setApiLink(QUrl("http://192.168.0.1/kcs/mainD2.swf?api_token=abc123"));
		
		THEN("It should be correctly parsed")
		{
			REQUIRE(game.server() == "192.168.0.1");
			REQUIRE(game.apiToken() == "abc123");
		}
	}
	
	GIVEN("An invalid API link")
	{
		game.setApiLink(QUrl("http://google.com/"));
		
		THEN("It should still make a valiant attempt")
		{
			REQUIRE(game.server() == "google.com");
			REQUIRE(game.apiToken() == "");
		}
	}
	
	GIVEN("Gibberish that isn't even a URL")
	{
		game.setApiLink(QUrl("sdafghjh"));
		
		THEN("It should just give up")
		{
			REQUIRE(game.server() == "");
			REQUIRE(game.apiToken() == "");
		}
	}
}
