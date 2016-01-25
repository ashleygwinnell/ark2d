/*
 * SplitPane.cpp
 *
 *  Created on: 1 Oct 2015
 *      Author: Ashley
 */

#include "SplitPane.h"

namespace ARK {
	namespace UI {

		SplitPane::SplitPane():
			AbstractUIComponent(),
			m_bounds(0,0,0),
			m_splitType(SPLIT_HORIZONTAL),
			m_splitLocation(0.5f),
			m_firstComponent(NULL),
			m_secondComponent(NULL)
		{

		}
		SplitPane::SplitPane(unsigned int splitType):
			AbstractUIComponent(),
			m_bounds(0,0,0),
			m_splitType(splitType),
			m_splitLocation(0.5f),
			m_firstComponent(NULL),
			m_secondComponent(NULL)
		{

		}
		SplitPane::SplitPane(unsigned int splitType, SceneNode* firstComponent, SceneNode* secondComponent):
			AbstractUIComponent(),
			m_bounds(0,0,0),
			m_splitType(splitType),
			m_splitLocation(0.5f),
			m_firstComponent(firstComponent),
			m_secondComponent(secondComponent)
		{
			addChild(firstComponent);
			addChild(secondComponent);
		}
		SplitPane::SplitPane(unsigned int splitType, SceneNode* firstComponent, SceneNode* secondComponent, float splitLocation):
			AbstractUIComponent(),
			m_bounds(0,0,0),
			m_splitType(splitType),
			m_splitLocation(splitLocation),
			m_firstComponent(firstComponent),
			m_secondComponent(secondComponent)
		{
			addChild(firstComponent);
			addChild(secondComponent);
		}
		void SplitPane::setSplitLocation(float f) {
			m_splitLocation = f;
		}
		void SplitPane::setLeftComponent(SceneNode* left) {
			m_firstComponent = left;
			addChild(m_firstComponent);
		}
		void SplitPane::setRightComponent(SceneNode* right) {
			m_secondComponent = right;
			addChild(m_secondComponent);
		}
		void SplitPane::setTopComponent(SceneNode* top) {
			m_firstComponent = top;
			addChild(m_firstComponent);
		}
		void SplitPane::setBottomComponent(SceneNode* bottom) {
			m_secondComponent = bottom;
			addChild(m_secondComponent);
		}

		void SplitPane::render() {
			ARK::Geometry::Cube* bounds = getBounds();
			Renderer* r = ARK2D::getRenderer();

			// update location/size/ui/layout of children.
            ARK::Geometry::Cube* firstBounds = (m_firstComponent != NULL)
            	? m_firstComponent->getBounds()
            	: SceneNode::s_dummyCube;
            ARK::Geometry::Cube* secondBounds = (m_secondComponent != NULL)
            	? m_secondComponent->getBounds()
            	: SceneNode::s_dummyCube;
			if (m_splitType == SPLIT_HORIZONTAL) {
				if (m_firstComponent != NULL) { 
					m_firstComponent->transform.position.set(0.0f, 0.0f, 0.0f);
   					m_firstComponent->transform.scale.set(1.0f, 1.0f, 1.0f);
                	m_firstComponent->setBounds((bounds->getWidth()*m_splitLocation), bounds->getHeight(), 0.0f);
                }
                if (m_secondComponent != NULL) { 
					m_secondComponent->transform.position.set(bounds->getWidth()*m_splitLocation, 0.0f, 0.0f);
                	m_secondComponent->transform.scale.set(1.0f, 1.0f, 1.0f);
                	m_secondComponent->setBounds(bounds->getWidth()*(1.0f-m_splitLocation), bounds->getHeight(), 0.0f);
                }
			} else if (m_splitType == SPLIT_VERTICAL)
            {
				if (m_firstComponent != NULL) { 
					m_firstComponent->transform.position.set(0.0f, 0.0f, 0.0f);
                	m_firstComponent->transform.scale.set(1.0f, 1.0f, 1.0f);
					m_firstComponent->setBounds(bounds->getWidth(), bounds->getHeight()*m_splitLocation, 0.0f);
				}

				if (m_secondComponent != NULL) { 
					m_secondComponent->transform.position.set(0.0f, bounds->getHeight()*m_splitLocation, 0.0f);
                	m_secondComponent->transform.scale.set(1.0f, 1.0f, 1.0f);
					m_secondComponent->setBounds(bounds->getWidth(), bounds->getHeight()*(1.0f-m_splitLocation), 0.0f);
				}
			}

			// render sub components
			renderChildren();

			// return line/gap/itself
			r->setDrawColor(Color::white);
			r->setLineWidth(2);
			if (m_splitType == SPLIT_HORIZONTAL) {
				r->drawLine(bounds->getWidth()*m_splitLocation, 0.0f, bounds->getWidth()*m_splitLocation, bounds->getHeight());
			} else {
				r->drawLine(0.0f, bounds->getHeight()*m_splitLocation, bounds->getWidth(), bounds->getHeight()*m_splitLocation);
			}
			r->setLineWidth(1);
		}
		ARK::Geometry::Cube* SplitPane::getBounds() {
			return &m_bounds;
		}
		void SplitPane::setBounds(float w, float h, float d) {
			m_bounds.set(0.0f, 0.0f, 0.0f, w, h, d);
		}


		bool SplitPane::keyPressed(unsigned int key) {
			if (!visible) { return false; }

			for(unsigned int i = 0; i < children.size(); i++) {
                if (children.at(i)->keyPressed(key) ) return true;
			}
	        return false;
		}
		bool SplitPane::keyReleased(unsigned int key) {
			if (!visible) { return false; }

			for(unsigned int i = 0; i < children.size(); i++) {
                if (children.at(i)->keyReleased(key) ) return true;
			}
	        return false;
		}
		bool SplitPane::mouseMoved(int x, int y, int oldx, int oldy) {
			if (!visible) { return false; }

			if (isGlobalPositionInBounds(x, y)) {
				m_state = STATE_OVER;
			} else {
				m_state = STATE_OFF;
			}
			
			bool consumed = false;
			for(unsigned int i = 0; i < children.size(); i++) {
                if (children.at(i)->mouseMoved(x, y, oldx, oldy)) { consumed = true; }
			}
			if (consumed) { m_state = STATE_OFF; return consumed; }

			return m_state == STATE_OVER;
		}

		SplitPane::~SplitPane() {

		}
	
	}
}