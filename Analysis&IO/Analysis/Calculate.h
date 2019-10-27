/*
	通过传入相应的计算表达式，返回计算结果
	如果计算错误，则返回错误代码
*/
#define CALCULATE_H
#include <windows.h>
#include<string>
//运算错误代码
#define	ERROR_CODE_NOZERO	1	//除数不能为0
#define	ERROR_CODE_WORRYNUM	2	//错误的数字
#define	ERROR_CODE_WORRYEXP	3	//错误的表达式

BOOL EvaluateExpression(const TCHAR * Expression, double &result, int &error_code);
BOOL EvaluateExpressionX(const std::string Expression, double &result, double x, int &error_code);
