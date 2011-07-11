/*
 * PathIO.cpp
 *
 *  Created on: 10 Jul 2011
 *      Author: Ashley
 */

#include "PathIO.h"

PathGroup* PathIO::createFromFile(string file) {
	char* c = StringUtil::file_get_contents(file.c_str());
	if (c == NULL) {
		ErrorDialog::createAndShow(StringUtil::append("Could not load Path file: ", file));
	}
	return createFromJSON(string(c));
}
PathGroup* PathIO::createFromJSON(string json) {

	try {
		JSONNode* root = libJSON::Parse(json);

		PathGroup* pathGroup = new PathGroup();
		pathGroup->setName(root->GetNode("name")->NodeAsString());
		pathGroup->setDescription(root->GetNode("description")->NodeAsString());

		JSONNode* paths = root->GetNode("paths");
		for(unsigned int i = 0; i < paths->NodeSize(); i++)
		{

			JSONNode* path = paths->Children[i];

			Path* pathObj = new Path();
			pathObj->setDuration(path->GetNode("duration")->NodeAsFloat());
			pathObj->setEasing(Easing::getByString(path->GetNode("easing")->NodeAsString()));

			JSONNode* points = path->GetNode("points");
			for(unsigned int j = 0; j < points->NodeSize(); j++)
			{
				JSONNode* point = points->Children[j];
				pathObj->addPoint(point->GetNode("x")->NodeAsInt(), point->GetNode("y")->NodeAsInt());
			}
			pathGroup->addPath(pathObj);
		}
		return pathGroup;

	} catch(...) {
		ErrorDialog::createAndShow("exception in PathIO::createFromJson");
		return NULL;
	}
}
string PathIO::getAsJSON(PathGroup* g) {
	return "";
}
