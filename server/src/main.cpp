#include <signal.h>
#include <unistd.h>
#include <iostream>
#include <boost/network/include/http/server.hpp>
#include <boost/network/utils/thread_pool.hpp>
namespace http = boost::network::http;

class handler_type;

using http_server = http::async_server<handler_type>;

class handler_type {
public:
	static void read_body(boost::iterator_range<char const *> input_range,
				boost::system::error_code error_code, size_t size,
				http_server::connection_ptr connection) {
	}
	void operator()(http_server::request const& request,
					http_server::connection_ptr connection) {
		connection->read(handler_type::read_body);
		std::string ip = source(request);
		std::cout << "[" << ip << "]: " << static_cast<std::string>(http::destination(request)) << std::endl;

		http_server::response_header headers[] = {
			{"Connection","close"},
			{"Content-Type", "text/html"},
		};
		connection->set_headers(boost::make_iterator_range(headers, headers + 2));
		connection->set_status(http_server::connection::accepted);
		connection->write("Hello, world!");
	}
};

static http_server* server_ptr;
static void signal_handler(int sig) {
	server_ptr->stop();
}

int main(int arg, char * argv[]) {
	handler_type handler;
	http_server::options options(handler);
	options.thread_pool(boost::make_shared<boost::network::utils::thread_pool>(2));
	http_server server(options.address("0.0.0.0").port("8000"));
	server_ptr = &server;
	
	boost::thread t1(boost::bind(&http_server::run, &server));
	boost::thread t2(boost::bind(&http_server::run, &server));
	if (signal(SIGINT, signal_handler) == SIG_ERR)
		return -1;
	std::cout << "start server" << std::endl;
	server.run();
	t1.join();
	t2.join();
	std::cout << "server has exited" << std::endl;
	return 0;
}
