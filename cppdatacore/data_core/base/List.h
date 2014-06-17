#ifndef LIST_H
#define LIST_H


#include "JSObject.h"
#include "MemoryManagement.h"

#ifndef NULL
#define NULL 0;
#endif /* NULL */

class LIST
{
public:
    JSObject** elements;//~~~~~~~~~~~~~~~Memory Management~~~~~~~~~~~~~~~~~

    int length;

    int max_size;
    int threshold;


    bool is_initialized = false;

    //API
    //find O(1)
    JSObject* find(int index);
    //find O(n)
    int findValue(JSObject* object);

    //insert O(n)
    bool insert(JSObject* object, int index);


    //del O(n)
    bool del(int index);

    //pop O(1)
    JSObject* pop();

    //push O(1)
    bool push(JSObject* object);

    //resize O(n)
    bool resize();

    //initialize default size=8;
    bool initialize();

    bool free();


    void test()
    {
        //        int i = 1 + 1;
    }

};


#endif /* LIST_H */
