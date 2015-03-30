
#include "NetTest.h"
#include "../../ARK.h"

namespace ARK {
	namespace Tests {

		NetTestStartState::NetTestStartState(): GameState() { index = 0; }
		unsigned int NetTestStartState::id() { return 0; }
		void NetTestStartState::enter(GameContainer* container, StateBasedGame* game, GameState* from) {}
		void NetTestStartState::leave(GameContainer* container, StateBasedGame* game, GameState* to) { } 
		void NetTestStartState::init(GameContainer* container, StateBasedGame* game) { }
		NetTestStartState::~NetTestStartState() { }
		
		void NetTestStartState::update(GameContainer* container, StateBasedGame* game, GameTimer* timer) 
		{
			Input* in = ARK2D::getInput();
			if (in->isKeyPressed(Input::KEY_UP)) {
				index = (index == 0)?1:0;
			} else if (in->isKeyPressed(Input::KEY_DOWN)) {
				index = (index == 0)?1:0;
			}
			if (in->isKeyPressed(Input::KEY_ENTER)) {
				NetTest* dg = dynamic_cast<NetTest*>(game);
				
				Mode m = (index==0)?Server:Client;
				NetTestState* state = dynamic_cast<NetTestState*>(game->getState(1));
				state->initialise(container, game, m);
				game->enterState(1);
			}
		} 

		void NetTestStartState::render(GameContainer* container, StateBasedGame* game, Renderer* r) 
		{
			string serverText = "Server";
			string clientText = "Client";
			if (index == 0) {
				serverText = string("< ") + serverText + string(" >");
			} else {
				clientText = string("< ") + clientText + string(" >");
			}

			r->drawString(serverText, container->getWidth()/2, 80, Renderer::ALIGN_CENTER, Renderer::ALIGN_TOP);
			r->drawString(clientText, container->getWidth()/2, 110, Renderer::ALIGN_CENTER, Renderer::ALIGN_TOP);
		}  

		
		// ------------------------------------------------------------------------------------------------------


		unsigned int NetTestState::id() { return 1; }
		void NetTestState::init(GameContainer* container, StateBasedGame* game) { }
		void NetTestState::leave(GameContainer* container, StateBasedGame* game, GameState* to) { } 
		NetTestState::~NetTestState() { }

		NetTestState::NetTestState():
			GameState(),
			m_connection(ProtocolId, 10.0f),
			m_events() 
		{

			m_showAcks = false;
			m_number = MathUtil::randBetween(0, 10);
		}
		
		void NetTestState::enter(GameContainer* container, StateBasedGame* game, GameState* from) 
		{
			//Socket::shutdownSockets();
		}

		void NetTestState::initialise(GameContainer* container, StateBasedGame* game, Mode mode) 
		{
			if (!ARK::Net::Socket::initializeSockets()) {
				ARK2D::getLog()->e("failed to initialize sockets");
				container->close();
			}
			
			m_mode = mode;
			m_address = Address(127, 0, 0, 1, ServerPort);
			
			const int port = m_mode == Server ? ServerPort : ClientPort;
			if ( !m_connection.start( port ) ) {
				ARK2D::getLog()->e(StringUtil::append("could not start connection on port: ", port));
				if (m_mode == Client) {
					ARK2D::getLog()->e("Trying second port.");
					if ( !m_connection.start( ClientPort2 ) ) {
						ARK2D::getLog()->e(StringUtil::append("could not start connection on port: ", ClientPort2));
						return;
					}
				} else {
					return;
				}
				
			}
			
			if ( m_mode == Client ) {
				m_connection.connect( m_address );
			} else {
				m_connection.listen();
			}
			
			m_connected = false;
			m_sendAccumulator = 0.0f;
			m_statsAccumulator = 0.0f;	
		}
		
