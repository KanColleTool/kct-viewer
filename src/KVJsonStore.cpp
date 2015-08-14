#include "KVJsonStore.h"
#include "KVUtil.h"
#include <QFile>
#include <QJsonDocument>

KVJsonStore::KVJsonStore(QString idKey):
	m_idKey(idKey)
{
	
}

KVJsonStore::~KVJsonStore()
{
	
}



QString KVJsonStore::idKey() const { return m_idKey; }
void KVJsonStore::setIdKey(QString v)
{
	m_idKey = v;
	this->rebuildIds();
	emit idKeyChanged(v);
}

const QSet<QString>& KVJsonStore::ids() const { return m_ids; }
const QList<QVariant>& KVJsonStore::data() const { return m_data; }



void KVJsonStore::add(const QVariant &v)
{
	if (m_ids.contains(extract(v, m_idKey).toString())) {
		return;
	}
	
	m_data.append(v);
	this->addIdFor(v);
}

QJsonArray KVJsonStore::toJson() const
{
	return QJsonArray::fromVariantList(m_data);
}

bool KVJsonStore::save(const QString &path) const
{
	QFile file(path);
	if (file.open(QIODevice::WriteOnly)) {
		file.write(QJsonDocument(this->toJson()).toJson());
		return true;
	}
	return false;
}



void KVJsonStore::rebuildIds()
{
	if (m_idKey.isEmpty()) {
		return;
	}
	
	m_ids.clear();
	for (const QVariant &v : m_data) {
		this->addIdFor(v);
	}
	
	emit idsChanged(m_ids);
}

void KVJsonStore::addIdFor(const QVariant &v)
{
	if (m_idKey.isEmpty()) {
		return;
	}
	
	QMap<QString,QVariant> map = v.toMap();
	if (map.contains(m_idKey)) {
		m_ids.insert(map.value(m_idKey).toString());
	}
}
