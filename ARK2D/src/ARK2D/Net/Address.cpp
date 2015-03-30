/*
 * Address.cpp
 *
 *  Created on: 28 March 2015
 *      Author: Ashley
 */

#include "Address.h"

namespace ARK {
	namespace Net { 
		Address::Address(): 
			m_address(0), 
			m_port(0) {
			
		}
	
		Address::Address(unsigned int address, unsigned short port):
			m_address(address),
			m_port(port) {
			
		}

		Address::Address(unsigned char a, unsigned char b, unsigned char c, unsigned char d, unsigned short port):
			m_address( ( a << 24 ) | ( b << 16 ) | ( c << 8 ) | d ),
			m_port(port) {
			
		}
	
		unsigned int Address::getAddress() const {
			return m_address;
		}
	
		unsigned char Address::getA() const {
			return ( unsigned char ) ( m_address >> 24 );
		}
	
		unsigned char Address::getB() const {
			return ( unsigned char ) ( m_address >> 16 );
		}
	
		unsigned char Address::getC() const {
			return ( unsigned char ) ( m_address >> 8 );
		}
	
		unsigned char Address::getD() const {
			return ( unsigned char ) ( m_address );
		}
	
		unsigned short Address::getPort() const { 
			return m_port;
		}
	
		bool Address::operator == ( const Address & other ) const
		{
			return m_address == other.m_address && m_port == other.m_port;
		}
	
		bool Address::operator != ( const Address & other ) const
		{
			return ! ( *this == other );
		}
		
		bool Address::operator < ( const Address & other ) const
		{
			// note: this is so we can use address as a key in std::map
			if ( m_address < other.m_address )
				return true;
			if ( m_address > other.m_address )
				return false;
			else
				return m_port < other.m_port;
		}

		Address::~Address() {

		}
	}
}
