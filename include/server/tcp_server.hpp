#pragma once

#include <boost/asio.hpp>
#include <functional>
#include <iostream>
#include <optional>
#include <server/tcp_connection.hpp>
#include <unordered_set>

namespace CHAT {
enum class IPV { V4, V6 };

// using boost::asio::ip::tcp;
namespace io = boost::asio;

class TcpServer
{
  using onJoinHandler = std::function<void(TcpConnection::pointer)>;
  using onLeaveHandler = std::function<void(TcpConnection::pointer)>;
  using onClientMessageHandler = std::function<void(std::string)>;

public:
  TcpServer(IPV ipv, int port);

  int run();
  void broadCast(const std::string& message);

  onJoinHandler onJoin;
  onJoinHandler onLeave;
  onClientMessageHandler onClientMessage;

private:
  void startAccept();

  IPV m_ipVersion;
  int m_port;

  io::io_context m_ioContext;
  io::ip::tcp::acceptor m_acceptor;
  std::optional<io::ip::tcp::socket> m_socket;
  std::unordered_set<TcpConnection::pointer> m_connections{};
};

}// namespace CHAT
