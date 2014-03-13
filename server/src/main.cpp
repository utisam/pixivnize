#include <boost/network/protocol/http/server.hpp>
#include <iostream>
#include <fstream>
#include "pma.h"
using namespace std;
using namespace pma;
namespace http = boost::network::http;

class handler_type;

using http_server = http::server<handler_type>;

node* pma_root;

class handler_type {
public:
	void operator()(http_server::request const &request,
					http_server::response &response) {
		const string& src = request.source;
		const string& dest = request.destination;
		const string& method = request.method;
		unsigned int majour = request.http_version_major;
		unsigned int minor = request.http_version_minor;
		cout << src << " -- " << method << ' ' << dest << " HTTP" << majour << '.' << minor << endl;

		string result;
		auto results = pma_root->match(request.body);
		overlap_filter(results);
		for (auto m : results) {
			string data = to_string(m.pos) + ',' + to_string(m.length) + '\n';
			result += data;
		}

		response = http_server::response::stock_reply(
			http_server::response::ok, result);
	}
	void log(http_server::string_type const &info) {
		std::cerr << "ERROR: " << info << endl;
	}
};

void init_pma_root(const string filename) {
	ifstream tags(filename);
	vector<string> ptns;
	string line;
	while (getline(tags, line)) {
		// string tag = line.substr(0, line.find('\t'));
		ptns.push_back(line);
	}
	pma_root = build(ptns);
}

int main(int argc, char * argv[]) {
	if (argc != 2) {
		cout << "Usage: " << argv[0] << " [tags]" << endl;
	}
	init_pma_root(argv[1]);
	cout << "complete!" << endl;

	handler_type handler;
	http_server::options options(handler);
	http_server server(options.address("0.0.0.0").port("8000"));
	server.run();
}
