#pragma once

#include "DataInRingBuffer.h"

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>

#include <thread>
#include <chrono> // NOTE!!! remove this when debug is done

#pragma comment(lib, "ws2_32.lib")

using std::cout;
using std::cerr;
using std::endl;

class DaqSignal;

class DataSender {
public:
	DataSender() : 
		socketClient(NULL), 
		isConnected(false), 
		signal(nullptr) {}

	virtual ~DataSender() {}

	/*
	int start() {
		static int countDataSent = 0;
		packet_adc_header_t header;

		char* buffer = new char[sizeof(packet_adc_header_t) + maxDataLength * sizeof(uint64_t)];

		while (countDataSent++ < 2) { // here change the number of packets to send
			const DataAdc4* ptr = nullptr;

			// fetch data from the ring buffer
			while (!queuePacket.remove(ptr)); // if false, continue requesting

			header.type = ptr->type;
			header.card = ptr->card;
			header.channel = ptr->channel;
			header.flags = ptr->flags;
			header.length = ptr->length;

			char* pbuf = buffer;
			memcpy(pbuf, &header, sizeof(packet_adc_header_t));
			pbuf += sizeof(packet_adc_header_t);
			memcpy(pbuf, ptr->data, sizeof(uint64_t) * header.length);

			int lenBuffer = sizeof(packet_adc_header_t) + sizeof(uint64_t) * header.length;
			cout << "lenBuffer: " << lenBuffer << " (" << sizeof(packet_adc_header_t)
				<< " + " << sizeof(uint64_t) << " * " << header.length << ")" << endl;

		}

		delete[] buffer;
		return 0;
	}
	*/
	void setSignal(DaqSignal* sig) {
		signal = sig;
	}

	//int start() { // NOTE!!! remove this after the debug
	//	while (signal->bRun) {
	//		//cout << "DataSender::start is running. [bRun=" << signal->bRun << "]" << endl;
	//		std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	//	}
	//	cout << "signal \"quit\" received @ DataSender." << endl;
	//	return 0;
	//}

	int attempt_to_send(bool* bSend) {
		static int countDataSent = 0;
		packet_adc_header_t header;

		if (!isConnected) {
			cout << "attemp to send, while isConnected = false"<< endl;
			return -1;
		}

		char* buffer = new char[sizeof(packet_adc_header_t) + maxDataLength * sizeof(uint64_t)];


		//while (countDataSent++ < 2) { // here change the number of packets to send


		




		while (true) {

			const DataAdc4* ptr = nullptr;

			// fetch data from the ring buffer
			while (!queuePacket.remove(ptr)) {
				if (!signal->bConnect) // when receiving signal of stop, clean up and end this sending thread
					goto end_of_sender;
			} // if false, continue requesting

			{
				header.type = ptr->type;
				header.card = ptr->card;
				header.channel = ptr->channel;
				header.flags = ptr->flags;
				header.length = ptr->length;

				char* pbuf = buffer;
				memcpy(pbuf, &header, sizeof(packet_adc_header_t));
				pbuf += sizeof(packet_adc_header_t);
				memcpy(pbuf, ptr->data, sizeof(uint64_t) * header.length);

				int lenBuffer = sizeof(packet_adc_header_t) + sizeof(uint64_t) * header.length;
				cout << "lenBuffer: " << lenBuffer << " (" << sizeof(packet_adc_header_t)
					<< " + " << sizeof(uint64_t) << " * " << header.length << ")" << endl;

				// packet_adc_print(buffer);

				int ret = send(socketClient, buffer, lenBuffer, 0);

				if (ret == SOCKET_ERROR) {
					cout << "Send message failed: countDataSent " << countDataSent << endl;
				}
				const size_t nRecvBuf = 4096;
				char recv_line[nRecvBuf];
				int recv_len = recv(socketClient, recv_line, nRecvBuf, 0);
				if (recv_len == -1) {
					cout << "Recv message error: countDataSent " << countDataSent << endl;
				}
				if (recv_len < nRecvBuf)
					recv_line[recv_len] = '\0';
				cout << "Received message: " << recv_line << endl;

			}

		}

end_of_sender:
		delete[] buffer;
		return 0;
	}

	void start(std::string ip, unsigned short port) {

		if (connect_to_server(ip, port) < 0) {
			cout << "Fail to connect to remote server. Quit current thread" << endl;
			return;
		}
		attempt_to_send(&signal->bSend);
		cout << "signal \"quit\" received @ DataSender, disconnect_from_server" << endl;
		disconnect_from_server();
	}

	inline bool connected() const { return isConnected; }
	
	int connect_to_server(std::string ip, unsigned short port) {

		cout << "Connecting to " << ip << " : " << port << " ..." << endl;

		WSADATA s;
		if (WSAStartup(MAKEWORD(2, 2), &s) != 0) {
			cout << "Init Windows socket failed! Error: " << GetLastError() << endl;
			return -1;
		}
		cout << "Initialized socket with WSAStartup" << endl;

		socketClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (socketClient == INVALID_SOCKET) {
			cout << "Create socket failed! Error: " << GetLastError() << endl;
			return -1;
		}
		cout << "Created socket" << endl;

		sockaddr_in addrClient;
		addrClient.sin_family = AF_INET;
		inet_pton(AF_INET, ip.c_str(), &addrClient.sin_addr);
		addrClient.sin_port = htons(port);
		memset(addrClient.sin_zero, 0x00, 8);

		int ret = connect(socketClient, (sockaddr*)&addrClient, sizeof(sockaddr));
		if (ret == SOCKET_ERROR) {
			cout << "Socket connect failed! Error: " << GetLastError() << endl;
			return -1;
		}
		cout << "Socket successfully connected to server " << endl;
		isConnected = true;
		return 0;
	}

	int disconnect_from_server() {
		closesocket(socketClient);
		cout << "Disconnect from server" << endl;
		isConnected = false;
		WSACleanup();
		return 0;
	}

private:

	SOCKET socketClient;
	bool isConnected;

	DaqSignal* signal;
};