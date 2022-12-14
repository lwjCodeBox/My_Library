#include <stdio.h>
#include <string.h>
#include "Calculator.h"

int main(void)
{
    char InfixExpression[100];
    char PostfixExpression[100];

    double Result = 0.0;

    memset(InfixExpression, 0, sizeof(InfixExpression));
    memset(PostfixExpression, 0, sizeof(PostfixExpression));

    printf("Enter Infix Expression:");
#if(0)
    scanf("%s", InfixExpression);
#else
    //strcat(InfixExpression, "1+3.334/(4.28*(110-7729))");
    strcat(InfixExpression, "1+3.334*10");
#endif
    puts("\n");

    // 중위식 표현을 후위식 표현으로 변환
    GetPostfix(InfixExpression, PostfixExpression);

    puts("\n========= Result =========");
    printf("Infix:%s\nPostfix:%s\n", InfixExpression, PostfixExpression);
    puts("========= Result =========\n");
    Result = Calculate(PostfixExpression);

    printf("Calculation Result : %f\n", Result);

    return 0;
}
