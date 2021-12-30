#include "pch.h"
#include "WJ_ServerSocket.h"

// #define _WINSOCK_DEPRECATED_NO_WARNINGS     -> framework.h에 넣어주세요!!
#include <ws2tcpip.h>     // InetPton 함수를 사용하기 위해!
#pragma comment(lib, "WS2_32.lib")  

// 기본 생성자 (내부적으로 27을 key로 사용함)
WJ_ServerSocket::WJ_ServerSocket()
{
	InitObject(27);
}

// 내부적으로 key를 고정하지 않고 외부에서 설정하도록 생성자에서 key 값을 입력 받음
WJ_ServerSocket::WJ_ServerSocket(unsigned char a_key)
{
	InitObject(a_key);
}

// 이 객체를 초기화하는 함수. 내부적으로 호출되는 함수임!
void WJ_ServerSocket::InitObject(unsigned char a_key)
{
	WSADATA temp;
	WSAStartup(0x0202, &temp);  // 소켓 라이브러리를 사용 가능 상태로 만든다.

	// 배열의 각 항목에 있는 h_socket 값을 INVALID_SOCKET으로 초기화해서
	// 접속된 사용자 소켓과 구별하는 용도로 사용한다. 이 값이 INVALID_SOCKET이면
	// ip_address에 저장된 값은 의미가 없기 때문에 ip_address 값은 초기화하지 않는다.
	for (int i = 0; i < MAX_USER_COUNT; i++) m_users[i].h_socket = INVALID_SOCKET;

	mh_listen_socket = INVALID_SOCKET;
	mp_notify_wnd = NULL;
	m_key = a_key;
	m_accept_msg_id = 25001;
	m_read_close_msg_id = 25002;
	mp_net_body_data = NULL;
	m_net_body_size = 0;
	mp_net_user = NULL;
}

// 객체 파괴자
WJ_ServerSocket::~WJ_ServerSocket()
{
	// 접속된 클라이언트가 있다면 관련 소켓을 모두 제거하고 리슨 소켓도 제거한다.
	DestroyClientSockets();

	// 데이터 수신에 사용하던 메모리가 있다면 메모리를 해제한다.
	if (mp_net_body_data != NULL) delete[] mp_net_body_data;

	WSACleanup();  // 소켓 라이브러리를 그만 사용하도록 설정한다.
}

// 서버에 접속한 모든 클라이언트의 접속을 해제하고 Listen 소켓도 종료하는 함수
void WJ_ServerSocket::DestroyClientSockets()
{
	UserData *p = m_users;
	for (int i = 0; i < MAX_USER_COUNT; i++, p++) {
		if (p->h_socket != INVALID_SOCKET) {  // 할당된 소켓이 있다면
			closesocket(p->h_socket);  // 소켓을 닫는다.
			p->h_socket = INVALID_SOCKET;  // 소켓을 다시 사용할 수 있도록 초기화한다.
		}
	}

	if (mh_listen_socket != INVALID_SOCKET) {  // 리슨 소켓이 만들어져 있다면
		closesocket(mh_listen_socket);  // 리슨 소켓을 닫는다.
		mh_listen_socket = INVALID_SOCKET;  // 리슨 소켓을 초기화한다.
	}
}

// 클라이언트로 1개의 네트워크 프레임 데이터를 하나 전송한다.
// 프레임 구성 : "Key(27), Message ID, Body size, Body Data"
void WJ_ServerSocket::SendFrameData(SOCKET ah_socket, char a_msg_id, const void *ap_data, int a_size)
{
	// 전송할 데이터 프레임을 저장하기 위한 메모리를 할당한다. 
	char *p_data = new char[a_size + 4];
	// 구별 키 값을 설정
	*p_data = m_key;
	// 메시지 아이디 설정
	*(p_data + 1) = a_msg_id;
	// 전송할 실제 데이터 크기를 설정
	*(unsigned short *)(p_data + 2) = a_size;
	// 전송할 실제 데이터를 복사
	memcpy(p_data + 4, ap_data, a_size);
	// ah_socket으로 데이터를 a_size + 4 크기 만큼 전송한다.
	send(ah_socket, p_data, a_size + 4, 0);
	// 프레임 저장에 사용한 메모리를 해제한다.
	delete[] p_data;
}

