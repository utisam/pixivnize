#ifndef _PMA_H
#define _PMA_H
#include <array>
#include <queue>
#include <vector>

/**
 * Pattan Matching Automata
 */
class PMA {
public:
	std::array<PMA*, 0x100> next;
	std::vector<int> accept;
	PMA() {
		next.fill(nullptr);
	}
};

PMA* buildPMA(const std::vector<std::string>& ptns) {
	PMA* root = new PMA;
	for (int i = 0; i < ptns.size(); ++i) {
		PMA* t = root;
		for (int c : ptns[i]) {
			c += 128;
			if (!t->next[c]) {
				t->next[c] = new PMA;
			}
			t = t->next[c];
		}
		t->accept.push_back(i);
	}
	std::cout << "add accept" << std::endl;
	// make failure link
	std::queue<PMA*> que;
	for (int c = 0; c < 255; ++c) {
		if (c != 128 && root->next[c]) {
			root->next[c]->next[128] = root;
			que.push(root->next[c]);
		}
		else {
			root->next[c] = root;
		}
	}
	std::cout << "failure root" << std::endl;
	while (!que.empty()) {
		PMA* t = que.front();
		que.pop();
		for (int c = 0; c < 255; ++c) {
			if (c != 128 && t->next[c]) {
				que.push(t->next[c]);
				PMA* r = t->next[128];
				while (!r->next[c]) r = r->next[128];
				t->next[c]->next[128] = r->next[c];
			}
		}
	}
	std::cout << "failure" << std::endl;
	return root;
}

std::vector<std::pair<int, int>> match(PMA* v, std::string t, const std::vector<std::string>& ptns) {
	std::vector<std::pair<int, int>> result;
	for (int i = 0; i < t.size(); ++i) {
		int c = t[i] + 128;
		while (!v->next[c]) v = v->next[128];
		v = v->next[c];
		for (int ac : v->accept) {
			const std::string& m = ptns[ac];
			result.push_back(std::make_pair(i - m.size() + 1, m.size()));
		}
	}
	return result;
}

#endif