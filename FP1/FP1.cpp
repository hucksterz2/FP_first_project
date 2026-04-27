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

function<void(Node*&, const Order&)> append = [&](Node*& head, const Order& o) -> void {
	(head == nullptr)
		? (head = new Node{ o, nullptr }, void())
		: append(head->next, o);
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