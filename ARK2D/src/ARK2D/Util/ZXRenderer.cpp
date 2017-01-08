/*
 * ZXRenderer.h
 *
 *  Created on: 07 Jan 2017
 *      Author: Ashley
 */

#include "ZXRenderer.h"
#include "../Core/Graphics/Renderer.h"

namespace ARK {
	namespace Util {
		unsigned int ZXRenderer::colorToInt(string s) {
			if (s == "black") {
				return COLOR_BLACK;
			} else if (s == "blue") {
				return COLOR_BLUE;
			} else if (s == "red") {
				return COLOR_RED;
			} else if (s == "magenta" || s == "pink") {
				return COLOR_MAGENTA;
			} else if (s == "green") {
				return COLOR_GREEN;
			} else if (s == "cyan") {
				return COLOR_CYAN;
			} else if (s == "yellow") {
				return COLOR_YELLOW;
			} else if (s == "white") {
				return COLOR_WHITE;
			}
			return COLOR_WHITE;
		}
		string ZXRenderer::intToColorString(unsigned int s) {
			if (s == COLOR_BLACK) {
				return "black";
			} else if (s == COLOR_BLUE) {
				return "blue";
			} else if (s == COLOR_RED) {
				return "red";
			} else if (s == COLOR_MAGENTA) {
				return "pink";
			} else if (s == COLOR_GREEN) {
				return "green";
			} else if (s == COLOR_CYAN) {
				return "cyan";
			} else if (s == COLOR_YELLOW) {
				return "yellow";
			} else if (s == COLOR_WHITE) {
				return "white";
			}
			return "white";
		}
		unsigned int ZXRenderer::colorNext(unsigned int c) {
			if (c == COLOR_WHITE) {
				return COLOR_BLACK;
			}
			return c+1;
		}

