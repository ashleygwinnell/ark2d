#ifndef JSONCHILDREN_H
#define JSONCHILDREN_H


#include <cstdlib> //for malloc, realloc, and free
#include <cstring> //for memmove

#include "Debug.h"  //for libJSON_ASSERT macro

#ifdef DEBUG
    #ifndef  ARK2D_WINDOWS
    	extern ErrorCallback Error;  //tell the user program that something went wrong
    #endif
#endif

/*
    This class is essentially a vector that has been heavily optimized for the specific purpose
    of holding JSONNode children.  It acts the same way as a vector, it has a automatically
    expanding array.  On destruction, this container automatically destroys everything contained
    in it as well, so that you libJSON doesn't have to do that.

    T is JSONNode*, I can't define it that way directly because JSONNode uses this container, and because
    the container deletes the children automatically, forward declaration can't be used
*/

class JSONNode;  //forward declaration

class jsonChildren {
    public:
        //starts completely empty and the array is not allocated
        jsonChildren(void){
            mysize = mycapacity = 0;
            array = 0;
        }

        //deletes the array and everything that is contained within it (using delete)
        ~jsonChildren(void){
            if (array){  //the following function calls are safe, but take more time than a check here
                deleteAll();
                free(array);
            }
        }

        //Adds something to the vector, doubling the array if necessary
        void push_back(JSONNode * item){
            if (mysize == mycapacity){  //it's full
                if (!mycapacity){  //the array hasn't been created yet
                    libJSON_ASSERT(!array, TEXT("Expanding a 0 capacity array, but not null"));
                    array = (JSONNode **)malloc(8 * sizeof(JSONNode *));  //8 seems average for JSON, and it's only 64 bytes
                    libJSON_ASSERT(array, TEXT("mallocing array failed"));
                    mycapacity = 8;
                } else {
                    mycapacity <<= 1;  //double the size of the array
                    array = (JSONNode **)realloc(array, mycapacity * sizeof(JSONNode *));
                    libJSON_ASSERT(array, TEXT("reallocing array failed"));
                }
            }
            array[mysize++] = item;
        }

        //gets an item out of the vector by it's position
        inline JSONNode * operator[] (unsigned int position) const {
            libJSON_ASSERT(position < mysize, TEXT("Using [] out of bounds"));
            libJSON_ASSERT(position < mycapacity, TEXT("Using [] out of bounds"));
            libJSON_ASSERT(array, TEXT("Array is null"));
            return array[position];
        }

        //returns the allocated capacity, but keep in mind that some might not be valid
        inline unsigned int capacity() const {
            return mycapacity;
        }

        //returns the number of valid objects within the vector
        inline unsigned int size() const {
            return mysize;
        }

        //tests whether or not the vector is empty
        inline bool empty() const {
            return mysize == 0;
        }

	   //tests whether or not the vector is empty
        inline bool not_empty() const {
            return mysize != 0;
        }

        //clears (and deletes) everything from the vector and sets it's size to 0
        inline void clear(){
            if (array){  //don't bother clearing anything if there is nothing in it
                libJSON_ASSERT(mycapacity == 0, TEXT("mycapacity is not zero, but array is null"));
                deleteAll();
                mysize = 0;
            }
            libJSON_ASSERT(mysize == 0, TEXT("mysize is not zero after clear"));
        }

        //returns the beginning of the array
        inline JSONNode * * begin(void) const {
            return array;
        }

        //returns the end of the array
        inline JSONNode * * end(void) const {
            return array + mysize;
        }

        //This function DOES NOT delete the item it points to
        inline void erase(JSONNode * * position){
            libJSON_ASSERT(array, TEXT("erasing something from a null array"));
            libJSON_ASSERT(position >= array, TEXT("position is beneat the start of the array"));
            libJSON_ASSERT(position < array + mysize, TEXT("erasing out of bounds"));
            memmove(position, position + 1, (mysize-- - (position - array) - 1) * sizeof(JSONNode *));
        }

        inline void reserve(unsigned int amount){
            libJSON_ASSERT(!array, TEXT("reserve is not meant to expand a preexisting array"));
            libJSON_ASSERT(!mycapacity, TEXT("reservec is not meant to expand a preexisting array"));
            libJSON_ASSERT(!mysize, TEXT("reserves is not meant to expand a preexisting array"));
            array = (JSONNode **)malloc(amount * sizeof(JSONNode *));  //8 seems average for JSON, and it's only 64 bytes
            libJSON_ASSERT(array, TEXT("mallocing array failed"));
            mycapacity = amount;
        }

        //shrinks the array to only as large as it needs to be to hold everything within it
        inline void shrink(){
            #ifdef JSON_LESS_MEMORY  //if memory doesn't matter to the user, go for speed, leave it the way it is
                if (mysize == 0){  //size is zero, we should completely free the array
                    free(array);  //free does checks for a null pointer, so don't bother checking
                    array = 0;
                } else {  //need to shrink it, using realloc
                    libJSON_ASSERT(array, TEXT("shrinking a null array that is not size 0"));
                    array = (JSONNode **)realloc(array, mysize);
                }
                mycapacity = mysize;
            #endif
        }

        #ifdef DEBUG
            //returns how much memory the container is currently using (minus the size of itself, which is already accounted for
            unsigned int GetMemoryUsage(void) const {
                return mycapacity * sizeof(JSONNode *);
            }
        #endif
    private:
        void deleteAll(void);  //implemented in JSONNode.cpp

        JSONNode * * array;  //the expandable array
        #ifdef JSON_LESS_MEMORY
            //maximum of 1 million children
            unsigned int mysize : 20;	    //the number of valid items
            unsigned int mycapacity : 20;   //the number of possible items
        #else
            unsigned int mysize;        //the number of valid items
            unsigned int mycapacity;    //the number of possible items
        #endif
};


#endif
