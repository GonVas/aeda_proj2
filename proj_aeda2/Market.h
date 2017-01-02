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
#include <unordered_set>


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

struct eqfunc {
	int operator() (const Client *s1, const Client *s2) const {
		return s1->get_nif() == s2->get_nif();
	}
};struct hfunc {
	int operator() (const Client *s1) const {
		return s1->get_nif();
	}
};struct compareNews {

	bool operator()(const News * r1 , const News * r2) {
		if (r1->get_indus()->get_name() != r2->get_indus()->get_name())
			return r1->get_indus()->get_name() < r2->get_indus()->get_name();
		else
			if (r1->get_date() != r2->get_date())
				return r1->get_date() < r2->get_date();
			else
				if (r1->get_indus()->get_name() != r2->get_indus()->get_name())
					return r1->get_indus()->get_name() < r2->get_indus()->get_name();
				else
					return (r1->get_newspaper() < r2->get_newspaper());
	}
};struct compareManagers {

	bool operator()(const Manager * r1, const Manager * r2) {
		return (r2->get_num_cli()< r1->get_num_cli());
	}
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

	void manage_dead_cli();
	Client * find_cli(string name);
	Client * find_cli(int nif);
	Client * change_dead_adress(int nif, string new_adress);
	

	friend void test(ostream & out);

private:
	vector<Share*> shares;
	vector<Client*> clients;
	set<Order*> sell_orders;
	set<Order*> buy_orders;
	vector<Transaction *> transactions;
	string filename;

	int day;

	set<News *, compareNews> news;
	priority_queue<Manager *, vector<Manager *>, compareManagers> managers;
	unordered_set<Client *, hfunc, eqfunc> dead_clients;

	bool share_updater(Client * cli, string indus, int amount);
};