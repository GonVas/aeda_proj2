#pragma once
#include <string>

using namespace std;

class Industry
{
public:
	Industry(string name, float value);
	string get_name() const ;
	float get_value() const;
	void update_value(float amount);
private:
	string name;
	float value;
};