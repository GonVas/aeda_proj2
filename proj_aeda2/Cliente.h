#pragma once
#include <string>
#include "Share.h"
#include <vector>
#include "Date.h"
#include <ostream>

using namespace std;

class Share;

class Client
{
public:
	Client(string name, int nif, float money);
	Client();
	int get_nif() const;
	string get_name() const;
	void add_share(Share* newone);
	int update_share(Share * tobe_updated, int amount);
	vector<Share* > get_shares();
	bool remove_share(Share * what);
	void set_last_trans(Date date);
	Date get_last_trans();

	int get_active() const;
	void set_active(bool yes);
	string get_adress() const;
	void set_adress(string new_adress) ;
	string get_phone() const;
	void set_phone(string new_phone);

	vector<Share *> * get_p_shares();
	float get_money();
	void update_money(float update);

	ostream & out(ostream & ou)
	{
		ou << "Client with name: " << this->name << "  Nif: " << this->nif << " adress: " << this->adress << " phone: " << this->phone << " active: " << this->active << endl;
		return ou;
	}

private:
	int nif;
	string name;
	vector<Share*> shares;
	float money;
	Date last_trans;
	bool active;
	string adress;
	string phone;
};