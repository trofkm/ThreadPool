#include "utils.h"

void someBusiness(int &result, const std::vector<int> &args)
{
    result = 0;

    for (int i = args.size() - 1; i >= 0; --i)
    {
        for (int j = 0; j < args.size(); ++j)
        {
            result += args[i] + args[j];
        }
    }
}

void sum(int& result, const std::vector<int> &args){
    for(int i =0;i<args.size();++i){
        result += args[i];
    }
}