// 접속한 모든 클라이언트에게 네트워크 프레임 데이트를 전송한다.
void WJ_ServerSocket::BroadcastFrameData(char a_msg_id, const void *ap_data, int a_size)
{
	// 전송할 데이터 프레임을 저장하기 위한 메모리를 할당한다. 
	char *p_data = new char[a_size + 4];
	// 구별 키 값을 설정
	*p_data = m_key;
	// 메시지 아이디 설정
	*(p_data + 1) = a_msg_id;
	// 전송할 실제 데이터 크기를 설정
	*(unsigned short *)(p_data + 2) = a_size;
	// 전송할 실제 데이터를 복사
	memcpy(p_data + 4, ap_data, a_size);

	UserData *p = m_users;
	// 접속된 모든 사용자에게 데이터를 전송한다.
	for (int i = 0; i < MAX_USER_COUNT; i++, p++) {
		// 소켓이 사용되고 있다면 p->h_socket으로 데이터를 a_size + 4 크기 만큼 전송한다.
		if (p->h_socket != INVALID_SOCKET) send(p->h_socket, p_data, a_size + 4, 0);
	}
	// 프레임 저장에 사용한 메모리를 해제한다.
	delete[] p_data;
}

// Listen 서비스를 시작할 때 사용하는 함수
void WJ_ServerSocket::StartListenService(const wchar_t *ap_ip_address, int a_port, CWnd *ap_notify_wnd, int a_accept_id, int a_read_close_id)
{
	mp_notify_wnd = ap_notify_wnd;
	m_accept_msg_id = a_accept_id;
	m_read_close_msg_id = a_read_close_id;

	// AF_INET 주소 체계를 사용하는 TCP 방식의 소켓 생성!!
	mh_listen_socket = socket(AF_INET, SOCK_STREAM, 0);

	// IP 주소 체계, 프로그램 식별 번호(포트 번호)
	sockaddr_in addr_data ={ AF_INET, htons(a_port), };
	// ap_ip_address 주소를 사용하는네트워크를 사용하도록 설정
	InetPton(AF_INET, ap_ip_address, &addr_data.sin_addr.s_addr);

	// addr_data에 설정된 정보를 사용하여 소켓을 네트워크 시스템에 연결한다.
	bind(mh_listen_socket, (sockaddr *)&addr_data, sizeof(addr_data));
	// 접속을 처리할 단위를 설저한다. 하지만 이 처리가 동시 처리가 아니라 
	// 순차처리라서 숫자가 크다고 빨라지지 않습니다.
	listen(mh_listen_socket, 5);
	// mh_listen_socket에 사용자가 접속을 시도(FD_ACCEPT)했을 때 
	// 현재 대화 상자(m_hWnd)에 25001번 메시지가 발생하도록 비동기를 설정한다.
	WSAAsyncSelect(mh_listen_socket, mp_notify_wnd->m_hWnd, m_accept_msg_id, FD_ACCEPT);
}

// 새로운 사용자가 접속했을 때, 이 함수를 호출하면 사용자의 접속을 처리한다.
UserData *WJ_ServerSocket::ProcessAccept()
{
	// 접속을 시도한 소켓(클라이언트와 통신할 소켓)의 주소를 저장할 변수
	sockaddr_in addr_data;
	// 자신의 사용할 주소 저장 변수의 크기를 계산
	int addr_data_size = sizeof(addr_data);

	// 클라이언트 소켓의 접속을 허락한다. 접속을 허락하면 복제 소켓이 생성되어
	// 그 핸들 값이 반환(h_client_socket에 저장)된다. 이때 addr_data 변수에는
	// 접속을 시도한 소켓의 IP 주소가 정수 값 형태로 저장된다.
	SOCKET h_client_socket = accept(mh_listen_socket, (sockaddr *)&addr_data, &addr_data_size);

	wchar_t temp_ip_address[32];
	// addr_data 변수에 정수 형태로 저장되어 있는 IP 주소를 문자열 형태로
	// 변경해서 temp_ip_address에 저장한다.
	InetNtop(AF_INET, &addr_data.sin_addr, temp_ip_address, 32);
	int index = AddNewUser(h_client_socket, temp_ip_address);
	// 만약, 배열 전체를 다 체크했는데 빈 공간이 없다면 최대 접속자 수를 
	// 초과한 접속이기 때문에 소켓을 해제하고 접속을 취소한다.
	if (index == MAX_USER_COUNT) {
		closesocket(h_client_socket);
		return NULL;  // 최대 접속자 수를 넘어서 접속에 실패 함!
	}
	return m_users + index;  // 현재 접속한 사용자의 정보가 담기 주소를 반환한다.
}

