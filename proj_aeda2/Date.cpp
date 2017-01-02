#include "Date.h"

Date::Date(int day, int month, int year)
{
	this->day = day;
	this->month = month;
	this->year = year;
}

Date::Date(string day_month_year)
{
	string current = day_month_year;
	this->day = stoi(current.substr(0, 2));
	current = current.substr(current.find('/') + 1);
	this->month = stoi(current.substr(0, 2));
	current = current.substr(current.find('/') +1);
	this->year = stoi(current);
}

Date::Date()
{
	this->day = -1;
	this->month = -1;
	this->year = -1;
}

int Date::get_year() const
{
	return this->year;
}

int Date::get_month() const
{
	return this->month;
}

int Date::get_day() const
{
	return this->day;
}

string Date::get_string() const
{
	string res;
	if (day < 10)
		res += '0';
	res += to_string(day);
	res += '/';
	if (month < 10)
		res += '0';
	res += to_string(month);
	res += '/';
	res += to_string(year);
	return res;
}
