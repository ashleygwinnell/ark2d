/*
 * FlowControl.cpp
 *
 *  Created on: 28 March 2015
 *      Author: Ashley
 */

#include "FlowControl.h"

#include <cstdio>

namespace ARK {
	namespace Net {

		FlowControl::FlowControl()
		{
			printf( "flow control initialized\n" );
			reset();
		}

		void FlowControl::reset()
		{
			mode = Bad;
			penalty_time = 4.0f;
			good_conditions_time = 0.0f;
			penalty_reduction_accumulator = 0.0f;
		}

		void FlowControl::update( float deltaTime, float rtt )
		{
			const float RTT_Threshold = 250.0f;

			if ( mode == Good )
			{
				if ( rtt > RTT_Threshold )
				{
					printf( "*** dropping to bad mode ***\n" );
					mode = Bad;
					if ( good_conditions_time < 10.0f && penalty_time < 60.0f )
					{
						penalty_time *= 2.0f;
						if ( penalty_time > 60.0f )
							penalty_time = 60.0f;
						printf( "penalty time increased to %.1f\n", penalty_time );
					}
					good_conditions_time = 0.0f;
					penalty_reduction_accumulator = 0.0f;
					return;
				}

				good_conditions_time += deltaTime;
				penalty_reduction_accumulator += deltaTime;

				if ( penalty_reduction_accumulator > 10.0f && penalty_time > 1.0f )
				{
					penalty_time /= 2.0f;
					if ( penalty_time < 1.0f )
						penalty_time = 1.0f;
					printf( "penalty time reduced to %.1f\n", penalty_time );
					penalty_reduction_accumulator = 0.0f;
				}
			}

			if ( mode == Bad )
			{
				if ( rtt <= RTT_Threshold )
					good_conditions_time += deltaTime;
				else
					good_conditions_time = 0.0f;

				if ( good_conditions_time > penalty_time )
				{
					printf( "*** upgrading to good mode ***\n" );
					good_conditions_time = 0.0f;
					penalty_reduction_accumulator = 0.0f;
					mode = Good;
					return;
				}
			}
		}

		float FlowControl::getSendRate()
		{
			return mode == Good ? 30.0f : 10.0f;
		}


	}
}
