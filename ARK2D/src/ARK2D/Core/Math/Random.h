/*
 * Random.h
 *
 *  Created on: 22 Nov 2015
 *      Author: Ashley
 */

#ifndef ARK_CORE_MATH_RANDOM_H_
#define ARK_CORE_MATH_RANDOM_H_

#include "../../Common/DLL.h"
#include <vector>
class MersenneTwister; 

namespace ARK {
    namespace Core {
        namespace Math {
            class ARK2D_API RandomImplementation {
                public:
                    RandomImplementation();
                    virtual void seed();
                    virtual void seed(unsigned long seed) = 0;
                    virtual int randBetween(int lower, int upper) = 0;
                    virtual float randBetweenf(float lower, float upper);
                    virtual ~RandomImplementation();
            };
            class ARK2D_API DefaultRandomImplementation : public RandomImplementation {
                public:
                    DefaultRandomImplementation();
                    virtual void seed(unsigned long seed);
                    virtual int randBetween(int lower, int upper);
                    virtual ~DefaultRandomImplementation();
            };
            class ARK2D_API MersenneTwisterRandomImplementation : public RandomImplementation {
                public:
                    MersenneTwister* m_generator;
                    MersenneTwisterRandomImplementation();
                    virtual void seed(unsigned long seed);
                    virtual int randBetween(int lower, int upper);
                    virtual ~MersenneTwisterRandomImplementation();
            };
            class ARK2D_API ParkMillerRandomImplementation : public RandomImplementation {
                public:
                    unsigned int m_seed;
                    ParkMillerRandomImplementation();
                    virtual void seed(unsigned long seed);
                    virtual int randBetween(int lower, int upper);
                    virtual ~ParkMillerRandomImplementation();
                private:
                    int num();

            };
            
            class ARK2D_API Random {
                public:
                    static const unsigned int GENERATOR_DEFAULT = 0;
                    static const unsigned int GENERATOR_MERSENNETWISTER = 1;
                    static const unsigned int GENERATOR_PARKMILLER = 2;

                public:

                    static std::vector<RandomImplementation*>* s_implementations;
                    static unsigned int s_implementation;

                    static void init();
                    static RandomImplementation* get();
                    static RandomImplementation* get(unsigned int i);
                    static void set(unsigned int i);
            };
        }
	}
}

#endif 