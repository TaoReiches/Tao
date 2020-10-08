#include "classMain.h"
#include "classUnit.h"

BeMain::BeMain()
{
    pkAttachMain = this;
    gameTime = 100;
    unit = new Unit();
    unit->AttachMain(pkAttachMain);
}

BeMain::~BeMain()
{

}

void BeMain::test()
{
    auto s = gUnit->get();
    auto h = gTime;
}
