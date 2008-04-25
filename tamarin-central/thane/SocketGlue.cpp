//
// $Id: $

# include <unistd.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <sys/ioctl.h>
# include <netinet/in.h>
# include <arpa/inet.h>

#include "avmthane.h"

namespace thane
{
	BEGIN_NATIVE_MAP(SocketClass)
        NATIVE_METHOD(flash_net_Socket_private_nb_connect,       SocketObject::connect)
        NATIVE_METHOD(flash_net_Socket_private_nb_disconnect,    SocketObject::disconnect)
        NATIVE_METHOD(flash_net_Socket_private_nb_read,          SocketObject::read)
        NATIVE_METHOD(flash_net_Socket_private_nb_write,         SocketObject::write)
	END_NATIVE_MAP()
		
	//
	// Socket
	//
	
	Socket::Socket()
	{
        m_descriptor = -1;
	}

	Socket::~Socket()
	{
 		if (m_descriptor >= 0) {
            close(m_descriptor);
            m_descriptor = -1;
 		}
	}

    /**
     * Returns -1 for error, 0 for keep trying, 1 for success.
     *
     * Note: No DNS lookup. Send in 127.0.0.1.
     */
    int Socket::connect (const char *host, int port)
    {
        if (-1 == m_descriptor) {
            memset(&m_host, 0, sizeof(m_host));
            m_host.sin_family = AF_INET;
            m_host.sin_port = htons(port);
            m_host.sin_addr.s_addr = inet_addr(host);
            if (m_host.sin_addr.s_addr == INADDR_NONE) {
                // TODO: throw an error instead
                return -1;
            }
            m_descriptor = socket(AF_INET, SOCK_STREAM, 0);
            if (-1 == m_descriptor) {
                // TODO: throw an error instead
                return -1;
            }
            int flags = fcntl(m_descriptor, F_GETFL, 0);
            fcntl(m_descriptor, F_SETFL, (flags > 0 ? flags : 0) | O_NONBLOCK);

            // fall through to connect
        }
        if (-1 == ::connect(m_descriptor, (struct sockaddr*) &m_host, sizeof(m_host))) {
            if (errno == EALREADY || errno == EINPROGRESS) {
                // tell caller to try us again later
                return 0;
            }
            if (errno != EISCONN) {
                // anything else is a real error
                return -1;
            }
        }
        // success!
        return 1;
    }

	void Socket::ThrowMemoryError()
	{
		// todo throw out of memory exception
		// m_toplevel->memoryError->throwError(kOutOfMemoryError);
	}

	//
	// SocketObject
	//
	
	SocketObject::SocketObject(VTable *ivtable,
                               ScriptObject *delegate)
		: ScriptObject(ivtable, delegate),
		  m_socket()
	{
		c.set(&m_socket, sizeof(Socket));
	}

    int SocketObject::connect (String *host, int port)
    {
        return m_socket.connect(host->toUTF8String()->c_str(), port);
    }

    void SocketObject::disconnect ()
    {
        // TODO
    }

    int SocketObject::write (ByteArrayObject *bytes)
    {
        // TODO
        return 0;
    }

    int SocketObject::read (ByteArrayObject *bytes)
	{
        if (m_socket.getDescriptor() < 0) {
            return -1;
        }
        int tot_bytes = 0;

        while (true) {
            int size = ::read(m_socket.getDescriptor(), m_socket.getBuffer(), SOCK_BUF_SZ);
            if (size == 0) {
                // nothing more to read
                return tot_bytes;
            }
            if (size == -1) {
                // let AS3 code throw an IOError
                return -1;
            }
            bytes->GetByteArray().Push(m_socket.getBuffer(), size);
            tot_bytes += size;
        }
	}


	//
	// SocketClass
	//

	SocketClass::SocketClass(VTable *vtable)
		: ClassClosure(vtable)
    {
        prototype = toplevel()->objectClass->construct();
	}

	ScriptObject* SocketClass::createInstance(VTable *ivtable,
												 ScriptObject *prototype)
    {
        return new (core()->GetGC(), ivtable->getExtraSize()) SocketObject(ivtable, prototype);
    }
}	


