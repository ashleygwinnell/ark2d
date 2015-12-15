#include "TitledPanel.h"

#include "../Graphics/Renderer.h"
#include "../Geometry/Cube.h"  
 
namespace ARK { 
	namespace UI { 

		TitledPanel::TitledPanel():   
			Panel(), 
			m_title(""), 
			m_content(NULL) {
			m_content = new Panel();
			m_content->transform.position.setY(20);
			m_content->setName("content");
			Panel::addChild(m_content);
		}
		TitledPanel::TitledPanel(Panel* content, string title):  
			Panel(), 
			m_title(title),
			m_content(content) {
			m_content->transform.position.setY(20);
			m_content->setName("content");
			Panel::addChild(m_content); 
		}
		void TitledPanel::setTitle(string title) {
			m_title = title;
		}
		void TitledPanel::addChild(SceneNode* node) {
			m_content->addChild(node);
		}
		void TitledPanel::setBounds(float w, float h, float d) {
			Panel::setBounds(w,h,d);
			m_content->setBounds(w, h - 20.0f, d);
			//m_content->position.set(0, 20, 0);
			m_content->transform.scale.set(1.0f, 1.0f, 1.0f);
			
		}
		void TitledPanel::render() {

			preRenderFromPivot(); 

			ARK::Geometry::Cube* bounds = getBounds();
			Renderer* r = ARK2D::getRenderer();
			r->setDrawColor(Color::black_50a);
			r->fillRect(0, 0, bounds->getWidth(), bounds->getHeight());

			r->setDrawColor(Color::white);
			r->drawLine(0.0f, 20.0f, bounds->getWidth(), 20.0f);
			
			r->drawString(m_title, 5, 10, Renderer::ALIGN_LEFT, Renderer::ALIGN_CENTER);

			Panel::renderBorder();

			postRenderFromPivot();



				// pivot.getX() * bounds->getWidth()
			//m_content.position.set(pivot.getX() * bounds->getWidth(), (pivot.getY() * bounds->getHeight()) + 20.0f, 0.0f);
			//m_content.position.set(0.0f, 20.0f, 0.0f);
   			
   			//m_content.setBounds(bounds->getWidth(), bounds->getHeight() - 20, 0.0f);

   			//m_content->position.set(pivot.getX() * bounds->getWidth() * -1.0f, (pivot.getY() * bounds->getHeight() * -1.0f) + 20.0f, 0);
   			//m_content->pivot.set(0.0f, 0.0f, 0.0f);

   			m_content->transform.position.set(0, 20 * (1.0f - pivot.getY()), 0);
   			m_content->pivot.set(pivot.getX(), pivot.getY(), pivot.getZ());

			m_content->preRender();
			m_content->render();
			m_content->postRender();


		}

	}
}