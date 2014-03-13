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
	string text = "xbabcdexBc";
	auto result = node->match(text);
	BOOST_REQUIRE_EQUAL(result[0], match_result(1, 3));
	BOOST_REQUIRE_EQUAL(result[1], match_result(2, 5));
	BOOST_REQUIRE_EQUAL(result[2], match_result(8, 2));
}

BOOST_AUTO_TEST_CASE(node_wb) {
	vector<string> ptns = {{
		"あい", "うえ", "あいうえ", "く", "かきくけこ",
	}};
	node* node = build(ptns);
	string text = "あいうえおかきくけこ";
	auto result = node->match(text);
	BOOST_REQUIRE_EQUAL(result[0], match_result(0, 2));
	BOOST_REQUIRE_EQUAL(result[1], match_result(0, 4));
	BOOST_REQUIRE_EQUAL(result[2], match_result(5, 5));
}

BOOST_AUTO_TEST_CASE(overlap) {
	vector<match_result> results = {
		match_result(0, 1),
		match_result(1, 2),
		match_result(1, 5),
		match_result(3, 2),
		match_result(4, 2),
		match_result(6, 2),
	};
	overlap_filter(results);
	BOOST_REQUIRE_EQUAL(results.size(), 3);
	BOOST_REQUIRE_EQUAL(results[0], match_result(0, 1));
	BOOST_REQUIRE_EQUAL(results[1], match_result(1, 5));
	BOOST_REQUIRE_EQUAL(results[2], match_result(6, 2));
}

BOOST_AUTO_TEST_SUITE_END()
