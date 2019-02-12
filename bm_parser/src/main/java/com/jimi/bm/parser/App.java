package com.jimi.bm.parser;

import java.io.IOException;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import com.jimi.bm.parser.dao.DbConnect;
import com.jimi.bm.parser.dao.SQL;
import com.jimi.bm.parser.entity.LocationInfo;
import com.jimi.bm.parser.model.TcpPack;
import com.jimi.bm.parser.parser.PackParser;


/**
 * 启动类
 * @type App
 * @Company 几米物联技术有限公司-自动化部
 * @author 汤如杰
 * @date 2019年1月30日
 */
public class App {

	public static Map<String, LocationInfo> locationMap = new HashMap<>();


	public static void main(String[] args) {
		DbConnect.startDbConnect();
		do {
			List<TcpPack> tcpPacks = TcpPack.dao.find(SQL.GET_UNSOLVE_TCPPACK);
			for (TcpPack tcpPack : tcpPacks) {
				try {
					PackParser.parserPack(tcpPack);
				} catch (IOException e) {
					e.printStackTrace();
					System.exit(1);
				}
			}
			try {
				Thread.sleep(500);
			} catch (InterruptedException e) {
				e.printStackTrace();
				System.exit(2);
			}
		} while (true);
	}
}
