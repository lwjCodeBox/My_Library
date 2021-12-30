#include "pch.h"
#include "WJ_ClientSocket.h"

// #define _WINSOCK_DEPRECATED_NO_WARNINGS     -> framework.h에 넣어주세요!!
#include <ws2tcpip.h>     // InetPton 함수를 사용하기 위해!
#pragma comment(lib, "WS2_32.lib")  

// 기본 생성자 (내부적으로 27을 key로 사용함)
WJ_ClientSocket::WJ_ClientSocket()
{
	InitObject(27);
}

// 내부적으로 key를 고정하지 않고 외부에서 설정하도록 생성자에서 key 값을 입력 받음
WJ_ClientSocket::WJ_ClientSocket(unsigned char a_key)
{
	InitObject(a_key);
}

// 객체 파괴자
WJ_ClientSocket::~WJ_ClientSocket()
{
	// 사용하던 소켓이 있으면 제거한다.
	CloseSocket();
	// 데이터 수신에 사용하던 메모리가 있다면 메모리를 해제한다.
	if (mp_net_body_data != NULL) delete[] mp_net_body_data;

	WSACleanup();  // 소켓 라이브러리를 그만 사용하도록 설정한다.
}

// 이 객체를 초기화하는 함수. 내부적으로 호출되는 함수임!
void WJ_ClientSocket::InitObject(unsigned char a_key)
{
	WSADATA temp;
	WSAStartup(0x0202, &temp);  // 소켓 라이브러리를 사용 가능 상태로 만든다.

	mh_socket = INVALID_SOCKET;
	m_is_connected = 0;
	m_connect_msg_id = 26001;
	m_read_close_msg_id = 26002;
	mp_notify_wnd = NULL;

	m_key = a_key;
	mp_net_body_data = NULL;
	m_net_body_size = 0;
}

// 정해진 Packet Frame으로 원하는 데이터를 전송하는 함수
void WJ_ClientSocket::SendFrameData(char a_msg_id, const void *ap_data, int a_size)
{
	// 전송할 데이터 프레임을 저장하기 위한 메모리를 할당한다. 
	// 헤더(4byte) + 바디(send_data_size)
	char *p_data = new char[a_size + 4];
	// 구별 키 값을 설정
	*p_data = m_key;
	// 메시지 아이디 설정
	*(p_data + 1) = a_msg_id;
	// 전송할 실제 데이터 크기를 설정
	// 헤더 부분에 데이터 사이즈를 저장함.
	*(unsigned short *)(p_data + 2) = a_size;
	// 전송할 실제 데이터를 복사
	memcpy(p_data + 4, ap_data, a_size);
	// mh_socket으로 데이터를 a_size + 4 크기 만큼 전송한다.
	send(mh_socket, p_data, a_size + 4, 0);
	// 프레임 저장에 사용한 메모리를 해제한다.
	delete[] p_data;
}

// 서버에 접속을 시도하는 함수. 접속할 서버의 아이피와 포트 그리고 서버 접속에 대한 결과를 
// 알려줄 메시지 아이디와  데이터가 수신되었거나 접속이 해제 되었을 때의 메시지 아이디를 설정한다.
void WJ_ClientSocket::ConnectToServer(const wchar_t *ap_ip_address, int a_port, CWnd *ap_notify_wnd, int a_connect_id, int a_read_close_id)
{
	if (mh_socket == INVALID_SOCKET) {
		// 사용할 메시지 아이디 설정
		m_connect_msg_id = a_connect_id;
		m_read_close_msg_id = a_read_close_id;
		// 메시지를 수신할 윈도우의 주소 저장
		mp_notify_wnd = ap_notify_wnd;

		// AF_INET 주소 체계를 사용하는 TCP 방식의 소켓 생성!!
		mh_socket = socket(AF_INET, SOCK_STREAM, 0);
		
		// IP 주소 체계, 프로그램 식별 번호(포트 번호)
		sockaddr_in addr_data ={ AF_INET, htons(a_port), };
		// ap_ip_address 주소를 사용하는네트워크를 사용하도록 설정
		InetPton(AF_INET, ap_ip_address, &addr_data.sin_addr.s_addr);
		
		// 서버 접속에 사용하는 connect 함수가 서버에 문제가 있거나 네트워크에 문제가 있으면
		// 최대 28초간 '응답 없음' 상태가 될수 있기 때문에 접속을 체크해주는 비동기를 설정한다.
		// mh_socket에 FD_CONNECT 이벤트가 발생하면 현재 대화 상자에 26001번 메시지가 발생한다.
		
		// 서버 접속을 비동기 방식으로 처리한다. 즉, 서버 접속에 대한 결과가
		// 나오면 ah_notify_wnd 윈도우로 a_connect_id(26001) 메시지를 전송해줍니다.
		WSAAsyncSelect(mh_socket, ap_notify_wnd->m_hWnd, a_connect_id, FD_CONNECT);
		// 서버에 접속이 진행되고 있음을 설정한다. (서버에 접속을 시도한다.)
		m_is_connected = 1; // 접속 시도중으로 상태를 변경한다.
		// 서버에 접속을 시도한다. 비동기 모드에서는 connect 함수가
		// 접속의 결과가 나올때까지 대기하지 않고 바로 빠져나온다.
		connect(mh_socket, (sockaddr *)&addr_data, sizeof(addr_data));
	}
}

