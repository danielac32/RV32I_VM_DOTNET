// libgcc_sim.c
unsigned int __udivsi3(unsigned int a, unsigned int b) {
    unsigned int quotient = 0;
    if (b == 0) return 0; // O manejar error según necesites
    while (a >= b) {
        a -= b;
        quotient++;
    }
    return quotient;
}

unsigned int __umodsi3(unsigned int a, unsigned int b) {
    if (b == 0) return 0; // O manejar error
    while (a >= b) a -= b;
    return a;
}

int __divsi3(int a, int b) {
    int sign = 1;
    if (a < 0) {
        a = -a;
        sign = -sign;
    }
    if (b < 0) {
        b = -b;
        sign = -sign;
    }
    return sign * (int)__udivsi3(a, b);
}

int __modsi3(int a, int b) {
    return a - __divsi3(a, b) * b;
}

int __mulsi3(int a, int b) {
    int result = 0;
    while (b) {
        if (b & 1) result += a;
        a <<= 1;
        b >>= 1;
    }
    return result;
}
