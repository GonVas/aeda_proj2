#pragma once
#include <string>
#include "Cliente.h"
using namespace std;


class Client;

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