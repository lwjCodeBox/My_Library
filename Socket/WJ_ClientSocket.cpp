#include "pch.h"
#include "WJ_ClientSocket.h"

// #define _WINSOCK_DEPRECATED_NO_WARNINGS     -> framework.h�� �־��ּ���!!
#include <ws2tcpip.h>     // InetPton �Լ��� ����ϱ� ����!
#pragma comment(lib, "WS2_32.lib")  

// �⺻ ������ (���������� 27�� key�� �����)
WJ_ClientSocket::WJ_ClientSocket()
{
	InitObject(27);
}

// ���������� key�� �������� �ʰ� �ܺο��� �����ϵ��� �����ڿ��� key ���� �Է� ����
WJ_ClientSocket::WJ_ClientSocket(unsigned char a_key)
{
	InitObject(a_key);
}

// ��ü �ı���
WJ_ClientSocket::~WJ_ClientSocket()
{
	// ����ϴ� ������ ������ �����Ѵ�.
	CloseSocket();
	// ������ ���ſ� ����ϴ� �޸𸮰� �ִٸ� �޸𸮸� �����Ѵ�.
	if (mp_net_body_data != NULL) delete[] mp_net_body_data;

	WSACleanup();  // ���� ���̺귯���� �׸� ����ϵ��� �����Ѵ�.
}

// �� ��ü�� �ʱ�ȭ�ϴ� �Լ�. ���������� ȣ��Ǵ� �Լ���!
void WJ_ClientSocket::InitObject(unsigned char a_key)
{
	WSADATA temp;
	WSAStartup(0x0202, &temp);  // ���� ���̺귯���� ��� ���� ���·� �����.

	mh_socket = INVALID_SOCKET;
	m_is_connected = 0;
	m_connect_msg_id = 26001;
	m_read_close_msg_id = 26002;
	mp_notify_wnd = NULL;

	m_key = a_key;
	mp_net_body_data = NULL;
	m_net_body_size = 0;
}

// ������ Packet Frame���� ���ϴ� �����͸� �����ϴ� �Լ�
void WJ_ClientSocket::SendFrameData(char a_msg_id, const void *ap_data, int a_size)
{
	// ������ ������ �������� �����ϱ� ���� �޸𸮸� �Ҵ��Ѵ�. 
	// ���(4byte) + �ٵ�(send_data_size)
	char *p_data = new char[a_size + 4];
	// ���� Ű ���� ����
	*p_data = m_key;
	// �޽��� ���̵� ����
	*(p_data + 1) = a_msg_id;
	// ������ ���� ������ ũ�⸦ ����
	// ��� �κп� ������ ����� ������.
	*(unsigned short *)(p_data + 2) = a_size;
	// ������ ���� �����͸� ����
	memcpy(p_data + 4, ap_data, a_size);
	// mh_socket���� �����͸� a_size + 4 ũ�� ��ŭ �����Ѵ�.
	send(mh_socket, p_data, a_size + 4, 0);
	// ������ ���忡 ����� �޸𸮸� �����Ѵ�.
	delete[] p_data;
}

