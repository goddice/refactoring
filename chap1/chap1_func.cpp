#include "nlohmann/json.hpp"
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <locale>

class Play {
public:
	std::string name;
	std::string type;
	Play() : name(""), type("") {}
	Play(const std::string& name_, const std::string& type_) : name(name_), type(type_) {}
};


class Performance {
public:
	std::string playID;
	int audience;
	Performance(const std::string& playID_, int audience_) : playID(playID_), audience(audience_) {}
};

class Invoice {
public:
	std::string customer;
	std::vector<Performance> performances;
	Invoice(const std::string& customer_) : customer(customer_) {}
	void addPerformance(const std::string& playID, int audience) {
		performances.emplace_back(playID, audience);
	}
};

std::string statement(Invoice& invoice, std::unordered_map<std::string, Play>& plays) {

	auto playFor = [&](const Performance& aPerformance) {
		return plays[aPerformance.playID];
	};

	auto usd = [&](int aNumber) {
		std::stringstream ss;
		ss.imbue(std::locale("en_US.UTF-8"));
		ss << std::showbase << std::put_money(aNumber);
		return ss.str();
	};

	auto volumeCreditsFor = [&](const Performance& aPerformance) {
		int result = 0;
		result += std::max(aPerformance.audience - 30, 0);
		if ("comedy" == playFor(aPerformance).type) {
			result += std::floor(aPerformance.audience / 5.0);
		}
		return result;
	};

	auto totalVolumeCredits = [&]() {
		int result = 0;
		for (auto& perf : invoice.performances) {
			result += volumeCreditsFor(perf);
		}
		return result;
	};

	auto amountFor = [&](const Performance& aPerformance) {
		int result = 0;

		if (playFor(aPerformance).type == "tragedy") {
			result = 40000;
			if (aPerformance.audience > 30) {
				result += 1000 * (aPerformance.audience - 30);
			}
		}
		else if (playFor(aPerformance).type == "comedy") {
			result = 30000;
			if (aPerformance.audience > 20) {
				result += 10000 + 500 * (aPerformance.audience - 20);
			}
			result += 300 * aPerformance.audience;
		}
		else {
			throw std::format("unknown type: {}", playFor(aPerformance).type);
		}

		return result;
	};

	auto totalAmount = [&]() {
		int result = 0;
		for (auto& perf : invoice.performances) {
			result += amountFor(perf);
		}
		return result;
	};

	std::string result = std::format("Statement for {}\n", invoice.customer);
	for (auto& perf : invoice.performances) {
		result += std::format("  {}: {} ({} seats)\n", playFor(perf).name, usd(amountFor(perf)), perf.audience);
	}
	result += std::format("Amount owed is {}\n", usd(totalAmount()));
	result += std::format("You earned {} credits\n", totalVolumeCredits());
	return result;
}

int main() {
	nlohmann::json plays_js = R"({
	  "hamlet": {
		"name": "Hamlet",
		"type": "tragedy"
	  },
	  "as-like": {
		"name": "As You Like It",
		"type": "comedy"
	  },
	  "othello": {
		"name": "Othello",
		"type": "tragedy"
	  }
	})"_json;

	nlohmann::json invoices_js = R"([
	  {
		"customer": "BigCo",
		"performances": [
		  {
			"playID": "hamlet",
			"audience": 55
		  },
		  {
			"playID": "as-like",
			"audience": 35
		  },
		  {
			"playID": "othello",
			"audience": 40
		  }
		]
	  }
	])"_json;

	std::unordered_map<std::string, Play> plays;
	for (auto& [key, value] : plays_js.items()) {
		plays[key] = Play(value["name"], value["type"]);
	}

	std::vector<Invoice> invoices;
	for (auto item : invoices_js) {
		Invoice inv(item["customer"]);
		for (auto perf : item["performances"]) {
			inv.addPerformance(perf["playID"], perf["audience"]);
		}
		invoices.emplace_back(inv);
	}

	for (auto& invoice : invoices) {
		std::cout << statement(invoice, plays);
	}

	return 0;
}