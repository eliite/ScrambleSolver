// Binary.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <bitset>
#include <chrono>
#include <format>
#include <vector>

int get_side(int s, int i);
void set_rotation(int i, int r);
int clamp(int, int, int);
int match(int i, int s); 

bool contains(int i);

static int square[] = {
        0b00110000010111,
        0b00010001111001,
        0b00011000100110,
        0b00111101110100,
        0b00011110100001,
        0b00000101111010,
        0b00011001100010,
        0b00000010111001,
        0b00011101000010
};

static int solve[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
static int c[] =     { 0, 0, 0, 0, 0, 0, 0, 0, 0 };

static std::chrono::steady_clock::time_point before, after;

int main()
{
    before = std::chrono::high_resolution_clock::now();
    
    solve[0] = square[0];
    for (int i = 1; i < 9; ) {
        while (c[i] < 10) {
            if (c[i] >= 9) {
                if (i == 1) {
                    solve[0] = square[++c[0] / 4];
                    set_rotation(0, c[0] % 4);

                    c[i] = 0;
                    break;
                }
                else {
                    c[i] = 0;
                    solve[i] = 0;
                    --i;
                    break;
                }
            }
            else if (!contains(square[c[i]])) {
                int ret = match(i, square[c[i]]);

                if (ret != -1) {
                    solve[i] = ret;
                    ++i;
                    break;
                }
            }
            ++c[i];
        }
    }
    after = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < 9; i++)
        printf("\n[OUTPUT] Square %i : [%i, %i, %i, %i]", i, get_side(solve[i], 0), get_side(solve[i], 1), get_side(solve[i], 2), get_side(solve[i], 3));

    printf("\n[MATH] Execution time: %lli microseconds", std::chrono::duration_cast<std::chrono::microseconds>(after - before).count());
}

int get_side(int s, int i) {

    int p = (3 * clamp(i - (s >> 12), 4, 0));
    int k = ((1 << 3) - 1) << p;

    int ret = (s & k) >> p;

    return ((ret & 0b100) >= 0b100) ? -(ret-0b100+1) : (ret+1);
}

int clamp(int val, int max, int min) {
    while (val >= max)
        val -= max;
    while (val < min)
        val += max;

    return val;
}

void set_rotation(int i, int r) {
    solve[i] += r * (1 << 12);
}

int match(int i, int s ) {
    switch (i) {
    case 1:
    case 2:
        for (int j = 0; j < 4; j++) {
            if (j > 0) s += (1 << 12);

            if (get_side(solve[i-1], 1) == -get_side(s, 3))
                return s;
        }
        return -1;
    case 3:
    case 6:
        for (int j = 0; j < 4; j++) {
            if (j > 0) s += (1 << 12);

            if (get_side(solve[i - 3], 2) == -get_side(s, 0)) 
                return s;
        }
        return -1;
    case 4:
    case 5:
    case 7:
    case 8:
        for (int j = 0; j < 4; j++) {
            if (j > 0) s += (1 << 12);

            if ((get_side(solve[i-1], 1) == -get_side(s, 3)) &&
                (get_side(solve[i-3], 2) == -get_side(s, 0)))
                return s;
        }
        return -1;
    default:
        return -1;
    }
}

bool contains(int i) {
    for (int j = 0; j < 9; j++)
        if ((i & solve[j]) == i)
            return true;
        else if (solve[j] == 0)
            return false;
 
    return false;
}
