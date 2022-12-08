#include <ctime>
#include "Random.h"

std::mt19937 Random::random((unsigned)time(0));

//Uses the seed to get a random integer from min to max
int Random::Int(int min, int max) {
	std::uniform_int_distribution<int> dist(min, max);
	return dist(random);
}

//Uses the seed to get a random float from min to max
float Random::Float(float min, float max) {
	std::uniform_real_distribution<float> dist(min, max);
	return dist(random);
}