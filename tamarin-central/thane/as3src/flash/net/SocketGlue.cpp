//
// $Id: $

# ifdef WIN32
# include <winsock2.h>
# else
# define INVALID_SOCKET -1
# include <sys/types.h>
# include <sys/socket.h>
# include <sys/ioctl.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <fcntl.h>
# include <errno.h>
# include <unistd.h>
# include <netdb.h>
# endif

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
        m_socket_ref = INVALID_SOCKET;
	}

	Socket::~Socket()
	{
		if (m_socket_ref != INVALID_SOCKET) {
# ifdef WIN32
            closesocket(m_socket_ref);
# else
            close(m_socket_ref);
# endif
            m_socket_ref = INVALID_SOCKET;
 		}
	}

    /**
     * Returns -1 for error, 0 for keep trying, 1 for success.
     */
    int Socket::connect (const char *host_name, int port)
    {
        // see if we've opened up a socket yet
        if (INVALID_SOCKET == m_socket_ref) {
            // if not, we may still be resolving the address
            struct hostent *resolution = gethostbyname(host_name);
            if (resolution == NULL) {
# ifdef WIN32
                if (WSAGetLastError() == WSATRY_AGAIN) {
# else
				if (errno == TRY_AGAIN) {
# endif
                    // tell caller to hit us again in a little while
                    return 0;
                }
                return -1;
            }

            // address finished resolving
            memset(&m_host, 0, sizeof(m_host));
            m_host.sin_family = resolution->h_addrtype;
            m_host.sin_port = htons((short) port);
            memcpy(&m_host.sin_addr.s_addr, resolution->h_addr, resolution->h_length);

            // if the address bits all went well, open the socket
            m_socket_ref = socket(AF_INET, SOCK_STREAM, 0);
            if (INVALID_SOCKET == m_socket_ref) {
                return -1;
            }

            // flag it as non-blocking
# ifdef WIN32
			u_long iMode = 1;
			ioctlsocket(m_socket_ref, FIONBIO, &iMode);
# else
            int flags = fcntl(m_socket_ref, F_GETFL, 0);
            fcntl(m_socket_ref, F_SETFL, (flags > 0 ? flags : 0) | O_NONBLOCK);
# endif

            // and fall through to connect
        }
        if (-1 == ::connect(m_socket_ref, (struct sockaddr*) &m_host, sizeof(m_host))) {
# ifdef WIN32
			switch(WSAGetLastError()) {
				case WSAEALREADY: case WSAEINVAL: case WSAEWOULDBLOCK:
# else
			switch(errno) {
				case EALREADY: case EINPROGRESS:
# endif
					// tell caller to try us again later
					return 0;
# ifdef WIN32
				case WSAEISCONN:
# else
				case EISCONN:
# endif
					// this is goodness
					break;
				default:
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
        if (INVALID_SOCKET != m_socket_ref) {
# ifdef WIN32
            closesocket(m_socket_ref);
# else
            close(m_socket_ref);
# endif
            m_socket_ref = INVALID_SOCKET;
        }
    }

    int Socket::read (ByteArrayFile &bytes)
    {
		if (m_socket_ref == INVALID_SOCKET) {
            return -1;
        }
        int tot_bytes = 0;

        while (true) {
            int size = recv(m_socket_ref, (char *) m_buffer, SOCK_BUF_SZ, 0);
            if (size == 0) {
                // size 0 from non-blocking socket means a closed connection
                return -2;
            }
            if (size == -1) {
# ifdef WIN32
				if (WSAGetLastError() != WSAEWOULDBLOCK) {
# else
				if (errno != EWOULDBLOCK) {
# endif
                    return -1;
                }
                // "try again" in non-blocking just means there was nothing to read
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
        if (m_socket_ref < 0) {
            return -1;
        }

        int avail = bytes.Available();
        if (avail == 0) {
            return 0;
        }
        int ptr = bytes.GetFilePointer();
        int size = send(m_socket_ref, (char *) (bytes.GetBuffer() + ptr), avail, 0);
        if (size == -1) {
# ifdef WIN32
				if (WSAGetLastError() != WSAEWOULDBLOCK) {
# else
				if (errno != EWOULDBLOCK) {
# endif
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


