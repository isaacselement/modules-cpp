#include "StringHelper.h"


std::string StringHelper::intToString(int n)
{
    std::stringstream ss ;
    ss << n;
    std::string s = ss.str();
    return s;
}



std::string StringHelper::floatToString(float n)
{
    std::stringstream ss ;
    ss << n;
    std::string s = ss.str();
    return s;
}


