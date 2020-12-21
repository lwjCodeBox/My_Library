#include <stdio.h>

#include "WJ_String.h"

void testF()
{
	WJ_String string = L"lee"; // 복사 생성자로 'lee' 문자열 대입

	string = L"tipssoft";   // 대입 연산자를 사용하여 대입

	WJ_String test(L"Hello");  // 객체 생성자를 사용하여 'Hello' 문자열 대입

	test = string;    // string 객체에 저장된 문자열을 test 객체에 복사
	
	//기존 문자열에 다른 문자열을 추가하는 코드는 다음과 같습니다.
	WJ_String my_string = L"tipssoft";
	my_string += L".com";     // string 객체에는 'tipssoft.com' 문자열이 저장됨

	WJ_String my_str2 = L"kkk";
	wprintf(L"%s\n", my_str2.GetString());

	my_str2.Format(L"이원종");
	wprintf(L"%s\n", my_str2.GetString());
	
	return;
}
void main()
{
	testF();

	WJ_String str(L"lee");
	WJ_String str2(L"won");
	WJ_String str3(L"jong");

	
	//if(L"leewon" == (str+ str2)) 
		//printf("OK\n");
	
		
	str2 = L"123" + str2;
	wprintf(L"%s\n", str2.GetString());
	str += L"kkk";
	wprintf(L"%s\n", str.GetString());

	printf("\n2. end\n");
}