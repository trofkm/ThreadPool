#include <iostream>
#include <thread>

#include <chrono>

#include "ThreadPool/ThreadPool.h"
#include "utils/utils.h"

int main()
{
    std::cout << "Initialize cmake test" << std::endl;

    auto maxThreadCount = std::thread::hardware_concurrency();
    std::cout << "Max Thread Count: " << maxThreadCount << std::endl;

    std::cout << "Init ThreadPool..." << std::endl;

    ThreadPool pool(3);

    std::vector<int> s1 = {1, 2, 3};
    int result1 = 0;
    std::vector<int> s2 = {4, 5};
    int result2 = 0;
    std::vector<int> s3 = {8, 9, 10};
    int result3 = 0;

    std::cout << "Adding tasks into pool..." << std::endl;
    auto time = std::chrono::system_clock::now();

    auto id1 = pool.addTask(sum, std::ref(result1), std::ref(s1));
    auto id2 = pool.addTask(sum, std::ref(result2), std::ref(s2));
    auto id3 = pool.addTask(sum, std::ref(result3), std::ref(s3));

    if (pool.isCalculated(id1))
    {
        std::cout << "id1: " << result1 << std::endl;
    }
    else
    {
        pool.wait(id1);
        std::cout<<"id1: "<<result1<<std::endl;
    }

    pool.waitAll();

    std::cout<<"id2: "<<result2<<std::endl;
    std::cout<<"id3: "<<result3<<std::endl;


    return 0;
}