// 통신에 사용하던 소켓을 제거하는 함수
void WJ_ClientSocket::CloseSocket()
{
	if (mh_socket != INVALID_SOCKET) {
		closesocket(mh_socket);       // 소켓을 닫는다.
		mh_socket = INVALID_SOCKET;   // 다시 사용할 수 있도록 초기화한다.
		m_is_connected = 0;           // 접속 안 됨으로 설정한다.
	}
}

// 서버가 전송한 데이터를 수신할때 사용하는 함수
int WJ_ClientSocket::ReceiveData(char *ap_data, int a_size)
{
	int total_size = 0, read_size, retry_count = 0;
	// a_size 크기만큼 다 수신될 때까지 반복한다.
	while (total_size < a_size) {
		// 남은 크기만큼 수신을 시도한다.
		read_size = recv(mh_socket, ap_data + total_size, a_size - total_size, 0);
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

// 수신된 데이터에서 프레임 헤더를 읽어서 처리하는 함수 1이면 정상적인 헤더이고
// 0이면 비정상적인 헤더이다.
int WJ_ClientSocket::ProcessHeaderInfo()
{
	unsigned char key;
	// 첫 바이트에 있는 key 값을 읽는다.
	recv(mh_socket, (char *)&key, 1, 0);
	// key 값이 지정된 key값과 일치하는지 비교한다.
	if (key == m_key) {
		recv(mh_socket, (char *)&m_net_msg_id, 1, 0);
		recv(mh_socket, (char *)&m_net_body_size, 2, 0);
		return 1;  // key가 일치하는 경우 (정상적인 헤더)
	}
	return 0;  // key가 다른 경우 (비정상적인 헤더)
}

// 수신된 데이터를 프레임 단위로 읽는 함수 (헤더와 바디 모두 읽는다.)
int WJ_ClientSocket::BeginNetworkProcess()
{
	// 끊어 읽기 및 재시도 읽기를 위해 FD_READ 이벤트를 제외하고 다시 비동기를 설정한다.
	WSAAsyncSelect(mh_socket, mp_notify_wnd->m_hWnd, m_read_close_msg_id, FD_CLOSE);

	// 수신된 데이터에서 헤더를 먼저 읽는다.
	int state = ProcessHeaderInfo();
	// 정상적인 헤더이고 Body 데이터의 크기가 0보다 크다면 Body 데이터도 추가로 수신한다.
	if (state && m_net_body_size > 0) {
		// Body 데이터를 저장하기 위한 메모리를 할당한다.
		mp_net_body_data = new char[m_net_body_size];
		// Body 데이터를 수신한다. 그리고 제대로 수신되었는지 여부를 state 변수에 저장한다.
		state = m_net_body_size == ReceiveData(mp_net_body_data, m_net_body_size);
	}
	return state;  // 수신 상태를 반환한다.
}

// 수신 작업이 완료되었을때 호출하는 함수 (FD_READ 재설정 및 메모리 해제)
void WJ_ClientSocket::EndNetworkProcess()
{
	// 다시 데이터 수신 상태를 확인할 수 있도록 FD_READ 이벤트를 재설정한다.
	WSAAsyncSelect(mh_socket, mp_notify_wnd->m_hWnd, m_read_close_msg_id, FD_READ | FD_CLOSE);
}

// 소켓 관련 비동기 메시지를 처리하는 함수
void WJ_ClientSocket::MessageProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == m_connect_msg_id) {  // 서버와의 접속 결과가 나온 경우!
		if (WSAGETSELECTERROR(lParam)) CloseSocket();  // 실패한 경우 소켓 제거함!
		else {
			m_is_connected = 2;  // 접속된 상태로 변경!
			// 접속된 소켓에 데이터가 수신되거나 접속이 해제되었을 때 메시지가 발생하도록
			// 비동기를 설정한다.
			WSAAsyncSelect(mh_socket, mp_notify_wnd->m_hWnd, m_read_close_msg_id, FD_READ | FD_CLOSE);
		}
		// 이 클래스를 사용하는 사람이 추가로 해야할 일이 있다면 처리하게 해준다.
		ConnectedProcess();
	} else if (message == m_read_close_msg_id) { 
		if (WSAGETSELECTEVENT(lParam) == FD_READ) {  // 데이터가 수신됨!!
			if (BeginNetworkProcess()) {  // 헤더를 읽는다.
				// 정상적으로 메시지를 처리했다면 마무리 작업(FD_READ 재설정 포함)도 호출한다.
				if(ProcessNetMessage()) EndNetworkProcess();

				// 데이터 수신에 사용하던 메모리가 있다면 메모리를 해제한다.
				if (mp_net_body_data != NULL) {
					delete[] mp_net_body_data;
					mp_net_body_data = NULL;
				}
			} else { // Key 값이 일치하지 않아서 서버와의 접속을 끊는다.
				CloseSocket();  // 소켓을 제거한다.
				ClosedProcess(1);  // 추가적인 작업을 할 수 있도록 함수 호출! (오류로 인한 종료)
			}
		} else {  // FD_CLOSE, 서버와 연결이 해제됨
			CloseSocket();  // 소켓을 제거한다.
			ClosedProcess(0); // 추가적인 작업을 할 수 있도록 함수 호출! (정상 종료)
		}
	}
}