		void NetTestState::update(GameContainer* container, StateBasedGame* game, GameTimer* timer) 
		{
			Input* in = ARK2D::getInput();
			if (in->isKeyPressed(Input::KEY_UP)) {
				
				signed int eventData[2];
				eventData[0] = m_number;
				eventData[1] = m_number + 1;
				m_events.add((const char*) &eventData, 8, 7, false);

				m_number++;
			} else if (in->isKeyPressed(Input::KEY_DOWN)) {
				
				signed int eventData[2];
				eventData[0] = m_number;
				eventData[1] = m_number - 1;
				m_events.add((const char*) &eventData, 8, 7, false);

				m_number--;
			}

			// update flow control
			if ( m_connection.isConnected() ) {
				m_flowControl.update( DeltaTime, m_connection.getReliabilitySystem().getRoundTripTime() * 1000.0f );
			}
			
			const float sendRate = m_flowControl.getSendRate();

			// detect changes in connection state
			if (m_connected && !m_connection.isConnected() )
			{
				if (m_mode == Server) {
					// client disappeared
					m_flowControl.reset();
					printf( "reset flow control\n" );
					m_connected = false;
				} else if (m_mode == Client) {
					// server disappeared. 
					m_connected = false;
				}


			}


			if ( !m_connected && m_connection.isConnected() )
			{
				printf( "client connected to server\n" );
				m_connected = true;
			}
			
			if ( !m_connected && m_connection.connectFailed() )
			{
				printf( "connection failed\n" );
		//		break;
				return;
			}
			
			// send and receive packets
			
			m_sendAccumulator += DeltaTime;
			
			while ( m_sendAccumulator > 1.0f / sendRate )
			{
				unsigned char packet[MAX_PACKET_SIZE];
				m_events.get(packet);

				if (packet[0] > 0) { 
					ARK2D::getLog()->v("sent event/s");
					parseReceivedPacket(packet, false);
				}

				m_connection.sendPacket( packet, sizeof( packet ) );
				m_sendAccumulator -= 1.0f / sendRate;
			}
			
			while ( true )
			{
				unsigned char packet[256];
				int bytes_read = m_connection.receivePacket( packet, sizeof(packet) );
				if ( bytes_read == 0 )
					break;

				// parse packet data, send to all other clients?
				signed int numEvents = packet[0];
				if (numEvents > 0) {
					ARK2D::getLog()->v(StringUtil::append("received events: ", numEvents));
					parseReceivedPacket(packet);
				}
			}
			
			// show packets that were acked this frame
			
			if (m_showAcks) { 
				unsigned int * acks = NULL;
				int ack_count = 0;
				m_connection.getReliabilitySystem().getAcks( &acks, ack_count );
				if ( ack_count > 0 )
				{
					printf( "acks: %d", acks[0] );
					for ( int i = 1; i < ack_count; ++i )
						printf( ",%d", acks[i] );
					printf( "\n" );
				}
			}

			// update connection
			m_connection.update( DeltaTime );

			// show connection stats
			
			m_statsAccumulator += DeltaTime;

			while ( m_statsAccumulator >= 0.25f && m_connection.isConnected() )
			{
				float rtt = m_connection.getReliabilitySystem().getRoundTripTime();
				
				unsigned int sent_packets = m_connection.getReliabilitySystem().getSentPackets();
				unsigned int acked_packets = m_connection.getReliabilitySystem().getAckedPackets();
				unsigned int lost_packets = m_connection.getReliabilitySystem().getLostPackets();
				
				float sent_bandwidth = m_connection.getReliabilitySystem().getSentBandwidth();
				float acked_bandwidth = m_connection.getReliabilitySystem().getAckedBandwidth();
				
				//printf( "rtt %.1fms, sent %d, acked %d, lost %d (%.1f%%), sent bandwidth = %.1fkbps, acked bandwidth = %.1fkbps\n", 
				//	rtt * 1000.0f, sent_packets, acked_packets, lost_packets, 
				//	sent_packets > 0.0f ? (float) lost_packets / (float) sent_packets * 100.0f : 0.0f, 
				//	sent_bandwidth, acked_bandwidth );
				
				m_statsAccumulator -= 0.25f;
			}

			ARK2D::getTimer()->sleep(DeltaTime);
		} 

		void NetTestState::parseReceivedPacket(unsigned char* packet, bool onReceived) 
		{
			signed int numEvents = packet[0];
			unsigned char ptr = 1;
			for(unsigned char i = 0; i < numEvents; i++) {
				unsigned char sz = packet[ptr];
				unsigned char type = packet[ptr+1];
				unsigned char* data = &packet[ptr+2];

				ARK2D::getLog()->v(StringUtil::append("type: ", int(type)));
				if (type == 7) {
					signed int from = packet[ptr+2];
					signed int to = packet[ptr+2+4];
					
					ARK2D::getLog()->v(StringUtil::append("from: ", from));
					ARK2D::getLog()->v(StringUtil::append("to: ", to));

					if (onReceived) {
						m_number = to;
					}

				}

				ptr += sz;
			}
		}

