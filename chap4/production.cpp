#include "production.h"
#include <algorithm>

Province::Province(const ProvinceData& doc) 
{
	_name = doc.name;
	_totalProduction = 0;
	_demand = doc.demand;
	_price = doc.price;
	for (const auto& producer : doc.producers) 
	{
		addProducer(Producer(this, producer));
	}
}

void Province::addProducer(Producer&& producer) 
{
	_producers.push_back(producer);
	_totalProduction += producer.production();
}

const std::string Province::name() const
{
	return _name;
}


std::vector<Producer>& Province::producers()
{
	return _producers;
}

const int Province::totalProduction() const
{
	return _totalProduction;
}

void Province::setTotalProduction(int aTotalProduction)
{
	_totalProduction = aTotalProduction;
}

const int Province::demand() const
{
	return _demand;
}

void Province::setDemand(const std::string& aDemandStr)
{
	_demand = std::stoi(aDemandStr);
}

const int Province::price() const
{
	return _price;
}

void Province::setPrice(const std::string& aPriceStr)
{
	_price = std::stoi(aPriceStr);
}

const int Province::shortfall() const
{
	return _demand - totalProduction();
}

const int Province::profit()
{
	return demandValue() - demandCost();
}

const int Province::demandCost()
{
	int remainingDemand = demand();
	int result = 0;
	std::sort(_producers.begin(), _producers.end(), [](const Producer& a, const Producer& b) {return a.cost() < b.cost(); });
	for (const auto& p : _producers) {
		int constribution = std::min(remainingDemand, p.production());
		remainingDemand -= constribution;
		result += constribution * p.cost();
	}
	return result;
}

const int Province::demandValue() const
{
	return satisfiedDemand() * price();
}

const int Province::satisfiedDemand() const
{
	return std::min(_demand, totalProduction());
}

Producer::Producer(Province* aProvince, const ProducerData& data)
{
	_province = aProvince;
	_cost = data.cost;
	_name = data.name;
	_production = data.production;
}

const std::string Producer::name() const
{
	return _name;
}

const int Producer::cost() const
{
	return _cost;
}

void Producer::setCost(const std::string& aCostStr)
{
	_cost = std::stoi(aCostStr);
}

const int Producer::production() const
{
	return _production;
}

void Producer::setProduction(const std::string& amountStr)
{
	int amount = std::stoi(amountStr);
	int newProduction = amount;
	_province->setTotalProduction(_province->totalProduction() + newProduction - _production);
	_production = newProduction;
}
