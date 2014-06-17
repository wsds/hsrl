#ifndef HASHTABLE_H
#define HASHTABLE_H


#include "JSObject.h"

#ifndef NULL
#define NULL 0;
#endif /* NULL */

class HashTable
{


    //find O(1)
    JSObject* find(char* key);

    //set O(1)
    bool set(char* key, JSObject* object);

    //del O(1)
    bool del(char* key);

    //resize O(n)
    bool resize();


    void test()
    {

        //        int i = 1 + 1;
    }

};


#endif /* HASHTABLE_H */

