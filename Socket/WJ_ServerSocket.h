#ifndef _TW_SERVER_SOCKET_H_
#define _TW_SERVER_SOCKET_H_

// Socket 관련 함수를 사용하기 위해 헤더파일과 라이브러리를 포함시킨다.
#include <WinSock2.h>               

#define MAX_USER_COUNT    100     // 서버에 접속 가능한 최대 클라이언트 수

// 1개의 클라이언트 정보를 저장할 구조체
struct UserData
{
	SOCKET h_socket;          // 개별 클라이언트와 통신할 소켓 핸들
	wchar_t ip_address[16];   // 접속한 클라이언트의 주소
};

class WJ_ServerSocket
{
protected:
	// 이 시스템을 사용할 전체 사용자 정보
	UserData m_users[MAX_USER_COUNT];
	// 클라이언트 접속에 사용할 Listen 소켓
	SOCKET mh_listen_socket;
	// 비동기 메시지의 번호 ( 접속과 읽기 그리고 종료 )
	int m_accept_msg_id, m_read_close_msg_id;

	// 현재 처리가 진행되고 있는 수신 데이터 정보
	unsigned char m_key;             // 유효 클라이언트 구별을 위한 Key 값
	unsigned char m_net_msg_id;      // 메시지 ID
	unsigned short m_net_body_size;  // Body 데이터의 크기
	char *mp_net_body_data;          // Body 데이터 정보
	UserData *mp_net_user;           // 데이터를 전송한 클라이언트 정보

	int m_temp_user_index;           // 가장 최근에 삭제한 사용자의 index
	UserData m_temp_user_data;       // 접속을 해제한 사용자 정보를 백업할 때 사용
	CWnd *mp_notify_wnd;             // 비동기 메시지를 받을 윈도우의 주소

public:
	// 기본 생성자 (내부적으로 27을 key로 사용함)
	WJ_ServerSocket();
	// 내부적으로 key를 고정하지 않고 외부에서 설정하도록 생성자에서 key 값을 입력 받음
	WJ_ServerSocket(unsigned char a_key);
	// 객체 파괴자
	virtual ~WJ_ServerSocket();

	// 이 객체를 초기화하는 함수. 내부적으로 호출되는 함수
	void InitObject(unsigned char a_key);
	// 서버가 관리하는 사용자 목록의 시작 주소를 반환하는 함수
	inline UserData *GetUserData() { return m_users; }
	// 리슨이 진행중인지 여부를 얻는 함수 (0:중지, 1:리슨중)
	inline char IsListenStart() { return mh_listen_socket != INVALID_SOCKET; }

	// Listen 서비스를 시작할 때 사용하는 함수
	// 서버 서비스를 시작할 IP 주소와 포트 번호 그리고 비동기 메시지를 받을 윈도우 핸들과
	// 비동기에 사용할 메시지 ID를 매개 변수로 전달해야한다. 비동기 메시지로 사용할 ID를
	// 적지 않으면 접속은 25001번 읽기와 종료는 25002번 메시지를 사용하도록 설정한다.
	void StartListenService(const wchar_t *ap_ip_address, int a_port, CWnd *ap_notify_wnd, 
							int a_accept_id = 25001, int a_read_close_id = 25002);
	// 서버에 접속한 모든 클라이언트의 접속을 해제하고 Listen 소켓도 종료하는 함수
	void DestroyClientSockets();
	// 클라이언트로 1개의 네트워크 프레임 데이터를 하나 전송한다.
	// 프레임 구성 : "Key(27), Message ID, Body size, Body Data"
	void SendFrameData(SOCKET ah_socket, char a_msg_id, const void *ap_data, int a_size);
	// 접속한 모든 클라이언트에게 네트워크 프레임 데이트를 전송한다.
	void BroadcastFrameData(char a_msg_id, const void *ap_data, int a_size);
	// 새로운 사용자가 접속했을 때, 이 함수를 호출하면 사용자의 접속을 처리한다.
	// 예를 들어, 25001번 메시지가 발생했을때 이 함수를 호출하면 새로운 사용자와
	// 통신할 복사 소켓을 생성하고 이 소켓을 m_users 배열에 등록한다.
	// 그리고 함수의 반환 값은 현재 접속한 사용자가 저장된 m_users 배열 항목의 주소이다.
	// 만약 최대 사용자 수를 넘어서 사용자가 접속에 실패했다면 NULL 값이 반환된다.
	UserData *ProcessAccept();
	// 소켓 핸들을 가지고 해당 소켓 핸들이 저장된 m_users 배열의 항목 주소를 얻는 함수
	UserData *GetUserData(SOCKET ah_socket);
	// 한 번에 받기 어려운 크기의 네트워크 메시지를 재시도하면서 받을 수 있도록 만든 함수
	// 이 함수가 반환한 값이 a_size에 명시한 값과 일치하지 않다면 중간에 오류가 발생한 것이다.
	int ReceiveData(SOCKET ah_socket, char *ap_data, int a_size);
	// m_users 배열에서 a_index 위치에 있는 항목의 주소를 얻는 함수
	inline UserData *GetUserData(int a_index) { return m_users + a_index; }

