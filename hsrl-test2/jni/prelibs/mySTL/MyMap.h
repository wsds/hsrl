#ifndef IMYMAP_H_
#define IMYMAP_H_
#include <map>

#include <string>


#ifndef NULL
#define NULL 0;
#endif /* NULL */

template <typename K, typename V>
class MyMap
{
public:
	MyMap(){
		this->map01 = new std::map<K, V>();
	};

	~MyMap(){
	};

	std::map<K, V>* map01;

	void put(const K key, V value){
		(*this->map01)[key] = value;
	};

	bool exist(const K key){
		if (this->map01->end() != this->map01->find(key))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	V* get(const K key){
		if (this->exist(key)){
			V& value = (*this->map01)[key];
			return &value;
		}
		else{
			return (V*)NULL;
		}

	};

	V*  operator[](const K key){
		return this->get(key);
	}

	bool del(const K key){
		if (this->map01->end() != this->map01->find(key))
		{
			this->map01->erase(this->map01->find(key));
			return true;
		}
		else
		{
			return false;
		}


	};
};


#endif /* IMYMAP_H_ */

