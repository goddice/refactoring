#pragma once

#include <string>
#include <vector>

class ProducerData
{
public:
	std::string name;
	int cost;
	int production;
	ProducerData(const std::string& aName, int aCost, int aProduction) : name(aName), cost(aCost), production(aProduction) {}
	ProducerData(const std::string& aName, int aCost) : name(aName), cost(aCost), production(0) {}
};

class ProvinceData
{
public:
	std::string name;
	std::vector<ProducerData> producers;
	int demand;
	int price;
	ProvinceData(const std::string& aName, const std::vector<ProducerData>& aProducerDataList, int aDemand, int aPrice):
		name(aName), producers(aProducerDataList), demand(aDemand), price(aPrice) {}
};

class Producer;

class Province
{
public:
	Province(const ProvinceData& doc);
	void addProducer(const Producer& producer);
	const std::string name() const;
	const std::vector<Producer> producers() const;
	const int totalProduction() const;
	void setTotalProduction(int aTotalProduction);
	const int demand() const;
	void setDemand(const std::string& aDemandStr);
	const int price() const;
	void setPrice(const std::string& aPriceStr);
	const int shortfall() const;
	const int profit();
	const int demandCost();
	const int demandValue() const;
	const int satisfiedDemand() const;
private:
	std::string _name;
	std::vector<Producer> _producers;
	int _totalProduction;
	int _demand;
	int _price;
};

class Producer 
{
public:
	Producer(Province* aProvince, const ProducerData& data);
	const std::string name() const;
	const int cost() const;
	void setCost(const std::string& aCostStr);
	const int production() const;
	void setProduction(const std::string& amountStr);
private:
	Province* _province;
	int _cost;
	std::string _name;
	int _production;
};