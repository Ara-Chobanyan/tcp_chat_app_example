#include <client/tcp_client.hpp>
#include <thread>

using namespace CHAT;

int main()
{

  const int port{ 3000 };
  TcpClient client{ "localhost", port };

  client.onMessage = [](const std::string& message) { std::cout << message; };

  std::thread onThread{ [&client] { client.run(); } };

  while (true) {
    std::string message{};
    getline(std::cin, message);

    if (message == "\\q") { break; }
    message += "\n";

    client.post(message);
  }

  client.stop();
  onThread.join();
  return 0;
}
