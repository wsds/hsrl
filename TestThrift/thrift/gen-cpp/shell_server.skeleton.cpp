// This autogenerated skeleton file illustrates how to build a server.
// You should copy it to another filename to avoid overwriting it.

#include "shell.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/TDispatchProcessor.h>

#include "shell_server.skeleton.h"

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

using std::shared_ptr;

using namespace  ::open;

class shellHandler : virtual public shellIf {
 public:
  shellHandler() {
    // Your initialization goes here
  }

  void shell(const std::string& query) {
    // Your implementation goes here
    printf("shell\n");
  }

};

int server_main(int argc, char **argv) {
  int port = 9090;
  shared_ptr<shellHandler> handler(new shellHandler());
  shared_ptr<TProcessor> processor(new shellProcessor(handler));
  shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
  shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
  shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

  TSimpleServer server(processor, serverTransport, transportFactory, protocolFactory);
  server.serve();
  return 0;
}

