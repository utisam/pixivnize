#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "pma.h"
using namespace std;

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
	PMA* pma = buildPMA(ptns);
	string msg;
	cin >> msg;
	for (auto p : match(pma, msg, ptns)) {
		string t = msg.substr(p.first, p.second);
		cout << p.first << ':' << p.second << '\t' << t << endl;
	}
	return 0;
}

