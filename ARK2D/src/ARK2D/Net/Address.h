/*
 * Address.h
 *
 *  Created on: 28 March 2015
 *      Author: Ashley
 */

#ifndef ARK_NET_ADDRESS_H_
#define ARK_NET_ADDRESS_H_

#include "../Includes.h"
#include "../Namespaces.h"

namespace ARK {
	namespace Net {

		/*!
		 * \brief Internet Address
		 *
		 * @author Glenn Fiedler
		 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
		 */
		class ARK2D_API Address {
			private:
				unsigned int m_address;
				unsigned short m_port;

			public:
				Address();
				Address(unsigned char a, unsigned char b, unsigned char c, unsigned char d, unsigned short port);
				Address(unsigned int address, unsigned short port);

				unsigned int getAddress() const;
				unsigned char getA() const;
				unsigned char getB() const;
				unsigned char getC() const;
				unsigned char getD() const;
				unsigned short getPort() const;
				void setBroadcast();
				void setPort(unsigned short port);
			
				bool operator == ( const Address & other ) const;
				bool operator != ( const Address & other ) const;
				bool operator < ( const Address & other ) const;

				string toString() const;
				~Address();
		};
	}
}

#endif /* ARK_NET_ADDRESS_H_ */
