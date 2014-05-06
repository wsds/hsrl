#ifndef IMYMAP_H_
#define IMYMAP_H_
#include <map>

#include <string>


#ifndef NULL
#define NULL 0;
#endif /* NULL */

template <typename KEYTYPE, typename VALUETYPE>
class MyMap
{
public:
	MyMap(){
		this->map01 = new std::map<KEYTYPE, VALUETYPE>();
	};

	~MyMap(){
	};

	std::map<KEYTYPE, VALUETYPE>* map01;

	void put(const KEYTYPE key, VALUETYPE value){
		(*this->map01)[key] = value;
	};

	bool exist(const KEYTYPE key){
		if (this->map01->end() != this->map01->find(key))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	VALUETYPE* get(const KEYTYPE key){
		if (this->exist(key)){
			VALUETYPE& value = (*this->map01)[key];
			return &value;
		}
		else{
			return (VALUETYPE*)NULL;
		}

	};

	VALUETYPE*  operator[](const KEYTYPE key){
		return this->get(key);
	}

	bool del(const KEYTYPE key){
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

