#pragma once

class Unit;

class BeMain
{
public:
	BeMain(void);
	~BeMain(void);

	void test();
	int gameTime;
	Unit* unit;

private:
	BeMain* pkAttachMain;
};

#define gMain		(*((BeMain *)pkAttachMain))
#define gTime		(gMain.gameTime)
#define gUnit		(gMain.unit)
