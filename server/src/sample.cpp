#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "pma.h"
using namespace std;
using namespace pma;

int main(int argc, char* argv[]) {
	if (argc != 2) {
		cout << "Usage: " << argv[0] << " [tags]" << endl;
	}
	ifstream tags(argv[1]);
	vector<string> ptns;
	while (tags) {
		string str;
		getline(tags, str);
		auto pos = str.find('\t');
		string tag = str.substr(0, pos);
		ptns.push_back(tag);
	}
	PMA* pma = build(ptns);
	cout << "complete!" << endl;

	string msg;
	while (cout << "message: ", cin >> msg) {
		for (auto m : pma->match(msg)) {
			cout << m << '\t' << m.matchstr(msg) << endl;
		}
	}
	return 0;
}

