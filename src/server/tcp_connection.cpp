#include <iostream>
#include <server/tcp_connection.hpp>

namespace CHAT {

TcpConnection::TcpConnection(io::ip::tcp::socket&& socket)
  : m_socket(std::move(socket))
{
  boost::system::error_code errorCode;
  std::stringstream name;
  name << m_socket.remote_endpoint();
  m_userName = name.str();
};

void TcpConnection::start(messageHandler&& messageHandler,
  errorHandler&& errorHandler)
{
  m_messageHandler = std::move(messageHandler);
  m_errorHandler = std::move(errorHandler);

  asyncRead();
};

void TcpConnection::post(const std::string& message)
{
  bool queueIdle = m_outgoingMessages.empty();
  m_outgoingMessages.push(message);

  if (queueIdle) { asyncWrite(); }
};

void TcpConnection::asyncRead()
{
  io::async_read_until(m_socket,
    m_streamBuf,
    "\n",
    [self = shared_from_this()](boost::system::error_code errorCode,
      size_t bytesTransferred) { self->onRead(errorCode, bytesTransferred); });
};

void TcpConnection::onRead(boost::system::error_code errorCode,
  size_t bytesTransferred)
{
  if (errorCode) {
    m_socket.close(errorCode);

    m_errorHandler();
    return;
  }

  std::stringstream message{};
  message << m_userName << ": " << std::istream(&m_streamBuf).rdbuf();
  // redundent rdbuf does this for us under the hood
  m_streamBuf.consume(bytesTransferred);

  m_messageHandler(message.str());
  asyncRead();
};

void TcpConnection::asyncWrite()
{
  io::async_write(m_socket,
    io::buffer(m_outgoingMessages.front()),
    [self = shared_from_this()](boost::system::error_code errorCode,
      size_t bytesTransferred) { self->onWrite(errorCode, bytesTransferred); });
};

void TcpConnection::onWrite(boost::system::error_code errorCode,
  size_t bytesTransferred)
{
  if (errorCode) {
    m_socket.close(errorCode);

    m_errorHandler();
    return;
  }

  m_outgoingMessages.pop();

  if (!m_outgoingMessages.empty()) { asyncWrite(); }
};
}// namespace CHAT
