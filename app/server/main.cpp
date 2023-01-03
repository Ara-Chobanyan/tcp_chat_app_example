#include <server/tcp_server.hpp>

int main()
{
  static const int port{ 3000 };
  CHAT::TcpServer server{ CHAT::IPV::V4, port };

  server.onJoin = [](const CHAT::TcpConnection::pointer& server) {
    std::cout << "User has joined the server: " << server->getUserName()
              << '\n';
  };

  server.onLeave = [](const CHAT::TcpConnection::pointer& server) {
    std::cout << "User has left the server: " << server->getUserName() << '\n';
  };

  server.onClientMessage = [&server](const std::string& message) {
    // parse the message
    // do game server things
    // send message to client
    server.broadCast(message);
  };

  server.run();

  return 0;
}
