#include "Cliente.h"

Client::Client(string name, int nif, float money)
{
	this->nif = nif;
	this->name = name;
	this->money = money;
}

Client::Client()
{
	this->nif = 0;
	this->name = "Default";
}

Client::Client(const Client * par)
{
	this->active = par->active;
	this->adress = par->adress;
	this->last_trans = par->last_trans;
	this->money = par->money;
	this->shares = par->shares;
	this->phone = par->phone;
	this->name = par->name;
	this->nif = par->nif;
}

int Client::get_nif() const
{
	return this->nif;
}

string Client::get_name() const
{
	return this->name;
}

void Client::add_share(Share * newone)
{
	for (int i = 0; i < this->shares.size(); i++)
	{
		if (this->shares.at(i)->get_indus() == newone->get_indus() && this->shares.at(i)->get_money() == newone->get_money())
		{
			this->shares.at(i)->set_amount(this->shares.at(i)->get_amount() + newone->get_amount());
			return;
		}
	}
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

void Client::set_last_trans(Date date)
{
	this->last_trans = date;
}

Date Client::get_last_trans()
{
	return this->last_trans;
}

int Client::get_active() const
{
	return this->active;
}

void Client::set_active(bool yes)
{
	this->active = yes;
}

string Client::get_adress() const
{
	return this->adress;
}

void Client::set_adress(string new_adress)
{
	this->adress = new_adress;
}

string Client::get_phone() const
{
	return this->phone;
}

void Client::set_phone(string new_phone)
{
	this->phone = new_phone;
}

vector<Share*>* Client::get_p_shares()
{
	return &(this->shares);
}

float Client::get_money()
{
	return this->money;
}

void Client::update_money(float update)
{
	this->money += update;
}