// ������ ������ �õ��ϴ� �Լ�. ������ ������ �����ǿ� ��Ʈ �׸��� ���� ���ӿ� ���� ����� 
// �˷��� �޽��� ���̵��  �����Ͱ� ���ŵǾ��ų� ������ ���� �Ǿ��� ���� �޽��� ���̵� �����Ѵ�.
void WJ_ClientSocket::ConnectToServer(const wchar_t *ap_ip_address, int a_port, CWnd *ap_notify_wnd, int a_connect_id, int a_read_close_id)
{
	if (mh_socket == INVALID_SOCKET) {
		// ����� �޽��� ���̵� ����
		m_connect_msg_id = a_connect_id;
		m_read_close_msg_id = a_read_close_id;
		// �޽����� ������ �������� �ּ� ����
		mp_notify_wnd = ap_notify_wnd;

		// AF_INET �ּ� ü�踦 ����ϴ� TCP ����� ���� ����!!
		mh_socket = socket(AF_INET, SOCK_STREAM, 0);
		
		// IP �ּ� ü��, ���α׷� �ĺ� ��ȣ(��Ʈ ��ȣ)
		sockaddr_in addr_data ={ AF_INET, htons(a_port), };
		// ap_ip_address �ּҸ� ����ϴ³�Ʈ��ũ�� ����ϵ��� ����
		InetPton(AF_INET, ap_ip_address, &addr_data.sin_addr.s_addr);
		
		// ���� ���ӿ� ����ϴ� connect �Լ��� ������ ������ �ְų� ��Ʈ��ũ�� ������ ������
		// �ִ� 28�ʰ� '���� ����' ���°� �ɼ� �ֱ� ������ ������ üũ���ִ� �񵿱⸦ �����Ѵ�.
		// mh_socket�� FD_CONNECT �̺�Ʈ�� �߻��ϸ� ���� ��ȭ ���ڿ� 26001�� �޽����� �߻��Ѵ�.
		
		// ���� ������ �񵿱� ������� ó���Ѵ�. ��, ���� ���ӿ� ���� �����
		// ������ ah_notify_wnd ������� a_connect_id(26001) �޽����� �������ݴϴ�.
		WSAAsyncSelect(mh_socket, ap_notify_wnd->m_hWnd, a_connect_id, FD_CONNECT);
		// ������ ������ ����ǰ� ������ �����Ѵ�. (������ ������ �õ��Ѵ�.)
		m_is_connected = 1; // ���� �õ������� ���¸� �����Ѵ�.
		// ������ ������ �õ��Ѵ�. �񵿱� ��忡���� connect �Լ���
		// ������ ����� ���ö����� ������� �ʰ� �ٷ� �������´�.
		connect(mh_socket, (sockaddr *)&addr_data, sizeof(addr_data));
	}
}

// ��ſ� ����ϴ� ������ �����ϴ� �Լ�
void WJ_ClientSocket::CloseSocket()
{
	if (mh_socket != INVALID_SOCKET) {
		closesocket(mh_socket);       // ������ �ݴ´�.
		mh_socket = INVALID_SOCKET;   // �ٽ� ����� �� �ֵ��� �ʱ�ȭ�Ѵ�.
		m_is_connected = 0;           // ���� �� ������ �����Ѵ�.
	}
}

// ������ ������ �����͸� �����Ҷ� ����ϴ� �Լ�
int WJ_ClientSocket::ReceiveData(char *ap_data, int a_size)
{
	int total_size = 0, read_size, retry_count = 0;
	// a_size ũ�⸸ŭ �� ���ŵ� ������ �ݺ��Ѵ�.
	while (total_size < a_size) {
		// ���� ũ�⸸ŭ ������ �õ��Ѵ�.
		read_size = recv(mh_socket, ap_data + total_size, a_size - total_size, 0);
		if (read_size == SOCKET_ERROR || read_size == 0) {  // �б⿡ ������ ���
			Sleep(10);   // 0.01�ʸ�ŭ ����Ѵ�.
			retry_count++;   // ��õ� Ƚ���� ������Ų��.
			if (retry_count > 300) break;  // 300ȸ ��õ� �Ҷ����� �����ϸ� �۾��� �ߴ��Ѵ�.
		} else {
			retry_count = 0;  // ��õ� Ƚ���� �ʱ�ȭ�Ѵ�.
			total_size += read_size;  // ���ŵ� �������� ũ�⸦ �ջ��Ѵ�.
		}
	}
	return total_size;   // ��ü ������ ����Ʈ�� ����Ѵ�.
}

// ���ŵ� �����Ϳ��� ������ ����� �о ó���ϴ� �Լ� 1�̸� �������� ����̰�
// 0�̸� ���������� ����̴�.
int WJ_ClientSocket::ProcessHeaderInfo()
{
	unsigned char key;
	// ù ����Ʈ�� �ִ� key ���� �д´�.
	recv(mh_socket, (char *)&key, 1, 0);
	// key ���� ������ key���� ��ġ�ϴ��� ���Ѵ�.
	if (key == m_key) {
		recv(mh_socket, (char *)&m_net_msg_id, 1, 0);
		recv(mh_socket, (char *)&m_net_body_size, 2, 0);
		return 1;  // key�� ��ġ�ϴ� ��� (�������� ���)
	}
	return 0;  // key�� �ٸ� ��� (���������� ���)
}

