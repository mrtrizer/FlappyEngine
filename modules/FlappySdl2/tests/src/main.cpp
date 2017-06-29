#include <iostream>

#include <SdlGlTextureResFactory.h>

using namespace std;

int main(int argc, char *argv[])
{
    auto resManager = std::make_shared<flappy::ResManager>();
    auto factory = std::make_shared<flappy::SdlGlTextureResFactory>(".");
    factory->load("test.png", resManager);
    cout << "Hello, World!" << endl;
    return 0;
}
