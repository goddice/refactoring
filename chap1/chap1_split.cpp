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
	Performance() : playID(""), audience(0) {};
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

class EnrichedPerformance : public Performance {
public:
	Play play;
	int amount;
	int volumeCredits;
};

class StatementData {
public:
	std::string customer;
	std::vector<EnrichedPerformance> performances;
	int totalAmount;
	int totalVolumeCredits;
};

StatementData createStatementData(Invoice& invoice, std::unordered_map<std::string, Play>& plays) {
	auto playFor = [&](const Performance& aPerformance) {
		return plays[aPerformance.playID];
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

	auto volumeCreditsFor = [&](const Performance& aPerformance) {
		int result = 0;
		result += std::max(aPerformance.audience - 30, 0);
		if ("comedy" == playFor(aPerformance).type) {
			result += std::floor(aPerformance.audience / 5.0);
		}
		return result;
	};

	auto totalVolumeCredits = [&](const StatementData& data) {
		return std::reduce(
			data.performances.begin(),
			data.performances.end(),
			0,
			[](int sum, const EnrichedPerformance& perf) {
				return sum + perf.volumeCredits;
			}
		);
	};

	auto totalAmount = [&](const StatementData& data) {
		return std::reduce(
			data.performances.begin(),
			data.performances.end(),
			0,
			[](int sum, const EnrichedPerformance& perf) {
				return sum + perf.amount;
			}
		);
	};

	StatementData result;
	result.customer = invoice.customer;
	std::transform(
		invoice.performances.begin(),
		invoice.performances.end(),
		std::back_inserter(result.performances),
		[&](const Performance& aPerformance) {
			EnrichedPerformance result;
			result.playID = aPerformance.playID;
			result.audience = aPerformance.audience;
			result.play = playFor(aPerformance);
			result.amount = amountFor(aPerformance);
			result.volumeCredits = volumeCreditsFor(aPerformance);
			return result;
		}
	);
	result.totalAmount = totalAmount(result);
	result.totalVolumeCredits = totalVolumeCredits(result);

	return result;
}

std::string statement(Invoice& invoice, std::unordered_map<std::string, Play>& plays) {

	auto usd = [&](int aNumber) {
		std::stringstream ss;
		ss.imbue(std::locale("en_US.UTF-8"));
		ss << std::showbase << std::put_money(aNumber);
		return ss.str();
	};

	auto renderPlainText = [&](const StatementData& data) {
		std::string result = std::format("Statement for {}\n", data.customer);
		for (auto& perf : data.performances) {
			result += std::format("  {}: {} ({} seats)\n", perf.play.name, usd(perf.amount), perf.audience);
		}
		result += std::format("Amount owed is {}\n", usd(data.totalAmount));
		result += std::format("You earned {} credits\n", data.totalVolumeCredits);
		return result;
	};

	return renderPlainText(createStatementData(invoice, plays));
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