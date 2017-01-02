#pragma once
#include <string>
#include "Cliente.h"
#include <vector>

class Manager
{
public:
	Manager(string name, vector<Client *> clients);
	vector<Client *> get_cli();
	string get_name();
	int get_num_cli() const;
	void add_client(Client * cli);
	void clear_clients();
	bool operator<(const Manager & rhs)
	{
		return !(this->clients.size() < rhs.get_num_cli());
	}
	bool operator==(const Manager & rhs)
	{
		return (this->clients.size() == rhs.get_num_cli());
	}
private:
	string name;
	vector<Client *> clients;
};

