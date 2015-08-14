#include "KVJsonStore.h"
#include "KVUtil.h"
#include <algorithm>

KVJsonStore::KVJsonStore()
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

QList<QString> KVJsonStore::sortKeys() const { return m_sortKeys; }
void KVJsonStore::setSortKeys(QList<QString> v)
{
	m_sortKeys = v;
	this->sortData();
	emit sortKeysChanged(v);
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
	this->sortData();
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

void KVJsonStore::sortData()
{
	qSort(m_data.begin(), m_data.end(), [&](const QVariant &a, const QVariant &b) -> bool {
		for (const QString &key : m_sortKeys) {
			QVariant keyA = extract(a, key);
			QVariant keyB = extract(b, key);
			
			if (keyA != keyB) {
				return keyA < keyB;
			}
		}
		
		return false;
	});
}
