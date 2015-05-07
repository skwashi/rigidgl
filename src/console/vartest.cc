#include <boost/variant.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <string>
#include <iostream>
#include <glm/glm.hpp>

#include "cvarsv.h"

typedef boost::variant<std::string, int> value_t;

using namespace rcv;

int main(int argc, char *argv[])
{

    CVar cvf = CVar("cvf", 20.f);
    CVar cvv3 = CVar("cvv3", glm::vec3(1, 2, 23));

    std::cout << cvf << std::endl;
    cvf.setString("25.0f");
    cvv3.setString("(2, 1, 250)");
    cvv3.setString("true");

    std::cout << cvf << std::endl;
    std::cout << cvf.get<float>() << std::endl;

    std::cout << cvv3 << std::endl;
    std::cout << cvv3.get<glm::vec3>() << std::endl;

    std::cout << cvf.getType() << std::endl;
    std::cout << cvv3.getType() << std::endl;
    std::cout << std::stoi("1") << std::endl;
    return 0;
}
