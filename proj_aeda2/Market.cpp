#include "Market.h"
#include "Date.h"
#include <iostream>





Transaction::Transaction(Client * from, Client * to, Share * share, string date)
{
	//Pair( FROM, TO   )
	this->both_cli = pair<Client *, Client *>(from, to);
	this->transacted = share;
	this->date = date;
}

Client * Transaction::get_from()
{
	return get<0>(this->both_cli);
}

Client * Transaction::get_to()
{
	return get<1>(this->both_cli);
}

Share * Transaction::get_share()
{
	return this->transacted;
}

string Transaction::get_date()
{
	return this->date;
}

Order::Order(Client * from, float price, string date, int amount, bool sell, string indus)
{
	//this->date = date;
	this->amount = amount;
	this->from = from;
	this->sell = sell;
	this->price = price;
	this->indus = indus;
}

Client * Order::get_from() const
{
	return this->from;
}

float Order::get_price() const
{
	return this->price;
}

string Order::get_date()
{
	return string();
}

int Order::get_amount()
{
	return this->amount;
}

bool Order::update_amount(int par)
{
	this->amount += par;
	return true;
}

bool Order::get_sell()
{
	return this->sell;
}

bool Order::operator<(const Order & rhs)
{
	if (this->get_price() == rhs.get_price())
		return (this->get_from()->get_name() < rhs.get_from()->get_name());
	else
		return (this->get_price() < rhs.get_price());
}

string Order::get_indus() const
{
	return this->indus;
}

bool Order::set_amount(int amount)
{
	this->amount = amount;
	return true;
}

Market::Market(string filename)
{
	this->filename = filename;
}

bool Market::add_client(string name, int nif)
{
	Client * newcli = new Client(name, nif ,0.0f);
	this->clients.push_back(newcli);
	return true;
}

bool Market::add_client(string name, int nif, vector<Share*> shares)
{
	Client * newcli = new Client(name, nif, 0.0f);
	for (int i = 0; i < shares.size(); i++)
		newcli->add_share(shares.at(i));
	this->clients.push_back(newcli);
	return true;
}

bool Market::add_client(Client * cli1)
{
	(this->clients.push_back(cli1));
	return true;
}

bool Market::add_order(Order * new_order)
{

	Order * final_order = new Order(*new_order); // order to be inserted 
		
		for (int i = 0; i < new_order->get_sell()==0?this->sell_orders.size():this->buy_orders.size(); i++)
		{
			//look in the sell orders set for the first match according to price and industry

			Order * match_order = look_order_match(final_order);
			if (match_order == nullptr) //no match found , break loop
				break;

			//Subtract the input order with the match order

			int sub_return; //If sub_return is negative match_order is bigger if positive input_order is 0 they are the same;
			sub_return = sub_orders(final_order, match_order);

			//Check which one is zero or below and if the input order is zero, end it. Else destroy the match order and subtract the input order and keep going
			int min_amount = min(final_order->get_amount(), match_order->get_amount());
			if (sub_return == 0)
			{
				this->remove_share_andgive(match_order->get_from(), final_order->get_from(), min_amount, match_order->get_price(), match_order->get_indus());
				final_order->get_from()->update_money(-min_amount*match_order->get_price());
				match_order->get_from()->update_money(min_amount*match_order->get_price());
				return remove_order(match_order);
			}
				
			else
			{
				if (sub_return < 0) //sub_return is less than 0 so the match order is suffecilently big and we need to decrease by the subs_amount and end it
				{
					this->remove_share_andgive(match_order->get_from(), final_order->get_from(), min_amount, match_order->get_price(), match_order->get_indus());
					final_order->get_from()->update_money(-min_amount*match_order->get_price());
					match_order->get_from()->update_money(min_amount*match_order->get_price());
					return match_order->set_amount(-sub_return);
				}
				else //sub_return is above 0 match order is not big enough need to loop it and destroy match order and update input order
				{
					this->remove_share_andgive(match_order->get_from(), final_order->get_from(), min_amount, match_order->get_price(), match_order->get_indus());
					final_order->get_from()->update_money(-min_amount*match_order->get_price());
					match_order->get_from()->update_money(min_amount*match_order->get_price());
					remove_order(match_order);
					i--;
					final_order->set_amount(sub_return);
				}
			}
		}
		
		//If the input order has reached the end of the sell_order set with any amount store it and return

		if (new_order->get_sell() == 0)
			this->buy_orders.insert(final_order);
		else
			this->sell_orders.insert(new_order);

	return true;
}

