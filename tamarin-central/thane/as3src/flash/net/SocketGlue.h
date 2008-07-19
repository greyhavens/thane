//
// $Id: $

#ifndef SOCKETGLUE_INCLUDED
#define SOCKETGLUE_INCLUDED

namespace thane
{

# ifdef WIN32
# include <winsock2.h>
# else
# include <netinet/in.h>
# include <sys/socket.h>
# endif

# define SOCK_BUF_SZ 0x1000

	class Socket
	{
	public:
		Socket();
		~Socket();

        int connect (const char *host_name, int port);
        void disconnect ();
        int read (ByteArrayFile &bytes);
        int write (ByteArrayFile &bytes);

	protected:
# ifdef WIN32
		SOCKET m_socket_ref;
# else
        int m_socket_ref;
# endif
        U8 m_buffer[SOCK_BUF_SZ];
        sockaddr_in m_host;
		void ThrowMemoryError();
	};

	class SocketObject : public ScriptObject
	{
	public:
		SocketObject(VTable *ivtable, ScriptObject *delegate);

		int connect (Stringp host, int port);
        void disconnect ();
        int read (ByteArrayObject *bytes);
        int write (ByteArrayObject *bytes);

		Socket& GetSocket() { return m_socket; }

	private:
		MMgc::Cleaner c;
        Socket m_socket;
	};

	//
	// SocketClass
	//
	
	class SocketClass : public ClassClosure
	{
    public:
		SocketClass(VTable *vtable);

		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);

		SocketObject *readFile(Stringp filename);

		DECLARE_NATIVE_MAP(SocketClass)
    };
}

#endif /* SOCKETGLUE_INCLUDED */
