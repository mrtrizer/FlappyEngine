#include "catch.h"
#include "fakeit.h"

#include <memory>

#include <Entity.h>
#include <boost/asio.hpp>
#include <boost/array.hpp>

using namespace flappy;
using namespace std;
using boost::asio::ip::tcp;

void test() {
    try
      {

        boost::asio::io_service io_service;

        tcp::resolver resolver(io_service);
        tcp::resolver::query query("129.6.15.28", "daytime");
        tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

        tcp::socket socket(io_service);
        boost::asio::connect(socket, endpoint_iterator);

        for (;;)
        {
          boost::array<char, 128> buf;
          boost::system::error_code error;

          size_t len = socket.read_some(boost::asio::buffer(buf), error);

          if (error == boost::asio::error::eof)
            break; // Connection closed cleanly by peer.
          else if (error)
            throw boost::system::system_error(error); // Some other error.

          std::cout.write(buf.data(), len);
        }
      }
      catch (std::exception& e)
      {
        std::cerr << e.what() << std::endl;
      }
}

TEST_CASE( "BoostAsio::asio()") {
    test();
}
