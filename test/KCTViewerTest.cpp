#include "KCTViewerTest.h"

void KCTViewerTest::initTestCase()
{
	translator.translation.insert(QString("124853853"), "Naka");		// 那珂 (Naka)
	translator.translation.insert(QString("3440185848"), QVariant());	// まるゆ (Maruyu)
	translator.state = KVTranslator::loaded;
}

void KCTViewerTest::testTranslationTranslated()
{
	// Try translating a translated string
	QCOMPARE(translator.translate("那珂"), QString("Naka"));
	QCOMPARE(translator.translate("\\u90A3\\u73C2"), QString("Naka"));
}

void KCTViewerTest::testTranslationUntranslated()
{	
	// Try translating an untranslated string
	QCOMPARE(translator.translate("まるゆ"), QString("まるゆ"));
	QCOMPARE(translator.translate("\\u307E\\u308B\\u3086"), QString("\\u307E\\u308B\\u3086"));
}

void KCTViewerTest::testTranslationUnknown()
{
	// Try translating an unknown string
	QCOMPARE(translator.translate("テスト"), QString("テスト"));
	QCOMPARE(translator.translate("\\u307E\\u308B\\u3086"), QString("\\u307E\\u308B\\u3086"));
}

QTEST_MAIN(KCTViewerTest)