	// 네트워크 메시지에서 헤더 정보를 처리하는 함수
	virtual int ProcessHeaderInfo(SOCKET ah_socket);
	// 네트워크로 데이터가 수신된 경우, 약속한 헤더를 읽고 Body 데이터까지
	// 읽어서 작업을 진행할수 있도록 준비하는 함수.
	// 이 함수는 FD_READ 이벤트에 의해 25002번 메시지가 발생했을때 사용하면 된다.
	virtual int BeginNetworkProcess(SOCKET ah_socket);
	// FD_READ 이벤트에 의해 발생된 25002번 메시지 작업이 완료되었을때 호출하면 된다.
	// BeginNetworkProcess 함수에서 만든 정보들을 정리하는 작업이 수행된다.
	virtual int EndNetworkProcess();
	// 새로운 사용자의 접속을 처리한다. 아래의 함수는 m_users 배열의 h_socket 값이
	// INVALID_SOCKET인 항목에 새로운 사용자 소켓을 저장한다.
	virtual int AddNewUser(SOCKET ah_new_socket, const wchar_t *ap_ip_address);
	// FD_CLOSE에 의해 25002번 메시지가 발생하여 사용자의 접속 정보를 해제할 때
	// 사용한다. 이 메시지는 25002번 메시지에서만 사용해야 하는 것은 아니고
	// 특정 사용자를 종료시킬 때 사용해도 됩니다. 두 번째 인자로 전달되는 ap_close_user에는
	// 접속을 해제한 사용자의 정보가 복사됩니다. 따라서 지역 변수로 UserData 구조체 변수를
	// 선언해서 해당 변수의 주소를 전달해야지 정보를 받을 수 있습니다.
	virtual void CloseSocket(SOCKET ah_socket, UserData *ap_close_user);


	// 새로운 클라이언트가 접속되고 추가적인 작업이 필요하다면 이 함수를 재정의 해서 추가하면 된다.
	virtual void AcceptUserProc(UserData *ap_user_data) {  }
	// 이 함수를 재정의해서 수신된 데이터를 어떻게 처리할 것인지 작업한다.
	// 성공적으로 작업이 끝났다면 1을 반환하고 작업에 실패했다면 0을 반환한다.
	// 0이 반환되면 FD_READ 재설정이 되지 않는다. (소켓이 제거되면 0을 반환하면 된다)
	virtual int ProcessNetMessage() { return 1; }
	// 접속이 해제된 경우에 호출되는 함수. a_error_flag 값이 1이면 오류가 발생해서
	// 해제된 것이고 0이면 서버에서 접속을 해제한 것입니다.
	// 접속이 해제되었을 때 어떤 작업을 추가하고 싶다면 이 함수를 재정의해서 작업하면 됩니다.
	virtual void CloseUserProc(UserData *ap_user_data, int a_error_flag) {  }
	// 소켓 관련 비동기 메시지를 처리하는 함수
	void MessageProc(UINT message, WPARAM wParam, LPARAM lParam);

	// 현재 서버가 실행된 컴퓨터에서 사용 가능한 IP 목록을 얻는 함수
	// 사용 가능한 IP의 개수가 반환됨
	int GetLocalNetworkAddress(wchar_t a_ip_list[][24], int a_max_count);
};

#endif