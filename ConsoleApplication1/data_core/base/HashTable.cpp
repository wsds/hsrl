#include "HashTable.h"

JSObject* HashTable::get(char* key)
{
	unsigned int hash = dictGenHashFunction(key, strlen(key));
	int index = hash%max_size;
	if (elements[index] != NULL){
		HashEntry* brother = elements[index];
		do{
			if (strcmp(brother->key, key) == 0){
				return (JSObject*)brother->value;
			}
			brother = brother->next;
		} while (brother != NULL);
	}

	return NULL;
}

bool HashTable::set(char* key, JSObject* value)
{
	HashEntry* element = new HashEntry();

	element->key = key;
	element->value = (JSNumber *)value;
	element->hash = dictGenHashFunction(key, strlen(key));

	int index = element->hash%max_size;
	if (elements[index] == NULL){
		elements[index] = element;
	}

	else{
		int level = 1;
		HashEntry** brother = elements+index;
		do {
			if (strcmp((*brother)->key, key) == 0){

				element->next = (*brother)->next;
				element->level = level;
				(*brother) = element;
				//free the old element "brother", put it in a pool.
				break;
			}
			else if ((*brother)->next != NULL){
				level++;
				brother = &((*brother)->next);
			}
			else{
				(*brother)->next = element;
				element->level = level;
				break;
			}
		} while ((*brother) != NULL);

	}

	this->length++;
	if (this->length > this->threshold)
	{
		this->resize();//asynchronous
	}

	return true;
}

bool HashTable::del(char* key)
{
	unsigned int hash = dictGenHashFunction(key, strlen(key));
	int index = hash%max_size;
	if (elements[index] != NULL){
		HashEntry* brother = elements[index];
		do{
			if (strcmp(brother->key, key) == 0){

				//free the old element
				return true;
			}
			brother = brother->next;
		} while (brother != NULL);
	}


	return false;
}

bool HashTable::resize()
{
	return true;
}

bool HashTable::initialize()
{
	this->max_size = 31;
	this->length = 0;
	this->threshold = (int)(this->max_size * 0.8);

	int mem_size = this->max_size*sizeof(void*);

	this->elements = (HashEntry**)JSMalloc(mem_size);

	this->is_initialized = true;

	return true;


}


static unsigned int dict_hash_function_seed = 5381;
/* MurmurHash2, by Austin Appleby
* Note - This code makes a few assumptions about how your machine behaves -
* 1. We can read a 4-byte value from any address without crashing
* 2. sizeof(int) == 4
*
* And it has a few limitations -
*
* 1. It will not work incrementally.
* 2. It will not produce the same results on little-endian and big-endian
*    machines.
*/
unsigned int dictGenHashFunction(const void *key, int len) {
	/* 'm' and 'r' are mixing constants generated offline.
	They're not really 'magic', they just happen to work well.  */
	unsigned int seed = dict_hash_function_seed;
	const unsigned int m = 0x5bd1e995;
	const int r = 24;

	/* Initialize the hash to a 'random' value */
	unsigned int h = seed ^ len;

	/* Mix 4 bytes at a time into the hash */
	const unsigned char *data = (const unsigned char *)key;

	while (len >= 4) {
		unsigned int k = *(unsigned int*)data;

		k *= m;
		k ^= k >> r;
		k *= m;

		h *= m;
		h ^= k;

		data += 4;
		len -= 4;
	}

	/* Handle the last few bytes of the input array  */
	switch (len) {
	case 3: h ^= data[2] << 16;
	case 2: h ^= data[1] << 8;
	case 1: h ^= data[0]; h *= m;
	};

	/* Do a few final mixes of the hash to ensure the last few
	* bytes are well-incorporated. */
	h ^= h >> 13;
	h *= m;
	h ^= h >> 15;

	return (unsigned int)h;
}


int strcmp(char *src, char *dst)
{

	int ret = 0;

	while (!(ret = *src - *dst) && *dst)

		++src, ++dst;

	if (ret < 0)

		ret = -1;

	else if (ret > 0)

		ret = 1;

	return(ret);
}

int strlen(char *str)
{
	char *eos = str;

	while (*eos++);

	return(eos - str - 1);
}