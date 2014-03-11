#include <iostream>
#include <queue>
#include "pma.h"
using namespace std;

namespace pma {

PMA* build(const vector<string>& ptns) {
	PMA* root = new PMA;
	for (int i = 0; i < ptns.size(); ++i) {
		root->add_pattern(ptns[i]);
	}
	cout << "add accept" << endl;
	// make failure link
	queue<PMA*> que;
	for (int c = 0; c < 255; ++c) {
		if (c != 128 && root->next[c]) {
			root->next[c]->next[128] = root;
			que.push(root->next[c]);
		}
		else {
			root->next[c] = root;
		}
	}
	cout << "failure from root" << endl;
	while (!que.empty()) {
		PMA* t = que.front();
		que.pop();
		for (int c = 0; c < 255; ++c) {
			if (c != 128 && t->next[c]) {
				que.push(t->next[c]);
				PMA* r = t->next[128]->nullnext(c);
				t->next[c]->next[128] = r->next[c];
			}
		}
	}
	cout << "failure" << endl;
	return root;
}

vector<MatchResult> PMA::match(const string& text) {
	vector<MatchResult> result;
	PMA* v = this;
	for (int i = 0; i < text.size(); ++i) {
		int c = text[i] + 128;
		v = v->nullnext(c)->next[c];
		for (int length : v->accept) {
			result.emplace_back(i - length + 1, length);
		}
	}
	return result;
}

}