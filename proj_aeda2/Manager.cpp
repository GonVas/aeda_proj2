#include "Manager.h"

Manager::Manager(string name, vector<Client*> clients)
{
	this->name = name;
	this->clients = clients;
}

vector<Client*> Manager::get_cli()
{
	return this->clients;
}

string Manager::get_name()
{
	return this->name;
}

int Manager::get_num_cli() const
{
	return this->clients.size();
}

void Manager::add_client(Client * cli)
{
	this->clients.push_back(cli);
}

void Manager::clear_clients()
{
	this->clients = vector<Client *>();
}
