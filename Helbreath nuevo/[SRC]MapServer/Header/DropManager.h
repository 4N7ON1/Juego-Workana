#pragma once

#define MAXTRADEEKITEMS 50
#define MAXTRADECONTRIBITEMS 50
#define MAXTRADECOINITEMS 50

enum TradeType
{
	DEF_TRADEEK,
	DEF_TRADECONTRIB,
	DEF_TRADECOIN,
	DEF_TRADEMAJS,
	DEF_TRADEBALLS,
	DEF_NOTSELECTED
};

class Trades
{
public:
	Trades();
	~Trades();
	void get_trade_ek();
	void get_trade_contrib();
	void get_trade_coin();
	void get_trade_majs();
	void send_trade_data(int client, short trade);
	void req_trade(int client, char * pData);
	void req_tradeball(int client, char * data);
	void getContributionBall(int client, int quant);
	void getCoinBall(int client, int quant);
	void getReputationBall(int client, int quant);
	void getMajesticBall(int client, int quant);
};

