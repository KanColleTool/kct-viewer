#include "test_common.h"
#include <KVNetworkReply.h>

SCENARIO("Data behaves as expected")
{
	KVNetworkAccessManager manager;
	
	QNetworkRequest req(QUrl("http://google.com/"));
	KVNetworkReply reply(&manager, manager.get(req));
	
	GIVEN("Some make-believe response data")
	{
		QByteArray data = QString("lorem ipsum dolor sit amet").toUtf8();
		reply.setData(data);
		
		THEN("Content-Length should be correct")
		{
			REQUIRE(reply.header(QNetworkRequest::ContentLengthHeader).toLongLong() == data.size());
			REQUIRE(reply.rawHeader("Content-Length") == QString::number(data.size()).toUtf8());
		}
		
		THEN("The full length should be available")
		{
			REQUIRE(reply.bytesAvailable() == data.size());
		}
		
		THEN("Reading it should get it all back")
		{
			REQUIRE(reply.readAll() == data);
		}
		
		THEN("Partial reading should work as expected")
		{
			REQUIRE(reply.read(5) == data.left(5));
			REQUIRE(reply.read(5) == data.mid(5, 5));
			REQUIRE(reply.readAll() == data.mid(5 + 5));
			REQUIRE(reply.readAll() == QByteArray());
		}
	}
	
	// Ensure we don't accidentally send the request at any point
	reply.abort();
}