bool Market::add_share(Share * new_one)
{
	this->shares.push_back(new_one);
	new_one->get_owner()->add_share(new_one);
	return true;
}

bool Market::remove_share(Share * wich)
{
	for (int i = 0; i < this->shares.size(); i++)
	{
		if (this->shares.at(i) == wich)
		{
			this->shares.erase(this->shares.begin() + i);
			wich->get_owner()->remove_share(wich);
			delete wich;
			return true;
		}
	}
	return false;
}

bool Market::update_order(Order * which, int amount, bool sell)
{
	set<Order*> * order_set = &(this->buy_orders);
	if (sell == 1)
		order_set = &(this->sell_orders);

	set<Order*>::iterator it;
	it = order_set->find(which);
	if (it == order_set->end())
		return false;

	if (amount >= which->get_amount())
	{
		order_set->erase(it);
		return true;
	}
	else
	{
		(*it)->update_amount(amount);
		return false;
	}


	return true;
}

bool Market::remove_order(Order * which)
{
	set<Order*> * order_set = &(this->buy_orders);
	if (which->get_sell() == 1)
		order_set = &(this->sell_orders);

	set<Order*>::iterator it = order_set->find(which);
	if (it == order_set->end())
		return false;

	order_set->erase(it);
	return true;
}

inline Order * Market::look_order_match(Order * input_order)
{

	set<Order*>::iterator set_it;
	for (set_it = this->sell_orders.begin(); set_it != this->sell_orders.end(); set_it++)
	{
		if ((*set_it)->get_price() <= input_order->get_price() && (*set_it)->get_indus() == input_order->get_indus())
			return (*set_it);
	}

	return nullptr;
}

inline int Market::sub_orders(Order * lhs, Order * rhs)
{
	return (lhs->get_amount() - rhs->get_amount());
}

inline bool Market::remove_share_andgive(Client * from, Client * to, int amount, float price, string indus)
{
	int curr_amount = amount;
	Share * final_share = new Share(indus, amount, to, price);
	to->add_share(final_share);
	Share * copy_share = new Share(*final_share);
	Transaction * new_trans = new Transaction(from, to, copy_share, "fwefw");
	this->transactions.push_back(new_trans);

	vector<Share *> * from_shares = from->get_p_shares();
	for (int i = 0; i < from_shares->size(); i++)
	{
		if ((from_shares->at(i))->get_indus() == indus)
		{
			int amount_from = from_shares->at(i)->get_amount();
			while (curr_amount != 0 && amount_from != 0)
			{
				curr_amount--; amount_from--;
			}
			if (amount_from == 0)
			{
				from_shares->erase(from_shares->begin() + i);
				i--;
			}
			if (curr_amount == 0)
			{
				if(amount_from != 0)
					from_shares->at(i)->set_amount(amount_from);
				return true;
			}
				
		}
	}
	return false;
}

bool Market::create_sell_order(Client * cli, string indus, int amount, float price)
{
	int selling_amount = 0;
	int current_amount = 0;

	set<Order*>::iterator it;
	for(it = this->sell_orders.begin() ; it != this->sell_orders.end(); it++)
	{ 
		if ((*it)->get_indus() == indus && (*it)->get_from() == cli)
			selling_amount += (*it)->get_amount();
	}

	for (int i = 0; i < cli->get_shares().size(); i++)
	{
		if (cli->get_shares().at(i)->get_indus() == indus)
			current_amount += cli->get_shares().at(i)->get_amount();
	}

	if (current_amount - selling_amount >= amount)
	{
		Order * sell_order = new Order(cli, price, "daw", amount, 1, indus);
		this->add_order(sell_order);
		return true;
	}

	return false;
}

