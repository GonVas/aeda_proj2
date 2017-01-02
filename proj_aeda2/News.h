#pragma once
#include "Date.h"
#include <string>
#include "Industry.h"
#include <ostream>

using namespace std;

class News
{
public:
	News(int rate, string newspaper, Date date, Industry * indus);
	News();
	Date get_date() const;
	Industry * get_indus() const;
	int get_rate() const;
	string get_newspaper() const;
	void set_rating(int new_rate);
	ostream & out(ostream & os);
	bool operator<(const News & rhs)
	{
		if (this->indus->get_name() < rhs.get_indus()->get_name())
			return true;
		else
			if (this->date < rhs.get_date())
				return true;
			else
				if (this->indus->get_name() < rhs.get_indus()->get_name())
					return true;
				else
					return (this->newspaper < rhs.get_newspaper());
	}
private:
	Industry * indus;
	int rate;
	string newspaper;
	Date date;
};