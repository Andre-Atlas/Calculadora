#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h> // Added for free(), atof(), malloc() etc.
#include "expressao.h"

#ifndef M_PI 
#define M_PI 3.14159265358979323846
#endif


int float_approx_equals(float a, float b, float epsilon) {
    if (isnan(a) && isnan(b)) return 1; 
    if (isnan(a) || isnan(b)) return 0; 
    return fabs(a - b) < epsilon;
}

int main() {
    Expressao testes[15]; 
    int num_testes = 15;

    // --- Testes Originais ---
    // Test 1
    strcpy(testes[0].inFixa, "( 3 + 4 ) * 5");
    strcpy(testes[0].posFixa, "3 4 + 5 *");
    testes[0].Valor = 35.0f;

    // Test 2
    strcpy(testes[1].inFixa, "7 * 2 + 4");
    strcpy(testes[1].posFixa, "7 2 * 4 +");
    testes[1].Valor = 18.0f;

    // Test 3
    strcpy(testes[2].inFixa, "8 + ( 5 * ( 2 + 4 ) )");
    strcpy(testes[2].posFixa, "8 5 2 4 + * +");
    testes[2].Valor = 38.0f;

    // Test 4
    strcpy(testes[3].inFixa, "( 6 / 2 + 3 ) * 4");
    strcpy(testes[3].posFixa, "6 2 / 3 + 4 *");
    testes[3].Valor = 24.0f;

    // Test 5
    strcpy(testes[4].inFixa, "9 + ( 5 * ( 2 + 8 * 4 ) )");
    strcpy(testes[4].posFixa, "9 5 2 8 4 * + * +");
    testes[4].Valor = 179.0f; // Correção: 8*4=32, 32+2=34, 34*5=170, 170+9=179

    // Test 6
    strcpy(testes[5].inFixa, "log ( 2 + 3 ) / 5");
    strcpy(testes[5].posFixa, "2 3 + log 5 /");
    testes[5].Valor = (float) (log10(5.0) / 5.0); // Approx 0.13979

    // Test 7
    strcpy(testes[6].inFixa, "( log ( 10 ) ^ 3 ) + 2");
    strcpy(testes[6].posFixa, "10 log 3 ^ 2 +");
    testes[6].Valor = (float) (pow(log10(10.0), 3) + 2.0); // log10(10)=1, 1^3=1, 1+2=3

    // Test 8
    strcpy(testes[7].inFixa, "( 45 + 60 ) * cos ( 30 )");
    strcpy(testes[7].posFixa, "45 60 + 30 cos *");
    testes[7].Valor = (float) (105.0 * cos(30.0 * M_PI / 180.0)); // Approx 90.9326

    // Test 9
    strcpy(testes[8].inFixa, "sen ( 45 ) ^ 2 + 0.5");
    strcpy(testes[8].posFixa, "45 sen 2 ^ 0.5 +");
    testes[8].Valor = (float) (pow(sin(45.0 * M_PI / 180.0), 2) + 0.5); // (sqrt(2)/2)^2 + 0.5 = 0.5 + 0.5 = 1.0

    // --- 6 Novos Testes ---

    // Test 10: Funções aninhadas
    strcpy(testes[9].inFixa, "log ( tg ( 60 ) )");
    strcpy(testes[9].posFixa, "60 tg log");
    testes[9].Valor = (float) log10(tan(60.0 * M_PI / 180.0)); // log10(sqrt(3)) approx 0.2385

    // Test 11: Associatividade à direita do operador de potência
    strcpy(testes[10].inFixa, "4 ^ 3 ^ 2");
    strcpy(testes[10].posFixa, "4 3 2 ^ ^");
    testes[10].Valor = (float) pow(4, pow(3, 2)); // 4^9 = 262144

    // Test 12: Mistura de operadores e funções
    strcpy(testes[11].inFixa, "raiz ( 144 ) + 2 * ( 10 - 4 )");
    strcpy(testes[11].posFixa, "144 raiz 2 10 4 - * +");
    testes[11].Valor = (float) (sqrt(144) + 2 * (10 - 4)); // 12 + 2 * 6 = 12 + 12 = 24

    // Test 13: Operador de módulo
    strcpy(testes[12].inFixa, "17 % 5 * 3");
    strcpy(testes[12].posFixa, "17 5 % 3 *");
    testes[12].Valor = (float) (fmod(17, 5) * 3); // 2 * 3 = 6

    // Test 14: Resultado negativo
    strcpy(testes[13].inFixa, "5 - 10 * 2");
    strcpy(testes[13].posFixa, "5 10 2 * -");
    testes[13].Valor = -15.0f; // 5 - 20 = -15

    // Test 15: Expressão complexa
    strcpy(testes[14].inFixa, "( 5 + sen ( 90 ) ) * ( log ( 100 ) - 1 ) ^ 2");
    strcpy(testes[14].posFixa, "5 90 sen + 100 log 1 - 2 ^ *");
    testes[14].Valor = (float) ((5 + sin(90.0 * M_PI / 180.0)) * pow((log10(100) - 1), 2)); // (5+1) * (2-1)^2 = 6 * 1^2 = 6

    printf("--- Running Tests ---\n");
    for (int i = 0; i < num_testes; ++i) {
        printf("\n## Test %d ##\n", i + 1);
        printf("Original Infix: \"%s\"\n", testes[i].inFixa);
        printf("Expected Postfix: \"%s\"\n", testes[i].posFixa);
        printf("Expected Value: %.4f\n", testes[i].Valor);

        char *calcPosfixa = getFormaPosFixa(testes[i].inFixa);
        if (calcPosfixa) {
            printf("Calculated Postfix: \"%s\" -> %s\n", calcPosfixa, strcmp(calcPosfixa, testes[i].posFixa) == 0 ? "PASS" : "CHECK");
        } else {
            printf("Calculated Postfix: NULL -> FAIL\n");
        }

        float valInfix = getValorInFixa(testes[i].inFixa);
        printf("Calculated Value (from Infix): %.4f -> %s\n", valInfix, float_approx_equals(valInfix, testes[i].Valor, 0.001f) ? "PASS" : "FAIL");

        if (calcPosfixa) {
            float valPosfix = getValorPosFixa(calcPosfixa);
            printf("Calculated Value (from Calc Postfix): %.4f -> %s\n", valPosfix, float_approx_equals(valPosfix, testes[i].Valor, 0.001f) ? "PASS" : "FAIL");
            
            char *finalInfix = getFormaInFixa(calcPosfixa);
            if (finalInfix) {
                printf("Calculated Infix (from Calc Postfix): \"%s\"\n", finalInfix);
                float valRecalcInfix = getValorInFixa(finalInfix);
                printf("Recalculated Value (from Calc Infix): %.4f -> %s\n", valRecalcInfix, float_approx_equals(valRecalcInfix, testes[i].Valor, 0.001f) ? "PASS" : "FAIL");
                free(finalInfix);
            } else {
                printf("Calculated Infix (from Calc Postfix): NULL -> FAIL\n");
            }
            free(calcPosfixa);
        }
    }
    
    printf("\n--- Additional Direct Use Example ---\n");
    char expr_str[] = "( 10 + 20 ) * 3 - raiz ( 81 ) / ( sen ( 30 ) * 6 )";
    // Expected: (30 * 3) - 9 / (0.5 * 6) = 90 - 9 / 3 = 90 - 3 = 87
    printf("Expression: %s\n", expr_str);
    
    char* pf = getFormaPosFixa(expr_str);
    if (pf) {
        printf("Postfix: %s\n", pf);
        float vpf = getValorPosFixa(pf);
        printf("Value from postfix: %.4f\n", vpf);
        
        char* inf = getFormaInFixa(pf);
        if (inf) {
            printf("Re-converted Infix: %s\n", inf);
            free(inf);
        }
        free(pf);
    } else {
        printf("Failed to convert direct use example to postfix.\n");
    }
    
    float vif = getValorInFixa(expr_str);
    printf("Value from infix directly: %.4f\n", vif);

    printf("\n--- Edge Case: Division by Zero Test ---\n");
    char div_zero_expr[] = "10 / ( 5 - 5 )";
    printf("Expression: %s\n", div_zero_expr);
    float div_zero_val = getValorInFixa(div_zero_expr);
    if (isnan(div_zero_val)) {
        printf("Result: NAN (Correctly handled division by zero)\n");
    } else {
        printf("Result: %.4f (Incorrect - division by zero not handled as NAN)\n", div_zero_val);
    }

    printf("\n--- Edge Case: Mismatched Parentheses Test ---\n");
    char mismatch_expr[] = "( 10 + 5"; // Missing closing parenthesis
    printf("Expression: %s\n", mismatch_expr);
    char* mismatch_pf = getFormaPosFixa(mismatch_expr);
    if (mismatch_pf == NULL) {
        printf("Postfix conversion: NULL (Correctly failed due to mismatched parentheses)\n");
    } else {
        printf("Postfix conversion: \"%s\" (Incorrect - should have failed)\n", mismatch_pf);
        free(mismatch_pf);
    }
    float mismatch_val = getValorInFixa(mismatch_expr);
     if (isnan(mismatch_val)) {
        printf("Value from Infix: NAN (Correctly handled error)\n");
    } else {
        printf("Value from Infix: %.4f (Error not propagated as NAN)\n", mismatch_val);
    }


    return 0;
}