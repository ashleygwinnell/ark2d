/*
 * ARK2DPathLoader.cpp
 *
 *  Created on: 12 Sep 2010
 *      Author: Ashley
 */

#include "../ARK2D_JSON.h"
#include "PathLoader.h"
#include "../StringUtil.h"
#include "../Cast.h"
#include "../Timeline.h"
#include "../ErrorDialog.h"

#include <stdlib.h>
#include <iostream>

using namespace std;

PathLoader::PathLoader(const char* file):
	m_file(file)
{
	m_file_contents = StringUtil::file_get_contents(file);

}

Path* PathLoader::load() {

	ErrorDialog::createAndShow("DEPRECATED. OMG.");
	return NULL;

	string filecontents = string(m_file_contents);
	JSONNode* node = libJSON::Parse(filecontents);

	Path* path = new Path();
	path->setId( node->GetNode("id")->_string );
	path->setName( node->GetNode("name")->_string );

	StringUtil::trim(node->GetNode("type")->_string, "\"");
	path->setType( node->GetNode("type")->_string );



//	std::cout << path->getType() << std::endl;

	JSONNode* points = node->GetNode("points");
	//cout << "size:";
	//cout << points->NodeSize() << endl;
	//cout << "x: " << points->NodeAt(0)->GetNode("x")->_string << endl;


	//for (JSONNode** point_it = points->Children.begin(); point_it != points->Children.end(); point_it++) {
	//	JSONNode* point = (*point_it);
	for (unsigned int i = 0; i < points->NodeSize(); i++) {
		JSONNode* point = points->NodeAt(i);

		PathPoint p;

		JSONNode* x_node = point->GetNode("x");
		if (x_node != NULL) {
			p.x = Cast::fromString<float>(x_node->_string);
		}

		JSONNode* y_node = point->GetNode("y");
		if (y_node != NULL) {
			p.y = Cast::fromString<float>(y_node->_string);
		}

		JSONNode* x_easing_node = point->GetNode("x_easing");
		if (x_easing_node != NULL) {
			StringUtil::trim(x_easing_node->_string, "\"");
			p.x_easing = Easing::getByString( x_easing_node->_string );
		}

		JSONNode* y_easing_node = point->GetNode("y_easing");
		if (y_easing_node != NULL) {
			StringUtil::trim(y_easing_node->_string, "\"");
			p.y_easing = Easing::getByString( y_easing_node->_string );
		}

		JSONNode* millis_node = point->GetNode("millis");
		if (millis_node != NULL) {
			p.millis = Cast::fromString<unsigned int>(millis_node->_string);
		}

		//cout << "adding point" << endl;
		path->addPoint(p);
	}

	return path;

}

PathLoader::~PathLoader() {

}
