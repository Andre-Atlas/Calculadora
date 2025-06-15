#include "expressao.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define MAX_STACK_SIZE 512
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// --- Stack for doubles (for postfix evaluation) ---
typedef struct {
    double items[MAX_STACK_SIZE];
    int top;
} DoubleStack;

void initDoubleStack(DoubleStack *s) {
    s->top = -1;
}
int isDoubleStackEmpty(DoubleStack *s) {
    return s->top == -1;
}
int isDoubleStackFull(DoubleStack *s) {
    return s->top == MAX_STACK_SIZE - 1;
}
void pushDouble(DoubleStack *s, double value) {
    if (!isDoubleStackFull(s)) {
        s->items[++(s->top)] = value;
    } else {
        fprintf(stderr, "Error: Double stack overflow\n");
    }
}
double popDouble(DoubleStack *s) {
    if (!isDoubleStackEmpty(s)) {
        return s->items[(s->top)--];
    }
    fprintf(stderr, "Error: Double stack underflow\n");
    return NAN; 
}
double peekDouble(DoubleStack *s) {
    if (!isDoubleStackEmpty(s)) {
        return s->items[s->top];
    }
    fprintf(stderr, "Error: Double stack peek on empty stack\n");
    return NAN;
}

// --- Stack for chars (for infix to postfix operators) ---
typedef struct {
    char items[MAX_STACK_SIZE][32]; 
    int top;
} CharStack;

void initCharStack(CharStack *s) {
    s->top = -1;
}
int isCharStackEmpty(CharStack *s) {
    return s->top == -1;
}
int isCharStackFull(CharStack *s) {
    return s->top == MAX_STACK_SIZE - 1;
}
void pushChar(CharStack *s, const char* value) {
    if (!isCharStackFull(s)) {
        if (strlen(value) < 32) { // Ensure token fits
             strcpy(s->items[++(s->top)], value);
        } else {
            fprintf(stderr, "Error: Operator/token too long for char stack\n");
        }
    } else {
        fprintf(stderr, "Error: Char stack overflow\n");
    }
}
char* popChar(CharStack *s) {
    if (!isCharStackEmpty(s)) {
        return s->items[(s->top)--];
    }
    fprintf(stderr, "Error: Char stack underflow\n");
    return NULL; 
}
char* peekChar(CharStack *s) {
    if (!isCharStackEmpty(s)) {
        return s->items[s->top];
    }
    return NULL; 
}


// --- Stack for strings (for postfix to infix conversion) ---
typedef struct {
    char items[MAX_STACK_SIZE][512]; 
    int top;
} StringStack;

void initStringStack(StringStack *s) {
    s->top = -1;
}
int isStringStackEmpty(StringStack *s) {
    return s->top == -1;
}
int isStringStackFull(StringStack *s) {
    return s->top == MAX_STACK_SIZE - 1;
}
void pushString(StringStack *s, const char* value) {
    if (!isStringStackFull(s)) {
        if (strlen(value) < 512) {
            strcpy(s->items[++(s->top)], value);
        } else {
            fprintf(stderr, "Error: String too long for stack\n");
        }
    } else {
        fprintf(stderr, "Error: String stack overflow\n");
    }
}
char* popString(StringStack *s) {
    if (!isStringStackEmpty(s)) {
        return s->items[(s->top)--];
    }
    fprintf(stderr, "Error: String stack underflow\n");
    return NULL;
}


// --- Helper Functions ---

int is_number(const char *token) {
    if (!token || *token == '\0') return 0;
    char *endptr;
    strtod(token, &endptr); // Check if it can be converted to double
    // If endptr is not at the end of the string, it means not the whole token was a number
    // (unless it's something like "123xyz", which strtod handles by stopping at 'x')
    return (*endptr == '\0'); 
}

int is_function(const char *token) {
    if (!token) return 0;
    return strcmp(token, "sen") == 0 || strcmp(token, "cos") == 0 ||
           strcmp(token, "tg") == 0 || strcmp(token, "log") == 0 ||
           strcmp(token, "raiz") == 0;
}

int precedence(const char *op) {
    if (!op) return 0;
    if (is_function(op)) return 4; 
    if (strcmp(op, "^") == 0) return 3;
    if (strcmp(op, "*") == 0 || strcmp(op, "/") == 0 || strcmp(op, "%") == 0) return 2;
    if (strcmp(op, "+") == 0 || strcmp(op, "-") == 0) return 1;
    return 0; 
}

int is_right_associative(const char *op) {
    if (!op) return 0;
    return strcmp(op, "^") == 0;
}

double degrees_to_radians(double degrees) {
    return degrees * (M_PI / 180.0);
}

