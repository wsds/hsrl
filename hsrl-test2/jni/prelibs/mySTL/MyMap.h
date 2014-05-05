#ifndef IMYMAP_H_
#define IMYMAP_H_
#include <map>

#include <string>


#ifndef NULL
#define NULL 0;
#endif /* NULL */
using namespace std;

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

	//typedef std::map<std::string, std::string> map02;

	//VALUETYPE test_value;
	VALUETYPE& get(const KEYTYPE key){
		if (this->exist(key)){
			VALUETYPE& value = (*this->map01)[key];
			return value;
		}
		else{
			const int& null = NULL;
			return (VALUETYPE&)null;
		}

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

	VALUETYPE&  operator[](const KEYTYPE key){
		return this->get(key);
	}

	void put(const KEYTYPE key, VALUETYPE value){
		(*this->map01)[key] = value;
		//test_value = value;
	};
};


#endif /* IMYMAP_H_ */

