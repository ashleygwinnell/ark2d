/*
 * Easing.h
 *
 *  Created on: 5 Jul 2011
 *      Author: Ashley
 */

#ifndef EASING_H_
#define EASING_H_

#include "../Namespaces.h"
#include "../Common/DLL.h"

#include <math.h>
#include <string>
using namespace std;

namespace ARK {
	namespace Tween {

		/*!
		 * \brief Easing a.k.a. interpolation between two values.
		 *
		 * Adapted from Robert Penner's easing implementation.
		 *
		 * @author Ashley Gwinnell
		 * @author Robert Penner.
		 * @see http://www.robertpenner.com/easing/easing_demo.html
		 * @see http://www.dzone.com/snippets/robert-penner-easing-equations
		 * @see file://C:\xampp\htdocs\C++\ARK2D\Docs\Easing.png
		 *
		 * (Generate your own)
		 * @see http://www.timotheegroleau.com/Flash/experiments/easing_function_generator.htm
		 *
		 */
		class ARK2D_API Easing {
			public:
				static const unsigned int LINEAR = 0;
				static const unsigned int QUADRATIC_IN = 1;
				static const unsigned int QUADRATIC_OUT = 2;
				static const unsigned int QUADRATIC_IN_OUT = 3;
				static const unsigned int CUBIC_IN = 4;
				static const unsigned int CUBIC_OUT = 5;
				static const unsigned int CUBIC_IN_OUT = 6;
				static const unsigned int QUARTIC_IN = 7;
				static const unsigned int QUARTIC_OUT = 8;
				static const unsigned int QUARTIC_IN_OUT = 9;
				static const unsigned int SINE_IN = 10;
				static const unsigned int SINE_OUT = 11;
				static const unsigned int SINE_IN_OUT = 12;
				static const unsigned int EXPONENTIAL_IN = 13;
				static const unsigned int EXPONENTIAL_OUT = 14;
				static const unsigned int EXPONENTIAL_IN_OUT = 15;
				static const unsigned int CIRCLE_IN = 16;
				static const unsigned int CIRCLE_OUT = 17;
				static const unsigned int CIRCLE_IN_OUT = 18;
				static const unsigned int ELASTIC_IN = 19;
				static const unsigned int ELASTIC_OUT = 20;
				static const unsigned int ELASTIC_IN_OUT = 21;
				static const unsigned int BOUNCE_IN = 22;
				static const unsigned int BOUNCE_OUT = 23;
				static const unsigned int BOUNCE_IN_OUT = 24;
				static const unsigned int BACK_IN = 25;
				static const unsigned int BACK_OUT = 26;
				static const unsigned int NONE = 27;

				static unsigned int getByString(string str);
				static string getByInt(unsigned int str);

				static double ease(unsigned int easing, double time, double start, double change, double duration);
				static double easebetween(unsigned int easing, double time, double start, double end, double duration);
				static double easebetweenback(unsigned int easingThere, unsigned int easingBack, double time, double start, double mid, double duration);

		};
	}
}

#endif /* EASING_H_ */
