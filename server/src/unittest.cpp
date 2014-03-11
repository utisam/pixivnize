#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>
#include "pma.h"
using namespace std;
using namespace pma;

BOOST_AUTO_TEST_SUITE(pma)

BOOST_AUTO_TEST_CASE(node_ascii) {
	vector<string> ptns = {{
		"ab", "bc", "bab", "d", "abcde",
	}};
	node* node = build(ptns);
	string text = "xbabcdex";
	auto result = node->match(text);
	BOOST_REQUIRE_EQUAL(result[0], match_result(1, 3));
	BOOST_REQUIRE_EQUAL(result[0].matchstr(text), "bab");
	BOOST_REQUIRE_EQUAL(result[1], match_result(2, 5));
	BOOST_REQUIRE_EQUAL(result[1].matchstr(text), "abcde");
}

BOOST_AUTO_TEST_CASE(node_wb) {
	vector<string> ptns = {{
		"あい", "うえ", "あいうえ", "く", "かきくけこ",
	}};
	node* node = build(ptns);
	string text = "あいうえおかきくけこ";
	auto result = node->match(text);
	BOOST_REQUIRE_EQUAL(result[0].matchstr(text), "あい");
	BOOST_REQUIRE_EQUAL(result[1].matchstr(text), "あいうえ");
	BOOST_REQUIRE_EQUAL(result[2].matchstr(text), "かきくけこ");
}

BOOST_AUTO_TEST_SUITE_END()
