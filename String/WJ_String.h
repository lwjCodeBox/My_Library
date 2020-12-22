#pragma once
#include <stdio.h>
#include <Windows.h>

// WJ_String 클래스는 문자열을 편리하게 괸라할 수 있도록 만든 클래스.
// https://blog.naver.com/tipsware/221140629230
class WJ_String
{
protected:
	wchar_t *mp_string; // 이 객체가 관리하는 문자열의 주소
	int m_length;       // 이 객체가 관리하는 문자열의 길이 ("abc"이면 3이 저장)

public:
	WJ_String(); // 기본 생성자
	//WJ_String(const char *ap_string);
	WJ_String(const wchar_t *ap_string);
	WJ_String(const WJ_String &ar_string); // 복사 생성자.

	virtual ~WJ_String();

	void InitialObject(const wchar_t *ap_string); // 객체 초기화 함수
	/*inline*/ int GetLength();				// 문자열의 길이를 알려주는 함수
	/*inline*/ const wchar_t *GetString();	// 문자열의 내용을 알려주는 함수
	void RemoveString();				// 이 객체가 관리하던 문자열을 제거한다.

	// 전달된 문자열을 복사해서 관리한다.
	void SetString(const wchar_t *ap_string);
	// 전달된 문자열의 주소를 그대로 사용한다. (복사를 하지 않고 원본을 그대로 사용함)
	void AttachString(wchar_t *ap_string, int a_lenght);
	// 저장된 문자열에 다른 문자열을 추가하는 경우에 사용한다.
	// ex) "abc"가 저장되어 있는데 AddString("def", 3); 이라고 하면
	// "abcdef"가 된다.
	void AddString(const wchar_t *ap_string, int a_length);

	// 대입 연산자 오버로딩 str = L"test";
	void operator=(const WJ_String &ar_string);
	// 문자열 확장(append)를 위한 연산자 오버로딩
	void operator+=(const WJ_String &ar_str);
	
	// 문자열을 더하기 위한 연산자 오버로딩
	friend WJ_String operator+(const WJ_String &ar_str1, const WJ_String &ar_str2)
	{
		WJ_String str = ar_str1;
		str.AddString(ar_str2.mp_string, ar_str2.m_length);
		return str;
	}
	// 문자열 비교를 위한 연산자 오버로딩 (같은지 확인하기 위한 연산자)
	friend char operator==(const WJ_String &ar_str1, const WJ_String &ar_str2)
	{
		if (ar_str1.m_length != ar_str2.m_length) return 0;
		return memcmp(ar_str1.mp_string, ar_str2.mp_string, ar_str1.m_length << 1) == 0; // "<< 1"하는 방식은 "* 2"와 같은 표현 방식이다.
	}
	// 문자열 비교를 위한 연산자 오버로딩 (다른지 확인하기 위한 연산자)
	friend char operator!=(const WJ_String &ar_str1, const WJ_String &ar_str2)
	{
		if (ar_str1.m_length != ar_str2.m_length) return 0;
		return memcmp(ar_str1.mp_string, ar_str2.mp_string, ar_str1.m_length << 1) != 0; // "<< 1"하는 방식은 "* 2"와 같은 표현 방식이다.
	}

	wchar_t *GetStrBuffer() { return mp_string; }

// 디버깅용으로 사용
public:
	// 디버깅용으로 사용하고 콘솔창에는 출력 (X)
	void DbgLog(LPCSTR ap_str, ...);
	// 디버깅용으로 사용하고 콘솔창에는 출력 (X)
	void DbgLogW(const wchar_t *ap_str, ...);
	// 디버깅용으로 사용하고 콘솔창에는 출력 (0)
	void DbgLogW_P(const wchar_t *ap_str, ...);
	// CString 객체에 Format() 함수 처럼 사용.
	void Format(const wchar_t *ap_str, ...);
};