#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

typedef enum {
    TOKEN_NUMBER,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_MULTIPLY,
    TOKEN_DIVIDE,
    TOKEN_OPEN_PAREN,
    TOKEN_CLOSE_PAREN,
    TOKEN_END,
    TOKEN_INVALID
} TokenType;

typedef struct {
    TokenType type;
    double value;
} Token;

const char *input;
size_t pos;

Token get_next_token() {
    while (input[pos] != '\0') {
        if (isspace(input[pos])) {
            pos++;
            continue;
        }
        if (isdigit(input[pos])) {
            double value = 0;
            while (isdigit(input[pos])) {
                value = value * 10 + (input[pos] - '0');
                pos++;
            }
            return (Token){TOKEN_NUMBER, value};
        }
        if (input[pos] == '+') {
            pos++;
            return (Token){TOKEN_PLUS, 0};
        }
        if (input[pos] == '-') {
            pos++;
            return (Token){TOKEN_MINUS, 0};
        }
        if (input[pos] == '*') {
            pos++;
            return (Token){TOKEN_MULTIPLY, 0};
        }
        if (input[pos] == '/') {
            pos++;
            return (Token){TOKEN_DIVIDE, 0};
        }
        if (input[pos] == '(') {
            pos++;
            return (Token){TOKEN_OPEN_PAREN, 0};
        }
        if (input[pos] == ')') {
            pos++;
            return (Token){TOKEN_CLOSE_PAREN, 0};
        }
        return (Token){TOKEN_INVALID, 0};
    }
    return (Token){TOKEN_END, 0};
}

double parse_expression();

double parse_factor() {
    Token token = get_next_token();
    
    if (token.type == TOKEN_NUMBER) {
        return token.value;
    } else if (token.type == TOKEN_OPEN_PAREN) {
        double result = parse_expression();
        token = get_next_token();
        if (token.type != TOKEN_CLOSE_PAREN) {
            fprintf(stderr, "Error: Expected closing parenthesis\n");
            exit(EXIT_FAILURE);
        }
        return result;
    }

    fprintf(stderr, "Error: Expected a number or opening parenthesis\n");
    exit(EXIT_FAILURE);
}

double parse_term() {
    double result = parse_factor();
    Token token = get_next_token();

    while (token.type == TOKEN_MULTIPLY || token.type == TOKEN_DIVIDE) {
        if (token.type == TOKEN_MULTIPLY) {
            result *= parse_factor();
        } else if (token.type == TOKEN_DIVIDE) {
            double divisor = parse_factor();
            if (divisor == 0) {
                fprintf(stderr, "Error: Division by zero\n");
                exit(EXIT_FAILURE);
            }
            result /= divisor;
        }
        token = get_next_token();
    }

    pos--;
    return result;
}

double parse_expression() {
    double result = parse_term();
    Token token = get_next_token();

    while (token.type == TOKEN_PLUS || token.type == TOKEN_MINUS) {
        if (token.type == TOKEN_PLUS) {
            result += parse_term();
        } else if (token.type == TOKEN_MINUS) {
            result -= parse_term();
        }
        token = get_next_token();
    }

    pos--;
    return result;
}

int main() {
    input = "3 + (5 * 2) - (4 / 2)";
    pos = 0;

    double result = parse_expression();

    printf("Result: %f\n", result);

    return 0;
}
