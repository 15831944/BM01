package com.jimi.bm.receiver.socket;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.concurrent.ConcurrentHashMap;


public class PackSocektServer {

	private ServerSocket serverSocket;

	public static ConcurrentHashMap<PackSocket, String> packSocketMap = new ConcurrentHashMap<>();


	public PackSocektServer(String ip, Integer port) {
		try {
			serverSocket = new ServerSocket(port);
		} catch (IOException e) {
			e.printStackTrace();
		}
	}


	public void listen() {
		while (true) {
			try {
				Socket socket = serverSocket.accept();
				PackSocket packSocket = new PackSocket(socket);
				packSocketMap.put(packSocket, "");
				packSocket.startReceiveThread();
			} catch (IOException e) {
				if (e.getMessage().equals("Socket is closed")) {
					break;
				} else {
					e.printStackTrace();
				}
			}
		}
	}

}
