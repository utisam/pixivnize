#ifndef _PMA_H
#define _PMA_H
#include <array>
#include <string>
#include <vector>

namespace pma {

/**
 * Node of Pattan Matching Automata
 */
class PMA {
private:
	PMA* add_path(const std::string& ptn, size_t pos=0) {
		if (pos == ptn.size()) return this;
		int c = ptn[pos] + 128;
		if (!this->next[c]) {
			this->next[c] = new PMA;
		}
		return this->next[c]->add_path(ptn, pos + 1);
	}
	void add_accept(int idx) {
		this->accept.push_back(idx);
	}
public:
	std::array<PMA*, 0x100> next;
	std::vector<int> accept;
	PMA() {
		next.fill(nullptr);
	}
	/**
	 * 文字列を追加する。
	 * これを行った後は失敗時の辺を追加する必要がある。
	 */
	void add_pattern(const std::string& ptn, int idx) {
		this->add_path(ptn)->add_accept(idx);
	}
	PMA* nullnext(int c) {
		if (this->next[c]) return this;
		return this->next[128]->nullnext(c);
	}
};

PMA* build(const std::vector<std::string>& ptns);

std::vector<std::pair<int, int>> match(
	PMA* v, std::string t,
	const std::vector<std::string>& ptns);

}

#endif