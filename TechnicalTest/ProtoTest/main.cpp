#include <iostream>
#include <memory>
#include <vector>

#include "game.pb.h"

#include "classMain.h"
#include "classUnit.h"

#include "TW_MemoryPool.h"


int main()
{
    TeMemoryPool<Unit> mpUnit(10);

    std::vector<std::unique_ptr<Unit>> testUnits;
    testUnits.resize(50);

    for (int i = 0; i < 20; ++i)
    {
        testUnits[i].reset(mpUnit.alloc(100 + i));
    }
    for (int i = 0; i < 20; ++i)
    {
        //mpUnit.free(testUnits[i].get());
        //testUnits[i].release();
        auto ss = testUnits.begin();
        mpUnit.free((*ss).get());
        (*ss).release();
        testUnits.erase(ss);
    }
    for (int i = 0; i < 25; ++i)
    {
        testUnits[i].reset(mpUnit.alloc(200 + i));
    }


    auto a = new BeMain();
    a->test();

    std::cout << "Hello world, I am protobuf testing" << std::endl;

    std::string output;

    Game::Player player;
    player.set_id(99999);

    player.SerializeToString(&output);
    std::cout << "With only player id = 99999, buffer size is: " << output.length() << std::endl;

    player.set_hp(500);
    player.SerializeToString(&output);
    std::cout << "With only player id = 99999, hp = 500, buffer size is: " << output.length() << std::endl;

    player.set_maxhp(50000);
    player.SerializeToString(&output);
    std::cout << "With only player id = 99999, hp = 500, maxhp = 50000, buffer size is: " << output.length() << std::endl;

    getchar();
}
