/*
 * Callbacks.cpp
 *
 *  Created on: 13 Feb 2013
 *      Author: Ashley
 */


#include "Callbacks.h"

#include "../ARK2D.h"
#include "Log.h"

namespace ARK {
	namespace Util {

		string Callback::ARGS_NONE = "";

		Callback::Callback():
			m_id(0),
			m_functionPointer(NULL),
			m_functionArgs("")
		{

		}
		void Callback::invoke()  
		{
			if (m_functionArgs.length() == 0) {
				void (*pt)() = (void(*)()) m_functionPointer;
				pt(); 
				return;
			}   
		}
		void Callback::invoke(unsigned int param) {
			if (m_functionArgs.length() == 0) {
				void (*pt)(unsigned int) = (void(*)(unsigned int)) m_functionPointer;
				pt(param); 
				return;
			}  
		}
		void Callback::invoke(string strparam) {
			if (m_functionArgs.length() == 0) {
				void (*pt)(string) = (void(*)(string)) m_functionPointer;
				pt(strparam); 
				return;
			}  
		}
 
		vector<ARK::Util::Callback> Callbacks::s_callbacks;
		void Callbacks::add(Callback c)   
		{
			s_callbacks.push_back(c);
		}
		void Callbacks::invoke(unsigned int id) 
		{
			for (unsigned int i = 0; i < s_callbacks.size(); ++i) 
			{
				if (s_callbacks.at(i).getId() == id) { 
					s_callbacks.at(i).invoke();
					return;
				}
			}

			ARK2D::getLog()->w(StringUtil::append("Could not find callback with id: ", id));
		}
		void Callbacks::invoke(unsigned int id, unsigned int param) 
		{
			for (unsigned int i = 0; i < s_callbacks.size(); ++i) 
			{
				if (s_callbacks.at(i).getId() == id) { 
					s_callbacks.at(i).invoke(param);
					return;
				}
			}

			ARK2D::getLog()->w(StringUtil::append("Could not find callback with id: ", id));
		}
		void Callbacks::invoke(unsigned int id, string strparam) 
		{
			for (unsigned int i = 0; i < s_callbacks.size(); ++i) 
			{
				if (s_callbacks.at(i).getId() == id) { 
					s_callbacks.at(i).invoke(strparam);
					return;
				}
			}

			ARK2D::getLog()->w(StringUtil::append("Could not find callback with id: ", id));
		}
	}
}

