#include <iostream>
#include <fstream>

#include "NanoSerialization/Core/Core.hpp"

#include "NanoSerialization/Yaml/File.hpp"
#include "NanoSerialization/Json/File.hpp"

using namespace Nano::Serialization;

using namespace std::literals;

int main(int argc, char* argv[])
{
    (void)argc; (void)argv;

    NS_LOG_TRACE("Hello, world!");
    return 0;
}