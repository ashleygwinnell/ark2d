/*
 * ParticleIO.cpp
 *
 *  Created on: 16 Apr 2011
 *      Author: Ashley
 */

#include "ParticleIO.h"
#include "ParticleSystem.h"
#include "ConfigurableEmitter.h"
#include "Particle.h"
#include "ParticlePool.h"
#include "../Color.h"
#include "../UI/ErrorDialog.h"
#include "../Image/Image.h"
#include "../Util/StringUtil.h"



ParticleSystem* ParticleIO::loadConfiguredSystem(string ref) {
	return loadConfiguredSystem(ref, Color::magenta);
}
ParticleSystem* ParticleIO::loadConfiguredSystem(string ref, const Color& mask) {
	TiXmlDocument m_xmldocument(ref.c_str());
	bool loadOkay = m_xmldocument.LoadFile();
	if (!loadOkay) {
		ErrorDialog::createAndShow(StringUtil::append("Could not load Particle System: ", ref));
		exit(0);
	}

	ParticleSystem* sys = new ParticleSystem(new Image("data/particle.png", mask));

	TiXmlElement* rootelement = m_xmldocument.FirstChildElement("system");
	if (rootelement == NULL) {
		ErrorDialog::createAndShow(string("Invalid particle system file. Root node must be <system>."));
	}
	if (strcmp(rootelement->Attribute("additive"), "true") == 0) {
		sys->setBlendingMode(ParticleSystem::BLEND_ADDITIVE);
	} else {
		sys->setBlendingMode(ParticleSystem::BLEND_COMBINE);
	}

	const char* points = rootelement->Attribute("points");
	if (points != NULL) {
		sys->setUsePoints(strcmp(points, "true") == 0);
	}

	TiXmlElement* emitter = 0;
	for (emitter = rootelement->FirstChildElement("emitter");
			emitter;
			emitter = emitter->NextSiblingElement("emitter")) {
		ConfigurableEmitter* e = new ConfigurableEmitter("new");
		elementToEmitter(e, emitter);
		sys->addEmitter(e);
	}

	sys->setRemoveCompletedEmitters(false);
	return sys;
}

void ParticleIO::elementToEmitter(ConfigurableEmitter* emitter, TiXmlElement* element) {

	const char* name = element->Attribute("name");
	if (name != NULL) {
		emitter->setName(name);
	}

	const char* renderType = element->Attribute("renderType");
	emitter->setUsePoints(Particle::INHERIT_POINTS);
	if (renderType != NULL) {
		if (strcmp(renderType, "quads") == 0) {
			emitter->setUsePoints(Particle::USE_QUADS);
		} else if (strcmp(renderType, "points") == 0) {
			emitter->setUsePoints(Particle::USE_POINTS);
		}
	}

	const char* useOriented = element->Attribute("useOriented");
	if (useOriented != NULL) {
		emitter->setUseOriented(strcmp(useOriented, "true") == 0);
	}

	const char* useAdditive = element->Attribute("useAdditive");
	if (useAdditive != NULL) {
		emitter->setUseAdditive(strcmp(useAdditive, "true") == 0);
	}

	parseRangeElement<int>(element->FirstChildElement("spawnInterval"), &emitter->m_spawnInterval);
	parseRangeElement<int>(element->FirstChildElement("spawnCount"), &emitter->m_spawnCount);
	parseRangeElement<int>(element->FirstChildElement("initialLife"), &emitter->m_initialLife);
	parseRangeElement<int>(element->FirstChildElement("initialSize"), &emitter->m_initialSize);
	parseRangeElement<int>(element->FirstChildElement("xOffset"), &emitter->m_xOffset);
	parseRangeElement<int>(element->FirstChildElement("yOffset"), &emitter->m_yOffset);
	parseRangeElement<int>(element->FirstChildElement("initialDistance"), &emitter->m_initialDistance);
	parseRangeElement<int>(element->FirstChildElement("speed"), &emitter->m_speed);
	parseRangeElement<int>(element->FirstChildElement("length"), &emitter->m_length);

	TiXmlElement* emitCount = element->FirstChildElement("emitCount");
	if (emitCount != NULL) {
		parseRangeElement<int>(emitCount, &emitter->m_emitCount);
	}


	parseValueElement(element->FirstChildElement("spread"), &emitter->m_spread);
	parseValueElement(element->FirstChildElement("angularOffset"), &emitter->m_angularOffset);
	parseValueElement(element->FirstChildElement("growthFactor"), &emitter->m_growthFactor);
	parseValueElement(element->FirstChildElement("gravityFactor"), &emitter->m_gravityFactor);

	TiXmlElement* windFactor = element->FirstChildElement("windFactor");
	if (windFactor != NULL) {
		parseValueElement(windFactor, &emitter->m_windFactor);
	}

	parseValueElement(element->FirstChildElement("startAlpha"), &emitter->m_startAlpha);
	parseValueElement(element->FirstChildElement("endAlpha"), &emitter->m_endAlpha);

	TiXmlElement* alpha = element->FirstChildElement("alpha");
	if (alpha != NULL) {
		parseValueElement(alpha, emitter->m_alpha);
	}

	TiXmlElement* size = element->FirstChildElement("size");
	if (size != NULL) {
		parseValueElement(size, emitter->m_size);
	}

	TiXmlElement* velocity = element->FirstChildElement("velocity");
	if (velocity != NULL) {
		parseValueElement(velocity, emitter->m_velocity);
	}

	TiXmlElement* scaleY = element->FirstChildElement("scaleY");
	if (scaleY != NULL) {
		parseValueElement(scaleY, emitter->m_scaleY);
	}


	TiXmlElement* color = element->FirstChildElement("color");

	TiXmlElement* step = 0;
	for (step = color->FirstChildElement("step");
			step;
			step = step->NextSiblingElement("step")) {

		float flOffset = 1.0f;
		float flR = 1.0f;
		float flG = 1.0f;
		float flB = 1.0f;

		const char* offset = step->Attribute("offset");
		if (offset != NULL) {
			flOffset = Cast::fromString<float>(offset);
		}

		const char* r = step->Attribute("r");
		if (r != NULL) {
			flR = Cast::fromString<float>(r);
		}

		const char* g = step->Attribute("g");
		if (g != NULL) {
			flG = Cast::fromString<float>(g);
		}

		const char* b = step->Attribute("b");
		if (b != NULL) {
			flB = Cast::fromString<float>(b);
		}

		emitter->addColorPoint(flOffset, new Color(int(flR*255), int(flG*255), int(flB*255), 255));

	}

	emitter->replay();




}

