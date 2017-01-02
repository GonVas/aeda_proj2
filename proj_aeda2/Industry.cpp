#include "Industry.h"

Industry::Industry(string name, float value)
{
	this->name = name;
	this->value = value;
}

string Industry::get_name() const
{
	return this->name;
}

float Industry::get_value() const
{
	return this->value;
}

void Industry::update_value(float amount)
{
	this->value += amount;
}