// 새로운 사용자의 접속을 처리한다. 아래의 함수는 m_users 배열의 h_socket 값이
// INVALID_SOCKET인 항목에 새로운 사용자 소켓을 저장한다.
int WJ_ServerSocket::AddNewUser(SOCKET ah_new_socket, const wchar_t *ap_ip_address)
{
	UserData *p = m_users;
	// h_client_socket은 지역 변수이기 때문에 이 함수가 종료되면 사라진다. 따라서
	// 자신이 관리하고 있는 클라이언트 정보 공간에 빈 곳이 있는지 확인해서 저장한다.
	for (int i = 0; i < MAX_USER_COUNT; i++, p++) {
		// 각 배열의 h_socket에 INVALID_SOCKET 값이 저장되어 있다면 해당 배열 항목은
		// 접속한 소켓의 핸들이 저장되지 않았다는 뜻이다. 즉, 비어있다는 뜻이다.
		if (p->h_socket == INVALID_SOCKET) {
			// 배열의 비어 있는 공간에 접속한 소켓의 핸들을 복사한다.
			p->h_socket = ah_new_socket;
			// 접속한 사용자의 IP를 복사한다.
			wcscpy_s(p->ip_address, 16, ap_ip_address);
			// h_client_socket에 FD_READ 또는 FD_CLOSE 이벤트가 발생하면 대화 상자로
			// 25002번 메시지를 전송하도록 비동기를 설정한다.
			WSAAsyncSelect(ah_new_socket, mp_notify_wnd->m_hWnd, m_read_close_msg_id, FD_READ | FD_CLOSE);
			return i;
		}
	}
	return MAX_USER_COUNT;  // 비어있는 항목이 없는 경우!
}

// 소켓 핸들을 가지고 해당 소켓 핸들이 저장된 m_users 배열의 항목 주소를 얻는 함수
UserData *WJ_ServerSocket::GetUserData(SOCKET ah_socket)
{
	// 자신이 관리하고 있는 클라이언트 중에서 어떤 클라이언트가 이벤트를
	// 발생시켰는지 확인하기 위해서 배열을 탐색한다.
	for (int i = 0; i < MAX_USER_COUNT; i++) {
		// ah_socket와 값이 일치하는 소켓이 현재 이벤트를 발생시킨 클라이언트이다.
		if (m_users[i].h_socket == ah_socket) return m_users + i;
	}
	return NULL; // 검색에 실패한 경우!
}

// FD_CLOSE에 의해 25002번 메시지가 발생하여 사용자의 접속 정보를 해제할 때사용한다. 
// 이 메시지는 25002번 메시지에서만 사용해야 하는 것은 아니고 특정 사용자를 종료시킬 때
// 사용해도 됩니다. 두 번째 인자로 전달되는 ap_close_user에는 접속을 해제한 
// 사용자의 정보가 복사됩니다. 따라서 지역 변수로 UserData 구조체 변수를 선언해서
// 해당 변수의 주소를 전달해야지 정보를 받을 수 있습니다.
void WJ_ServerSocket::CloseSocket(SOCKET ah_socket, UserData *ap_close_user)
{
	// ah_socket에 해당하는 사용자 정보를 찾는다.
	UserData *p_user = GetUserData(ah_socket);
	// 사용자 정보를 찾았고 이 정보를 복사할 메모리가 전달되었다면
	// 삭제될 사용자 정보를 ap_close_user에 복사한다.
	if (p_user != NULL){
		// 삭제된 사용자의 index 위치를 저장한다.
		m_temp_user_index = p_user - m_users;
		// 삭제할 사용자 정보를 백업할 메모리 주소가 전달되었다면 정보를 백업한다.
		if(ap_close_user != NULL) memcpy(ap_close_user, p_user, sizeof(UserData));
		// 배열에 저장해서 관리하던 소켓을 초기화한다.
		p_user->h_socket = INVALID_SOCKET;
	}	
	closesocket(ah_socket);  // 소켓을 제거한다.
}

