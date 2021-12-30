#ifndef _TW_SERVER_SOCKET_H_
#define _TW_SERVER_SOCKET_H_

// Socket ���� �Լ��� ����ϱ� ���� ������ϰ� ���̺귯���� ���Խ�Ų��.
#include <WinSock2.h>               

#define MAX_USER_COUNT    100     // ������ ���� ������ �ִ� Ŭ���̾�Ʈ ��

// 1���� Ŭ���̾�Ʈ ������ ������ ����ü
struct UserData
{
	SOCKET h_socket;          // ���� Ŭ���̾�Ʈ�� ����� ���� �ڵ�
	wchar_t ip_address[16];   // ������ Ŭ���̾�Ʈ�� �ּ�
};

class WJ_ServerSocket
{
protected:
	// �� �ý����� ����� ��ü ����� ����
	UserData m_users[MAX_USER_COUNT];
	// Ŭ���̾�Ʈ ���ӿ� ����� Listen ����
	SOCKET mh_listen_socket;
	// �񵿱� �޽����� ��ȣ ( ���Ӱ� �б� �׸��� ���� )
	int m_accept_msg_id, m_read_close_msg_id;

	// ���� ó���� ����ǰ� �ִ� ���� ������ ����
	unsigned char m_key;             // ��ȿ Ŭ���̾�Ʈ ������ ���� Key ��
	unsigned char m_net_msg_id;      // �޽��� ID
	unsigned short m_net_body_size;  // Body �������� ũ��
	char *mp_net_body_data;          // Body ������ ����
	UserData *mp_net_user;           // �����͸� ������ Ŭ���̾�Ʈ ����

	int m_temp_user_index;           // ���� �ֱٿ� ������ ������� index
	UserData m_temp_user_data;       // ������ ������ ����� ������ ����� �� ���
	CWnd *mp_notify_wnd;             // �񵿱� �޽����� ���� �������� �ּ�

public:
	// �⺻ ������ (���������� 27�� key�� �����)
	WJ_ServerSocket();
	// ���������� key�� �������� �ʰ� �ܺο��� �����ϵ��� �����ڿ��� key ���� �Է� ����
	WJ_ServerSocket(unsigned char a_key);
	// ��ü �ı���
	virtual ~WJ_ServerSocket();

	// �� ��ü�� �ʱ�ȭ�ϴ� �Լ�. ���������� ȣ��Ǵ� �Լ�
	void InitObject(unsigned char a_key);
	// ������ �����ϴ� ����� ����� ���� �ּҸ� ��ȯ�ϴ� �Լ�
	inline UserData *GetUserData() { return m_users; }
	// ������ ���������� ���θ� ��� �Լ� (0:����, 1:������)
	inline char IsListenStart() { return mh_listen_socket != INVALID_SOCKET; }

	// Listen ���񽺸� ������ �� ����ϴ� �Լ�
	// ���� ���񽺸� ������ IP �ּҿ� ��Ʈ ��ȣ �׸��� �񵿱� �޽����� ���� ������ �ڵ��
	// �񵿱⿡ ����� �޽��� ID�� �Ű� ������ �����ؾ��Ѵ�. �񵿱� �޽����� ����� ID��
	// ���� ������ ������ 25001�� �б�� ����� 25002�� �޽����� ����ϵ��� �����Ѵ�.
	void StartListenService(const wchar_t *ap_ip_address, int a_port, CWnd *ap_notify_wnd, 
							int a_accept_id = 25001, int a_read_close_id = 25002);
	// ������ ������ ��� Ŭ���̾�Ʈ�� ������ �����ϰ� Listen ���ϵ� �����ϴ� �Լ�
	void DestroyClientSockets();
	// Ŭ���̾�Ʈ�� 1���� ��Ʈ��ũ ������ �����͸� �ϳ� �����Ѵ�.
	// ������ ���� : "Key(27), Message ID, Body size, Body Data"
	void SendFrameData(SOCKET ah_socket, char a_msg_id, const void *ap_data, int a_size);
	// ������ ��� Ŭ���̾�Ʈ���� ��Ʈ��ũ ������ ����Ʈ�� �����Ѵ�.
	void BroadcastFrameData(char a_msg_id, const void *ap_data, int a_size);
	// ���ο� ����ڰ� �������� ��, �� �Լ��� ȣ���ϸ� ������� ������ ó���Ѵ�.
	// ���� ���, 25001�� �޽����� �߻������� �� �Լ��� ȣ���ϸ� ���ο� ����ڿ�
	// ����� ���� ������ �����ϰ� �� ������ m_users �迭�� ����Ѵ�.
	// �׸��� �Լ��� ��ȯ ���� ���� ������ ����ڰ� ����� m_users �迭 �׸��� �ּ��̴�.
	// ���� �ִ� ����� ���� �Ѿ ����ڰ� ���ӿ� �����ߴٸ� NULL ���� ��ȯ�ȴ�.
	UserData *ProcessAccept();
	// ���� �ڵ��� ������ �ش� ���� �ڵ��� ����� m_users �迭�� �׸� �ּҸ� ��� �Լ�
	UserData *GetUserData(SOCKET ah_socket);
	// �� ���� �ޱ� ����� ũ���� ��Ʈ��ũ �޽����� ��õ��ϸ鼭 ���� �� �ֵ��� ���� �Լ�
	// �� �Լ��� ��ȯ�� ���� a_size�� ����� ���� ��ġ���� �ʴٸ� �߰��� ������ �߻��� ���̴�.
	int ReceiveData(SOCKET ah_socket, char *ap_data, int a_size);
	// m_users �迭���� a_index ��ġ�� �ִ� �׸��� �ּҸ� ��� �Լ�
	inline UserData *GetUserData(int a_index) { return m_users + a_index; }

