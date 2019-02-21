#pragma once
#include<assert.h>
#include<stdlib.h>

typedef int Key;

typedef enum {
	EMPTY,
	EXIST,
	DELETED
}State;

typedef int(*HashFuncType)(Key key, int capacity);

typedef struct Element {
	Key key;
	State state;
}Element;

typedef struct HashTable
{
	Element *table;
	int size;
	int capacity;
	HashFuncType hashFunc;        //哈希函数
}HashTable;
//初始化
void HashInit(HashTable *pHT, int capacity, HashFuncType hashFunc)
{
	pHT->table = (Element*)malloc(sizeof(Element)*capacity);
	assert(pHT->table);
	pHT->size = 0;
	pHT->capacity = capacity;
	pHT->hashFunc = hashFunc;

	for (int i = 0; i < capacity; i++)
	{
		pHT->table[i].state = EMPTY;
	}
}

//销毁
void HashDestory(HashTable *pHT)
{
	free(pHT->table);
}

//查找
int HashSearch(HashTable *pHT, Key key)
{
	int index = pHT->hashFunc(key, pHT->capacity);
	while (pHT->table[index].state != EMPTY)
	{
		if (pHT->table[index].key == key 
			&& pHT->table[index].state == EXIST)
		{
			return index;
		}
		else
		{
			index = (index + 1) % pHT->capacity;
		}
	}
	return -1;
}
//插入
int HashInsert(HashTable *pHT, Key key);

void ExpandIfRequired(HashTable *pHT)
{
	if (10 * pHT->size / pHT->capacity < 7)
		return;
	//int newCapacity = pHT->capacity * 2;
	//Element *newTable = (Element*)malloc(sizeof(Element)*newCapacity);
	//assert(newTable);

	////newTable的状态更新为EMPTY
	//for (int i = 0; i < newCapacity; i++)
	//{
	//	newTable[i].state = EMPTY;
	//}
	////老数据迁移

	//free(pHT->table);
	//pHT->table = newTable;
	//pHT->capacity = newCapacity;

	HashTable newHT;
	HashInit(&newHT, pHT->capacity * 2, pHT->hashFunc);
	for (int i = 0; i < pHT->capacity; i++)
	{
		if (pHT->table[i].state == EXIST)
		{
			HashInsert(&newHT,pHT->table[i].key);
		}
	}
	free(pHT->table);
	pHT->table = newHT.table;
	pHT->capacity = newHT.capacity;
}

int HashInsert(HashTable *pHT, Key key)
{
	ExpandIfRequired(pHT);

	int index = pHT->hashFunc(key, pHT->capacity);
	while (1)
	{
		if (pHT->table[index].key == key && pHT->table[index].state == EXIST)
			return -1;
		if (pHT->table[index].state != EXIST)
		{
			pHT->table[index].key = key;
			pHT->table[index].state = EXIST;
			pHT->size++;
			return 0;
		}
		index = (index + 1) % pHT->capacity;
	}
}

int HashRemove(HashTable *pHT, Key key)
{
	int index = pHT->hashFunc(key, pHT->capacity);
	while (pHT->table[index].state != EMPTY)
	{
		if (pHT->table[index].key == key
			&& pHT->table[index].state == EXIST)
		{
			pHT->table[index].state = DELETED;
			return 0;
		}
		else
		{
			index = (index + 1) % pHT->capacity;
		}
	}
	return -1;
}

int mod(Key key, int capacity)
{
	return key % capacity;
}
