#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <float.h>
#include <time.h>
#include <stdint.h>

int number_ex(int from, int to)
{
    if (from > to) {
        int tmp = from;

        from = to;
        to = tmp;
    }

	int returnValue = 0;

    returnValue = ((rand() % (to - from + 1)) + from);

    return returnValue;
}

double uniform_random(double a, double b)
{
	return rand() / (RAND_MAX + 1.f) * (b - a) + a;
}

int MINMAX(int min, int value, int max)
{
    int tv;

    tv = (min > value ? min : value);
    return (max < tv) ? max : tv;
}

#define number(from, to) number_ex(from, to)

float gauss_random(float avg, float sigma)
{
	static bool haveNextGaussian = false;
	static float nextGaussian = 0.0f;

	if (haveNextGaussian) 
	{
		haveNextGaussian = false;
		return nextGaussian * sigma + avg;
	} 
	else 
	{
		double v1, v2, s;
		do { 
			//v1 = 2 * nextDouble() - 1;   // between -1.0 and 1.0
			//v2 = 2 * nextDouble() - 1;   // between -1.0 and 1.0
			v1 = uniform_random(-1.f, 1.f);
			v2 = uniform_random(-1.f, 1.f);
			s = v1 * v1 + v2 * v2;
		} while (s >= 1.f || fabs(s) < FLT_EPSILON);
		double multiplier = sqrtf(-2 * logf(s)/s);
		nextGaussian = v2 * multiplier;
		haveNextGaussian = true;
		return v1 * multiplier * sigma + avg;
	}
}

uint32_t calculate_bonus(int min_avg, int min_skill) {
    uint32_t iterations = 0;

    int iSkillBonus;
    int iNormalHitBonus;
    do {
        iSkillBonus = MINMAX(-30, (int) (gauss_random(0, 5) + 0.5f), 30);
        iNormalHitBonus = 0;
        if (abs(iSkillBonus) <= 20)
            iNormalHitBonus = -2 * iSkillBonus + abs(number(-8, 8) + number(-8, 8)) + number(1, 4);
        else
            iNormalHitBonus = -2 * iSkillBonus + number(1, 5);

        iterations++;
    } while(iSkillBonus < min_skill && iNormalHitBonus < min_avg);

    printf("skill_damage: %d, average_damage: %d, ", iSkillBonus, iNormalHitBonus);

    return iterations;
}

#define bonuses_per_second 5
#define items_in_parallel 5

int main(int argc, char* argv[]) {
    printf("Average time bonussing %d items in parallel switching %d bonusses per second\n", items_in_parallel, bonuses_per_second);
    if (argc < 2) {
        printf("usage: %s <min_average> [<min_skill>] [<iterations>]\n", argv[0]);
        return 1;
    }

    int min_avg = atoi(argv[1]);

    int min_skill = 100;
    if (argc > 2) {
        min_skill = atoi(argv[2]);
    }

    int iterations = 100;
    if (argc > 3) {
        iterations =  atoi(argv[3]);
    }

    srand(time(NULL));

    uint32_t sum = 0;

    for (int i = 0; i < iterations; i++)
    {
        uint32_t iterations = calculate_bonus(min_avg, min_skill);

        uint32_t time = iterations / bonuses_per_second; // seconds
        time = time / 60; // minutes
        time = time / items_in_parallel;

        sum += time;

        printf("time: %d minutes (%d hours, %d days)\n", time, time / 60, time / 60 / 24);
    }

    sum = sum / iterations;

    printf("took an average of %d minutes (%d hours, %d days)\n", sum, sum / 60, sum / 60 / 24);

    return 0;
}