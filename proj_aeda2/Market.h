#pragma once
#include <string>
#include <vector>
#include <algorithm>
#include <set>

using namespace std;

class Share;
class Client;
class Order;
class Transaction;
class Market;

class Share
{
public:
	Share(string indus, int amount, Client * owner, float money);
	Client * get_owner();
	int get_amount();
	void set_amount(int amount);
	string get_indus();
	float get_money();
private:
	int amount;
	float money;
	string indus;
	Client * owner;
};

class Client
{
public:
	Client(string name, int nif);
	Client();
	int get_nif();
	string get_name() const;
	void add_share(Share* newone);
	int update_share(Share * tobe_updated, int amount);
	vector<Share* > get_shares();
	bool remove_share(Share * what);
private:
	int nif;
	string name;
	vector<Share*> shares;
};

class Transaction
{
public:
	Transaction(Client * from, Client * to, Share* share, string date);
	Client * get_from();
	Client * get_to();
	Share* get_share();
	string get_date();

private:
	pair<Client *, Client *> both_cli;
	Share* transacted;
	string date;
};

class Order
{
public:
	Order(Client * from, float price, string date, int amount, bool sell, string indus);
	Client * get_from() const;
	float get_price() const;
	string get_date();
	int get_amount();
	bool update_amount(int par);
	bool get_sell();
	bool operator<(const Order & rhs);
	string get_indus() const;
	bool set_amount(int amount);
private:
	bool sell;
	float price;
	int amount;
	Client * from;
	string indus;
};

class Market
{
public:
	Market(string filename);
	bool add_client(string name, int nif);
	bool add_client(string name, int nif, vector<Share*> shares);
	bool add_client(Client * cli1);
	bool add_order(Order * new_order);
	bool add_share(Share * new_one);
	bool remove_share(Share * wich);
	bool update_order(Order * which, int amount, bool sell);


	bool remove_order(Order * which);

	inline Order * look_order_match(Order * input_order);
	inline int sub_orders(Order* lhs, Order * rhs);

private:
	vector<Share*> shares;
	vector<Client*> clients;
	set<Order*> sell_orders;
	set<Order*> buy_orders;
	vector<Transaction *> transactions;
	string filename;

	bool share_updater(Client * cli, string indus, int amount);
};