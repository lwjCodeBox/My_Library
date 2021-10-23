#include "pch.h"
#include "WJ_String.h"

WJ_String::WJ_String()
{
    InitialObject(NULL);
}
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

WJ_String::WJ_String(const wchar_t *ap_string)
{
    InitialObject(ap_string);
}
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

WJ_String::WJ_String(const char *ap_string)
{
    mp_string = NULL;
    AsciiToUnicode((char *)ap_string);
}
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

WJ_String::WJ_String(const WJ_String &ar_string)
{
    InitialObject(ar_string.mp_string);
}
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

// https://lwj789.tistory.com/45
void WJ_String::AsciiToUnicode(char *ap_string)
{
    wchar_t strUnicode[256] = { 0, };
    char	strUTF8[256] = { 0, };

    //strcpy_s(strUTF8, 256, "UTF-8글자"); // UTF-8 문자라고 가정하고..
    strcpy_s(strUTF8, 256, ap_string); // UTF-8 문자라고 가정하고..
    int nLen = MultiByteToWideChar(CP_UTF8, 0, strUTF8, strlen(strUTF8), NULL, NULL);
    MultiByteToWideChar(CP_UTF8, 0, strUTF8, strlen(strUTF8), strUnicode, nLen);

    InitialObject(strUnicode);
}
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

void WJ_String::InitialObject(const wchar_t *ap_string)
{
    if (ap_string == NULL) {
        m_length = 0;
        mp_string = new wchar_t[1];
        mp_string[0] = 0;
    }
    else {
        m_length = wcslen(ap_string);
        mp_string = new wchar_t[m_length + 1];
        memcpy(mp_string, ap_string, (m_length + 1) << 1); // "<< 1"하는 방식은 "* 2"와 같은 표현 방식이다.
    }
}
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

WJ_String::~WJ_String()
{
    delete[] mp_string;
}
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

void WJ_String::RemoveString()
{
    SetString(NULL);
}
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

void WJ_String::SetString(const wchar_t *ap_string)
{
    delete[] mp_string;

    InitialObject(ap_string);
}
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

void WJ_String::AttachString(wchar_t *ap_string, int a_length)
{
    delete[] mp_string;

    m_length = a_length;
    mp_string = ap_string;
}
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

void WJ_String::AddString(const wchar_t *ap_string, int a_length)
{
    int total_length = m_length + a_length;

    wchar_t *p_string = new wchar_t[total_length + 1];

    memcpy(p_string, mp_string, m_length << 1); // "<< 1"하는 방식은 "* 2"와 같은 표현 방식이다.
    memcpy(p_string + m_length, ap_string, (a_length + 1) << 1); // "<< 1"하는 방식은 "* 2"와 같은 표현 방식이다.

    delete[] mp_string;
    mp_string = p_string;
    m_length = total_length;
}
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

void WJ_String::operator=(const WJ_String &ar_string)
{
    delete[] mp_string;
    m_length = ar_string.m_length;

    mp_string = new wchar_t[m_length + 1];
    memcpy(mp_string, ar_string.mp_string, (m_length + 1) << 1); // "<< 1"하는 방식은 "* 2"와 같은 표현 방식이다.
}
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

void WJ_String::operator+=(const WJ_String &ar_str)
{
    AddString(ar_str.mp_string, ar_str.m_length);
}
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

const wchar_t *WJ_String::GetString()
{
    return mp_string;
}
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

int WJ_String::GetLength()
{
    return m_length;
}
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// https://m.blog.naver.com/PostView.nhn?blogId=sinarn&logNo=130180639399&proxyReferer=https:%2F%2Fwww.google.com%2F
void WJ_String::DbgLog(LPCSTR ap_str, ...)
{
    va_list args;

    // 시작
    va_start(args, ap_str);

    // "abc"가 들어오면 문자열은 크기는 3이지만 맨 마지막 위치한 곳에 '\0'을 추가 해야하기 때문에
    // 문자열의 크기는 4가 되야한다.
    // 가변인자로 이루어진 문자열의 크기를 구한다. (_vscprintf doesn't count terminating '\0')
    int len = _vscprintf(ap_str, args) + 1; // _vscprintf for '\0'

    // 위에서 구한 크기 +1만큼 pBuf에 메모리를 할당한다.
    char *pBuf = (char *)malloc(sizeof(char) * len);

    if (pBuf) {
        // 문자열을 pBuf에 입력한다.
        // 문자열 크기가 4가 되어야 하지만 vsprintf() 반환 값이 3인 이유는 '\0' 전까지의 문자열을 의미함.
        int size = vsprintf(pBuf, ap_str, args);
        OutputDebugStringA(pBuf);
        printf("[len %d] [size %d] %s", len, size, pBuf);
        free(pBuf);
    }

    va_end(args);
}
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

void WJ_String::DbgLogW(const wchar_t *ap_str, ...)
{
    va_list args;
    va_start(args, ap_str);

    int len = _vscwprintf(ap_str, args) + 1;
    wchar_t *pBuf = (wchar_t *)malloc(sizeof(wchar_t) * len);

    if (pBuf) {
        int size = vswprintf(pBuf, len, ap_str, args);
        OutputDebugStringW(pBuf);
        wprintf(L"[len %d] [size %d] %s", len, size, pBuf);

        free(pBuf);
    }

    va_end(args);
}
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

void WJ_String::DbgLogW_P(const wchar_t *ap_str, ...)
{
    va_list args;
    va_start(args, ap_str);

    int len = _vscwprintf(ap_str, args) + 1;
    wchar_t *pBuf = (wchar_t *)malloc(sizeof(wchar_t) * len);

    if (pBuf) {
        int size = vswprintf(pBuf, len, ap_str, args);
        OutputDebugStringW(pBuf);
        wprintf(L"[len %d] [size %d] %s", len, size, pBuf);

        free(pBuf);
    }

    va_end(args);
}
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

#include <locale.h>
void WJ_String::Format(const wchar_t *ap_str, ...)
{
    _wsetlocale(LC_ALL, L"korean");

    va_list args;
    va_start(args, ap_str);

    int len = _vscwprintf(ap_str, args) + 1;
    wchar_t *pBuf = (wchar_t *)malloc(sizeof(wchar_t) * len);

    if (pBuf) {
        delete[] mp_string;

        vswprintf(pBuf, len, ap_str, args);
        
        mp_string = pBuf;
    }

    va_end(args);
}
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+