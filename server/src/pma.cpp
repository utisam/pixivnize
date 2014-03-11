#include <queue>
#include "pma.h"
using namespace std;

namespace pma {

node* build(const vector<string>& ptns) {
	node* root = new node;
	for (int i = 0; i < ptns.size(); ++i) {
		root->add_pattern(ptns[i]);
	}
	// make failure link
	queue<node*> que;
	for (int c = 0; c < 255; ++c) {
		if (c != 128 && root->next[c]) {
			root->next[c]->next[128] = root;
			que.push(root->next[c]);
		}
		else {
			root->next[c] = root;
		}
	}
	while (!que.empty()) {
		node* t = que.front();
		que.pop();
		for (int c = 0; c < 255; ++c) {
			if (c != 128 && t->next[c]) {
				que.push(t->next[c]);
				node* r = t->next[128]->nullnext(c);
				t->next[c]->next[128] = r->next[c];
			}
		}
	}
	return root;
}

vector<match_result> node::match(const string& text) {
	vector<match_result> result;
	node* v = this;
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