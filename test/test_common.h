#include <catch.hpp>
#include <QString>

/// Allow printing of QStrings to std::cout
inline std::ostream& operator<<(std::ostream& os, const QString& string) {
    return os << string.toStdString();
}
