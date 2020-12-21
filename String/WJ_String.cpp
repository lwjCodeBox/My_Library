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

//WJ_String::WJ_String(const char *ap_string)
//{
//    mp_string = NULL;
//    AsciiToUnicode((char *)ap_string);
//}
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

WJ_String::WJ_String(const WJ_String &ar_string)
{
    InitialObject(ar_string.mp_string);
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