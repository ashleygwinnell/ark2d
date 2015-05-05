/*
 * HSVShader.h
 *
 *  Created on: 4 May 2015
 *      Author: Ashley
 */

#include "HSVShader.h"
#include "HSVGeometryShader.h"
#include "HSVTextureShader.h"

namespace ARK { 
	namespace Graphics { 

		HSVShader::HSVShader() {
			geometry = new HSVGeometryShader();
			texture = new HSVTextureShader();
		}
		void HSVShader::load() {
			geometry->load();
			texture->load();
		}
		void HSVShader::set(float h, float s, float v) {
			geometry->h = h;
			geometry->s = s;
			geometry->v = v;

			texture->h = h;
			texture->s = s;
			texture->v = v;
		}
		float HSVShader::getH() {
			return geometry->h;
		}
		float HSVShader::getS() {
			return geometry->s;
		}
		float HSVShader::getV() {
			return geometry->v;
		}
		void HSVShader::start() {
			geometry->start();
			texture->start();
		}
		void HSVShader::stop() {
			geometry->stop();
			texture->stop();
		}
		HSVShader::~HSVShader() {
			delete geometry;
			delete texture;
		}
	}
}
