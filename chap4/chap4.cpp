#include <catch2/catch_test_macros.hpp>

#include "production.h"
ProvinceData sampleProvinceData() 
{
	return { "Asia",{{"Byzantium",10,9},{"Attalia",12,10},{"Sinope",10,6}},30,20 };
}

TEST_CASE("province")
{
	Province asia(sampleProvinceData());
	SECTION("shortfall")
	{
		REQUIRE(asia.shortfall() == 5);
	}
	SECTION("profit")
	{
		REQUIRE(asia.profit() == 230);
	}
	SECTION("change production")
	{
		asia.producers()[0].setProduction("20");
		REQUIRE(asia.shortfall() == -6);
		REQUIRE(asia.profit() == 292);
	}
	SECTION("zero demand")
	{
		asia.setDemand("0");
		REQUIRE(asia.shortfall() == -25);
		REQUIRE(asia.profit() == 0);
	}
	SECTION("negative demand")
	{
		asia.setDemand("-1");
		REQUIRE(asia.shortfall() == -26);
		REQUIRE(asia.profit() == -10);
	}
	SECTION("empty string demand")
	{
		REQUIRE_THROWS_AS(asia.setDemand(""), std::exception);
	}
}

TEST_CASE("no producers")
{
	Province noProducers({ "No producers",{},30,20 });	
	SECTION("shortfall")
	{
		REQUIRE(noProducers.shortfall() == 30);
	}
	SECTION("profit")
	{
		REQUIRE(noProducers.profit() == 0);
	}
}