News * Market::generate_news(string newspaper, Date date, Industry * indus, int desired_rate)
{
	int final_rate = desired_rate;
	if (desired_rate == -1)
	{
		srand(time(NULL));
		final_rate = rand() % 11 +1 ;
	}
	News * res = new News(final_rate, newspaper, date, indus);
	news.insert(res);
	return res;
}

void Market::add_news(News * newss)
{
	this->news.insert(newss);
}

vector<News*> Market::get_news_dates(Industry * indus, Date d1, Date d2)
{
	set<News *>::iterator news_it;
	vector<News *> res;
	for (news_it = this->news.begin(); news_it != this->news.end(); news_it++)
	{
		bool first_cond = (*news_it)->get_date() < d2;
		bool second_cond = (((*news_it)->get_date() < d1));
		if (first_cond && !second_cond)
			res.push_back((*news_it));
	}
	return res;
}

vector<News*> Market::get_news_indus(string indus)
{
	set<News *>::iterator news_it;
	vector<News *> res;
	for (news_it = this->news.begin(); news_it != this->news.end(); news_it++)
	{
		if ((*news_it)->get_indus()->get_name() == indus)
			res.push_back((*news_it));
	}
	return res;
}

int Market::current_manager = -1;

void Market::generate_managers(int how_many)
{
	vector<Manager *> all_managers;
	for (int i = 0; i < how_many; i++)
	{
		string name = "generic_manager";
		name += to_string(i);
		all_managers.push_back(new Manager(name, vector<Client *>()));
	}
	for (int j = 0; j < this->clients.size(); j++)
	{
		all_managers.at(j%how_many)->add_client(this->clients.at(j));
	}
	for (int l = 0; l < all_managers.size(); l++)
	{
		this->managers.push(all_managers.at(l));
	}
	current_manager = how_many;
}

void Market::add_manager(string name)
{
	priority_queue<Manager *> newone = this->managers;
	vector<Manager *> all_man;
	while(!newone.empty())
	{
		//newone.top()->clear_clients();
		all_man.push_back(newone.top());
		newone.pop();
	}
	Manager * new_man = new Manager(name, vector<Client *>());
	all_man.push_back(new_man);

	for (int j = 0; j < all_man.size(); j++)
	{
		all_man.at(j)->clear_clients();
	}

	for (int j = 0; j < this->clients.size(); j++)
	{
		all_man.at(j%all_man.size())->add_client(this->clients.at(j));
	}
	for (int l = 0; l < all_man.size(); l++)
		newone.push(all_man.at(l));
	this->managers = newone;

}

Manager * Market::top_manager()
{
	return this->managers.top();
}

