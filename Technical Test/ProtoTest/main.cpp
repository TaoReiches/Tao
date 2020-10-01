#include <iostream>

#include "game.pb.h"

int main()
{
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
