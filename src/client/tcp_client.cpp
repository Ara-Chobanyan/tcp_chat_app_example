#include <client/tcp_client.hpp>

namespace CHAT {

TcpClient::TcpClient(const std::string& address, int port)
  : m_socket(m_ioContext)
{

  io::ip::tcp::resolver resolve{ m_ioContext };
  m_endPoints = resolve.resolve(address, std::to_string(port));
};

void TcpClient::run()
{
  io::async_connect(m_socket,
    m_endPoints,
    [this](boost::system::error_code errorCode,
      const io::ip::tcp::endpoint& /*endPoint*/) {
      if (!errorCode) { asyncRead(); }
    });

  m_ioContext.run();
};

void TcpClient::stop()
{
  boost::system::error_code errorCode;
  m_socket.close(errorCode);

  if (errorCode) {
    // process error
  }
};

void TcpClient::post(const std::string& message)
{
  bool queueIdle = m_outGoingMessage.empty();
  m_outGoingMessage.push(message);

  if (queueIdle) { asyncWrite(); }
};

void TcpClient::asyncRead()
{
  io::async_read_until(m_socket,
    m_streamBuf,
    '\n',
    [this](boost::system::error_code errorCode, size_t bytesTransferred) {
      onRead(errorCode, bytesTransferred);
    });
};

void TcpClient::onRead(boost::system::error_code errorCode,
  size_t /*bytesTransferred*/)
{
  if (errorCode) {
    stop();
    return;
  }

  std::stringstream message{};
  message << std::istream{ &m_streamBuf }.rdbuf();
  onMessage(message.str());
  asyncRead();
};

void TcpClient::asyncWrite()
{
  io::async_write(m_socket,
    io::buffer(m_outGoingMessage.front()),
    [this](boost::system::error_code errorCode, size_t bytesTransferred) {
      onWrite(errorCode, bytesTransferred);
    });
};

void TcpClient::onWrite(boost::system::error_code errorCode,
  size_t /*bytesTransferred*/)
{
  if (errorCode) {
    stop();
    return;
  }

  m_outGoingMessage.pop();

  if (!m_outGoingMessage.empty()) { asyncWrite(); }
};

}// namespace CHAT
