#pragma once

#include <boost/asio.hpp>
#include <iostream>
#include <queue>

namespace CHAT {

namespace io = boost::asio;
using messageHandler = std::function<void(std::string)>;

class TcpClient
{
public:
  TcpClient(const std::string& address, int port);

  void run();
  void stop();
  void post(const std::string& message);

private:
  void asyncRead();
  void onRead(boost::system::error_code errorCode, size_t bytesTransferred);
  void asyncWrite();
  void onWrite(boost::system::error_code errorCode, size_t bytesTransferred);

public:
  messageHandler onMessage;

private:
  io::io_context m_ioContext{};
  io::ip::tcp::socket m_socket;

  io::ip::tcp::resolver::results_type m_endPoints;

  const size_t maxSize{ 65536 };
  io::streambuf m_streamBuf{ maxSize };
  std::queue<std::string> m_outGoingMessage{};
};

}// namespace CHAT
