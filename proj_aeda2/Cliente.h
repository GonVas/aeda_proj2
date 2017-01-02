#pragma once
#include <string>
#include "Share.h"
#include <vector>
#include "Date.h"

using namespace std;

class Share;

class Client
{
public:
	Client(string name, int nif, float money);
	Client();
	int get_nif();
	string get_name() const;
	void add_share(Share* newone);
	int update_share(Share * tobe_updated, int amount);
	vector<Share* > get_shares();
	bool remove_share(Share * what);
	void set_last_trans(Date date);
	Date get_last_trans();

	vector<Share *> * get_p_shares();
	float get_money();
	void update_money(float update);
private:
	int nif;
	string name;
	vector<Share*> shares;
	float money;
	Date last_trans;
};