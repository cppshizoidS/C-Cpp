#include <cstdio>   //finding the greatest common factor
#include <clocale>
#include <cstdlib>
#include <ctime>

using namespace std;


long gcd01(long a, long b) {

    long nod = 1L;
    for (long i = a; i > 0; i--) {
        if (a % i == 0 && b % i == 0) {
            nod = i;
            break;
        }
    }
    return nod;
}

long min(long a, long b) {
    return a > b ? b : a;
}

long gcd02(long a, long b) {

    long nod = 1L;
    for (long i = min(a, b); i > 0; i--) {
        if (a % i == 0 && b % i == 0) {
            nod = i;
            break;
        }
    }
    return nod;
}

long gcd03(long a, long b) {

    long nod = 1L;

    if (a > b) {
        long tmp = a;
        a = b;
        b = tmp;
    }

    while (a > 1L && b > 1L) {
        for (long i = 2; i <= a; i++) {
            if (a % i == 0 && b % i == 0) {
                nod *= i;
                a /= i;
                b /= i;
                break;
            }
            if (a % i == 0) {
                a /= i;
                break;
            }
            if (b % i == 0) {
                b /= i;
                break;
            }
        }
    }
    return nod;
}

long gcd04(long a, long b) {

    if (a == b) {
        return a;
    }
    if (a > b) {
        long tmp = a;
        a = b;
        b = tmp;
    }
    return gcd04(a, b - a);
}

long gcd05(long a, long b) {

    while (a != b) {
        if (a > b) {
            long tmp = a;
            a = b;
            b = tmp;
        }
        b = b - a;
    }
    return a;
}

long gcd06(long a, long b) {
    if (a == 0L)
        return b;
    if (b == 0L)
        return a;
    if (a == b)
        return a;
    if (a == 1L || b == 1L)
        return 1L;
    if (a % 2L == 0L && b % 2L == 0L)
        return 2L * gcd06(a / 2L, b / 2L);
    if (a % 2L == 0L && b % 2L != 0L)
        return gcd06(a / 2L, b);
    if (a % 2L != 0L && b % 2L == 0L)
        return gcd06(a, b / 2L);
    if (a < b)
        return gcd06((b - a) / 2L, a);
    else
        return gcd06((a - b) / 2L, b);
}

long gcd07(long a, long b) {
    long nod = 1L;
    long tmp;
    if (a == 0L)
        return b;
    if (b == 0L)
        return a;
    if (a == b)
        return a;
    if (a == 1L || b == 1L)
        return 1L;
    while (a != 0 && b != 0) {
        if (a % 2L == 0L && b % 2L == 0L) {
            nod *= 2L;
            a /= 2L;
            b /= 2L;
            continue;
        }
        if (a % 2L == 0L && b % 2L != 0L) {
            a /= 2L;
            continue;
        }
        if (a % 2L != 0L && b % 2L == 0L) {
            b /= 2L;
            continue;
        }
        if (a > b) {
            tmp = a;
            a = b;
            b = tmp;
        }
        tmp = a;
        a = (b - a) / 2L;
        b = tmp;
    }
    if (a == 0)
        return nod * b;
    else
        return nod * a;
}

long gcd08(long a, long b) {
    long nod = 1L;
    long tmp;
    if (a == 0L)
        return b;
    if (b == 0L)
        return a;
    if (a == b)
        return a;
    if (a == 1L || b == 1L)
        return 1L;
    while (a != 0 && b != 0) {
        if (((a & 1L) | (b & 1L)) == 0L) {
            nod <<= 1L;
            a >>= 1L;
            b >>= 1L;
            continue;
        }
        if (((a & 1L) == 0L) && (b & 1L)) {
            a >>= 1L;
            continue;
        }
        if ((a & 1L) && ((b & 1L) == 0L)) {
            b >>= 1L;
            continue;
        }
        if (a > b) {
            tmp = a;
            a = b;
            b = tmp;
        }
        tmp = a;
        a = (b - a) >> 1L;
        b = tmp;
    }
    if (a == 0)
        return nod * b;
    else
        return nod * a;
}

struct sub {
    long(*func)(long, long);
    const char* comm;
};


sub arr[] = {
    { gcd01, "01 перебор от произвольного числа       " },
    { gcd02, "02 перебор от минимального числа        " },
    { gcd03, "03 с разложением на делители            " },
    { gcd04, "04 алгоритм Евклида рекурсивный         " },
    { gcd05, "05 алгоритм Евклида итерационный        " },
    { gcd06, "06 бинарный алгоритм рекурсивный        " },
    { gcd07, "07 бинарный алгоритм итерационный       " },
    { gcd08, "08 бинарный алгоритм итерац. со сдвигом " }
};


const unsigned int RAND_TIMES = 500u;
const unsigned long REPEAT_TIMES = 10000uL;

int main() {

    clock_t the_time;
    double elapsed_time;

    setlocale(LC_ALL, "Russian");

    long a, b, c, nod;

    srand((unsigned int)time(NULL));
    double times[sizeof(arr) / sizeof(sub)] = { 0.0 };

    for (unsigned int t = 0u; t < RAND_TIMES; t++) {

        c = rand() % 50 + 1;
        a = (rand() % 1000 + 1) * c;
        b = (rand() % 1000 + 1) * c;

        for (unsigned int alg = 0u; alg < sizeof(arr) / sizeof(sub); alg++) {

            the_time = clock();

            for (unsigned long m = 0uL; m < REPEAT_TIMES; m++) {
                nod = arr[alg].func(a, b);
            }

            elapsed_time = double(clock() - the_time) / CLOCKS_PER_SEC;
            times[alg] += elapsed_time;
        }
        printf("%4u НОД(%7ld, %7ld) = %7ld\n", t + 1, a, b, nod);
    }

    printf("\nСреднее время для %d пар случайных чисел:\n", RAND_TIMES);
    for (unsigned int alg = 0; alg < sizeof(arr) / sizeof(sub); alg++) {
        printf("%s: %8.4f сек.\n", arr[alg].comm, times[alg] / RAND_TIMES);
    }

    return 0;

}