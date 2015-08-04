#include "KVUtil.h"

#include <QRegularExpression>
#include <QChar>
#include <QDebug>

//
// Taken from http://stackoverflow.com/a/7047000
//
QString unescape(QString str) {
	QRegularExpression re("\\\\u([0-9a-fA-F]{4})");
	for(auto match = re.match(str); match.hasMatch(); match = re.match(str)) {
		str.replace(match.capturedStart(), match.capturedLength(),
		            QChar(match.captured(1).toUShort(0, 16)));
	}

	return str;
}

quint32 crc32(quint32 crc, const void *buf, size_t size)
{
	const quint8 *p = (quint8*)buf;
	crc = crc ^ ~0U;

	while (size--)
		crc = crc32_tab[(crc ^ *p++) & 0xFF] ^ (crc >> 8);

	return crc ^ ~0U;
}
