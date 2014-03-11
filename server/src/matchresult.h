#ifndef _MATCH_RESULT_H
#define _MATCH_RESULT_H
#include <ostream>
#include <string>

namespace pma {

class MatchResult {
public:
	const int pos, length;
	MatchResult() : pos(0), length(0) {}
	MatchResult(int p, int l) : pos(p), length(l) {}
	bool operator==(const MatchResult& o) const {
		return o.pos == pos && o.length == length;
	}
	std::string matchstr(const std::string& str) const {
		return str.substr(pos, length);
	}
};
inline std::ostream& operator<<(std::ostream& ost, const MatchResult& o) {
	return ost << '(' << o.pos << ',' << o.length << ')';
}

}

#endif