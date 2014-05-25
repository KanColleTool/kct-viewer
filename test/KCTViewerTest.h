#ifndef KCTVIEWERTEST_H
#define KCTVIEWERTEST_H

#include <QtTest/QtTest>
#include "../src/KVTranslator.h"

class KCTViewerTest : public QObject
{
	Q_OBJECT
	
private slots:
	void initTestCase();

	void testTranslationTranslated();
	void testTranslationUntranslated();
	void testTranslationUnknown();
	
protected:
	KVTranslator translator;
};

#endif
