#include <iostream>
#include <string>
#include <sstream>
#include <functional>
using namespace std;

struct Order {
	int id;
	int orderAmount;
	string categorty;
	string date;
};

struct Node {
	Order data;
	Node* next;
};

struct StrNode {
	string data;
	StrNode* next;
};

function<void(Node*&, const Order&)> append = [&](Node*& head, const Order& o) -> void {
	(head == nullptr)
		? (head = new Node{ o, nullptr }, void())
		: append(head->next, o);
	};

function<void(StrNode*&, const string&)> appendStr =
[&](StrNode*& head, const string& s) -> void {
	(head == nullptr)
		? (head = new StrNode{ s, nullptr }, void())
		: appendStr(head->next, s);
	};

auto parseOrder = [](const string& line) -> Order {
	Order o;
	stringstream ss(line);
	string token;

	getline(ss, token, ','); o.id = stoi(token);
	getline(ss, token, ','); o.orderAmount = stoi(token);
	getline(ss, token, ','); o.categorty = token.substr(1);
	getline(ss, token, ','); o.date = token.substr(1);

	return o;
	};

function<void(Node*)> printList = [&](Node* node) -> void {
	(node == nullptr)
		? void()
		: (cout << node->data.id << ", "
			<< node->data.orderAmount << ", "
			<< node->data.categorty << ", "
			<< node->data.date << "\n",
			printList(node->next));
	};

function < Node* (Node*, function < bool(const Order&)>)> filter =
[&](Node* node, function<bool(const Order&)> pred) -> Node* {
	return (node == nullptr)
		? nullptr
		: (pred(node->data)
			? new Node{ node->data, filter(node->next, pred) }
			: filter(node->next, pred));
			};

auto makeDataFilter = [](const string& from, const string& to) {
	return [from, to](const Order& o) -> bool {
		return (o.date >= from && o.date <= to) ? true : false;
		};
	};

auto makePositiveFilter = []() {
	return[](const Order& o) -> bool {
		return o.orderAmount > 0 ? true : false;
		};
	};

auto makeCategoryFilter = [](StrNode* catList) {
	return [catList](const Order& o) -> bool {
		function<bool(StrNode*)> contains = [&](StrNode* node) -> bool {
			return (node == nullptr)
				? false
				: (node->data == o.categorty
					? true
					: contains(node->next));
			};
		return contains(catList);
		};
	};

function<int(Node*)> calcTotal = [&](Node* node) -> int {
	return (node == nullptr)
		? 0
		: node->data.orderAmount + calcTotal(node->next);
	};

function <Order(Node*, Order)> calcMax = [&](Node* node, Order cur) -> Order {
	return (node == nullptr)
		? cur
		: calcMax(node->next,
			(node->data.orderAmount > cur.orderAmount
				? node->data
				: cur));
	};

function<int(Node*, const string&)> calcSumByCategory =
[&](Node* node, const string& cat) -> int {
	return (node == nullptr)
		? 0
		: (node->data.categorty == cat
			? node->data.orderAmount + calcSumByCategory(node->next, cat)
			: calcSumByCategory(node->next, cat));
	};

function<int(Node*, const string&)> calcCountByCategory =
[&](Node* node, const string& cat) -> int {
	return (node == nullptr)
		? 0
		: (node->data.categorty == cat
			? 1 + calcCountByCategory(node->next, cat)
			: calcCountByCategory(node->next, cat));
	};

function<void(Node*, StrNode*)> printAvgByCategory =
[&](Node* orders, StrNode* catNode) -> void {
	(catNode == nullptr)
		? void()
		: (cout << catNode->data << ": "
			<< calcSumByCategory(orders, catNode->data) /
			calcCountByCategory(orders, catNode->data) << "\n",
			printAvgByCategory(orders, catNode->next));
	};