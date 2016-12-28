#include <iostream>
#include "Market.h"
#include <string>

using namespace std;

void main()
{
	//Share newone("pao", 100, nullptr);
	
	//cout << "Share created with indus: " << newone.get_indus() << " amount: " << newone.get_amount() << endl;
	Market first("cenas");
	Client cli1("1", 1);
	first.add_client(&cli1);
	Client cli2("2", 2);
	first.add_client(&cli2);
	Share share1("cao", 100, &cli2, 10.5f);
	Share share2("gato", 400, &cli1, 12.5f);
	first.add_share(&share1);
	first.add_share(&share2);
	Order sell_ord(&cli1, 5, "wd", 10, 1, "cao");
	Order sell_ord2(&cli1, 5, "wd", 5, 1, "cao");
	Order sell_ord3(&cli1, 5, "wd", 10, 1, "pato");
	Order sell_ord4(&cli1, 10, "wd", 5, 1, "cao");
	Order buy_ord(&cli2, 5, "wd", 20, 0, "cao");
	first.add_order(&sell_ord);
	first.add_order(&sell_ord2);
	first.add_order(&sell_ord3);
	first.add_order(&sell_ord4);
	first.add_order(&buy_ord);

	return;
}