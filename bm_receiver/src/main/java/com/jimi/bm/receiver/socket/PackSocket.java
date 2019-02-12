package com.jimi.bm.receiver.socket;

import java.io.IOException;
import java.net.Socket;
import java.net.SocketException;
import java.net.SocketTimeoutException;
import java.util.ArrayList;
import java.util.List;

import com.jimi.bm.receiver.parser.PackParser;

import cc.darhao.dautils.api.BytesParser;


/**
 * 包接收Socket
 * @type PackSocket
 * @Company 几米物联技术有限公司-自动化部
 * @author 汤如杰
 * @date 2019年1月30日
 */
public class PackSocket {

	private Socket socket;

	private Thread receiveThread;


	public PackSocket(Socket socket) {
		this.socket = socket;
		try {
			// 设置发送缓冲区大小
			socket.setSendBufferSize(1024 * 1024 * 10);
			// 设置接收超时时间
			socket.setSoTimeout(1800 * 1000);
		} catch (SocketException e) {
			e.printStackTrace();
		}
	}


	/**
	 * 开启接收线程
	 */
	public void startReceiveThread() {
		byte[] startChars = new byte[2];
		byte[] lengthChars = new byte[2];
		List<Byte> pack = new ArrayList<Byte>();
		receiveThread = new Thread(() -> {
			try {
				while (true) {
					pack.clear();
					if (receiveThread.isInterrupted()) {
						break;
					}
					int readCount = 0;
					int i = 0;
					// 去读开始字符
					while (readCount < startChars.length) {
						i = socket.getInputStream().read(startChars);
						if (i == -1) {
							throw new IOException("读取到结束符ff， 远程设备已断开连接");
						}
						readCount += i;
					}
					if (startChars[0] != 0x79 || startChars[1] != 0x79) {
						continue;
					}
					readCount = 0;
					pack.addAll(BytesParser.cast(startChars));
					// 读取数据包长度
					while (readCount < lengthChars.length) {
						i = socket.getInputStream().read(lengthChars);
						if (i == -1) {
							throw new IOException("读取到结束符ff， 远程设备已断开连接");
						}
						readCount += i;
					}
					int length = BytesParser.parseBytesToInteger(BytesParser.cast(lengthChars));
					pack.addAll(BytesParser.cast(lengthChars));
					byte[] content = new byte[length + 2];
					readCount = 0; // 已经成功读取的字节的个数
					while (readCount < length + 2) {
						i = socket.getInputStream().read(content, readCount, length + 2 - readCount);
						if (i == -1) {
							throw new IOException("读取到结束符ff， 远程设备已断开连接");
						}
						readCount += i;
					}
					pack.addAll(BytesParser.cast(content));
					System.out.println(BytesParser.parseBytesToHexString(pack) + " ********************");
					PackParser.parserPack(pack, this);
				}
			} catch (SocketTimeoutException e) {
				e.printStackTrace();
				stop();
			} catch (IOException e) {
				e.printStackTrace();
				stop();
			}
		});
		receiveThread.start();

	}


	/**
	 * 结束线程，关闭socket
	 */
	public void stop() {
		PackSocektServer.packSocketMap.remove(this);
		try {
			this.socket.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
		if (receiveThread != null && receiveThread.isAlive()) {
			receiveThread.interrupt();
		}

	}


	/**
	 * 发送包
	 * @param pack
	 * @throws IOException
	 */
	public void send(List<Byte> pack) throws IOException {
		byte[] temp = BytesParser.cast(pack);
		synchronized (this) {
			this.socket.getOutputStream().write(temp);
		}
	}
}
