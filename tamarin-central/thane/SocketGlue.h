//
// $Id: $

#ifndef SOCKETGLUE_INCLUDED
#define SOCKETGLUE_INCLUDED

namespace thane
{
# include <netinet/in.h>
# include <sys/socket.h>

# define SOCK_BUF_SZ 0x1000

	class Socket
	{
	public:
		Socket();
		~Socket();

        int connect (const char *host, int port);
        void disconnect ();
        int read (ByteArrayFile &bytes);
        int write (ByteArrayFile &bytes);

	protected:
        U8 m_buffer[SOCK_BUF_SZ];
        int m_descriptor;
        sockaddr_in m_host;

		void ThrowMemoryError();
	};

	class SocketObject : public ScriptObject
	{
	public:
		SocketObject(VTable *ivtable, ScriptObject *delegate);

		int connect (String *host, int port);
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
