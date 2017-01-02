#include "News.h"

News::News(int rate, string newspaper, Date date, Industry * indus)
{
	this->rate = rate;
	this->newspaper = newspaper;
	this->date = date;
	this->indus = indus;
}

News::News()
{
}

Date News::get_date() const
{
	return this->date;
}

Industry * News::get_indus() const
{
	return this->indus;
}

int News::get_rate() const
{
	return this->rate;
}

string News::get_newspaper() const
{
	return this->newspaper;
}

void News::set_rating(int new_rate)
{
	this->rate = new_rate;
}

ostream & News::out(ostream & os)
{
	os << " Industry: " << this->indus->get_name() << " got a news with rating: " << this->rate << " on date: " << this->date.get_string() << endl;
	return os;
}
