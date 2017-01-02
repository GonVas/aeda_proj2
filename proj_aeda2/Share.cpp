#include "Share.h"

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