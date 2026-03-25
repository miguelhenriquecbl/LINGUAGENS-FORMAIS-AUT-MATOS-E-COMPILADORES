#include <stdio.h>
#include <ctype.h>

typedef enum { E0, E1, E2, E3, E4, E5, E6, E7, DEAD} State;

int is_not_digit_1_9 (char c);

static State step(State s, char x) {

    switch (s) {
    case E0:
        if (!is_not_digit_1_9(x)) return E3;
        if (x == '+' || x == '-') return E1;
        if (x == '.') return E2;
        break;

    case E1:
        if (!is_not_digit_1_9(x)) return E3;
        if (x == '.') return E2;
        break;

    case E2:
        if (!is_not_digit_1_9(x)) return E4;
        break;
        
    case E3:
        if (!is_not_digit_1_9(x)) return E3;
        if (x == 'E' || x == 'e') return E5;
        if (x == '.') return E4;
        break;

    case E4:
        if (!is_not_digit_1_9(x)) return E4;
        if (x == 'E' || x == 'e') return E5;
        break;
        
    case E5:
        if (!is_not_digit_1_9(x)) return E7;
        if (x == '+' || x == '-') return E6;
        break;

    case E6:
        if (!is_not_digit_1_9(x)) return E7;
        break;
        
    case E7:
        if (!is_not_digit_1_9(x)) return E7;
        break;

    default:
        break;
    }

    return DEAD;

}

int main() {

    char buf[1024];

    while (fgets(buf, sizeof buf, stdin)) {
        State e0 = E0;
        int ok = 1;

        for (size_t i = 0; buf[i]; i++) {
            unsigned char ch = (unsigned char) buf[i];

            if (ch == '\n' || ch == '\r' || isspace(ch)) continue;
            if (is_not_digit_1_9(ch) && ch != '.' && ch != '+' && ch != '-' && ch != 'E' && ch != 'e') { ok = 0; break; }

            e0 = step(e0, (char)ch);
            if (e0 == DEAD) { ok = 0; break;}
        }

        if (ok && (e0 == E3 || e0 == E4 || e0 == E7) ) puts("ACEITA");
        else puts("REJEITA");
    }

    return 0;
}


int is_not_digit_0_9 (char c) {
    return (c != '0' && c != '1' && c != '2' && c != '3' && c != '4' && c != '5' && c != '6' && c != '7' && c != '8' && c != '9');
}
