#include <thread>

#include "config_reader.h"
#include "game_loader.h"
#include "presenter.h"

void test() {
    unsigned int n = std::thread::hardware_concurrency();
    int x;
    std::thread t1([&](){x = 5;});
    std::thread t2([&](){x = 7;});

    t1.join();
    t2.join();

    std::cout << "Available threads: " << n << std::endl;
}
int main()
{
    test();
    using namespace TowerDefence;
    json config;
    JsonReader reader{config};
    auto mainConfig = reader.fromFile("../cfg/main_config.json");

    GameLoader loader{*mainConfig};

    Presenter presenter{loader.getView(), loader.getModel()};
    try {
        presenter.present();
    }catch (const std::exception& e) {
        std::cerr <<"Error: "<< e.what() << std::endl;
    }
    return 0;
}