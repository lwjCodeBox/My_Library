#include "pch.h"
#include "WJ_ServerSocket.h"

// #define _WINSOCK_DEPRECATED_NO_WARNINGS     -> framework.h�� �־��ּ���!!
#include <ws2tcpip.h>     // InetPton �Լ��� ����ϱ� ����!
#pragma comment(lib, "WS2_32.lib")  

// �⺻ ������ (���������� 27�� key�� �����)
WJ_ServerSocket::WJ_ServerSocket()
{
	InitObject(27);
}

// ���������� key�� �������� �ʰ� �ܺο��� �����ϵ��� �����ڿ��� key ���� �Է� ����
WJ_ServerSocket::WJ_ServerSocket(unsigned char a_key)
{
	InitObject(a_key);
}

// �� ��ü�� �ʱ�ȭ�ϴ� �Լ�. ���������� ȣ��Ǵ� �Լ���!
void WJ_ServerSocket::InitObject(unsigned char a_key)
{
	WSADATA temp;
	WSAStartup(0x0202, &temp);  // ���� ���̺귯���� ��� ���� ���·� �����.

	// �迭�� �� �׸� �ִ� h_socket ���� INVALID_SOCKET���� �ʱ�ȭ�ؼ�
	// ���ӵ� ����� ���ϰ� �����ϴ� �뵵�� ����Ѵ�. �� ���� INVALID_SOCKET�̸�
	// ip_address�� ����� ���� �ǹ̰� ���� ������ ip_address ���� �ʱ�ȭ���� �ʴ´�.
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

// ��ü �ı���
WJ_ServerSocket::~WJ_ServerSocket()
{
	// ���ӵ� Ŭ���̾�Ʈ�� �ִٸ� ���� ������ ��� �����ϰ� ���� ���ϵ� �����Ѵ�.
	DestroyClientSockets();

	// ������ ���ſ� ����ϴ� �޸𸮰� �ִٸ� �޸𸮸� �����Ѵ�.
	if (mp_net_body_data != NULL) delete[] mp_net_body_data;

	WSACleanup();  // ���� ���̺귯���� �׸� ����ϵ��� �����Ѵ�.
}

// ������ ������ ��� Ŭ���̾�Ʈ�� ������ �����ϰ� Listen ���ϵ� �����ϴ� �Լ�
void WJ_ServerSocket::DestroyClientSockets()
{
	UserData *p = m_users;
	for (int i = 0; i < MAX_USER_COUNT; i++, p++) {
		if (p->h_socket != INVALID_SOCKET) {  // �Ҵ�� ������ �ִٸ�
			closesocket(p->h_socket);  // ������ �ݴ´�.
			p->h_socket = INVALID_SOCKET;  // ������ �ٽ� ����� �� �ֵ��� �ʱ�ȭ�Ѵ�.
		}
	}

	if (mh_listen_socket != INVALID_SOCKET) {  // ���� ������ ������� �ִٸ�
		closesocket(mh_listen_socket);  // ���� ������ �ݴ´�.
		mh_listen_socket = INVALID_SOCKET;  // ���� ������ �ʱ�ȭ�Ѵ�.
	}
}

// Ŭ���̾�Ʈ�� 1���� ��Ʈ��ũ ������ �����͸� �ϳ� �����Ѵ�.
// ������ ���� : "Key(27), Message ID, Body size, Body Data"
void WJ_ServerSocket::SendFrameData(SOCKET ah_socket, char a_msg_id, const void *ap_data, int a_size)
{
	// ������ ������ �������� �����ϱ� ���� �޸𸮸� �Ҵ��Ѵ�. 
	char *p_data = new char[a_size + 4];
	// ���� Ű ���� ����
	*p_data = m_key;
	// �޽��� ���̵� ����
	*(p_data + 1) = a_msg_id;
	// ������ ���� ������ ũ�⸦ ����
	*(unsigned short *)(p_data + 2) = a_size;
	// ������ ���� �����͸� ����
	memcpy(p_data + 4, ap_data, a_size);
	// ah_socket���� �����͸� a_size + 4 ũ�� ��ŭ �����Ѵ�.
	send(ah_socket, p_data, a_size + 4, 0);
	// ������ ���忡 ����� �޸𸮸� �����Ѵ�.
	delete[] p_data;
}

// ������ ��� Ŭ���̾�Ʈ���� ��Ʈ��ũ ������ ����Ʈ�� �����Ѵ�.
void WJ_ServerSocket::BroadcastFrameData(char a_msg_id, const void *ap_data, int a_size)
{
	// ������ ������ �������� �����ϱ� ���� �޸𸮸� �Ҵ��Ѵ�. 
	char *p_data = new char[a_size + 4];
	// ���� Ű ���� ����
	*p_data = m_key;
	// �޽��� ���̵� ����
	*(p_data + 1) = a_msg_id;
	// ������ ���� ������ ũ�⸦ ����
	*(unsigned short *)(p_data + 2) = a_size;
	// ������ ���� �����͸� ����
	memcpy(p_data + 4, ap_data, a_size);

	UserData *p = m_users;
	// ���ӵ� ��� ����ڿ��� �����͸� �����Ѵ�.
	for (int i = 0; i < MAX_USER_COUNT; i++, p++) {
		// ������ ���ǰ� �ִٸ� p->h_socket���� �����͸� a_size + 4 ũ�� ��ŭ �����Ѵ�.
		if (p->h_socket != INVALID_SOCKET) send(p->h_socket, p_data, a_size + 4, 0);
	}
	// ������ ���忡 ����� �޸𸮸� �����Ѵ�.
	delete[] p_data;
}

// Listen ���񽺸� ������ �� ����ϴ� �Լ�
void WJ_ServerSocket::StartListenService(const wchar_t *ap_ip_address, int a_port, CWnd *ap_notify_wnd, int a_accept_id, int a_read_close_id)
{
	mp_notify_wnd = ap_notify_wnd;
	m_accept_msg_id = a_accept_id;
	m_read_close_msg_id = a_read_close_id;

	// AF_INET �ּ� ü�踦 ����ϴ� TCP ����� ���� ����!!
	mh_listen_socket = socket(AF_INET, SOCK_STREAM, 0);

	// IP �ּ� ü��, ���α׷� �ĺ� ��ȣ(��Ʈ ��ȣ)
	sockaddr_in addr_data ={ AF_INET, htons(a_port), };
	// ap_ip_address �ּҸ� ����ϴ³�Ʈ��ũ�� ����ϵ��� ����
	InetPton(AF_INET, ap_ip_address, &addr_data.sin_addr.s_addr);

	// addr_data�� ������ ������ ����Ͽ� ������ ��Ʈ��ũ �ý��ۿ� �����Ѵ�.
	bind(mh_listen_socket, (sockaddr *)&addr_data, sizeof(addr_data));
	// ������ ó���� ������ �����Ѵ�. ������ �� ó���� ���� ó���� �ƴ϶� 
	// ����ó���� ���ڰ� ũ�ٰ� �������� �ʽ��ϴ�.
	listen(mh_listen_socket, 5);
	// mh_listen_socket�� ����ڰ� ������ �õ�(FD_ACCEPT)���� �� 
	// ���� ��ȭ ����(m_hWnd)�� 25001�� �޽����� �߻��ϵ��� �񵿱⸦ �����Ѵ�.
	WSAAsyncSelect(mh_listen_socket, mp_notify_wnd->m_hWnd, m_accept_msg_id, FD_ACCEPT);
}

// ���ο� ����ڰ� �������� ��, �� �Լ��� ȣ���ϸ� ������� ������ ó���Ѵ�.
UserData *WJ_ServerSocket::ProcessAccept()
{
	// ������ �õ��� ����(Ŭ���̾�Ʈ�� ����� ����)�� �ּҸ� ������ ����
	sockaddr_in addr_data;
	// �ڽ��� ����� �ּ� ���� ������ ũ�⸦ ���
	int addr_data_size = sizeof(addr_data);

	// Ŭ���̾�Ʈ ������ ������ ����Ѵ�. ������ ����ϸ� ���� ������ �����Ǿ�
	// �� �ڵ� ���� ��ȯ(h_client_socket�� ����)�ȴ�. �̶� addr_data ��������
	// ������ �õ��� ������ IP �ּҰ� ���� �� ���·� ����ȴ�.
	SOCKET h_client_socket = accept(mh_listen_socket, (sockaddr *)&addr_data, &addr_data_size);

	wchar_t temp_ip_address[32];
	// addr_data ������ ���� ���·� ����Ǿ� �ִ� IP �ּҸ� ���ڿ� ���·�
	// �����ؼ� temp_ip_address�� �����Ѵ�.
	InetNtop(AF_INET, &addr_data.sin_addr, temp_ip_address, 32);
	int index = AddNewUser(h_client_socket, temp_ip_address);
	// ����, �迭 ��ü�� �� üũ�ߴµ� �� ������ ���ٸ� �ִ� ������ ���� 
	// �ʰ��� �����̱� ������ ������ �����ϰ� ������ ����Ѵ�.
	if (index == MAX_USER_COUNT) {
		closesocket(h_client_socket);
		return NULL;  // �ִ� ������ ���� �Ѿ ���ӿ� ���� ��!
	}
	return m_users + index;  // ���� ������ ������� ������ ��� �ּҸ� ��ȯ�Ѵ�.
}

// ���ο� ������� ������ ó���Ѵ�. �Ʒ��� �Լ��� m_users �迭�� h_socket ����
// INVALID_SOCKET�� �׸� ���ο� ����� ������ �����Ѵ�.
int WJ_ServerSocket::AddNewUser(SOCKET ah_new_socket, const wchar_t *ap_ip_address)
{
	UserData *p = m_users;
	// h_client_socket�� ���� �����̱� ������ �� �Լ��� ����Ǹ� �������. ����
	// �ڽ��� �����ϰ� �ִ� Ŭ���̾�Ʈ ���� ������ �� ���� �ִ��� Ȯ���ؼ� �����Ѵ�.
	for (int i = 0; i < MAX_USER_COUNT; i++, p++) {
		// �� �迭�� h_socket�� INVALID_SOCKET ���� ����Ǿ� �ִٸ� �ش� �迭 �׸���
		// ������ ������ �ڵ��� ������� �ʾҴٴ� ���̴�. ��, ����ִٴ� ���̴�.
		if (p->h_socket == INVALID_SOCKET) {
			// �迭�� ��� �ִ� ������ ������ ������ �ڵ��� �����Ѵ�.
			p->h_socket = ah_new_socket;
			// ������ ������� IP�� �����Ѵ�.
			wcscpy_s(p->ip_address, 16, ap_ip_address);
			// h_client_socket�� FD_READ �Ǵ� FD_CLOSE �̺�Ʈ�� �߻��ϸ� ��ȭ ���ڷ�
			// 25002�� �޽����� �����ϵ��� �񵿱⸦ �����Ѵ�.
			WSAAsyncSelect(ah_new_socket, mp_notify_wnd->m_hWnd, m_read_close_msg_id, FD_READ | FD_CLOSE);
			return i;
		}
	}
	return MAX_USER_COUNT;  // ����ִ� �׸��� ���� ���!
}

// ���� �ڵ��� ������ �ش� ���� �ڵ��� ����� m_users �迭�� �׸� �ּҸ� ��� �Լ�
UserData *WJ_ServerSocket::GetUserData(SOCKET ah_socket)
{
	// �ڽ��� �����ϰ� �ִ� Ŭ���̾�Ʈ �߿��� � Ŭ���̾�Ʈ�� �̺�Ʈ��
	// �߻����״��� Ȯ���ϱ� ���ؼ� �迭�� Ž���Ѵ�.
	for (int i = 0; i < MAX_USER_COUNT; i++) {
		// ah_socket�� ���� ��ġ�ϴ� ������ ���� �̺�Ʈ�� �߻���Ų Ŭ���̾�Ʈ�̴�.
		if (m_users[i].h_socket == ah_socket) return m_users + i;
	}
	return NULL; // �˻��� ������ ���!
}

// FD_CLOSE�� ���� 25002�� �޽����� �߻��Ͽ� ������� ���� ������ ������ ������Ѵ�. 
// �� �޽����� 25002�� �޽��������� ����ؾ� �ϴ� ���� �ƴϰ� Ư�� ����ڸ� �����ų ��
// ����ص� �˴ϴ�. �� ��° ���ڷ� ���޵Ǵ� ap_close_user���� ������ ������ 
// ������� ������ ����˴ϴ�. ���� ���� ������ UserData ����ü ������ �����ؼ�
// �ش� ������ �ּҸ� �����ؾ��� ������ ���� �� �ֽ��ϴ�.
void WJ_ServerSocket::CloseSocket(SOCKET ah_socket, UserData *ap_close_user)
{
	// ah_socket�� �ش��ϴ� ����� ������ ã�´�.
	UserData *p_user = GetUserData(ah_socket);
	// ����� ������ ã�Ұ� �� ������ ������ �޸𸮰� ���޵Ǿ��ٸ�
	// ������ ����� ������ ap_close_user�� �����Ѵ�.
	if (p_user != NULL){
		// ������ ������� index ��ġ�� �����Ѵ�.
		m_temp_user_index = p_user - m_users;
		// ������ ����� ������ ����� �޸� �ּҰ� ���޵Ǿ��ٸ� ������ ����Ѵ�.
		if(ap_close_user != NULL) memcpy(ap_close_user, p_user, sizeof(UserData));
		// �迭�� �����ؼ� �����ϴ� ������ �ʱ�ȭ�Ѵ�.
		p_user->h_socket = INVALID_SOCKET;
	}	
	closesocket(ah_socket);  // ������ �����Ѵ�.
}

// Ŭ���̾�Ʈ�� ������ �����͸� �����Ҷ� ����ϴ� �Լ�
int WJ_ServerSocket::ReceiveData(SOCKET ah_socket, char *ap_data, int a_size)
{
	int total_size = 0, read_size, retry_count = 0;
	// a_size ũ�⸸ŭ �� ���ŵ� ������ �ݺ��Ѵ�.
	while (total_size < a_size) {
		// ���� ũ�⸸ŭ ������ �õ��Ѵ�.
		read_size = recv(ah_socket, ap_data + total_size, a_size - total_size, 0);
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

// ��Ʈ��ũ �޽������� ��� ������ ó���ϴ� �Լ�
int WJ_ServerSocket::ProcessHeaderInfo(SOCKET ah_socket)
{
	unsigned char key;
	// ù ����Ʈ�� �ִ� key ���� �д´�.
	recv(ah_socket, (char *)&key, 1, 0);
	// key ���� ������ key���� ��ġ�ϴ��� ���Ѵ�.
	if (key == m_key) {
		recv(ah_socket, (char *)&m_net_msg_id, 1, 0);
		recv(ah_socket, (char *)&m_net_body_size, 2, 0);
		return 1;  // key�� ��ġ�ϴ� ��� (�������� ���)
	}
	return 0;  // key�� �ٸ� ��� (���������� ���)
}

// ��Ʈ��ũ�� �����Ͱ� ���ŵ� ���, ����� ����� �а� Body �����ͱ���
// �о �۾��� �����Ҽ� �ֵ��� �غ��ϴ� �Լ�.
// �� �Լ��� FD_READ �̺�Ʈ�� ���� 25002�� �޽����� �߻������� ����ϸ� �ȴ�.
int WJ_ServerSocket::BeginNetworkProcess(SOCKET ah_socket)
{
	// ���� �б� �� ��õ� �б⸦ ���� FD_READ �̺�Ʈ�� �����ϰ� �ٽ� �񵿱⸦ �����Ѵ�.
	WSAAsyncSelect(ah_socket, mp_notify_wnd->m_hWnd, m_read_close_msg_id, FD_CLOSE);
	// �����͸� ������ Ŭ���̾�Ʈ�� ������ ã�Ƽ� mp_net_user�� �����Ѵ�.
	mp_net_user = GetUserData(ah_socket);

	// ���ŵ� �����Ϳ��� ����� ���� �д´�.
	int state = ProcessHeaderInfo(ah_socket);
	// �������� ����̰� Body �������� ũ�Ⱑ 0���� ũ�ٸ� Body �����͵� �߰��� �����Ѵ�.
	if (state && m_net_body_size > 0) {
		// Body �����͸� �����ϱ� ���� �޸𸮸� �Ҵ��Ѵ�.
		mp_net_body_data = new char[m_net_body_size];
		// Body �����͸� �����Ѵ�. �׸��� ����� ���ŵǾ����� ���θ� state ������ �����Ѵ�.
		state = m_net_body_size == ReceiveData(ah_socket, mp_net_body_data, m_net_body_size);
	}
	return state;
}

// FD_READ �̺�Ʈ�� ���� �߻��� 25002�� �޽��� �۾��� �Ϸ�Ǿ����� ȣ���ϸ� �ȴ�.
// BeginNetworkProcess �Լ����� ���� �������� �����ϴ� �۾��� ����ȴ�.
int WJ_ServerSocket::EndNetworkProcess()
{
	// �ٽ� ������ ���� ���¸� Ȯ���� �� �ֵ��� FD_READ �̺�Ʈ�� �缳���Ѵ�.
	WSAAsyncSelect(mp_net_user->h_socket, mp_notify_wnd->m_hWnd, m_read_close_msg_id, FD_READ | FD_CLOSE);
	return 1;
}

// ���� ���� �񵿱� �޽����� ó���ϴ� �Լ�
void WJ_ServerSocket::MessageProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == m_accept_msg_id) { // ���ο� Ŭ���̾�Ʈ�� ������ �õ��ϴ� ���!
		UserData *p_user = ProcessAccept();  // �⺻���� ���� ó���� �Ѵ�.
		if (p_user != NULL) AcceptUserProc(p_user);  // �߰����� ����ó���� �� �� �ֵ��� �Լ� ȣ��
	} else if (message == m_read_close_msg_id) {
		if (WSAGETSELECTEVENT(lParam) == FD_READ) {  // �����Ͱ� ���ŵ�!!
			if (BeginNetworkProcess((SOCKET)wParam)) {  // ����� �д´�.
				// ���������� �޽����� ó���ߴٸ� ������ �۾�(FD_READ �缳�� ����)�� ȣ���Ѵ�.
				if (1 == ProcessNetMessage()) EndNetworkProcess();

				// ������ ���ſ� ����ϴ� �޸𸮰� �ִٸ� �޸𸮸� �����Ѵ�.
				if (mp_net_body_data != NULL) {
					delete[] mp_net_body_data;
					mp_net_body_data = NULL;
				}
			} else {   // Key ���� ��ġ���� �ʾƼ� �������� ������ ���´�.
				// ������ ������ ������ �����ϰ� ������ ������ ����� ������
				// m_temp_user_data�� �����Ѵ�.
				CloseSocket(mp_net_user->h_socket, &m_temp_user_data);
				// ������ ������ Ŭ���̾�Ʈ�� �߰����� �۾��� �Ҽ� �ֵ��� �Լ� ȣ��
				CloseUserProc(&m_temp_user_data, 1);
			}
		} else {  // FD_CLOSE, ����� �� �����!
			// ������ ������ ������ �����ϰ� ������ ������ ����� ������
			// m_temp_user_data�� �����Ѵ�.
			CloseSocket((SOCKET)wParam, &m_temp_user_data);
			// ������ ������ Ŭ���̾�Ʈ�� �߰����� �۾��� �Ҽ� �ֵ��� �Լ� ȣ��
			CloseUserProc(&m_temp_user_data, 0);
		}
	}
}
// ���� ������ ����� ��ǻ�Ϳ��� ��� ������ IP ����� ��� �Լ�
int WJ_ServerSocket::GetLocalNetworkAddress(wchar_t a_ip_list[][24], int a_max_count)
{
	int count = 0;
	char host_name[128] = { 0, };
	addrinfo *result = NULL, *ptr, hints;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	gethostname(host_name, 128);  // ���� ��ǻ���� ȣ��Ʈ ��Ī�� ��´�.

	if (!getaddrinfo(host_name, NULL, &hints, &result)) {  // ȣ��Ʈ�� �ִ� IP ������ ��´�.
		ptr = result;
		unsigned char *p_pos;
		while (ptr != NULL && count < a_max_count) {
			// ������ �Ǿ� �ִ� IP �ּҸ� ���ڿ��� ��ȯ�ϱ� ���� IP�� ����� ��ġ�� �ּҸ� ��´�.
			p_pos = &((sockaddr_in *)ptr->ai_addr)->sin_addr.S_un.S_un_b.s_b1;
			// �������� IP4 ������ ���ڿ��� ��ȯ�Ѵ�.
			swprintf_s(a_ip_list[count++], 24, L"%hhu.%hhu.%hhu.%hhu", *p_pos, *(p_pos+1), *(p_pos+2), *(p_pos+3));
			// ���� IP ������ �̵��Ѵ�.
			ptr = ptr->ai_next;
		}
		freeaddrinfo(result);
	}
	return count;  // ��� IP ������ ��ȯ�Ѵ�.
}