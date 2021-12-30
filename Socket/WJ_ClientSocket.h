#ifndef _WJ_CLIENT_SOCKET_H_
#define _WJ_CLIENT_SOCKET_H_

// Socket 관련 함수를 사용하기 위해 헤더파일과 라이브러리를 포함시킨다.
#include <WinSock2.h>               

class WJ_ClientSocket
{
protected:
	SOCKET mh_socket;     // 통신할 소켓의 핸들
	char m_is_connected;  // 서버와의 접속 상태 (0: 접속안됨, 1: 접속 시도중, 2: 접속됨)
	// 접속할 때 사용할 메시지 ID와 데이터가 수신되거나 접속이 종료되었을대 사용할 메시지 ID
	int m_connect_msg_id, m_read_close_msg_id;

	unsigned char m_key;             // key 값을 저장할 변수
	unsigned char m_net_msg_id;      // 메시지 ID
	unsigned short m_net_body_size;  // Body 데이터의 크기
	char *mp_net_body_data;          // Body 데이터 정보
	CWnd *mp_notify_wnd;             // 소켓 메시지를 받을 윈도우 객체의 주소

public:
	// 기본 생성자 (내부적으로 27을 key로 사용함)
	WJ_ClientSocket();
	// 내부적으로 key를 고정하지 않고 외부에서 설정하도록 생성자에서 key 값을 입력 받음
	WJ_ClientSocket(unsigned char a_key );  
	// 객체 파괴자
	virtual ~WJ_ClientSocket();

	// 이 객체를 초기화하는 함수. 내부적으로 호출되는 함수임!
	void InitObject(unsigned char a_key);

	// 서버와 접속 상태를 알려주는 함수 (0: 접속 안됨, 1: 접속 됨)
	int IsConnect() { return m_is_connected == 2; }
	// 정해진 Packet Frame으로 원하는 데이터를 전송하는 함수
	void SendFrameData(char a_msg_id, const void *ap_data, int a_size);
	// 서버에 접속을 시도하는 함수. 접속할 서버의 아이피와 포트
	// 그리고 서버 접속에 대한 결과를 알려줄 메시지 아이디와 
	// 데이터가 수신되었거나 접속이 해제 되었을 때의 메시지 아이디를 설정한다.
	void ConnectToServer(const wchar_t *ap_ip_address, int a_port, CWnd *ap_notify_wnd, 
						int a_connect_id = 26001, int a_read_close_id = 26002);
	// 통신에 사용하던 소켓을 제거하는 함수
	void CloseSocket();
	// 서버가 전송한 데이터를 수신할때 사용하는 함수
	int ReceiveData(char *ap_data, int a_size);
	// 수신된 데이터에서 프레임 헤더를 읽어서 처리하는 함수 1이면 정상적인 헤더이고
	// 0이면 비정상적인 헤더이다.
	int ProcessHeaderInfo();
	// 수신된 데이터를 프레임 단위로 읽는 함수 (헤더와 바디 모두 읽는다.)
	int BeginNetworkProcess();
	// 수신 작업이 완료되었을때 호출하는 함수 (FD_READ 재설정 및 메모리 해제)
	void EndNetworkProcess();
	// 소켓 관련 비동기 메시지를 처리하는 함수
	void MessageProc(UINT message, WPARAM wParam, LPARAM lParam);
	// 서버에 접속한 결과가 나오면 호출되는 함수. 
	// 접속 결과가 나왔을 때 어떤 작업을 하고 싶다면 이 함수를 재정의해서 사용 됩니다.
	virtual void ConnectedProcess() { }
	// 이 함수를 재정의해서 수신된 데이터를 어떻게 처리할 것인지 작업한다.
	// 성공적으로 작업이 끝났다면 1을 반환하고 작업에 실패했다면 0을 반환한다.
	// 0이 반환되면 FD_READ 재설정이 되지 않는다. (소켓이 제거되면 0을 반환하면 된다)
	virtual int ProcessNetMessage() { return 1; }
	// 접속이 해제된 경우에 호출되는 함수. a_error_flag 값이 1이면 오류가 발생해서
	// 해제된 것이고 0이면 서버에서 접속을 해제한 것입니다.
	// 접속이 해제되었을 때 어떤 작업을 하고 싶다면 이 함수를 재정의해서 작업하면 됩니다.
	virtual void ClosedProcess(int a_error_flag) { }
};

#endif