		void NetTestState::render(GameContainer* container, StateBasedGame* game, Renderer* r) {
			int middleX = container->getWidth()/2;
			r->drawString("connected: ", middleX, 50, Renderer::ALIGN_RIGHT, Renderer::ALIGN_TOP);
			r->drawString(Cast::boolToString(m_connected), middleX, 50, Renderer::ALIGN_LEFT, Renderer::ALIGN_TOP);

			if (m_connected) { 

				
				float rtt = m_connection.getReliabilitySystem().getRoundTripTime();
				unsigned int sent_packets = m_connection.getReliabilitySystem().getSentPackets();
				unsigned int acked_packets = m_connection.getReliabilitySystem().getAckedPackets();
				unsigned int lost_packets = m_connection.getReliabilitySystem().getLostPackets();
				
				float sent_bandwidth = m_connection.getReliabilitySystem().getSentBandwidth();
				float acked_bandwidth = m_connection.getReliabilitySystem().getAckedBandwidth();

				r->drawString("round trip time: ", middleX, 90, Renderer::ALIGN_RIGHT, Renderer::ALIGN_TOP);
				r->drawString(Cast::toString<float>(rtt), middleX, 90, Renderer::ALIGN_LEFT, Renderer::ALIGN_TOP);

				r->drawString("sent packets: ", middleX, 130, Renderer::ALIGN_RIGHT, Renderer::ALIGN_TOP);
				r->drawString(Cast::toString<unsigned int>(sent_packets), middleX, 130, Renderer::ALIGN_LEFT, Renderer::ALIGN_TOP);
				r->drawString("acknowledged packets: ", middleX, 160, Renderer::ALIGN_RIGHT, Renderer::ALIGN_TOP);
				r->drawString(Cast::toString<unsigned int>(acked_packets), middleX, 160, Renderer::ALIGN_LEFT, Renderer::ALIGN_TOP);
				r->drawString("lost packets: ", middleX, 190, Renderer::ALIGN_RIGHT, Renderer::ALIGN_TOP);
				r->drawString(Cast::toString<unsigned int>(lost_packets), middleX, 190, Renderer::ALIGN_LEFT, Renderer::ALIGN_TOP);

				r->drawString("sent bandwidth: ", middleX, 230, Renderer::ALIGN_RIGHT, Renderer::ALIGN_TOP);
				r->drawString(Cast::toString<float>(sent_bandwidth), middleX, 230, Renderer::ALIGN_LEFT, Renderer::ALIGN_TOP);

				r->drawString("acknowledged bandwidth: ", middleX, 260, Renderer::ALIGN_RIGHT, Renderer::ALIGN_TOP);
				r->drawString(Cast::toString<float>(acked_bandwidth), middleX, 260, Renderer::ALIGN_LEFT, Renderer::ALIGN_TOP);
			}

			r->drawString(Cast::toString<float>(m_number), middleX, 290, Renderer::ALIGN_CENTER, Renderer::ALIGN_TOP,0.0f, 4.0f);

			string typeStr = "Server";
			if (m_mode == Client) { typeStr = "Client"; }
			r->drawString(typeStr, middleX, 390, Renderer::ALIGN_CENTER, Renderer::ALIGN_TOP, 0.0f, 1.5f);
		}  

		// ---------------------------------------------------------------------------------------------------------






		NetTest::NetTest(): StateBasedGame("Net Test") {

		}
		void NetTest::initStates(GameContainer* container) {
			addState(new NetTestStartState());
			addState(new NetTestState());
			enterState((unsigned int) 0);
		}
		void NetTest::update(GameContainer* container, GameTimer* timer) {
			StateBasedGame::update(container, timer);
		}
		void NetTest::render(GameContainer* container, Renderer* r) {
			StateBasedGame::render(container, r);
			r->setDrawColor(Color::white);
			r->drawString("Netcode Test", 20, 20, Renderer::ALIGN_LEFT, Renderer::ALIGN_TOP);
		}
		void NetTest::resize(GameContainer* container, int width, int height) {
			StateBasedGame::resize(container, width, height);
		}
		NetTest::~NetTest() {

		}
		int NetTest::start() {
			ARK::Tests::NetTest* test = new ARK::Tests::NetTest();
			GameContainer container(*test, 640, 480, 32, false);
			container.start();
			return 0;
		}



	}
}