// --- Core Function Implementations ---

char *getFormaPosFixa(char *StrInFixa) {
    CharStack opStack;
    initCharStack(&opStack);
    // Allocate enough space: each token + a space, plus null terminator. Max 2 chars per input char (e.g. "1" -> "1 ")
    char *output = (char *)malloc(strlen(StrInFixa) * 2 + 2); 
    if (!output) {
        fprintf(stderr, "Error: Memory allocation failed for postfix string.\n");
        return NULL;
    }
    output[0] = '\0';
    
    // strtok modifies the string, so make a copy
    char tempInput[strlen(StrInFixa) + 1];
    strcpy(tempInput, StrInFixa);

    char *token;

    token = strtok(tempInput, " "); // Using strtok
    while (token != NULL) {
        if (is_number(token)) {
            strcat(output, token);
            strcat(output, " ");
        } else if (is_function(token)) {
            pushChar(&opStack, token);
        } else if (strcmp(token, "(") == 0) {
            pushChar(&opStack, token);
        } else if (strcmp(token, ")") == 0) {
            while (!isCharStackEmpty(&opStack) && strcmp(peekChar(&opStack), "(") != 0) {
                strcat(output, popChar(&opStack));
                strcat(output, " ");
            }
            if (!isCharStackEmpty(&opStack) && strcmp(peekChar(&opStack), "(") == 0) {
                popChar(&opStack); // Pop '('
            } else {
                fprintf(stderr, "Error: Mismatched parentheses in infix expression (missing '('?).\n");
                free(output);
                return NULL;
            }
            if (!isCharStackEmpty(&opStack) && is_function(peekChar(&opStack))) {
                strcat(output, popChar(&opStack));
                strcat(output, " ");
            }
        } else { // Operator
            while (!isCharStackEmpty(&opStack) &&
                   strcmp(peekChar(&opStack), "(") != 0 &&
                   (precedence(peekChar(&opStack)) > precedence(token) ||
                    (precedence(peekChar(&opStack)) == precedence(token) && !is_right_associative(token)))) {
                strcat(output, popChar(&opStack));
                strcat(output, " ");
            }
            pushChar(&opStack, token);
        }
        token = strtok(NULL, " "); // Using strtok
    }

    while (!isCharStackEmpty(&opStack)) {
        if (strcmp(peekChar(&opStack), "(") == 0) { // Check for unmatched opening parenthesis
             fprintf(stderr, "Error: Mismatched parentheses (remaining '(') in infix expression.\n");
             free(output);
             return NULL;
        }
        strcat(output, popChar(&opStack));
        strcat(output, " ");
    }

    // Remove trailing space if any
    if (strlen(output) > 0 && output[strlen(output) - 1] == ' ') {
        output[strlen(output) - 1] = '\0'; 
    }
    return output;
}


float getValorPosFixa(char *StrPosFixa) {
    DoubleStack valStack;
    initDoubleStack(&valStack);
    
    char tempInput[strlen(StrPosFixa) + 1];
    strcpy(tempInput, StrPosFixa);

    char *token;

    token = strtok(tempInput, " "); // Using strtok
    while (token != NULL) {
        if (is_number(token)) {
            pushDouble(&valStack, atof(token));
        } else { 
            double val1, val2, result;
            if (is_function(token)) { 
                if (isDoubleStackEmpty(&valStack)) {fprintf(stderr, "Error: Insufficient operands for function %s\n", token); return NAN;}
                val1 = popDouble(&valStack);
                if (strcmp(token, "log") == 0) { 
                    if (val1 <= 0) {fprintf(stderr, "Error: Log of non-positive value.\n"); return NAN;}
                    result = log10(val1);
                } else if (strcmp(token, "sen") == 0) {
                    result = sin(degrees_to_radians(val1)); 
                } else if (strcmp(token, "cos") == 0) {
                    result = cos(degrees_to_radians(val1)); 
                } else if (strcmp(token, "tg") == 0) {
                    // Avoid tan of 90, 270 degrees etc. (where cos is 0)
                    if (fabs(cos(degrees_to_radians(val1))) < 1e-9) { // Check if cos is very close to 0
                        fprintf(stderr, "Error: Tangent undefined (division by zero).\n"); return NAN;
                    }
                    result = tan(degrees_to_radians(val1)); 
                } else if (strcmp(token, "raiz") == 0) {
                    if (val1 < 0) {fprintf(stderr, "Error: Square root of negative value.\n"); return NAN;}
                    result = sqrt(val1);
                } else {
                     fprintf(stderr, "Error: Unknown function %s\n", token); return NAN;
                }
                pushDouble(&valStack, result);
            } else { 
                if (isDoubleStackEmpty(&valStack)) {fprintf(stderr, "Error: Insufficient operands for operator %s (missing op2)\n", token); return NAN;}
                val2 = popDouble(&valStack);
                if (isDoubleStackEmpty(&valStack)) {fprintf(stderr, "Error: Insufficient operands for operator %s (missing op1)\n", token); return NAN;}
                val1 = popDouble(&valStack);

                if (strcmp(token, "+") == 0) result = val1 + val2;
                else if (strcmp(token, "-") == 0) result = val1 - val2;
                else if (strcmp(token, "*") == 0) result = val1 * val2;
                else if (strcmp(token, "/") == 0) {
                    if (fabs(val2) < 1e-9) {fprintf(stderr, "Error: Division by zero.\n"); return NAN;} // Check for division by zero
                    result = val1 / val2;
                } else if (strcmp(token, "%") == 0) {
                    if (fabs(val2) < 1e-9) {fprintf(stderr, "Error: Modulo by zero.\n"); return NAN;} // Check for modulo by zero
                    result = fmod(val1, val2);
                } else if (strcmp(token, "^") == 0) {
                    result = pow(val1, val2);
                } else {
                    fprintf(stderr, "Error: Unknown operator %s\n", token); return NAN;
                }
                pushDouble(&valStack, result);
            }
        }
        token = strtok(NULL, " "); // Using strtok
    }

    if (valStack.top == 0) { // Exactly one value should remain
        return (float)popDouble(&valStack);
    } else if (valStack.top == -1){
        fprintf(stderr, "Error: No result in stack. Malformed postfix or empty expression.\n");
        return NAN;
    }
    else {
        fprintf(stderr, "Error: Malformed postfix expression (too many operands left on stack).\n");
        while(!isDoubleStackEmpty(&valStack)) popDouble(&valStack); 
        return NAN;
    }
}


