/*
 * UITestState.h
 *
 *  Created on: Mar 24, 2011.
 *      Author: ashleygwinnell
 */

#ifndef ARK_TESTS_NETTEST_H_
#define ARK_TESTS_NETTEST_H_

#include "../../ARK.h"

namespace ARK {
	namespace Tests {


		const int ServerPort = 30000;
		const int ClientPortRangeStart = 30001;
		const int ClientPortRangeEnd = 30008;
		const int ProtocolId = 0x11223344;
		
		const float DeltaTime = 1.0f / 30.0f;
		const float SendRate = 1.0f / 30.0f;

		const int DiscoveryServerPort = 31000;
		const int DiscoveryClientPort = 31001;
		const int DiscoveryProtocolId = 0x22334455;


		enum Mode {
			Client,
			Server
		};

		class ARK2D_API NetTestStartState : public GameState {
			public:
				signed int index;
			public:
				NetTestStartState();
				void enter(GameContainer* container, StateBasedGame* game, GameState* from);
				void leave(GameContainer* container, StateBasedGame* game, GameState* to);

				unsigned int id();
				void init(GameContainer* container, StateBasedGame* game);
				void update(GameContainer* container, StateBasedGame* game, GameTimer* timer);
				void render(GameContainer* container, StateBasedGame* game, Renderer* r);

				virtual ~NetTestStartState();
		};
		class ARK2D_API NetTestDiscoveryState : public GameState {
			public:
				Discovery m_discoveryConnection;	
				signed int m_selectedIndex;
			public:
				NetTestDiscoveryState();
				void enter(GameContainer* container, StateBasedGame* game, GameState* from);
				void leave(GameContainer* container, StateBasedGame* game, GameState* to);

				unsigned int id();
				void init(GameContainer* container, StateBasedGame* game);
				void initialise(GameContainer* container, StateBasedGame* game);
				void update(GameContainer* container, StateBasedGame* game, GameTimer* timer);
				void render(GameContainer* container, StateBasedGame* game, Renderer* r);

				virtual ~NetTestDiscoveryState();
		};

		class ARK2D_API NetTestState : public GameState {
			public:
				Mode m_mode;
				Address m_address;
				ReliableConnection m_connection;
				Discovery m_discoveryConnection;
				FlowControl m_flowControl;

				bool m_connected;
				float m_sendAccumulator;
				float m_statsAccumulator;

				bool m_showAcks;

				EventQueue m_events;
				signed int m_number;

			public:

				NetTestState();
				void enter(GameContainer* container, StateBasedGame* game, GameState* from);
				void leave(GameContainer* container, StateBasedGame* game, GameState* to);

				unsigned int id();
				void init(GameContainer* container, StateBasedGame* game);
				void update(GameContainer* container, StateBasedGame* game, GameTimer* timer);
				void render(GameContainer* container, StateBasedGame* game, Renderer* g);

				void initialise(GameContainer* container, StateBasedGame* game, Mode mode);
				void parseReceivedPacket(unsigned char* packet, bool onReceived = true);

				virtual ~NetTestState();
		};

		class ARK2D_API NetTest : public StateBasedGame {
			public:
				NetTest();

				virtual void initStates(GameContainer* container);
				virtual void update(GameContainer* container, GameTimer* timer);
				virtual void render(GameContainer* container, Renderer* g);
				virtual void resize(GameContainer* container, int width, int height);

				static int start();

				virtual ~NetTest();
		};

	}
}



#endif /* ARK_TESTS_NETTEST_H_ */