// 클라이언트가 전송한 데이터를 수신할때 사용하는 함수
int WJ_ServerSocket::ReceiveData(SOCKET ah_socket, char *ap_data, int a_size)
{
	int total_size = 0, read_size, retry_count = 0;
	// a_size 크기만큼 다 수신될 때까지 반복한다.
	while (total_size < a_size) {
		// 남은 크기만큼 수신을 시도한다.
		read_size = recv(ah_socket, ap_data + total_size, a_size - total_size, 0);
		if (read_size == SOCKET_ERROR || read_size == 0) {  // 읽기에 실패한 경우
			Sleep(10);   // 0.01초만큼 대기한다.
			retry_count++;   // 재시도 횟수를 증가시킨다.
			if (retry_count > 300) break;  // 300회 재시도 할때까지 실패하면 작업을 중단한다.
		} else {
			retry_count = 0;  // 재시도 횟수를 초기화한다.
			total_size += read_size;  // 수신된 데이터의 크기를 합산한다.
		}
	}
	return total_size;   // 전체 수신한 바이트를 출력한다.
}

// 네트워크 메시지에서 헤더 정보를 처리하는 함수
int WJ_ServerSocket::ProcessHeaderInfo(SOCKET ah_socket)
{
	unsigned char key;
	// 첫 바이트에 있는 key 값을 읽는다.
	recv(ah_socket, (char *)&key, 1, 0);
	// key 값이 지정된 key값과 일치하는지 비교한다.
	if (key == m_key) {
		recv(ah_socket, (char *)&m_net_msg_id, 1, 0);
		recv(ah_socket, (char *)&m_net_body_size, 2, 0);
		return 1;  // key가 일치하는 경우 (정상적인 헤더)
	}
	return 0;  // key가 다른 경우 (비정상적인 헤더)
}

// 네트워크로 데이터가 수신된 경우, 약속한 헤더를 읽고 Body 데이터까지
// 읽어서 작업을 진행할수 있도록 준비하는 함수.
// 이 함수는 FD_READ 이벤트에 의해 25002번 메시지가 발생했을때 사용하면 된다.
int WJ_ServerSocket::BeginNetworkProcess(SOCKET ah_socket)
{
	// 끊어 읽기 및 재시도 읽기를 위해 FD_READ 이벤트를 제외하고 다시 비동기를 설정한다.
	WSAAsyncSelect(ah_socket, mp_notify_wnd->m_hWnd, m_read_close_msg_id, FD_CLOSE);
	// 데이터를 전송한 클라이언트의 정보를 찾아서 mp_net_user에 저장한다.
	mp_net_user = GetUserData(ah_socket);

	// 수신된 데이터에서 헤더를 먼저 읽는다.
	int state = ProcessHeaderInfo(ah_socket);
	// 정상적인 헤더이고 Body 데이터의 크기가 0보다 크다면 Body 데이터도 추가로 수신한다.
	if (state && m_net_body_size > 0) {
		// Body 데이터를 저장하기 위한 메모리를 할당한다.
		mp_net_body_data = new char[m_net_body_size];
		// Body 데이터를 수신한다. 그리고 제대로 수신되었는지 여부를 state 변수에 저장한다.
		state = m_net_body_size == ReceiveData(ah_socket, mp_net_body_data, m_net_body_size);
	}
	return state;
}

