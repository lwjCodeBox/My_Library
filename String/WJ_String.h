#pragma once
#include <stdio.h>
#include <Windows.h>
#include <iostream>

// WJ_String Ŭ������ ���ڿ��� ���ϰ� ������ �� �ֵ��� ���� Ŭ����.
// https://blog.naver.com/tipsware/221140629230
class WJ_String
{
protected:
	wchar_t *mp_string; // �� ��ü�� �����ϴ� ���ڿ��� �ּ�
	int m_length;       // �� ��ü�� �����ϴ� ���ڿ��� ���� ("abc"�̸� 3�� ����)

public:
	WJ_String(); // �⺻ ������
	WJ_String(const char *ap_string);
	WJ_String(const wchar_t *ap_string);
	WJ_String(const WJ_String &ar_string); // ���� ������.

	virtual ~WJ_String();

	void AsciiToUnicode(char *ap_string);

	// UTF8������ ���ڿ��� �����ڵ� ���·� �����ϴ� �Լ�. �� �Լ��� ��ȯ ����
	// �����ڵ�� ����� ���ڿ��� �ּ��̴�. �� �ּҴ� �����Ҵ�� �޸𸮶� 
	// �����ʿ��� �ݵ�� delete[] �����ڷ� �����ؾ� �Ѵ�.
	wchar_t *UTF8_To_Unicode(const char *ap_src_string)
	{
		// \0 ���� ������ ũ�Ⱑ ��ȯ��
		int temp_length = MultiByteToWideChar(CP_UTF8, 0, ap_src_string, -1, NULL, 0);
		// ���ڿ� ��ȯ�� ���ؼ� �޸𸮸� �Ҵ��Ѵ�.
		wchar_t *p_unicode_str = new wchar_t[temp_length];
		// �޸� �Ҵ翡 �����ߴٸ� UTF8 ������ ���ڿ��� �����ڵ� ���ڿ��� ��ȯ�Ѵ�.
		if (p_unicode_str != NULL)
			MultiByteToWideChar(CP_UTF8, 0, ap_src_string, -1, p_unicode_str, temp_length);
		return p_unicode_str;
	}

	// UTF8 ���ڿ��� �����ڵ� ���ڿ��� ��ȯ�ϴ� �Լ�
	inline int UTF8_To_Unicode(wchar_t *ap_dest_string, const char *ap_src_string)
	{
		// \0 ���� ������ ũ�Ⱑ ��ȯ��
		int temp_length = MultiByteToWideChar(CP_UTF8, 0, ap_src_string, -1, NULL, 0);
		if (ap_dest_string != NULL) {
			MultiByteToWideChar(CP_UTF8, 0, ap_src_string, -1, ap_dest_string, temp_length);
		}
		return temp_length - 1;
	}

	// �����ڵ� ���ڿ��� UTF8 ���ڿ��� ��ȯ�ϴ� �Լ�
	inline int Unicode_To_UTF8(char *ap_dest_string, const wchar_t *ap_src_string)
	{
		// �����ڵ� ������ ���ڿ��� UTF8 �������� ���������� �ʿ��� �޸� ������ ũ�⸦ ��´�.
		int temp_length = WideCharToMultiByte(CP_UTF8, 0, ap_src_string, -1, NULL, 0, NULL, NULL);
		// �����ڵ带 UTF8�ڵ�� ��ȯ�Ѵ�.
		if (ap_dest_string != NULL) {
			WideCharToMultiByte(CP_UTF8, 0, ap_src_string, -1, ap_dest_string, temp_length, NULL, NULL);
		}
		return temp_length;
	}
	
	void InitialObject(const wchar_t *ap_string); // ��ü �ʱ�ȭ �Լ�
	/*inline*/ int GetLength();				// ���ڿ��� ���̸� �˷��ִ� �Լ�
	/*inline*/ const wchar_t *GetString();	// ���ڿ��� ������ �˷��ִ� �Լ�
	void RemoveString();				// �� ��ü�� �����ϴ� ���ڿ��� �����Ѵ�.

	// ���޵� ���ڿ��� �����ؼ� �����Ѵ�.
	void SetString(const wchar_t *ap_string);
	// ���޵� ���ڿ��� �ּҸ� �״�� ����Ѵ�. (���縦 ���� �ʰ� ������ �״�� �����)
	void AttachString(wchar_t *ap_string, int a_lenght);
	// ����� ���ڿ��� �ٸ� ���ڿ��� �߰��ϴ� ��쿡 ����Ѵ�.
	// ex) "abc"�� ����Ǿ� �ִµ� AddString("def", 3); �̶�� �ϸ�
	// "abcdef"�� �ȴ�.
	void AddString(const wchar_t *ap_string, int a_length);

	// ���� ������ �����ε� str = L"test";
	void operator=(const WJ_String &ar_string);
	// ���ڿ� Ȯ��(append)�� ���� ������ �����ε�
	void operator+=(const WJ_String &ar_str);

	// ���ڿ��� ���ϱ� ���� ������ �����ε�
	friend WJ_String operator+(const WJ_String &ar_str1, const WJ_String &ar_str2)
	{
		WJ_String str = ar_str1;
		str.AddString(ar_str2.mp_string, ar_str2.m_length);
		return str;
	}
	// ���ڿ� �񱳸� ���� ������ �����ε� (������ Ȯ���ϱ� ���� ������)
	friend char operator==(const WJ_String &ar_str1, const WJ_String &ar_str2)
	{
		if (ar_str1.m_length != ar_str2.m_length) return 0;
		return memcmp(ar_str1.mp_string, ar_str2.mp_string, ar_str1.m_length << 1) == 0; // "<< 1"�ϴ� ����� "* 2"�� ���� ǥ�� ����̴�.
	}
	// ���ڿ� �񱳸� ���� ������ �����ε� (�ٸ��� Ȯ���ϱ� ���� ������)
	friend char operator!=(const WJ_String &ar_str1, const WJ_String &ar_str2)
	{
		if (ar_str1.m_length != ar_str2.m_length) return 0;
		return memcmp(ar_str1.mp_string, ar_str2.mp_string, ar_str1.m_length << 1) != 0; // "<< 1"�ϴ� ����� "* 2"�� ���� ǥ�� ����̴�.
	}


	friend std::wostream &operator << (std::wostream &os, const WJ_String &s)
	{
		os << s.mp_string;
		return os;
	}

	friend std::wistream &operator >> (std::wistream &is, WJ_String &s)
	{
		wchar_t buf[1024];
		is >> buf;
		s.SetString(buf);

		return is;
	}

	wchar_t *GetStrBuffer() { return mp_string; }

	// ���������� ���
public:
	// ���������� ����ϰ� �ܼ�â���� ��� (X)
	void DbgLog(LPCSTR ap_str, ...);
	// ���������� ����ϰ� �ܼ�â���� ��� (X)
	void DbgLogW(const wchar_t *ap_str, ...);
	// ���������� ����ϰ� �ܼ�â���� ��� (0)
	void DbgLogW_P(const wchar_t *ap_str, ...);
	// CString ��ü�� Format() �Լ� ó�� ���.
	void Format(const wchar_t *ap_str, ...);
};