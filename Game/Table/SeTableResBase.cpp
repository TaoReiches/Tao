#include "SeTableResBase.h"

TableResArray::TableResArray()
{

}

TableResArray::~TableResArray()
{

}

void TableResArray::pushBack(SeTableResBase* object)
{
	m_kObjects.push_back(object);
}

void TableResArray::insert(SeTableResBase* object, int index)
{
	m_kObjects.insert(m_kObjects.begin() + index, object);
}

SeTableResBase* TableResArray::getObjectAt(int index)
{
	return this->m_kObjects[index];
}

int TableResArray::getSize()
{
	return (int)m_kObjects.size();
}