#include <iostream>
#include <memory>
#include <vector>

#include "game.pb.h"

#include "classMain.h"
#include "classUnit.h"

#include "TW_MemoryPool.h"
#include <functional>

#pragma pack(push, 1)
class EmptyClass
{
public:
    EmptyClass(){};
    int getTestValue()
    {
        return 1000000;
    }

private:
    int a = 0;
    short b = 0;
    char c = 0;
};
#pragma pack(pop)

int main()
{
    auto len = sizeof(EmptyClass);
    auto emptyClass = new EmptyClass();
    auto len2 = sizeof(emptyClass);
    auto empv = emptyClass->getTestValue();

    auto u = new Unit();
    auto i = u->GetBaseId();


    TeMemoryPool<Unit> mpUnit(10);

    std::vector<std::unique_ptr<Unit, std::function<void(Unit*)>>> testUnits;
    std::vector<std::shared_ptr<Unit>> sharePointerTest;
    // testUnits.resize(50);

    for (int i = 0; i < 20; ++i)
    {
        auto tempUnit = std::unique_ptr<Unit, std::function<void(Unit*)>>(mpUnit.alloc(100+i),[&mpUnit](Unit* p){mpUnit.free(p);});
        testUnits.push_back(std::move(tempUnit));

        auto shareUnit = std::shared_ptr<Unit>(mpUnit.alloc(1000+i), [&mpUnit](Unit* p) {mpUnit.free(p); });
        sharePointerTest.push_back(shareUnit);
        //testUnits[i].reset(tempUnit);
    }
    //std::remove(testUnits.begin(), testUnits.end());
    for (int i = 0; i < 20; ++i)
    {
        //mpUnit.free(testUnits[i].get());
        //testUnits[i].release();
        auto ss = testUnits.begin();
        //mpUnit.free((*ss).get());
        //(*ss).release();
        testUnits.erase(ss);

        sharePointerTest.pop_back();
    }
    for (int i = 0; i < 25; ++i)
    {
        auto tempUnit = std::unique_ptr<Unit, std::function<void(Unit*)>>(mpUnit.alloc(200 + i), [&mpUnit](Unit* p) {mpUnit.free(p); });
        testUnits.push_back(std::move(tempUnit));
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
