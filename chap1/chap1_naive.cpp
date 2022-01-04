#include "nlohmann/json.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>

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
	int totalAmount = 0;
	int volumeCredits = 0;
	std::string result = std::format("Statement for {}\n", invoice.customer);

	for (auto& perf : invoice.performances) {
		Play play = plays[perf.playID];
		int thisAmount = 0;

		if (play.type == "tragedy") {
			thisAmount = 40000;
			if (perf.audience > 30) {
				thisAmount += 1000 * (perf.audience - 30);
			}
		}
		else if (play.type == "comedy") {
			thisAmount = 30000;
			if (perf.audience > 20) {
				thisAmount += 10000 + 500 * (perf.audience - 20);
			}
			thisAmount += 300 * perf.audience;
		}
		else {
			throw std::format("unknown type: {}", play.type);
		}

		volumeCredits += std::max(perf.audience - 30, 0);

		if (play.type == "comedy") {
			volumeCredits += std::floor(perf.audience / 5.0);
		}

		result += std::format("  {}: ${:.2f} ({} seats)\n", play.name, thisAmount / 100.0, perf.audience);
		totalAmount += thisAmount;
	}

	result += std::format("Amount owed is ${:.2f}\n", totalAmount / 100.0);
	result += std::format("You earned {} credits\n", volumeCredits);
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