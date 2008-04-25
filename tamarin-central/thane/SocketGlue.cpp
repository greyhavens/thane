//
// $Id: $

# include <sys/types.h>
# include <sys/socket.h>
# include <sys/ioctl.h>
# include <unistd.h>

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

    bool SocketObject::connect (String *host, int port)
    {
        // TODO
        return false;
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


