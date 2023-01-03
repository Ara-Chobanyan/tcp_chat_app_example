#pragma once

#include <boost/asio.hpp>
#include <iostream>
#include <memory>
#include <queue>

namespace CHAT {

namespace io = boost::asio;
using boost::asio::ip::tcp;
using messageHandler = std::function<void(std::string)>;
using errorHandler = std::function<void()>;

class TcpConnection : public std::enable_shared_from_this<TcpConnection>
{
public:
  using pointer = std::shared_ptr<TcpConnection>;

  static pointer create(io::ip::tcp::socket&& socket)
  {
    return pointer(new TcpConnection(std::move(socket)));
  };

  inline const std::string& getUserName() const { return m_userName; };
  tcp::socket& socket() { return m_socket; }

  void start(messageHandler&& messageHandler, errorHandler&& errorHandler);
  void post(const std::string& message);

private:
  explicit TcpConnection(io::ip::tcp::socket&& socket);

  // wait for a new message from client
  void asyncRead();
  void onRead(boost::system::error_code errorCode, size_t bytesTransferred);

  void asyncWrite();
  void onWrite(boost::system::error_code errorCode, size_t bytesTransferred);

  tcp::socket m_socket;
  std::string m_userName{};
  std::queue<std::string> m_outgoingMessages{};

  const size_t maxSize{ 65536 };
  io::streambuf m_streamBuf{ maxSize };

  messageHandler m_messageHandler;
  errorHandler m_errorHandler;
};

}// namespace CHAT
