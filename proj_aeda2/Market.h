#pragma once
#include <string>
#include <vector>
#include <algorithm>
#include <set>
#include <ostream>
#include "Date.h"
#include "Industry.h"
#include "News.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "Manager.h"
#include "Cliente.h"
#include "Share.h"
#include <queue>


using namespace std;

class Order;
class Transaction;
class Market;

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
	static int current_manager;
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
	inline bool remove_share_andgive(Client * from, Client * to, int amount, float price, string indus);
	bool create_sell_order(Client * cli, string indus, int amount, float price);

	News * generate_news(string newspaper, Date date, Industry * indus, int desired_rate = -1);
	void add_news(News * newss);
	vector<News *> get_news_dates(Industry * indus, Date d1, Date d2);
	vector<News *> get_news_indus(string indus);

	void generate_managers(int how_many);
	void add_manager(string name);
	Manager * top_manager();
	void menu(ostream & out);


	friend void test(ostream & out);

private:
	vector<Share*> shares;
	vector<Client*> clients;
	set<Order*> sell_orders;
	set<Order*> buy_orders;
	vector<Transaction *> transactions;
	string filename;

	int day;

	set<News *> news;
	priority_queue<Manager *> managers;

	bool share_updater(Client * cli, string indus, int amount);
};