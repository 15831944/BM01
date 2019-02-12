package com.jimi.bm.receiver;

import com.jimi.bm.receiver.dao.DbConnect;
import com.jimi.bm.receiver.socket.PackSocektServer;


/**
 * 启动类
 * @type App
 * @Company 几米物联技术有限公司-自动化部
 * @author 汤如杰
 * @date 2019年1月30日
 */
public class App {

	public static void main(String[] args) {
		DbConnect.startDbConnect();
		PackSocektServer packSocektServer = new PackSocektServer("127.0.0.1", 6888);
		packSocektServer.listen();
		while (true) {
			try {
				Thread.sleep(500);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
	}
}
