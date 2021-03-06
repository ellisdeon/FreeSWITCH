#ifndef SERVER_PSTREAM_HPP_INCLUDED
#define SERVER_PSTREAM_HPP_INCLUDED

#ifdef WIN32
#include <winsock.h>  /* For XMLRPC_SOCKET (= SOCKET) */
typedef int socklen_t;
#else
#include <sys/socket.h>
#endif

#include <xmlrpc-c/config.h>  /* For XMLRPC_SOCKET */
#include <xmlrpc-c/c_util.h>
#include <xmlrpc-c/registry.hpp>
#include <xmlrpc-c/packetsocket.hpp>

namespace xmlrpc_c {

class XMLRPC_DLLEXPORT serverPstreamConn {

public:

    struct constrOpt_impl;

    class XMLRPC_DLLEXPORT constrOpt {
    public:
        constrOpt();
        ~constrOpt();

        constrOpt & registryPtr       (xmlrpc_c::registryPtr      const& arg);
        constrOpt & registryP         (const xmlrpc_c::registry * const& arg);
        constrOpt & socketFd          (XMLRPC_SOCKET  const& arg);

    private:
        struct constrOpt_impl * implP;
        friend class serverPstreamConn;
    };

    serverPstreamConn(constrOpt const& opt);

    ~serverPstreamConn();

    void
    runOnce(xmlrpc_c::callInfo * const callInfoP,
            volatile const int * const interruptP,
            bool *               const eofP);

    void
    runOnce(volatile const int * const interruptP,
            bool *               const eofP);

    void
    runOnce(bool * const eofP);

    void
    runOnceNoWait(callInfo * const callInfoP,
                  bool *     const eofP,
                  bool *     const didOneP);

    void
    runOnceNoWait(bool * const eofP,
                  bool * const didOneP);

    void
    runOnceNoWait(bool * const eofP);

    void
    run(xmlrpc_c::callInfo * const callInfoP,
        volatile const int * const interruptP);

    void
    run(volatile const int * const interruptP);

    void
    run();

private:
    struct serverPstreamConn_impl * implP;
};


class XMLRPC_DLLEXPORT serverPstream {

public:

    struct constrOpt_impl;

    class XMLRPC_DLLEXPORT constrOpt {
    public:
        constrOpt();
        ~constrOpt();

        constrOpt & registryPtr       (xmlrpc_c::registryPtr      const& arg);
        constrOpt & registryP         (const xmlrpc_c::registry * const& arg);
        constrOpt & socketFd          (XMLRPC_SOCKET  const& arg);

    private:
        struct constrOpt_impl * implP;
        friend class serverPstream;
    };

    serverPstream(constrOpt const& opt);

    virtual ~serverPstream();  // This makes it polymorphic

    void
    runSerial(volatile const int * const interruptP);

    void
    runSerial();

    void
    terminate();
    
    class shutdown : public xmlrpc_c::registry::shutdown {
    public:
        shutdown(xmlrpc_c::serverPstream * const severAbyssP);
        virtual ~shutdown();
        void doit(std::string const& comment, void * const callInfo) const;
    private:
        xmlrpc_c::serverPstream * const serverPstreamP;
    };

private:
    struct serverPstream_impl * implP;
};

// Note: there is no xmlrpc_c::callInfo_serverPstreamConn .  That's
// because the serverPstreamConn server is so low-level that the user
// defines his own derived class of xmlrpc_c::callInfo.  He creates an
// object of that class and passes it to the 'runOnce' method.  The
// server then passes it on through to the user's XML-RPC method 
// execute() method.

class XMLRPC_DLLEXPORT callInfo_serverPstream : public xmlrpc_c::callInfo {
/*----------------------------------------------------------------------------
   This is information about how an XML-RPC call arrived to the server.  It is
   available to the user's XML-RPC method execute() method, so for example an
   XML-RPC method might execute differently depending upon the IP address of
   the client.

   This is for a user of a xmlrpc_c::serverPstream server.
-----------------------------------------------------------------------------*/
public:
    callInfo_serverPstream(
        xmlrpc_c::serverPstream * const serverP,
        struct sockaddr           const clientAddr,
        socklen_t                 const clientAddrSize);

    xmlrpc_c::serverPstream * const serverP;
        // The server that is processing the RPC.
    struct sockaddr const clientAddr;
        // The address (typically, IP address and TCP port) of the XML-RPC
        // client.  This is a Unix OS type.
    socklen_t const clientAddrSize;
        // Size in bytes of the valid part of 'clientAddr'.  (Usually implied
        // by type of socket, as well as the address type member of
        // 'clientAddr', but here because it's technically part of the POSIX
        // socket interface).
};


} // namespace

#endif
