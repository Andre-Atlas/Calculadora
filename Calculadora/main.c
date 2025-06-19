#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h> 
#include "expressao.h"

#ifndef M_PI 
#define M_PI 3.14159265358979323846
#endif


int float_approx_equals(float a, float b, float epsilon) {
    if (isnan(a) && isnan(b)) return 1;
    if (isnan(a) || isnan(b)) return 0;
    return fabs(a - b) < epsilon;
}

void displayMenu() {
    printf("\n--- Menu ---\n");
    printf("1. Calcular Expressao Posfixa\n");
    printf("2. Calcular Expressao Infixa\n");
    printf("3. Converter Expressao Posfixa para Infixa\n");
    printf("4. Converter Expressao Infixa para Posfixa\n");
    printf("5. Executar Testes Automaticos\n");
    printf("6. Ajuda (Listar operadores e funcoes)\n");
    printf("7. Sair\n");
    printf("Escolha uma opcao: ");
}

void runAutomatedTests() {
    Expressao testes[15];
    int num_testes = 15;

    // --- Original Tests ---
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
    testes[4].Valor = 179.0f;

    // Test 6
    strcpy(testes[5].inFixa, "log ( 2 + 3 ) / 5");
    strcpy(testes[5].posFixa, "2 3 + log 5 /");
    testes[5].Valor = (float) (log10(5.0) / 5.0);

    // Test 7
    strcpy(testes[6].inFixa, "( log ( 10 ) ^ 3 ) + 2");
    strcpy(testes[6].posFixa, "10 log 3 ^ 2 +");
    testes[6].Valor = (float) (pow(log10(10.0), 3) + 2.0);

    // Test 8
    strcpy(testes[7].inFixa, "( 45 + 60 ) * cos ( 30 )");
    strcpy(testes[7].posFixa, "45 60 + 30 cos *");
    testes[7].Valor = (float) (105.0 * cos(30.0 * M_PI / 180.0));

    // Test 9
    strcpy(testes[8].inFixa, "sen ( 45 ) ^ 2 + 0.5");
    strcpy(testes[8].posFixa, "45 sen 2 ^ 0.5 +");
    testes[8].Valor = (float) (pow(sin(45.0 * M_PI / 180.0), 2) + 0.5);

    // --- 6 New Tests ---
    // Test 10: Nested functions
    strcpy(testes[9].inFixa, "log ( tg ( 60 ) )");
    strcpy(testes[9].posFixa, "60 tg log");
    testes[9].Valor = (float) log10(tan(60.0 * M_PI / 180.0));

    // Test 11: Right associativity of power operator
    strcpy(testes[10].inFixa, "4 ^ 3 ^ 2");
    strcpy(testes[10].posFixa, "4 3 2 ^ ^");
    testes[10].Valor = (float) pow(4, pow(3, 2));

    // Test 12: Mix of operators and functions
    strcpy(testes[11].inFixa, "raiz ( 144 ) + 2 * ( 10 - 4 )");
    strcpy(testes[11].posFixa, "144 raiz 2 10 4 - * +");
    testes[11].Valor = (float) (sqrt(144) + 2 * (10 - 4));

    // Test 13: Modulo operator
    strcpy(testes[12].inFixa, "17 % 5 * 3");
    strcpy(testes[12].posFixa, "17 5 % 3 *");
    testes[12].Valor = (float) (fmod(17, 5) * 3);

    // Test 14: Negative result
    strcpy(testes[13].inFixa, "5 - 10 * 2");
    strcpy(testes[13].posFixa, "5 10 2 * -");
    testes[13].Valor = -15.0f;

    // Test 15: Complex expression
    strcpy(testes[14].inFixa, "( 5 + sen ( 90 ) ) * ( log ( 100 ) - 1 ) ^ 2");
    strcpy(testes[14].posFixa, "5 90 sen + 100 log 1 - 2 ^ *");
    testes[14].Valor = (float) ((5 + sin(90.0 * M_PI / 180.0)) * pow((log10(100) - 1), 2));


    printf("--- Running Tests ---\n");
    for (int i = 0; i < num_testes; ++i) {
        printf("\n## Test %d ##\n", i + 1);
        printf("Original Infix: \"%s\"\n", testes[i].inFixa);
        printf("Expected Postfix: \"%s\"\n", testes[i].posFixa);
        printf("Expected Value: %.2f\n", testes[i].Valor);

        char *calcPosfixa = getFormaPosFixa(testes[i].inFixa);
        if (calcPosfixa) {
            printf("Calculated Postfix: \"%s\" -> %s\n", calcPosfixa, strcmp(calcPosfixa, testes[i].posFixa) == 0 ? "PASS" : "CHECK");
        } else {
            printf("Calculated Postfix: NULL -> FAIL\n");
        }

        float valInfix = getValorInFixa(testes[i].inFixa);
        printf("Calculated Value (from Infix): %.2f -> %s\n", valInfix, float_approx_equals(valInfix, testes[i].Valor, 0.001f) ? "PASS" : "FAIL");

        if (calcPosfixa) {
            float valPosfix = getValorPosFixa(calcPosfixa);
            printf("Calculated Value (from Calc Postfix): %.2f -> %s\n", valPosfix, float_approx_equals(valPosfix, testes[i].Valor, 0.001f) ? "PASS" : "FAIL");

            char *finalInfix = getFormaInFixa(calcPosfixa);
            if (finalInfix) {
                printf("Calculated Infix (from Calc Postfix): \"%s\"\n", finalInfix);
                float valRecalcInfix = getValorInFixa(finalInfix);
                printf("Recalculated Value (from Calc Infix): %.2f -> %s\n", valRecalcInfix, float_approx_equals(valRecalcInfix, testes[i].Valor, 0.001f) ? "PASS" : "FAIL");
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
        printf("Value from postfix: %.2f\n", vpf);

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
    printf("Value from infix directly: %.2f\n", vif);

    printf("\n--- Edge Case: Division by Zero Test ---\n");
    char div_zero_expr[] = "10 / ( 5 - 5 )";
    printf("Expression: %s\n", div_zero_expr);
    float div_zero_val = getValorInFixa(div_zero_expr);
    if (isnan(div_zero_val)) {
        printf("Result: NAN (Correctly handled division by zero)\n");
    } else {
        printf("Result: %.2f (Incorrect - division by zero not handled as NAN)\n", div_zero_val);
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
        printf("Value from Infix: %.2f (Error not propagated as NAN)\n", mismatch_val);
    }
}

// Function to display help information
void displayHelp() {
    printf("\n--- Ajuda ---\n");
    printf("Este programa permite calcular e converter expressoes matematicas.\n");
    printf("Operadores suportados:\n");
    printf("  +: Adicao\n");
    printf("  -: Subtracao\n");
    printf("  *: Multiplicacao\n");
    printf("  /: Divisao\n");
    printf("  %%: Modulo (resto da divisao)\n");
    printf("  ^: Potenciacao\n");
    printf("Funcoes suportadas (os argumentos devem estar em graus para funcoes trigonometricas):\n");
    printf("  sen(x): Seno de x\n");
    printf("  cos(x): Cosseno de x\n");
    printf("  tg(x): Tangente de x\n");
    printf("  log(x): Logaritmo de x (base 10)\n");
    printf("  raiz(x): Raiz quadrada de x\n");
    printf("As expressoes devem ser digitadas com espacos entre cada numero, operador ou parentese.\n");
    printf("Exemplos: ( 3 + 4 ) * 5,  sen ( 30 ) + log ( 10 )\n");
}

int main() {
    int choice;
    char expression[512];
    char *result_str;
    float result_val;

    do {
        displayMenu();
        if (scanf("%d", &choice) != 1) {
            printf("Entrada invalida. Por favor, digite um numero.\n");
            // Clear input buffer
            while (getchar() != '\n');
            continue;
        }
        // Clear input buffer for the next scanf/fgets
        while (getchar() != '\n');

        switch (choice) {
            case 1: // Calcular Expressao Posfixa
                printf("Digite a expressao posfixa (separada por espacos): ");
                fgets(expression, sizeof(expression), stdin);
                expression[strcspn(expression, "\n")] = '\0'; // Remove newline

                result_val = getValorPosFixa(expression);
                if (!isnan(result_val)) {
                    printf("Valor da expressao posfixa: %.2f\n", result_val);
                } else {
                    printf("Erro ao calcular a expressao posfixa. Verifique o formato.\n");
                }
                break;
            case 2: // Calcular Expressao Infixa
                printf("Digite a expressao infixa (separada por espacos): ");
                fgets(expression, sizeof(expression), stdin);
                expression[strcspn(expression, "\n")] = '\0'; // Remove newline

                result_val = getValorInFixa(expression);
                if (!isnan(result_val)) {
                    printf("Valor da expressao infixa: %.2f\n", result_val);
                } else {
                    printf("Erro ao calcular a expressao infixa. Verifique o formato.\n");
                }
                break;
            case 3: // Converter Expressao Posfixa para Infixa
                printf("Digite a expressao posfixa (separada por espacos): ");
                fgets(expression, sizeof(expression), stdin);
                expression[strcspn(expression, "\n")] = '\0'; // Remove newline

                result_str = getFormaInFixa(expression);
                if (result_str) {
                    printf("Forma Infixa: \"%s\"\n", result_str);
                    free(result_str);
                } else {
                    printf("Erro ao converter para infixa. Verifique o formato da expressao posfixa.\n");
                }
                break;
            case 4: // Converter Expressao Infixa para Posfixa
                printf("Digite a expressao infixa (separada por espacos): ");
                fgets(expression, sizeof(expression), stdin);
                expression[strcspn(expression, "\n")] = '\0'; // Remove newline

                result_str = getFormaPosFixa(expression);
                if (result_str) {
                    printf("Forma Posfixa: \"%s\"\n", result_str);
                    free(result_str);
                } else {
                    printf("Erro ao converter para posfixa. Verifique o formato da expressao infixa.\n");
                }
                break;
            case 5: // Executar Testes Automaticos
                runAutomatedTests();
                break;
            case 6: // Ajuda
                displayHelp();
                break;
            case 7: // Sair
                printf("Saindo do programa. Ate mais!\n");
                break;
            default:
                printf("Opcao invalida. Por favor, tente novamente.\n");
                break;
        }
    } while (choice != 7);

    return 0;
}
