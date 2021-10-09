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
	DataSender() : socketClient(NULL), bConnected(false) {}
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

	int start() { // NOTE!!! remove this after the debug
		while (signal->bRun) {
			//cout << "DataSender::start is running. [bRun=" << signal->bRun << "]" << endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(2000));
		}
		cout << "signal \"quit\" received @ DataSender." << endl;
		return 0;
	}

/* NOTE!!! uncomment this if the debug for thread is done!
	int start() {
		static int countDataSent = 0;
		packet_adc_header_t header;

		char* buffer = new char[sizeof(packet_adc_header_t) + maxDataLength * sizeof(uint64_t)];

		if (connect_to_server() < 0) {
			cerr << "Fail to connect to remote server. Quit current thread" << endl;
			getchar();
			return -1;
		}

		//while (countDataSent++ < 2) { // here change the number of packets to send
		while (signal->bRun || !queuePacket.isEmpty()) {
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

			// packet_adc_print(buffer);

			int ret = send(socketClient, buffer, lenBuffer, 0);

			if (ret == SOCKET_ERROR) {
				cerr << "Send message failed: countDataSent " << countDataSent << endl;
			}
			char recv_line[4096];
			int recv_len = recv(socketClient, recv_line, 4096, 0);
			if (recv_len == -1) {
				cerr << "Recv message error: countDataSent " << countDataSent << endl;
			}
			if (recv_len < 4096)
				recv_line[recv_len] = '\0';
			cout << "Received message: " << recv_line << endl;



		}
		cout << "signal \"quit\" received @ DataSender." << endl;
		disconnect_from_server();
		delete[] buffer;
		return 0;
	}
*/
	
private:
	int connect_to_server() {
		// const char* ip = "10.2.2.179";
		const char* ip = "10.11.1.11";
		const u_short port = 8080;

		WSADATA s;
		if (WSAStartup(MAKEWORD(2, 2), &s) != 0) {
			cerr << "Init Windows socket failed! Error: " << GetLastError() << endl;
			return -1;
		}
		socketClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (socketClient == INVALID_SOCKET) {
			cerr << "Create socket failed! Error: " << GetLastError() << endl;
			return -1;
		}
		sockaddr_in addrClient;
		addrClient.sin_family = AF_INET;
		inet_pton(AF_INET, ip, &addrClient.sin_addr);
		addrClient.sin_port = htons(port);
		memset(addrClient.sin_zero, 0x00, 8);

		int ret = connect(socketClient, (sockaddr*)&addrClient, sizeof(sockaddr));
		if (ret == SOCKET_ERROR) {
			cerr << "Socket connect failed! Error: " << GetLastError() << endl;
			return -1;
		}
		else {
			cout << "Socket successfully connected to server " << endl;
			bConnected = true;
			return 0;
		}
	}
	int disconnect_from_server() {
		closesocket(socketClient);
		cout << "Disconnect from server" << endl;
		bConnected = false;
		WSACleanup();
		return 0;
	}

	SOCKET socketClient;
	bool bConnected;

	DaqSignal* signal;
};