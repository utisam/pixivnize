#include <signal.h>
#include <unistd.h>
#include <array>
#include <iostream>
#include <boost/network/include/http/server.hpp>
#include <boost/network/utils/thread_pool.hpp>
using namespace std;
namespace http = boost::network::http;

class handler_type;

using http_server = http::async_server<handler_type>;

class handler_type {
public:
	static void bad_request(http_server::connection_ptr connection) {
		connection->set_status(http_server::connection::bad_request);
		array<http_server::response_header, 2> headers = {{
			{"Connection", "close"},
			{"Content-Type", "Content-Type: text/html; charset=utf-8"},
		}};
		connection->write("Bad Request");
	}
	static void read_body(boost::iterator_range<char const *> input_range,
				boost::system::error_code error_code, size_t size,
				http_server::connection_ptr connection) {
		cout << size << endl;
		if (size <= (1 << 20)) {
			connection->set_status(http_server::connection::accepted);
			array<http_server::response_header, 2> headers = {{
				{"Connection", "close"},
				{"Content-Type", "Content-Type: application/json; charset=utf-8"},
			}};
			connection->set_headers(headers);
			for (char c : input_range) {
				cout << c;
			}
			cout << endl;
			connection->write("[\"Hello, world!\"]");
		}
		else {
			bad_request(connection);
		}
	}
	void operator()(http_server::request const& request,
					http_server::connection_ptr connection) {
		string ip = source(request);
		string dest = static_cast<string>(http::destination(request));
		cout << "[" << ip << "]: " << dest << endl;

		if (request.method == "POST") {
			connection->read(handler_type::read_body);
		}
		else {
			bad_request(connection);
		}
	}
};

const int THREAD_SIZE = 2;
static http_server* server_ptr;
static void signal_handler(int sig) {
	server_ptr->stop();
}

int main(int arg, char * argv[]) {
	handler_type handler;
	http_server::options options(handler);
	auto pool = boost::make_shared<boost::network::utils::thread_pool>(THREAD_SIZE);
	options.thread_pool(pool);
	http_server server(options.address("0.0.0.0").port("8000"));
	server_ptr = &server;
	
	array<boost::thread, THREAD_SIZE> threads = {{
		boost::thread(boost::bind(&http_server::run, &server)),
		boost::thread(boost::bind(&http_server::run, &server)),
	}};
	if (signal(SIGINT, signal_handler) == SIG_ERR)
		return -1;
	cout << "starting server" << endl;
	server.run();
	for (auto& t : threads) t.join();
	cout << "server has exited" << endl;
	return 0;
}
