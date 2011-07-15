/*
 * ParticleIO.h
 *
 *  Created on: 16 Apr 2011
 *      Author: Ashley
 */

#ifndef PARTICLEIO_H_
#define PARTICLEIO_H_

#include <string>
using namespace std;

class Color;
class ParticleSystem;
class ConfigurableEmitter;
class Value;
#include "Range.h"

#include "../vendor/tinyxml/tinyxml.h"

class ParticleIO {
	public:
		static ParticleSystem* loadConfiguredSystem(string ref);
		static ParticleSystem* loadConfiguredSystem(string ref, const Color& mask);

	private:
		static void elementToEmitter(ConfigurableEmitter* emitter, TiXmlElement* element);

		static void parseValueElement(TiXmlElement* element, Value* value);

		template <class T>
		static void parseRangeElement(TiXmlElement* element, Range<T>* range);
};

#endif /* PARTICLEIO_H_ */
