#include <boost/system/system_error.hpp>
#include <server/tcp_connection.hpp>
#include <server/tcp_server.hpp>

namespace CHAT {

using boost::asio::ip::tcp;
//---------------------------------------------------------------------------
TcpServer::TcpServer(IPV ipv, int port)
  : m_ipVersion(ipv), m_port(port),
    m_acceptor(m_ioContext,
      tcp::endpoint(m_ipVersion == IPV::V4 ? tcp::v4() : tcp::v6(), m_port)){};

//---------------------------------------------------------------------------
int TcpServer::run()
{
  try {
    startAccept();
    m_ioContext.run();
  } catch (std::exception& e) {
    std::cerr << e.what() << '\n';
    return -1;
  }
  return 0;
};

//---------------------------------------------------------------------------
void TcpServer::broadCast(const std::string& message)
{
  for (const auto& connection : m_connections) { connection->post(message); }
};

//---------------------------------------------------------------------------
void TcpServer::startAccept()
{
  m_socket.emplace(m_ioContext);

  m_acceptor.async_accept(
    *m_socket, [this](const boost::system::error_code& error) {
      auto connection = TcpConnection::create(std::move(*m_socket));

      if (onJoin) { onJoin(connection); }

      m_connections.insert(connection);

      if (!error) {
        connection->start(
          [this](const std::string& message) {
            if (onClientMessage) { onClientMessage(message); }
          },
          [&, weak = std::weak_ptr(connection)] {
            if (auto shared = weak.lock();
                shared && (m_connections.erase(shared) != 0U)) {
              if (onLeave) { onLeave(shared); }
            }
          });
      }
      startAccept();
    });
}

//---------------------------------------------------------------------------
}// namespace CHAT
