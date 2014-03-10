#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>
#include "pma.h"

BOOST_AUTO_TEST_SUITE(pma)

BOOST_AUTO_TEST_CASE(pma_ascii) {
	std::vector<std::string> ptns = {{
		"ab", "bc", "bab", "d", "abcde",
	}};
	PMA* pma = buildPMA(ptns);
	auto result = match(pma, "xbabcdex", ptns);
	BOOST_REQUIRE_EQUAL(result[0].first, 1);
	BOOST_REQUIRE_EQUAL(result[0].second, 3);
	BOOST_REQUIRE_EQUAL(result[1].first, 2);
	BOOST_REQUIRE_EQUAL(result[1].second, 5);
}

BOOST_AUTO_TEST_CASE(pma_wb) {
	std::vector<std::string> ptns = {{
		"あい", "うえ", "あいうえ", "く", "かきくけこ",
	}};
	PMA* pma = buildPMA(ptns);
	std::string text = "あいうえおかきくけこ";
	auto result = match(pma, text, ptns);
	BOOST_REQUIRE_EQUAL(text.substr(result[0].first, result[0].second), "あい");
	BOOST_REQUIRE_EQUAL(text.substr(result[1].first, result[1].second), "あいうえ");
	BOOST_REQUIRE_EQUAL(text.substr(result[2].first, result[2].second), "かきくけこ");
}

BOOST_AUTO_TEST_SUITE_END()
