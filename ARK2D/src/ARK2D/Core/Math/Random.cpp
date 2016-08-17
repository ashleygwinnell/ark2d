/*
 * Random.cpp
 *
 *  Created on: 22 Nov 2015
 *      Author: Ashley
 */

#include "Random.h"

#include "../ARK2D.h"
#include "../GameContainer.h"
#include "../../vendor/mersennetwister/mt.h"

namespace ARK {
    namespace Core {
        namespace Math {


            // Blank C/C++ implementation
            RandomImplementation::RandomImplementation() {

            }
            void RandomImplementation::seed() {
                seed(ARK2D::getContainer()->getTimer()->millis());
            }
            float RandomImplementation::randBetweenf(float lower, float upper) {
                if (lower == upper) { return upper; }
                if (lower > upper) { float teacup = lower; lower = upper; upper = teacup; }

                float r = float(randBetween(0, 101)) / 100.0f;
                float diff = upper - lower;
                return lower + (r * diff);
            }
            RandomImplementation::~RandomImplementation() {

            }


            // std C/C++ implementation
            DefaultRandomImplementation::DefaultRandomImplementation():
                RandomImplementation() {

            }
            void DefaultRandomImplementation::seed(unsigned long seed) {
                ARK2D::getLog()->i("Setting DefaultRandomImplementation seed.");
                srand(seed);
            }
            int DefaultRandomImplementation::randBetween(int lower, int upper) {
                if (lower == upper) { return upper; }
                if (lower > upper) { int teacup = lower; lower = upper; upper = teacup; }
                return lower + (rand() % (upper-lower));
            }
            DefaultRandomImplementation::~DefaultRandomImplementation() {

            }


            // Mersenne Twister implementation.
            MersenneTwisterRandomImplementation::MersenneTwisterRandomImplementation():
                RandomImplementation(),
                m_generator(NULL) {
                    m_generator = new MersenneTwister();
            }
            void MersenneTwisterRandomImplementation::seed(unsigned long seed) {
                ARK2D::getLog()->i("Setting MersenneTwisterRandomImplementation seed.");
                m_generator->init_genrand(seed);
            }
            int MersenneTwisterRandomImplementation::randBetween(int lower, int upper) {
                if (lower == upper) { return upper; }
                if (lower > upper) { int teacup = lower; lower = upper; upper = teacup; }
                return lower + (m_generator->genrand_int32() % (upper-lower));
            }
            MersenneTwisterRandomImplementation::~MersenneTwisterRandomImplementation() {
                delete m_generator;
                m_generator = NULL;
            }


            // Lehmer implementation.
            ParkMillerRandomImplementation::ParkMillerRandomImplementation():
                RandomImplementation() {

            }
            void ParkMillerRandomImplementation::seed(unsigned long seed) {
                ARK2D::getLog()->i("Setting ParkMillerRandomImplementation seed.");
                m_seed = seed;
            }
            int ParkMillerRandomImplementation::randBetween(int lower, int upper) {
                if (lower == upper) { return upper; }
                if (lower > upper) { int teacup = lower; lower = upper; upper = teacup; }
                return lower + (num() % (upper-lower));
            }
            int ParkMillerRandomImplementation::num() {
                m_seed = ((uint64_t)m_seed * 48271UL) % 2147483647UL;
                return m_seed;
            }
            ParkMillerRandomImplementation::~ParkMillerRandomImplementation() {

            }



            // ---------------------------------------------------
            // Random
            // ---------------------------------------------------
            std::vector<RandomImplementation* >* Random::s_implementations = NULL;
            unsigned int Random::s_implementation = 0;

            void Random::init() {
                if (s_implementations == NULL) {
                    s_implementations = new std::vector<RandomImplementation*>();
                    s_implementations->push_back( new DefaultRandomImplementation() );
                    s_implementations->push_back( new MersenneTwisterRandomImplementation() );
                    s_implementations->push_back( new ParkMillerRandomImplementation() );

                    for(unsigned int i = 0; i < s_implementations->size(); ++i) {
                        s_implementations->at(i)->seed();
                    }
                }
            }
            void Random::set(unsigned int i) {
                s_implementation = i;
            }
            RandomImplementation* Random::get() {
                return get(s_implementation);
            }
            RandomImplementation* Random::get(unsigned int i) {
                return s_implementations->at(i);
            }
        }

	}
}
