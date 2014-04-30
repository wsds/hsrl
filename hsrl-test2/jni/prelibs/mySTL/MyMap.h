#ifndef IMYMAP_H_
#define IMYMAP_H_
#include <map>

template <typename KEYTYPE, typename VALUETYPE>
class MyMap
{
public:
	MyMap(){
		this->map = new std::map<KEYTYPE, VALUETYPE>();
	};

	~MyMap(){
	};

	std::map<KEYTYPE, VALUETYPE>* map;

	VALUETYPE get(const KEYTYPE key){
	
		VALUETYPE value = (*this->map)[key];
		return value;
	
	};

	void put(const KEYTYPE key, VALUETYPE value){
		(*this->map)[key] = value;	
	};
};


#endif /* IMYMAP_H_ */

