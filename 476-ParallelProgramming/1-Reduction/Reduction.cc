/*
  Filename   : Reduction.cc
  Author     : Steve Zelek
  Course     : CSCI 476-01
  Assignment : Program 1
  Description: Add N numbers together serially, while emulating a multicore algorithm.
*/

#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <functional>

typedef std::mt19937 rng_type;

std::vector<int>
createNums();

std::vector<int>
reduction(std::vector<int> nums, std::function<int(int, int)> operation);

void
reductionStage(std::vector<int> &nums, std::function<int(int, int)> operation, unsigned stageNum);

void
printSummary(std::vector<int>& nums, std::vector<int> & reducedNums);

void
printVector (std::vector<int> vec);

int
add (int x, int y){
    return x + y;
}


int main() {
    std::vector<int> originalNums = createNums();

    std::function<int(int, int)> operation = add; //modify function used in reduction

    std::vector<int> reducedNums = reduction(originalNums, operation);

    printSummary(originalNums, reducedNums);

    return 0;
}

std::vector<int> reduction(std::vector<int> nums, std::function<int(int, int)> operation){
    int numStages = std::ceil(std::log2(nums.size()));
    for (unsigned stage = 0; stage < numStages; ++stage){
        std::cout << "Stage " << stage << std::endl;
        std::cout << "-------" << std::endl;
        reductionStage(nums, operation, stage);
    }

    return nums; //a modified copy
}

void reductionStage(std::vector<int> &nums, std::function<int(int, int)> operation, unsigned stageNum){
    unsigned moduloNumber = pow(2, stageNum+1);
    unsigned offSet = moduloNumber/2;
    unsigned coreNum = 0;

    while (coreNum + offSet < nums.size())
    {
        nums[coreNum] = operation(nums[coreNum], nums[coreNum+offSet]);
        std::cout << "Recv: " << coreNum << " from " << (coreNum+offSet)
                  << ", v = "   << nums[coreNum+offSet]
                  << ", result = " << nums[coreNum]
                  << std::endl
                  << std::endl;;

        coreNum += moduloNumber;
    }
}

std::vector<int> createNums(){
    unsigned num_processors;

    std::cout << "p ==> ";
    std::cin >> num_processors;
    std::cout << std::endl;

    std::random_device rd;
    std::mt19937 generator (rd());
    std::uniform_int_distribution<int> distribution(10,99);

    std::vector<int> nums;
    for (unsigned i = 0; i < num_processors; ++i){
        //nums.push_back(distribution(generator));
        nums.push_back(i+1);
    }

    return nums;
}

void printSummary(std::vector<int>& nums, std::vector<int> & sums){
    std::cout << "Summary" << std::endl;
    std::cout << "=======" << std::endl;
    std::cout << "A[] = ";
    printVector(nums);
    std::cout << std::endl;

    std::cout << "S[] = ";
    printVector(sums);
    std::cout << std::endl;

    std::cout << "# stages     = " << std::ceil(std::log2(nums.size())) << std::endl;
    std::cout << "# operations = " << nums.size() - 1 << std::endl;
}

void printVector (std::vector<int> vec){
    const std::string separator = ", ";
    std::string sep = "";
    std::cout << "{ ";
    for(const auto& e : vec) {
        std::cout << sep << e;
        sep = separator;
    }
    std::cout << " }";
}