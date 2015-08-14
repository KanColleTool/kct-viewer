#include <catch.hpp>
#include <QString>
#include <QUrl>

/// Allow printing of QStrings to std::cout
inline std::ostream& operator<<(std::ostream& os, const QString& string) {
    return os << string.toStdString();
}

// Allow printing of QByteArrays to std::cout
inline std::ostream& operator<<(std::ostream& os, const QByteArray& data) {
	return os << data.toStdString();
}

// Allow printing of QUrls to std::cout
inline std::ostream& operator<<(std::ostream& os, const QUrl& url) {
	return os << url.toString();
}
