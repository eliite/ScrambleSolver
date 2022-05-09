// Binary.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <bitset>
#include <chrono>
#include <math.h>
#include <format>
#include <vector>
#include <Windows.h>

void set_side(int a, int b, int i);
void set_side(int a, int b);
int calculate_last();

void reset();
int get_side(int s, int i);
void set_rotation(int i, int r);
int clamp(int, int, int);
int match(int i, int s); 

bool contains(int i);

static int square[] = {
        0b00110000010111,
        0b00010001111001,
        0b00011000100101,
        0b00111101110100,
        0b00011110100001,
        0b00000101111010,
        0b00011001100110,
        0b00000010111101,
        0b00011101000010
};

static int solve[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
static int c[] =     { 0, 0, 0, 0, 0, 0, 0, 0, 0 };

static int count[] = { 0, 0, 0 };

static int used = 0;

static std::chrono::steady_clock::time_point before, after;

int main()
{
    before = std::chrono::high_resolution_clock::now();

    set_side(0, 0);
    for (int i = 1; i < 9; ) {
        while (c[i] < 9) {
            ++count[0];

            //printf("c[%i,%i,%i,%i,%i,%i,%i,%i,%i]\n", c[0], c[1], c[2], c[3], c[4], c[5], c[6], c[7], c[8]);

            if (i == 8) {
                int ret = calculate_last();
                int m = match(i, square[ret]);
                if (m != -1) {
                    set_side(i, m, ret);
                    c[i] = ret;
                    i = 9;
                    break;
                }
                else {
                    ++c[i - 1];
                    --i;
                    break;
                }
            }

            if (!contains(c[i])) {
                int ret = match(i, square[c[i]]);

                if (ret != -1) {
                    set_side(i, ret, c[i]);
                    ++i;

                    break;
                }
            }
            if (c[i] == 8) {
                if (c[i - 1] < 8) {
                    c[i] = 0;
                    ++c[i - 1];

                    solve[i] = 0;

                    if (i == 1) {
                        set_side(0, c[0] >> 2);
                        set_rotation(0, c[0] & 3);
                    }
                    else --i;
                }
                else if (i != 1) {
                    if (i > 2) {
                        solve[i] = 0;
                        solve[i - 1] = 0;

                        c[i] = 0;
                        c[i - 1] = 0;
                        ++c[i - 2];

                        i -= 2;
                    }
                    else {
                        solve[i] = 0;
                        solve[i - 1] = 0;

                        c[i] = 0;
                        c[i - 1] = 0;

                        set_side(0, ++c[0] >> 2);
                        set_rotation(0, c[0] & 3);

                        i = 1;
                    }
                } 
                else {
                    c[i] = 0;

                    solve[i] = 0;

                    set_side(0, ++c[0] >> 2);
                    set_rotation(0, c[0] & 3);

                    i = 1;
                }
                reset();
                break;
            }
            ++c[i];
        }
    }
    after = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < 9; i++)
        printf("\n[OUTPUT] Square %i : [%i, %i, %i, %i]", i, get_side(solve[i], 0), get_side(solve[i], 1), get_side(solve[i], 2), get_side(solve[i], 3));

    printf("\n\n[SQUARES]\n%i(%i) %i(%i) %i(%i)\n%i(%i) %i(%i) %i(%i)\n%i(%i) %i(%i) %i(%i)\n", 
        c[0]/4, (solve[0] & (0b11 << 12))>> 12, c[1], (solve[1] & (0b11 << 12))>> 12, c[2], (solve[2] & (0b11 << 12)) >> 12,
        c[3], (solve[3] & (0b11 << 12))>> 12, c[4], (solve[4] & (0b11 << 12))>> 12, c[5], (solve[5] & (0b11 << 12)) >> 12,
        c[6], (solve[6] & (0b11 << 12))>> 12, c[7], (solve[7] & (0b11 << 12))>> 12, c[8], (solve[8] & (0b11 << 12)) >> 12);

    printf("\n[OUTPUT] iterations = %i", count[0]);
    printf("\n[OUTPUT] matches = %i", count[1]);
    printf("\n[OUTPUT] contains = %i", count[2]);
    printf("\n[MATH] Execution time: %lli microseconds", std::chrono::duration_cast<std::chrono::microseconds>(after - before).count());

    Sleep(INFINITE);
}

int calculate_last() {
    int temp = used;
    temp ^= 0b111111111;

    return log2(temp);
}

void set_side(int a, int b) {
    solve[a] = square[b];

    used |= (1 << b);
}

void set_side(int a, int b, int i) {
    solve[a] = b;

    used |= (1 << i);
}

int get_side(int s, int i) {

    int p = 9-(3 * clamp(i - (s >> 12), 4, 0));
    int k = (7) << p;

    int ret = (s & k) >> p;

    return ((ret & 4) >= 4) ? -(ret-3) : (ret+1);
}

int clamp(int val, int max, int min) {
    while (val >= max)
        val -= max;
    while (val < min)
        val += max;

    return val;
}

void set_rotation(int i, int r) {
    solve[i] += r * 4096;
}

int match(int i, int s) {
    ++count[1];

    switch (i) {
    case 1:
    case 2:
        for (int j = 0; j < 4; j++) {
            if (j > 0) s += 4096;

            if (get_side(solve[i-1], 1) == -get_side(s, 3))
                return s;
        }
        return -1;
    case 3:
    case 6:
        for (int j = 0; j < 4; j++) {
            if (j > 0) s += 4096;

            if (get_side(solve[i - 3], 2) == -get_side(s, 0)) 
                return s;
        }
        return -1;
    case 4:
    case 5:
    case 7:
    case 8:
        for (int j = 0; j < 4; j++) {
            if (j > 0) s += 4096;

            if ((get_side(solve[i-1], 1) == -get_side(s, 3)) &&
                (get_side(solve[i-3], 2) == -get_side(s, 0)))
                return s;
        }
        return -1;
    default:
        return -1;
    }
}

void reset() {
    int z = 0;

    used = 0;
    for (int i = 1; i < 9; i++) {
        if (solve[i] == 0 && ++z == 2)
            return;

        used |= (1 << c[i]);
    }
    used |= (1 << (c[0] / 4));
}

bool contains(int i) {
    ++count[2];

    if (((used >> i) & 1) == 1) 
        return true;
    else
        return false;
}
