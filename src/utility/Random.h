#pragma once
#include <random>
#include <ctime>

//set up as a singleton
class Random 
{
  private:
    std::mt19937 mGeneratorEngine; //generates the random numbers

    Random()
    {
      mGeneratorEngine.seed(std::time(0)); //seed it only once
    }; 
    
  public:
    Random(const Random&)             = delete;
    Random& operator=(const Random&)  = delete;
    Random(Random&&)                  = delete;
    Random& operator=(Random&&)       = delete;
	
    static Random& GetInstance()
	  {
		  static Random sInstance;
		  return sInstance;
	  };
       
    int drawNumber(int min, int max)
    {
      return std::uniform_int_distribution<int>{min, max}(mGeneratorEngine);
    }

    float drawNumber(float min, float max)
    {
      return std::uniform_real_distribution<float>{min, max}(mGeneratorEngine);
    }
};