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

//combine strings
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
        expression[pos++] = ' '; 
    }
    expression[pos - 1] = '\0';  

    return expression;
}

// Convert a string to an unsigned integer
int convert_to_base(const char *str) {
    if (strncmp(str, "0b", 2) == 0) {  // Binary
        int value = 0;
        for (int i = 2; str[i] != '\0'; i++) {
            value = (value << 1) + (str[i] - '0');  // Shift left and add the bit
        }
        return value;
    } else if (strncmp(str, "0x", 2) == 0) {  // Hexadecimal
        int value = 0;
        for (int i = 2; str[i] != '\0'; i++) {
            if (isdigit(str[i])) {
                value = (value << 4) + (str[i] - '0');  // Shift left and add the digit
            } else {
                value = (value << 4) + (tolower(str[i]) - 'a' + 10);  // Convert a-f to 10-15
            }
        }
        return value;
    } else {  // Decimal
        int value = 0;
        for (int i = 0; str[i] != '\0'; i++) {
            value = value * 10 + (str[i] - '0');  // Multiply by 10 and add the digit
        }
        return value;
    }
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

        char temp[next_token - token + 1];
        strncpy(temp, token, next_token - token);
        temp[next_token - token] = '\0';  

        if (strcmp(temp, "add") == 0 || strcmp(temp, "sub") == 0 || strcmp(temp, "mult") == 0 || strcmp(temp, "div") == 0) {
            int b = pop();
            int a = pop(); 
            if (strcmp(temp, "add") == 0) {
                push(a + b);
            } else if (strcmp(temp, "sub") == 0) {
                push(a - b);
            } else if (strcmp(temp, "mult") == 0) {
                push(a * b);
            } else if (strcmp(temp, "div") == 0) {
                if (b != 0) {
                    push(a / b);  
                } else {
                    printf("Division by zero!\n");
                    exit(1);
                }
            }
        } else {
            int value = convert_to_base(temp);
            push(value);
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
    printf("%d\n", result);  

    free(expression);
    return 0;
}
