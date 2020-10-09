#pragma once
#include <vector>

class SeTableResBase
{
public:
	SeTableResBase(){}
};

class TableResArray
{
public:
	TableResArray();
	~TableResArray();

	void pushBack(SeTableResBase* object);
	void insert(SeTableResBase* object, int index);
	SeTableResBase* getObjectAt(int index);

	int getSize();

private:
	std::vector<SeTableResBase*> m_kObjects;
};