char *getFormaInFixa(char *StrPosFixa) {
    StringStack exprStack;
    initStringStack(&exprStack);
    
    char tempInput[strlen(StrPosFixa) + 1];
    strcpy(tempInput, StrPosFixa);

    char *token;
    char buffer[512]; 

    token = strtok(tempInput, " "); // Using strtok
    while (token != NULL) {
        if (is_number(token)) {
            pushString(&exprStack, token);
        } else if (is_function(token)) { 
            if (isStringStackEmpty(&exprStack)) {fprintf(stderr, "Error: Insufficient operands for function %s in P->I\n", token); return NULL;}
            char* op1_str_ptr = popString(&exprStack); 
            if (!op1_str_ptr) return NULL; // Error from popString
            char op1_copy[512]; // Make a copy as popString returns pointer to internal buffer
            strcpy(op1_copy, op1_str_ptr); 

            snprintf(buffer, sizeof(buffer), "%s ( %s )", token, op1_copy);
            pushString(&exprStack, buffer);
        } else { // Binary operators
            if (isStringStackEmpty(&exprStack)) {fprintf(stderr, "Error: Insufficient operands for op %s in P->I (op2)\n", token); return NULL;}
            char* op2_str_ptr = popString(&exprStack);
            if (!op2_str_ptr) return NULL;
            char op2_copy[512];
            strcpy(op2_copy, op2_str_ptr);

            if (isStringStackEmpty(&exprStack)) {fprintf(stderr, "Error: Insufficient operands for op %s in P->I (op1)\n", token); return NULL;}
            char* op1_str_ptr = popString(&exprStack);
            if (!op1_str_ptr) return NULL;
            char op1_copy[512];
            strcpy(op1_copy, op1_str_ptr);
            
            snprintf(buffer, sizeof(buffer), "( %s %s %s )", op1_copy, token, op2_copy);
            pushString(&exprStack, buffer);
        }
        token = strtok(NULL, " "); // Using strtok
    }

    if (exprStack.top == 0) { // Exactly one expression string should remain
        char* finalExpr_ptr = popString(&exprStack);
        if (!finalExpr_ptr) return NULL;
        char* result = strdup(finalExpr_ptr); 
        if (!result) {fprintf(stderr, "Error: Memory allocation failed for final infix string.\n"); return NULL;}
        return result;
    } else {
        fprintf(stderr, "Error: Malformed postfix expression for infix conversion (stack not empty or too many items).\n");
        while(!isStringStackEmpty(&exprStack)) popString(&exprStack); 
        return NULL;
    }
}

float getValorInFixa(char *StrInFixa) {
    char *posFixaExpr = getFormaPosFixa(StrInFixa);
    if (posFixaExpr == NULL) {
        return NAN; 
    }
    float result = getValorPosFixa(posFixaExpr);
    free(posFixaExpr); 
    return result;
}