#ifndef _node_H
#define _node_H
#include <array>
#include <string>
#include <vector>
#include "match_result.h"

namespace pma {

/**
 * パターンマッチングオートマトンのノード。
 * 
 * build関数を用いて生成して下さい。
 * 
 * グラフ内に閉路がある。
 * そのため子をデストラクタで単純にdeleteすると二重に開放して死ぬ。
 * メモリリークが問題になる場合には外部からdeleteするアルゴリズムが必要。
 */
class node {
private:
	node* add_path(const std::string& ptn, size_t pos=0) {
		if (pos == ptn.size()) return this;
		int c = ptn[pos] + 128;
		if (!this->next[c]) {
			this->next[c] = new node;
		}
		return this->next[c]->add_path(ptn, pos + 1);
	}
	void add_accept(int length) {
		this->accept.push_back(length);
	}
public:
	std::array<node*, 0x100> next;
	std::vector<int> accept;
	node() {
		next.fill(nullptr);
	}
	/**
	 * 文字列を追加する。
	 * これを行った後は失敗時の辺を追加する必要がある。
	 */
	void add_pattern(const std::string& ptn) {
		this->add_path(ptn)->add_accept(ptn.size());
	}
	node* nullnext(int c) {
		if (this->next[c]) return this;
		return this->next[128]->nullnext(c);
	}
	/**
	 * text内でnodeが受理する文字列を探す。
	 * 結果は [(pos, length), ...] の形で得られる。
	 */
	std::vector<match_result> match(const std::string& text);
};

/**
 * 文字列パターンからパターンマッチングオートマトンを生成する。
 *
 * 戻り値はヒープ上のルートノード。
 */
node* build(const std::vector<std::string>& ptns);

std::vector<match_result>& overlap_filter(std::vector<match_result>& results);

}

#endif