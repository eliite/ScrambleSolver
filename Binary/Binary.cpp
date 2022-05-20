#include <chrono>
#include <Windows.h>

// calculate the entire board
inline void calculate();
// backtrack when two squares are impossible matches
inline void reset(int j);
// rotate a piece on the solved board
inline void set_rotation(int i, int r);
// set end[a] = b, where the piece index is i
inline void set_side(int a, int b, int i);
// set end[a] = start[b], where the piece index is b
inline void set_side(int a, int b);
// print the metrics and full output of the program
inline void print_board();

// with rotations, we can +-max to get an equal rotation
inline int clamp(int, int, int);
// deconstruct binary integer to find a certain side
inline int get_side(int s, int i);
// attempt to match two squares
inline int match(int i, int s); 

// use a 9-bit string to identify placed squares
inline bool contains(int i);

const int start[] = {
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

int end[] =     { 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    count[] =   { 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    metric[] =  { 0, 0, 0 },
    used =        0;

std::chrono::steady_clock::time_point before, after;

int main()
{
    calculate();
    print_board();
}


inline void calculate() {
    before = std::chrono::high_resolution_clock::now();

    set_side(0, 0);
    for (int i = 1; i < 9; ) {
        while (count[i] < 9) {
            ++metric[0];

            if (!contains(count[i])) {
                int ret = match(i, start[count[i]]);

                if (ret != -1) {
                    set_side(i, ret, count[i]);
                    ++i;

                    break;
                }
            }

            if (count[i] == 8) {
                count[i] = 0;
                end[i] = 0;
                if (count[i - 1] < 8) {
                    ++count[i - 1];

                    if (i == 1) {
                        set_side(0, count[0] >> 2);
                        set_rotation(0, count[0] & 3);
                    }
                    else --i;
                }
                else if (i != 1) {
                    end[i - 1] = 0;

                    count[i - 1] = 0;
                    switch (i) {
                    case 2:
                        set_side(0, ++count[0] >> 2);
                        set_rotation(0, count[0] & 3);

                        i = 1;
                        break;
                    default:
                        i -= 2;
                        ++count[i];
                        break;
                    }
                }
                else {
                    set_side(0, ++count[0] >> 2);
                    set_rotation(0, count[0] & 3);

                    i = 1;
                }
                reset(i);
                break;
            }
            ++count[i];
        }
    }
    after = std::chrono::high_resolution_clock::now();
}

inline void print_board() {
    for (int i = 0; i < 9; i++)
        printf("[OUTPUT] Square %i : [%i, %i, %i, %i]\n", i, get_side(end[i], 0), get_side(end[i], 1), get_side(end[i], 2), get_side(end[i], 3));

    printf("\n[SQUARES]\n%i(%i)\t%i(%i)\t%i(%i)\n%i(%i)\t%i(%i)\t%i(%i)\t\n%i(%i)\t%i(%i)\t%i(%i)\t\n",
        count[0] / 4, (end[0] >> 12) & 3, count[1], (end[1] >> 12) & 3, count[2], (end[2] >> 12) & 3,
        count[3], (end[3] >> 12) & 3, count[4], (end[4] >> 12) & 3, count[5], (end[5] >> 12) & 3,
        count[6], (end[6] >> 12) & 3, count[7], (end[7] >> 12) & 3, count[8], (end[8] >> 12) & 3);

    printf("\n[OUTPUT] iterations = %i", metric[0]);
    printf("\n[OUTPUT] contains = %i", metric[2]);
    printf("\n[OUTPUT] matches = %i", metric[1]);
    printf("\n[MATH] Execution time: %lli microseconds", std::chrono::duration_cast<std::chrono::microseconds>(after - before).count());

    Sleep(INFINITE);
}

inline void reset(int j) {
    int z = 0;

    if (count[0] == 0)
        ++z;

    used = 0 | (1 << (count[0] >> 2));
    for (int i = 1; i <= j; ++i) {
        if (end[i] == 0 && ++z == 2)
            return;

        used |= (1 << count[i]);
    }
}

inline void set_rotation(int i, int r) {
    end[i] += (r << 12);
}

inline void set_side(int a, int b) {
    end[a] = start[b];

    used |= (1 << b);
}

inline void set_side(int a, int b, int i) {
    end[a] = b;

    used |= (1 << i);
}

inline int clamp(int val, int max, int min) {
    while (val >= max)
        val -= max;
    while (val < min)
        val += max;

    return val;
}

inline int get_side(int s, int i) {
    int p = 9-(3 * clamp(i - (s >> 12), 4, 0));
    int k = 7 << p;

    int ret = (s & k) >> p;

    return ((ret & 4) >= 4) ? -(ret-3) : (ret+1);
}

inline int match(int i, int s) {
    ++metric[1];

    switch (i) {
    case 1:
    case 2:
        for (int j = 0; j < 4; ++j) {
            if (j > 0) s += 4096;

            if (get_side(end[i-1], 1) == -get_side(s, 3))
                return s;
        }
        return -1;
    case 3:
    case 6:
        for (int j = 0; j < 4; ++j) {
            if (j > 0) s += 4096;

            if (get_side(end[i - 3], 2) == -get_side(s, 0)) 
                return s;
        }
        return -1;
    case 4:
    case 5:
    case 7:
    case 8:
        for (int j = 0; j < 4; ++j) {
            if (j > 0) s += 4096;

            if ((get_side(end[i-1], 1) == -get_side(s, 3)) &&
                (get_side(end[i-3], 2) == -get_side(s, 0)))
                return s;
        }
        return -1;
    }
}

inline bool contains(int i) {
    ++metric[2];

    return ((used >> i) & 1);
}