	// ��Ʈ��ũ �޽������� ��� ������ ó���ϴ� �Լ�
	virtual int ProcessHeaderInfo(SOCKET ah_socket);
	// ��Ʈ��ũ�� �����Ͱ� ���ŵ� ���, ����� ����� �а� Body �����ͱ���
	// �о �۾��� �����Ҽ� �ֵ��� �غ��ϴ� �Լ�.
	// �� �Լ��� FD_READ �̺�Ʈ�� ���� 25002�� �޽����� �߻������� ����ϸ� �ȴ�.
	virtual int BeginNetworkProcess(SOCKET ah_socket);
	// FD_READ �̺�Ʈ�� ���� �߻��� 25002�� �޽��� �۾��� �Ϸ�Ǿ����� ȣ���ϸ� �ȴ�.
	// BeginNetworkProcess �Լ����� ���� �������� �����ϴ� �۾��� ����ȴ�.
	virtual int EndNetworkProcess();
	// ���ο� ������� ������ ó���Ѵ�. �Ʒ��� �Լ��� m_users �迭�� h_socket ����
	// INVALID_SOCKET�� �׸� ���ο� ����� ������ �����Ѵ�.
	virtual int AddNewUser(SOCKET ah_new_socket, const wchar_t *ap_ip_address);
	// FD_CLOSE�� ���� 25002�� �޽����� �߻��Ͽ� ������� ���� ������ ������ ��
	// ����Ѵ�. �� �޽����� 25002�� �޽��������� ����ؾ� �ϴ� ���� �ƴϰ�
	// Ư�� ����ڸ� �����ų �� ����ص� �˴ϴ�. �� ��° ���ڷ� ���޵Ǵ� ap_close_user����
	// ������ ������ ������� ������ ����˴ϴ�. ���� ���� ������ UserData ����ü ������
	// �����ؼ� �ش� ������ �ּҸ� �����ؾ��� ������ ���� �� �ֽ��ϴ�.
	virtual void CloseSocket(SOCKET ah_socket, UserData *ap_close_user);


	// ���ο� Ŭ���̾�Ʈ�� ���ӵǰ� �߰����� �۾��� �ʿ��ϴٸ� �� �Լ��� ������ �ؼ� �߰��ϸ� �ȴ�.
	virtual void AcceptUserProc(UserData *ap_user_data) {  }
	// �� �Լ��� �������ؼ� ���ŵ� �����͸� ��� ó���� ������ �۾��Ѵ�.
	// ���������� �۾��� �����ٸ� 1�� ��ȯ�ϰ� �۾��� �����ߴٸ� 0�� ��ȯ�Ѵ�.
	// 0�� ��ȯ�Ǹ� FD_READ �缳���� ���� �ʴ´�. (������ ���ŵǸ� 0�� ��ȯ�ϸ� �ȴ�)
	virtual int ProcessNetMessage() { return 1; }
	// ������ ������ ��쿡 ȣ��Ǵ� �Լ�. a_error_flag ���� 1�̸� ������ �߻��ؼ�
	// ������ ���̰� 0�̸� �������� ������ ������ ���Դϴ�.
	// ������ �����Ǿ��� �� � �۾��� �߰��ϰ� �ʹٸ� �� �Լ��� �������ؼ� �۾��ϸ� �˴ϴ�.
	virtual void CloseUserProc(UserData *ap_user_data, int a_error_flag) {  }
	// ���� ���� �񵿱� �޽����� ó���ϴ� �Լ�
	void MessageProc(UINT message, WPARAM wParam, LPARAM lParam);

	// ���� ������ ����� ��ǻ�Ϳ��� ��� ������ IP ����� ��� �Լ�
	// ��� ������ IP�� ������ ��ȯ��
	int GetLocalNetworkAddress(wchar_t a_ip_list[][24], int a_max_count);
};

#endif