//
// $Id: $

# include <sys/types.h>
# include <sys/socket.h>
# include <sys/ioctl.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <fcntl.h>
# include <errno.h>
# include <unistd.h>
# include <netdb.h>

# include "avmthane.h"

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
     */
    int Socket::connect (const char *host_name, int port)
    {
        // see if we've opened up a socket yet
        if (-1 == m_descriptor) {
            // if not, we may still be resolving the address
            struct hostent *resolution = gethostbyname(host_name);
            if (resolution == NULL) {
                if (h_errno == TRY_AGAIN) {
                    // tell caller to hit us again in a little while
                    return 0;
                }
                return -1;
            }

            // address finished resolving
            memset(&m_host, 0, sizeof(m_host));
            m_host.sin_family = resolution->h_addrtype;
            m_host.sin_port = htons(port);
            memcpy(&m_host.sin_addr.s_addr, resolution->h_addr, resolution->h_length);

            // if the address bits all went well, open the socket
            m_descriptor = socket(AF_INET, SOCK_STREAM, 0);
            if (-1 == m_descriptor) {
                return -1;
            }

            // flag it as non-blocking
            int flags = fcntl(m_descriptor, F_GETFL, 0);
            fcntl(m_descriptor, F_SETFL, (flags > 0 ? flags : 0) | O_NONBLOCK);

            // and fall through to connect
        }
        if (-1 == ::connect(m_descriptor, (struct sockaddr*) &m_host, sizeof(m_host))) {
            if (errno == EALREADY || errno == EINPROGRESS) {
                // tell caller to try us again later
                return 0;
            }
            if (errno != EISCONN) {
                // anything else is a real error
                disconnect();
                return -1;
            }
        }
        // success!
        return 1;
    }

    void Socket::disconnect ()
    {
        if (-1 != m_descriptor) {
            close(m_descriptor);
            m_descriptor = -1;
        }
    }

    int Socket::read (ByteArrayFile &bytes)
    {
        if (m_descriptor < 0) {
            return -1;
        }
        int tot_bytes = 0;

        while (true) {
            int size = ::read(m_descriptor, m_buffer, SOCK_BUF_SZ);
            if (size == -1) {
                if (errno != EAGAIN) {
                    // TODO: throw an error
                    return -1;
                }
                // EOF in non-blocking just means there was nothing to read
                size = 0;
            }

            if (size > 0) {
                bytes.Push(m_buffer, size);
                tot_bytes += size;
            }

            if (size < SOCK_BUF_SZ) {
                // nothing more to read
                return tot_bytes;
            }
        }
    }

    int Socket::write (ByteArrayFile &bytes)
    {
        if (m_descriptor < 0) {
            return -1;
        }

        int avail = bytes.Available();
        if (avail == 0) {
            return 0;
        }
        int ptr = bytes.GetFilePointer();
        int size = ::write(m_descriptor, bytes.GetBuffer() + ptr, avail);
        if (size == -1) {
            if (errno != EAGAIN) {
                // TODO: throw an error
                return -1;
            }
            // no non-blocking writes are possible
            size = 0;
        }

        if (size > 0) {
            bytes.Seek(ptr + size);
        }
        return size;
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

    int SocketObject::connect (Stringp host, int port)
    {
        return m_socket.connect(host->toUTF8String()->c_str(), port);
    }

    void SocketObject::disconnect ()
    {
        m_socket.disconnect();
    }

    int SocketObject::write (ByteArrayObject *bytes)
    {
        return m_socket.write(bytes->GetByteArrayFile());
    }

    int SocketObject::read (ByteArrayObject *bytes)
	{
        return m_socket.read(bytes->GetByteArrayFile());
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


