#pragma once
#include <string>
#include <ostream>

using namespace std;

class Date
{
public:
	Date(int day, int month, int year);
	Date(string day_month_year);
	Date();
	int get_year() const;
	int get_month() const;
	int get_day() const;
	string get_string() const;

	bool operator<(const Date & rhs)
	{
		if (this->year != rhs.get_year())
			return this->year < rhs.get_year();
		else
			if (this->month != rhs.get_month())
				return (this->month < rhs.get_month());
			else
				return (this->day < rhs.get_day());
	}
	bool operator==(const Date & rhs)
	{
		return (this->day == rhs.get_day() && this->month == rhs.get_month() && this->year == rhs.get_year());
	}
	bool operator!=(const Date & rhs)
	{
		return !(this->day == rhs.get_day() && this->month == rhs.get_month() && this->year == rhs.get_year());
	}

	ostream& out(ostream& os)
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
		os << res;
		return os;
	}
		
private:
	int day;
	int month;
	int year;
};