		void ZXRenderer::init(int widthTiles, int heightTiles) {
			widthInTiles = widthTiles;
			heightInTiles = heightTiles;

			m_renderTargetFG = new FBO(widthInTiles * 8, heightInTiles * 8);
			m_renderTargetFG->name = "ZX renderTargetFG";
			m_renderTargetFG->setClearScreenOnBind(true);

			m_renderTarget = new FBO(widthInTiles * 8, heightInTiles * 8);
			m_renderTarget->name = "ZX renderTarget";
			m_renderTarget->setClearScreenOnBind(true);

			m_image = (*m_renderTarget->getImage())->getSubImage(0, 0, widthInTiles * 8, heightInTiles * 8);

			m_attributeColoursDim.push_back(new Color("#000000")); // black
			m_attributeColoursDim.push_back(new Color("#0000CD")); // blue
			m_attributeColoursDim.push_back(new Color("#CD0000")); // red
			m_attributeColoursDim.push_back(new Color("#CD00CD")); // magenta
			m_attributeColoursDim.push_back(new Color("#00CD00")); // green
			m_attributeColoursDim.push_back(new Color("#00CDCD")); // cyan
			m_attributeColoursDim.push_back(new Color("#CDCD00")); // yellow
			m_attributeColoursDim.push_back(new Color("#CDCDCD")); // white
			m_attributeColoursBright.push_back(new Color("#000000")); // black
			m_attributeColoursBright.push_back(new Color("#0000FF")); // blue
			m_attributeColoursBright.push_back(new Color("#FF0000")); // red
			m_attributeColoursBright.push_back(new Color("#FF00FF")); // magenta
			m_attributeColoursBright.push_back(new Color("#00FF00")); // green
			m_attributeColoursBright.push_back(new Color("#00FFFF")); // cyan
			m_attributeColoursBright.push_back(new Color("#FFFF00")); // yellow
			m_attributeColoursBright.push_back(new Color("#FFFFFF")); // white

			m_attributeBorder = COLOR_BLACK;
			for(unsigned int i = 0; i < widthInTiles * heightInTiles; ++i) {
				m_attributeBlocksBG.push_back(COLOR_BLACK);
				m_attributeBlocksFG.push_back(COLOR_WHITE);
				m_attributeBlocksSet.push_back(true);
			}

			m_stencilbounds = true;

		}
		void ZXRenderer::resize() {
			m_renderTarget->resize(widthInTiles*8, heightInTiles*8);
			m_renderTargetFG->resize(widthInTiles*8, heightInTiles*8);
		}
		void ZXRenderer::reset() {
			border(COLOR_BLACK);
			for(unsigned char x = 0; x < widthInTiles; x++) {
				for(unsigned char y = 0; y < heightInTiles; y++) {
					bg(x, y, COLOR_BLACK);
					fg(x, y, COLOR_WHITE);
					bright(x, y, true);
				}
			}
		}
		void ZXRenderer::border(unsigned char color) {
			m_attributeBorder = color;
		}
		void ZXRenderer::fg(unsigned char x, unsigned char y, unsigned char color) {
			if (x >= widthInTiles) { return; }
			if (y >= heightInTiles) { return; }
			y = heightInTiles - 1 - y;
			m_attributeBlocksFG[(y * widthInTiles) + x] = color;
		}
		void ZXRenderer::bg(unsigned char x, unsigned char y, unsigned char color) {
			if (x >= widthInTiles) { return; }
			if (y >= heightInTiles) { return; }
			y = heightInTiles - 1 - y;
			m_attributeBlocksBG[(y * widthInTiles) + x] = color;
		}
		void ZXRenderer::bright(unsigned char x, unsigned char y, bool bright) {
			if (x >= widthInTiles) { return; }
			if (y >= heightInTiles) { return; }
			y = heightInTiles - 1 - y;
			m_attributeBlocksSet[(y * widthInTiles) + x] = bright;
		}
		void ZXRenderer::start() {
			Renderer* r = ARK2D::getRenderer();

			// Anything renderered after this goes into foreground FBO.
			m_renderTargetFG->setClearColor(Color::black_0a);
			m_renderTargetFG->bind();
			m_renderTargetFG->bind_2d();

			r->setDrawColor(Color::white);
			r->setBlendMode(Renderer::BLEND_NORMAL);
		}
		void ZXRenderer::stop() {

			Renderer* r = ARK2D::getRenderer();
			m_renderTargetFG->unbind_2d();
			m_renderTargetFG->unbind();

			// Cool. Now combine the two buffers into one image.
			m_renderTarget->setClearColor(Color::black_0a);
			m_renderTarget->bind();
			m_renderTarget->bind_2d();

			r->setDrawColor(Color::white);
			r->setBlendMode(Renderer::BLEND_NORMAL);

			__renderBG();
			__renderFG();

			m_renderTarget->unbind_2d();
			m_renderTarget->unbind();

			//r->disableStencil();
		}
		void ZXRenderer::__renderBG() {
			Renderer* r = ARK2D::getRenderer();

			int x = 0;
			int y = (heightInTiles-1) * 8;
			r->setDrawColor(Color::black);
			r->fillRect(0, 0, widthInTiles * 8, heightInTiles * 8);
			for(unsigned int i = 0; i < m_attributeBlocksBG.size(); ++i)
			{
		        bool set = m_attributeBlocksSet[i];
				unsigned char ab = m_attributeBlocksBG[i];

				if (ab != COLOR_BLACK) {
					Color* c = (!set)
						? m_attributeColoursDim[ab]
						: m_attributeColoursBright[ab];
					r->setDrawColor(*c);
					r->fillRect(x, y, 8, 8);
				}
				x += 8;
				if (x == widthInTiles * 8) {
					x = 0;
					y -= 8;
				}
			}
		}
		void ZXRenderer::__renderFG() {
			Renderer* r = ARK2D::getRenderer();

			unsigned int t = m_renderTarget->getTextureId();
			int x = 0;
			int y = (heightInTiles-1) * 8;
			float tycalc = (heightInTiles*8.0f) / float(m_renderTarget->getHeight());
			float tx = 0.0f;
			float ty = 1.0f - tycalc;
			float txeach = (1.0f / float(widthInTiles));
			float tyeach = (tycalc / float(heightInTiles));
			//r->getBatch()->setEnabled(true);
			for(unsigned int i = 0; i < m_attributeBlocksFG.size(); ++i)
			{
				bool set = m_attributeBlocksSet.at(i);
				unsigned char abg = m_attributeBlocksBG[i];
				unsigned char ab = m_attributeBlocksFG[i];
				if (abg != ab) {

					Color* c = (!set)
						? m_attributeColoursDim[ab]
						: m_attributeColoursBright[ab];

					r->setDrawColor(*c);
					(*m_renderTargetFG->getImage())->drawSubImage(
						x, y, x, y,
						8,8,8,8
					);
				}

				x += 8;
				tx += txeach;

				if (x == widthInTiles*8) {
					x = 0;
					y -= 8;
				}
				if (tx == 1.0f) {
					tx = 0.0f;
					ty += tyeach;
				}
			}
		}
		Image* ZXRenderer::getImage() {
			return m_image;
		}
	}
}
