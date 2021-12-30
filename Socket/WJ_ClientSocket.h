#ifndef _WJ_CLIENT_SOCKET_H_
#define _WJ_CLIENT_SOCKET_H_

// Socket ���� �Լ��� ����ϱ� ���� ������ϰ� ���̺귯���� ���Խ�Ų��.
#include <WinSock2.h>               

class WJ_ClientSocket
{
protected:
	SOCKET mh_socket;     // ����� ������ �ڵ�
	char m_is_connected;  // �������� ���� ���� (0: ���Ӿȵ�, 1: ���� �õ���, 2: ���ӵ�)
	// ������ �� ����� �޽��� ID�� �����Ͱ� ���ŵǰų� ������ ����Ǿ����� ����� �޽��� ID
	int m_connect_msg_id, m_read_close_msg_id;

	unsigned char m_key;             // key ���� ������ ����
	unsigned char m_net_msg_id;      // �޽��� ID
	unsigned short m_net_body_size;  // Body �������� ũ��
	char *mp_net_body_data;          // Body ������ ����
	CWnd *mp_notify_wnd;             // ���� �޽����� ���� ������ ��ü�� �ּ�

public:
	// �⺻ ������ (���������� 27�� key�� �����)
	WJ_ClientSocket();
	// ���������� key�� �������� �ʰ� �ܺο��� �����ϵ��� �����ڿ��� key ���� �Է� ����
	WJ_ClientSocket(unsigned char a_key );  
	// ��ü �ı���
	virtual ~WJ_ClientSocket();

	// �� ��ü�� �ʱ�ȭ�ϴ� �Լ�. ���������� ȣ��Ǵ� �Լ���!
	void InitObject(unsigned char a_key);

	// ������ ���� ���¸� �˷��ִ� �Լ� (0: ���� �ȵ�, 1: ���� ��)
	int IsConnect() { return m_is_connected == 2; }
	// ������ Packet Frame���� ���ϴ� �����͸� �����ϴ� �Լ�
	void SendFrameData(char a_msg_id, const void *ap_data, int a_size);
	// ������ ������ �õ��ϴ� �Լ�. ������ ������ �����ǿ� ��Ʈ
	// �׸��� ���� ���ӿ� ���� ����� �˷��� �޽��� ���̵�� 
	// �����Ͱ� ���ŵǾ��ų� ������ ���� �Ǿ��� ���� �޽��� ���̵� �����Ѵ�.
	void ConnectToServer(const wchar_t *ap_ip_address, int a_port, CWnd *ap_notify_wnd, 
						int a_connect_id = 26001, int a_read_close_id = 26002);
	// ��ſ� ����ϴ� ������ �����ϴ� �Լ�
	void CloseSocket();
	// ������ ������ �����͸� �����Ҷ� ����ϴ� �Լ�
	int ReceiveData(char *ap_data, int a_size);
	// ���ŵ� �����Ϳ��� ������ ����� �о ó���ϴ� �Լ� 1�̸� �������� ����̰�
	// 0�̸� ���������� ����̴�.
	int ProcessHeaderInfo();
	// ���ŵ� �����͸� ������ ������ �д� �Լ� (����� �ٵ� ��� �д´�.)
	int BeginNetworkProcess();
	// ���� �۾��� �Ϸ�Ǿ����� ȣ���ϴ� �Լ� (FD_READ �缳�� �� �޸� ����)
	void EndNetworkProcess();
	// ���� ���� �񵿱� �޽����� ó���ϴ� �Լ�
	void MessageProc(UINT message, WPARAM wParam, LPARAM lParam);
	// ������ ������ ����� ������ ȣ��Ǵ� �Լ�. 
	// ���� ����� ������ �� � �۾��� �ϰ� �ʹٸ� �� �Լ��� �������ؼ� ��� �˴ϴ�.
	virtual void ConnectedProcess() { }
	// �� �Լ��� �������ؼ� ���ŵ� �����͸� ��� ó���� ������ �۾��Ѵ�.
	// ���������� �۾��� �����ٸ� 1�� ��ȯ�ϰ� �۾��� �����ߴٸ� 0�� ��ȯ�Ѵ�.
	// 0�� ��ȯ�Ǹ� FD_READ �缳���� ���� �ʴ´�. (������ ���ŵǸ� 0�� ��ȯ�ϸ� �ȴ�)
	virtual int ProcessNetMessage() { return 1; }
	// ������ ������ ��쿡 ȣ��Ǵ� �Լ�. a_error_flag ���� 1�̸� ������ �߻��ؼ�
	// ������ ���̰� 0�̸� �������� ������ ������ ���Դϴ�.
	// ������ �����Ǿ��� �� � �۾��� �ϰ� �ʹٸ� �� �Լ��� �������ؼ� �۾��ϸ� �˴ϴ�.
	virtual void ClosedProcess(int a_error_flag) { }
};

#endif