// ���ŵ� �����͸� ������ ������ �д� �Լ� (����� �ٵ� ��� �д´�.)
int WJ_ClientSocket::BeginNetworkProcess()
{
	// ���� �б� �� ��õ� �б⸦ ���� FD_READ �̺�Ʈ�� �����ϰ� �ٽ� �񵿱⸦ �����Ѵ�.
	WSAAsyncSelect(mh_socket, mp_notify_wnd->m_hWnd, m_read_close_msg_id, FD_CLOSE);

	// ���ŵ� �����Ϳ��� ����� ���� �д´�.
	int state = ProcessHeaderInfo();
	// �������� ����̰� Body �������� ũ�Ⱑ 0���� ũ�ٸ� Body �����͵� �߰��� �����Ѵ�.
	if (state && m_net_body_size > 0) {
		// Body �����͸� �����ϱ� ���� �޸𸮸� �Ҵ��Ѵ�.
		mp_net_body_data = new char[m_net_body_size];
		// Body �����͸� �����Ѵ�. �׸��� ����� ���ŵǾ����� ���θ� state ������ �����Ѵ�.
		state = m_net_body_size == ReceiveData(mp_net_body_data, m_net_body_size);
	}
	return state;  // ���� ���¸� ��ȯ�Ѵ�.
}

// ���� �۾��� �Ϸ�Ǿ����� ȣ���ϴ� �Լ� (FD_READ �缳�� �� �޸� ����)
void WJ_ClientSocket::EndNetworkProcess()
{
	// �ٽ� ������ ���� ���¸� Ȯ���� �� �ֵ��� FD_READ �̺�Ʈ�� �缳���Ѵ�.
	WSAAsyncSelect(mh_socket, mp_notify_wnd->m_hWnd, m_read_close_msg_id, FD_READ | FD_CLOSE);
}

// ���� ���� �񵿱� �޽����� ó���ϴ� �Լ�
void WJ_ClientSocket::MessageProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == m_connect_msg_id) {  // �������� ���� ����� ���� ���!
		if (WSAGETSELECTERROR(lParam)) CloseSocket();  // ������ ��� ���� ������!
		else {
			m_is_connected = 2;  // ���ӵ� ���·� ����!
			// ���ӵ� ���Ͽ� �����Ͱ� ���ŵǰų� ������ �����Ǿ��� �� �޽����� �߻��ϵ���
			// �񵿱⸦ �����Ѵ�.
			WSAAsyncSelect(mh_socket, mp_notify_wnd->m_hWnd, m_read_close_msg_id, FD_READ | FD_CLOSE);
		}
		// �� Ŭ������ ����ϴ� ����� �߰��� �ؾ��� ���� �ִٸ� ó���ϰ� ���ش�.
		ConnectedProcess();
	} else if (message == m_read_close_msg_id) { 
		if (WSAGETSELECTEVENT(lParam) == FD_READ) {  // �����Ͱ� ���ŵ�!!
			if (BeginNetworkProcess()) {  // ����� �д´�.
				// ���������� �޽����� ó���ߴٸ� ������ �۾�(FD_READ �缳�� ����)�� ȣ���Ѵ�.
				if(ProcessNetMessage()) EndNetworkProcess();

				// ������ ���ſ� ����ϴ� �޸𸮰� �ִٸ� �޸𸮸� �����Ѵ�.
				if (mp_net_body_data != NULL) {
					delete[] mp_net_body_data;
					mp_net_body_data = NULL;
				}
			} else { // Key ���� ��ġ���� �ʾƼ� �������� ������ ���´�.
				CloseSocket();  // ������ �����Ѵ�.
				ClosedProcess(1);  // �߰����� �۾��� �� �� �ֵ��� �Լ� ȣ��! (������ ���� ����)
			}
		} else {  // FD_CLOSE, ������ ������ ������
			CloseSocket();  // ������ �����Ѵ�.
			ClosedProcess(0); // �߰����� �۾��� �� �� �ֵ��� �Լ� ȣ��! (���� ����)
		}
	}
}
