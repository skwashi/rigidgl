/**
 * @author Jonas Ransjö
 */

#ifndef GLUTILS_H
#define GLUTILS_H

#include <string>
#include <fstream>
#include <streambuf>

namespace rgl {

inline std::string readFile(const char* filename) 
{
    std::ifstream ifs(filename);
    std::string str((std::istreambuf_iterator<char>(ifs)),
                    std::istreambuf_iterator<char>());
    ifs.close();
    return str;
}

}


#endif
