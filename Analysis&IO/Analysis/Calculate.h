/*
	ͨ��������Ӧ�ļ�����ʽ�����ؼ�����
	�����������򷵻ش������
*/
#define CALCULATE_H
#include <windows.h>
#include<string>
//����������
#define	ERROR_CODE_NOZERO	1	//��������Ϊ0
#define	ERROR_CODE_WORRYNUM	2	//���������
#define	ERROR_CODE_WORRYEXP	3	//����ı��ʽ

BOOL EvaluateExpression(const TCHAR * Expression, double &result, int &error_code);
BOOL EvaluateExpressionX(const std::string Expression, double &result, double x, int &error_code);
