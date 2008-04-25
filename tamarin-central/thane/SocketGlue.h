//
// $Id: $

#ifndef SOCKETGLUE_INCLUDED
#define SOCKETGLUE_INCLUDED

namespace thane
{
# define SOCK_BUF_SZ 0x1000

	class Socket
	{
	public:
		Socket();
		~Socket();

        int getDescriptor () { return m_descriptor; }
        U8 *getBuffer () { return m_buffer; }

	protected:
        U8 m_buffer[SOCK_BUF_SZ];
        int m_descriptor;

		void ThrowMemoryError();
	};

	class SocketObject : public ScriptObject
	{
	public:
		SocketObject(VTable *ivtable, ScriptObject *delegate);

		bool connect (String *host, int port);
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
