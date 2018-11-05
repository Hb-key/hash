#include"HashTable.h"
void Test()
{
	HashTable ht;
	HashInit(&ht, 10, mod);

	HashDestory(&ht);
}

int main()
{
	Test();
}