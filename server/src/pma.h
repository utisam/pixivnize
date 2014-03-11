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
	void add_accept(int length) {
		this->accept.push_back(length);
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
	void add_pattern(const std::string& ptn) {
		this->add_path(ptn)->add_accept(ptn.size());
	}
	PMA* nullnext(int c) {
		if (this->next[c]) return this;
		return this->next[128]->nullnext(c);
	}
	/**
	 * text内でPMAが受理する文字列を探す。
	 * 結果は [(pos, length), ...] の形で得られる。
	 */
	std::vector<std::pair<int, int>> match(const std::string& text);
};

/**
 * 文字列パターンからPMAを生成する
 */
PMA* build(const std::vector<std::string>& ptns);

}

#endif