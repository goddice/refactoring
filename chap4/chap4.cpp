#include <catch2/catch_test_macros.hpp>

#include "production.h"
ProvinceData sampleProvinceData() 
{
	return { "Asia",{{"Byzantium",10,9},{"Attalia",12,10},{"Sinope",10,6}},30,20 };
}

TEST_CASE("province")
{
	SECTION("shortfall")
	{
		Province asia(sampleProvinceData());
		REQUIRE(asia.shortfall() == 5);
	}
	SECTION("profit")
	{
		Province asia(sampleProvinceData());
		REQUIRE(asia.profit() == 230);
	}
}