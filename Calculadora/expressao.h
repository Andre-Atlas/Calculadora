#ifndef EXPRESSAO_H
#define EXPRESSAO_H

typedef struct {
    char posFixa[512];  // Expressão na forma pos-fixa, como 3 12 4+* [cite: 16]
    char inFixa [512];  // Expressão na forma infixa, como 3* (12 + 4) [cite: 16]
    float Valor;        // Valor numérico da expressão [cite: 16]
} Expressao;

char *getFormaInFixa (char *Str);        // Retorna a forma inFixa de Str (posFixa) [cite: 17]
char *getFormaPosFixa (char *Str);       // Retorna a forma posFixa de Str (inFixa) [cite: 18]
float getValorPosFixa (char *StrPosFixa); // Calcula o valor de Str (na forma posFixa) [cite: 19]
float getValorInFixa (char *StrInFixa);  // Calcula o valor de Str (na forma inFixa) [cite: 20]

#endif