void ParticleIO::parseValueElement(TiXmlElement* element, Value* value) {
	const char* type = element->Attribute("type");
	const char* v = element->Attribute("value");

	if (type == NULL || strlen(type) == 0) {
		// support for old style which did not write the type
		SimpleValue* simpleValue = NULL;
		RandomValue* randomValue = NULL;
		if ((simpleValue = dynamic_cast<SimpleValue*>(value)) != NULL) {
			simpleValue->setValue(Cast::fromString<float>(v));
		} else if ((randomValue = dynamic_cast<RandomValue*>(value)) != NULL) {
			randomValue->setValue(Cast::fromString<float>(v));
		} else {
			std::cout << "problems reading element, skipping..." << std::endl;
		}
	} else {
		if (strcmp(type, "simple") == 0) {
			(dynamic_cast<SimpleValue*>(value))->setValue(Cast::fromString<float>(v));
		} else if (strcmp(type, "random") == 0) {
			(dynamic_cast<RandomValue*>(value))->setValue(Cast::fromString<float>(v));
		} else if (strcmp(type, "linear") == 0) {
			const char* min = element->Attribute("min");
			const char* max = element->Attribute("max");
			const char* active = element->Attribute("active");

			TiXmlElement* point = 0;
			vector<Vector2<float>* > curve;
			for (point = element->FirstChildElement("point");
					point;
					point = point->NextSiblingElement("point")) {
				float x = Cast::fromString<float>(point->Attribute("x"));
				float y = Cast::fromString<float>(point->Attribute("y"));
				curve.push_back(new Vector2<float>(x, y));
			}

			LinearInterpolator* li = static_cast<LinearInterpolator*>(value);
			li->setCurve(curve);
			li->setMin(Cast::fromString<int>(min));
			li->setMax(Cast::fromString<int>(max));
			li->setActive(strcmp(active, "true") == 0);

		} else {
			std::cout << "unknown type detected: " << type << std::endl;
		}
	}
}

template <class T>
void ParticleIO::parseRangeElement(TiXmlElement* element, Range<T>* range) {
	range->setMin(Cast::fromString<T>(element->Attribute("min")));
	range->setMax(Cast::fromString<T>(element->Attribute("max")));
	range->setEnabled(strcmp(element->Attribute("enabled"), "true") == 0);
}

