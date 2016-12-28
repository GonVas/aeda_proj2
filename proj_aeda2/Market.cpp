#include "Market.h"

Share::Share(string indus, int amount, Client * owner, float money)
{
	this->amount = amount;
	this->indus = indus;
	this->owner = owner;
	this->money = money;
}

Client * Share::get_owner()
{
	return this->owner;
}

int Share::get_amount()
{
	return this->amount;
}

void Share::set_amount(int amount)
{
	this->amount = amount;
}

string Share::get_indus()
{
	return this->indus;
}

float Share::get_money()
{
	return this->money;
}

Client::Client(string name, int nif)
{
	this->nif = nif;
	this->name = name;
}

Client::Client()
{
	this->nif = 0;
	this->name = "Default";
}

int Client::get_nif()
{
	return this->nif;
}

string Client::get_name() const
{
	return this->name;
}

void Client::add_share(Share * newone)
{
	this->shares.push_back(newone);
}

int Client::update_share(Share * tobe_updated, int amount)
{
	Share * found = nullptr;
	int pos = 0;
	for (int i = 0; i < this->shares.size(); i++)
		if (tobe_updated == this->shares.at(i))
		{
			found = this->shares.at(i);
			pos = i;
		}

	if (found == nullptr)
		return -1; //CLIENT DOESNT HAVE SHARE

	if (found->get_amount() <= amount)
	{
		this->shares.erase(this->shares.begin() + pos);
		return (amount - found->get_amount());
	}
	else
	{
		found->set_amount(found->get_amount() - amount);
		return 1;
	}
	return 0;
}

vector<Share*> Client::get_shares()
{
	return this->shares;
}

bool Client::remove_share(Share * what)
{
	for (int i = 0; i < this->shares.size(); i++)
	{
		if (this->shares.at(i) == what)
		{
			this->shares.erase(this->shares.begin() + i);
			return true;
		}
	}
	return false;
}

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
	Client * newcli = new Client(name, nif);
	this->clients.push_back(newcli);
	return true;
}

bool Market::add_client(string name, int nif, vector<Share*> shares)
{
	Client * newcli = new Client(name, nif);
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



//bool Market::add_order(Order * new_order)
//{
//	if (new_order->get_sell() == 0)
//	{
//		//##############BUYORDER##################
//		set<Order*>::iterator set_it;
//		for (set_it = this->sell_orders.begin(); set_it != this->sell_orders.end(); set_it++)
//		{
//			if ((*set_it)->get_price() <= new_order->get_price())
//			{
//				Client * buying_from = (*set_it)->get_from();
//				Client * buying_to = new_order->get_from();
//				Order * buying_from_ord = (*set_it);
//
//				if (buying_from_ord->get_amount() == new_order->get_amount())
//				{
//					this->sell_orders.erase(set_it);
//					Share * newone = new Share(buying_from_ord->get_indus(), buying_from_ord->get_amount(), buying_to, buying_from_ord->get_price());
//					buying_to->add_share(newone);
//					this->shares.push_back(newone);
//					return true;
//				}
//
//
//			}
//		}
//
//		this->buy_orders.insert(new_order);
//
//
//	}
//
//	else
//		this->sell_orders.insert(new_order);
//	return true;
//}





bool Market::add_order(Order * new_order)
{

	Order * final_order = new Order(*new_order); // order to be inserted 


	if (new_order->get_sell() == 0)
	{
		//##############BUYORDER##################
		
		for (int i = 0; i < this->sell_orders.size(); i++)
		{
			//look in the sell orders set for the first match according to price and industry

			Order * match_order = look_order_match(final_order);
			if (match_order == nullptr) //no match found , break loop
				break;

			//Subtract the input order with the match order

			int sub_return; //If sub_return is negative match_order is bigger if positive input_order is 0 they are the same;
			sub_return = sub_orders(final_order, match_order);

			//Check which one is zero or below and if the input order is zero, end it. Else destroy the match order and subtract the input order and keep going

			if (sub_return == 0)
				return remove_order(match_order);
			else
			{
				if (sub_return < 0) //sub_return is less than 0 so the match order is suffecilently big and we need to decrease by the subs_amount and end it
					return match_order->set_amount(-sub_return);
				else //sub_return is above 0 match order is not big enough need to loop it and destroy match order and update input order
				{
					remove_order(match_order);
					i--;
					final_order->set_amount(sub_return);
				}
			}
		}
		
		//If the input order has reached the end of the sell_order set with any amount store it and return

		this->buy_orders.insert(final_order);
	}
	else
	{
		//##############SELLORDER##################
		this->sell_orders.insert(new_order);
	}

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