void Market::menu(ostream & out)
{
	out << string(100, '\n');
	string state = "main";

	while(state != "exit")
	{
		if (state == "main")
		{
			out << "Day: " << this->day << endl << endl;
			out << "Menu options: " << endl;	
			out << "1 - Client option, transactions and 1º project related." << endl;
			out << "2 - News options" << endl;
			out << "3 - Manager options" << endl;
			out << "4 - Inactive Clients options" << endl;
			out << "5 - Advance a day" << endl;

			out << "0 - Exit" << endl;
			int opt;
			while (cin || opt > 5 || opt < 0)
				cin >> opt;




		}

	}

bool Market::share_updater(Client * cli, string indus, int amount)
{
	int current_amount = amount;
	int i = 0;
	while(current_amount != 0 || i != cli->get_shares().size())
	{
		if (cli->get_shares().at(i)->get_indus() == indus) //found shares to trade
		{
			Share * curr_share = cli->get_shares().at(i); //share of client to trade
			if (curr_share->get_amount() <= current_amount)
			{
				current_amount -= curr_share->get_amount();
				this->remove_share(curr_share);
				i--;
			}
			else
				if (curr_share->get_amount() > current_amount)
				{
					curr_share->set_amount(curr_share->get_amount() - current_amount);
					current_amount = 0;
				}
		}
		i++;
	}
	if(current_amount == 0)
		return false;
	return true;
}

void test(ostream & out)
{


	Market first("M1");
	//Fill with random clients
	out << "Client Creating: ";

	Client cli1("1", 1, 100.0f);
	first.add_client(&cli1);
	Client cli2("2", 2, 200.0f);
	first.add_client(&cli2);
	Client cli3("3", 3, 300.0f);
	first.add_client(&cli3);
	Client cli4("4", 4, 400.0f);
	first.add_client(&cli4);
	Client cli5("5", 5, 500.0f);
	first.add_client(&cli5);
	Client cli6("6", 6, 600.0f);
	first.add_client(&cli6);

	if (first.clients.size() != 6 || first.clients.at(3)->get_nif() != 4)
	{
		out << "Error" << endl;
		return;
	}
	else
		out << "OK" << endl;

//==================================Share================================================================
	out << "Share Creating: ";

	Share share1("cao", 100, &cli1, 5.0f);
	first.add_share(&share1);
	Share share2("gato", 15, &cli1, 12.5f);
	first.add_share(&share2);
	Share share3("pato", 10, &cli1, 5.0f);
	first.add_share(&share3);
	if (first.shares.at(0)->get_amount() != 100 || first.shares.size() != 3 || first.clients.at(0)->get_shares().at(1)->get_indus() != "gato")
	{
		out << "Error" << endl;
		return;
	}

	Share share4("cao", 100, &cli2, 5.0f);
	first.add_share(&share4);
	Share share5("gato", 15, &cli2, 12.5f);
	first.add_share(&share5);
	Share share6("pato", 10, &cli2, 50.0f);
	first.add_share(&share6);
	if (first.shares.at(3)->get_amount() != 100 || first.shares.size() != 6 || first.clients.at(1)->get_shares().at(2)->get_indus() != "pato")
	{
		out << "Error" << endl;
		return;
	}

	Share share7("cao", 100, &cli3, 5.0f);
	first.add_share(&share7);
	Share share8("gato", 15, &cli4, 12.5f);
	first.add_share(&share8);
	Share share9("pato", 10, &cli5, 50.0f);
	first.add_share(&share9);
	if (first.shares.at(8)->get_amount() != 10 || first.shares.size() != 9 || first.clients.at(2)->get_shares().at(0)->get_indus() != "cao")
	{
		out << "Error" << endl;
		return;
	}
	else
		out << "OK" << endl;

//=================================NEWS===================================================================
	bool working = true;
	Industry * indus1 = new Industry("indus1", 1000);
	Industry * indus2 = new Industry("indus2", 5);

	//News * paper1 = new News(1, "jn", Date("05/01/2001"), indus1);
	first.generate_news("jn", Date("05/01/2001"), indus1);
	first.generate_news("jn", Date("06/01/2001"), indus1);
	first.generate_news("jn", Date("05/02/2001"), indus1);
	first.generate_news("jn", Date("06/01/2001"), indus2);
	first.generate_news("ex", Date("05/02/2001"), indus1);

	vector<News *> res = first.get_news_dates(indus1, Date("04/01/2001"), Date("07/01/2001"));
	if (res.size() != 3)
		working = false;

	vector<News *> res1 = first.get_news_indus(indus1->get_name());
	vector<News *> res2 = first.get_news_indus(indus2->get_name());
	if (res1.size() != 4 && res2.size() != 1)
		working = false;

//==================================MANAGERS===========================================================

	first.generate_managers(3);
	Manager * top = first.top_manager();
	first.add_manager("lol");
	first.add_manager("lol1");
	first.add_manager("lol2");
	first.add_manager("lol3");
	first.add_manager("lol4");
	first.add_manager("lol5");


//==================================ORDERS================================================================

	Date test(2, 4, 23);
	Date test1("03/04/23");

	if (test < test1)
		cout << "True and "; test.out(cout) << endl;

	out << "Order and Transaction Creating: ";
	
	Order sell_ord(&cli1, 15, "wd", 15, 1, "gato");
	first.add_order(&sell_ord);
	if (first.sell_orders.size() != 1 || (*first.sell_orders.begin())->get_amount() != 15 || (*first.sell_orders.begin())->get_indus() != "gato")
		working = false;

	Order sell_ord2(&cli1, 15, "wd", 30, 1, "cao");
	first.add_order(&sell_ord2);
	set<Order*>::iterator pos = first.sell_orders.find(&sell_ord2);
	if (first.sell_orders.size() != 2 || (*pos)->get_amount() != 30 || (*pos)->get_indus() != "cao")
		working = false;

	Order sell_ord3(&cli2, 5, "wd", 5, 1, "pato");
	first.add_order(&sell_ord3);
	pos = first.sell_orders.find(&sell_ord3);
	if (first.sell_orders.size() != 3 || (*pos)->get_amount() != 5 || (*pos)->get_indus() != "pato")
		working = false;

	//Order sell_ord4(&cli2, 5, "wd", 15, 1, "gato");
	//first.add_order(&sell_ord4);
	//pos = first.sell_orders.find(&sell_ord4);
	//if (first.sell_orders.size() != 4 || (*pos)->get_amount() != 15 || (*pos)->get_indus() != "gato")
	//	working = false;

	//TRANSACTION
		//complete destruction
	Order buy_ord(&cli2, 15, "wd", 15, 0, "gato");
	first.add_order(&buy_ord);
	//pos = first.buy_orders.find(&buy_ord);
	if (first.sell_orders.size() != 2 || first.buy_orders.size() != 0
		|| first.transactions.at(0)->get_share()->get_amount() != 15
		|| first.transactions.at(0)->get_share()->get_indus() != "gato" || first.clients.at(1)->get_money() != 200-15*15)
		working = false;

		//Buy order destruied but not match order
	Order buy_ord2(&cli1, 5, "wd", 2, 0, "pato");
	first.add_order(&buy_ord2);
	//pos = first.buy_orders.find(&buy_ord);
	if (first.sell_orders.size() != 2 || first.buy_orders.size() != 0
		|| first.transactions.at(1)->get_share()->get_amount() != 2
		|| first.transactions.at(1)->get_share()->get_indus() != "pato" || first.clients.at(0)->get_money() != 325  - 2*5
		|| first.clients.at(0)->get_shares().at(1)->get_amount() != 12)
		working = false;

		//Buy order Not destruid but match order is

	Order buy_ord3(&cli2, 15, "wd", 40, 0, "cao");
	float client_money = first.clients.at(1)->get_money() - 30 * 15;
	first.add_order(&buy_ord3);
	//pos = first.buy_orders.find(&buy_ord);
	if (first.sell_orders.size() != 1 || first.buy_orders.size() != 1
		|| first.transactions.at(2)->get_share()->get_amount() != 30
		|| first.transactions.at(2)->get_share()->get_indus() != "cao" 
		|| first.clients.at(1)->get_money() != client_money)
		working = false;


	//Creating testing
	if (first.create_sell_order(first.clients.at(0), "pato", 15, 100) != false || first.create_sell_order(first.clients.at(0), "pato", 12, 100) != true || first.create_sell_order(first.clients.at(0), "pato", 3, 100) != false)
		working = false;

	//Order sell_ord2(&cli1, 5, "wd", 5, 1, "cao");
	//Order sell_ord3(&cli1, 5, "wd", 10, 1, "pato");
	//Order sell_ord4(&cli1, 10, "wd", 5, 1, "cao");
	//Order buy_ord(&cli2, 10, "wd", 5, 0, "gato");
	//first.add_order(&sell_ord);
	//first.add_order(&sell_ord2);
	//first.add_order(&sell_ord3);
	//first.add_order(&sell_ord4);
	//first.add_order(&buy_ord);

	if (working == false)
	{
		out << "Error" << endl;
		return;
	}
	else
		out << "OK" << endl;

	out << "Tests over all Passed." << endl;

	return;


}