// FD_READ 이벤트에 의해 발생된 25002번 메시지 작업이 완료되었을때 호출하면 된다.
// BeginNetworkProcess 함수에서 만든 정보들을 정리하는 작업이 수행된다.
int WJ_ServerSocket::EndNetworkProcess()
{
	// 다시 데이터 수신 상태를 확인할 수 있도록 FD_READ 이벤트를 재설정한다.
	WSAAsyncSelect(mp_net_user->h_socket, mp_notify_wnd->m_hWnd, m_read_close_msg_id, FD_READ | FD_CLOSE);
	return 1;
}

// 소켓 관련 비동기 메시지를 처리하는 함수
void WJ_ServerSocket::MessageProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == m_accept_msg_id) { // 새로운 클라이언트가 접속을 시도하는 경우!
		UserData *p_user = ProcessAccept();  // 기본적인 접속 처리를 한다.
		if (p_user != NULL) AcceptUserProc(p_user);  // 추가적인 접속처리를 할 수 있도록 함수 호출
	} else if (message == m_read_close_msg_id) {
		if (WSAGETSELECTEVENT(lParam) == FD_READ) {  // 데이터가 수신됨!!
			if (BeginNetworkProcess((SOCKET)wParam)) {  // 헤더를 읽는다.
				// 정상적으로 메시지를 처리했다면 마무리 작업(FD_READ 재설정 포함)도 호출한다.
				if (1 == ProcessNetMessage()) EndNetworkProcess();

				// 데이터 수신에 사용하던 메모리가 있다면 메모리를 해제한다.
				if (mp_net_body_data != NULL) {
					delete[] mp_net_body_data;
					mp_net_body_data = NULL;
				}
			} else {   // Key 값이 일치하지 않아서 서버와의 접속을 끊는다.
				// 접속이 해제된 소켓을 제거하고 접속을 해제한 사용자 정보를
				// m_temp_user_data에 저장한다.
				CloseSocket(mp_net_user->h_socket, &m_temp_user_data);
				// 접속이 해제된 클라이언트에 추가적인 작업을 할수 있도록 함수 호출
				CloseUserProc(&m_temp_user_data, 1);
			}
		} else {  // FD_CLOSE, 상대편 이 종료됨!
			// 접속이 해제된 소켓을 제거하고 접속을 해제한 사용자 정보를
			// m_temp_user_data에 저장한다.
			CloseSocket((SOCKET)wParam, &m_temp_user_data);
			// 접속이 해제된 클라이언트에 추가적인 작업을 할수 있도록 함수 호출
			CloseUserProc(&m_temp_user_data, 0);
		}
	}
}
// 현재 서버가 실행된 컴퓨터에서 사용 가능한 IP 목록을 얻는 함수
int WJ_ServerSocket::GetLocalNetworkAddress(wchar_t a_ip_list[][24], int a_max_count)
{
	int count = 0;
	char host_name[128] = { 0, };
	addrinfo *result = NULL, *ptr, hints;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	gethostname(host_name, 128);  // 현재 컴퓨터의 호스트 명칭을 얻는다.

	if (!getaddrinfo(host_name, NULL, &hints, &result)) {  // 호스트에 있는 IP 정보를 얻는다.
		ptr = result;
		unsigned char *p_pos;
		while (ptr != NULL && count < a_max_count) {
			// 정수로 되어 있는 IP 주소를 문자열로 변환하기 위해 IP가 저장된 위치의 주소를 얻는다.
			p_pos = &((sockaddr_in *)ptr->ai_addr)->sin_addr.S_un.S_un_b.s_b1;
			// 정숫값을 IP4 형식의 문자열로 변환한다.
			swprintf_s(a_ip_list[count++], 24, L"%hhu.%hhu.%hhu.%hhu", *p_pos, *(p_pos+1), *(p_pos+2), *(p_pos+3));
			// 다음 IP 정보로 이동한다.
			ptr = ptr->ai_next;
		}
		freeaddrinfo(result);
	}
	return count;  // 얻는 IP 개수를 반환한다.
}