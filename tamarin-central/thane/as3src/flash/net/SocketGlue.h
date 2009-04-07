//
// $Id$
//
// Thane - an enhancement of Mozilla/Adobe's Tamarin project with partial Flash Player compatibility
// Copyright (C) 2008-2009 Three Rings Design, Inc.
//
// Redistribution and use in source and binary forms, with or without modification, are permitted
// provided that the following conditions are met:
// 
// 1. Redistributions of source code must retain the above copyright notice, this list of
//    conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice, this list of
//    conditions and the following disclaimer in the documentation and/or other materials provided
//    with the distribution.
// 
// THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES,
// INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
// PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
// TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
// LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef SOCKETGLUE_INCLUDED
#define SOCKETGLUE_INCLUDED

# ifdef WIN32
# include <winsock2.h>
# else
# include <netinet/in.h>
# include <sys/socket.h>
# endif

namespace avmthane
{

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

		int nb_connect (Stringp host, int port);
        void nb_disconnect ();
        int nb_read (ByteArrayObject *bytes);
        int nb_write (ByteArrayObject *bytes);

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
    };
}

#endif /* SOCKETGLUE_INCLUDED */
