#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define STACK_SIZE 100

int stack[STACK_SIZE];  
int top = -1;

// Push
void push(int value) { 
    if (top < STACK_SIZE - 1) {
        stack[++top] = value;
    } else {
        printf("Stack overflow!\n");
        exit(1);
    }
}

// Pop
int pop(void) {  
    if (top >= 0) {
        return stack[top--];
    } else {
        printf("Stack underflow!\n");
        exit(1);  
    }
}

// Utility function to combine strings
char *combine_strings(int argc, char *argv[]) {
    // Calculate total length needed for expression
    int total_length = 0;
    for (int i = 1; i < argc; i++) {
        // Compute length of each argument plus one for space or null terminator
        for (int j = 0; argv[i][j] != '\0'; j++) {
            total_length++;
        }
        total_length++;  // For space
    }

    // Allocate memory for expression
    char *expression = (char *)malloc(total_length);
    if (expression == NULL) {
        printf("Memory allocation failed!\n");
        exit(1);  
    }

    // Combine all arguments into a single expression string
    int pos = 0;
    for (int i = 1; i < argc; i++) {
        int j = 0;
        while (argv[i][j] != '\0') {
            expression[pos++] = argv[i][j++];
        }
        expression[pos++] = ' ';  // Add space between arguments
    }
    expression[pos - 1] = '\0';  // Null-terminate the string

    return expression;
}

// Calculator
int calculator(const char *expression) { 
    const char *token = expression;
    while (*token != '\0') {
        // Skip leading whitespace
        while (*token == ' ') token++;

        // Find end of current token
        const char *next_token = token;
        while (*next_token != ' ' && *next_token != '\0') next_token++;

        char *endptr;
        long value = strtol(token, &endptr, 10);
        if (endptr == next_token) {
            push((int)value);  // Cast to int
        } else {
            int b = pop();
            int a = pop(); 
            if (strncmp(token, "add", next_token - token) == 0) {
                push(a + b);
            } else if (strncmp(token, "sub", next_token - token) == 0) {
                push(a - b);
            } else if (strncmp(token, "mult", next_token - token) == 0) {
                push(a * b);
            } else if (strncmp(token, "div", next_token - token) == 0) {
                if (b != 0) {
                    push(a / b);  // Integer division
                } else {
                    printf("Division by zero!\n");
                    exit(1);
                }
            } else {
                printf("Unknown operator: ");
                for (const char *c = token; c < next_token; c++) {
                    printf("%c", *c);
                }
                printf("\n");
                exit(1);
            }
        }
        token = (*next_token == '\0') ? next_token : next_token + 1;
    }
    int result = pop();
    return result;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <expression>\n", argv[0]);
        return 1;
    }

    char *expression = combine_strings(argc, argv);
    int result = calculator(expression);
    printf("%d\n", result);  // %d for integer output

    free(expression);
    return 0;
}
