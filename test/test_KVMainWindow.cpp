#include <catch.hpp>
#include <KVMainWindow.h>

SCENARIO("API Links can be handled")
{
	KVMainWindow win;
	
	GIVEN("A set of credentials")
	{
		QString server = "192.168.0.1";
		QString apiToken = "abc123";
		win.setCredentials(server, apiToken);
		
		THEN("The credentials should be stored")
		{
			REQUIRE(win.server == server);
			REQUIRE(win.apiToken == apiToken);
		}
		
		THEN("The API link should be correct")
		{
			REQUIRE(win.apiLink() == QUrl("http://192.168.0.1/kcs/mainD2.swf?api_token=abc123"));
		}
	}
	
	GIVEN("An API link")
	{
		win.setApiLink(QUrl("http://192.168.0.1/kcs/mainD2.swf?api_token=abc123"));
		
		THEN("It should be correctly parsed")
		{
			REQUIRE(win.server == "192.168.0.1");
			REQUIRE(win.apiToken == "abc123");
		}
	}
	
	GIVEN("An invalid API link")
	{
		win.setApiLink(QUrl("http://google.com/"));
		
		THEN("It should still make a valiant attempt")
		{
			REQUIRE(win.server == "google.com");
			REQUIRE(win.apiToken == "");
		}
	}
	
	GIVEN("Gibberish that isn't even a URL")
	{
		win.setApiLink(QUrl("sdafghjh"));
		
		THEN("It should just give up")
		{
			REQUIRE(win.server == "");
			REQUIRE(win.apiToken == "");
		}
	}
}
