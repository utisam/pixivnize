#ifndef _MATCH_RESULT_H
#define _MATCH_RESULT_H
#include <ostream>
#include <string>

namespace pma {

class match_result {
public:
	const int pos, length;
	match_result() : pos(0), length(0) {}
	match_result(int p, int l) : pos(p), length(l) {}
	bool operator==(const match_result& o) const {
		return o.pos == pos && o.length == length;
	}
	std::string matchstr(const std::string& str) const {
		return str.substr(pos, length);
	}
};
inline std::ostream& operator<<(std::ostream& ost, const match_result& o) {
	return ost << '(' << o.pos << ',' << o.length << ')';
}

}

#endif