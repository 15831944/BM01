package com.jimi.bm.receiver.parser;

import java.io.IOException;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.Date;
import java.util.List;

import com.jimi.bm.receiver.dao.SQL;
import com.jimi.bm.receiver.model.Bus;
import com.jimi.bm.receiver.model.GisCard;
import com.jimi.bm.receiver.model.TcpPack;
import com.jimi.bm.receiver.pack.Model;
import com.jimi.bm.receiver.pack.PackSpecialChar;
import com.jimi.bm.receiver.pack.Protocol;
import com.jimi.bm.receiver.socket.PackSocektServer;
import com.jimi.bm.receiver.socket.PackSocket;
import com.jimi.bm.receiver.util.CRCUtil;

import cc.darhao.dautils.api.BytesParser;
import cc.darhao.dautils.api.CRC16Util;;


/**
 * 数据包解析类
 * @type PackParser
 * @Company 几米物联技术有限公司-自动化部
 * @author 汤如杰
 * @date 2019年1月30日
 */
public class PackParser {

	private static String REPLY_CARDID_PACK = "79 79 00 14 70 00 00 00 01 05 00 20 00 06 01 53 4A 51 4C 23 00 01 4B C4 0D 0A";
	private static List<Byte> replyCardIdBytes;
	private static int count = 0;
	static {
		replyCardIdBytes = BytesParser.parseHexStringToBytes(REPLY_CARDID_PACK);
	}


	public static void parserPack(List<Byte> pack, PackSocket packSocket) throws IOException {
		int size = pack.size();
		int index = 5;
		int baseLen = 5;
		int status = 0;
		String imei = "";
		TcpPack tcpPack = new TcpPack();
		Calendar calendar = Calendar.getInstance();
		Date receiveTime = calendar.getTime();
		tcpPack.setReceiveTime(receiveTime);
		// 判断数据包的包头，包尾
		if (pack.get(0).intValue() == PackSpecialChar.CILENT_START.getNum()[0] && pack.get(1).intValue() == PackSpecialChar.CILENT_START.getNum()[1] && pack.get(size - 2) == PackSpecialChar.END.getNum()[0] && pack.get(size - 1) == PackSpecialChar.END.getNum()[1] && pack.get(4) == Protocol.PROTOCOL_1.getNum()) {
			short crc = (short) BytesParser.parseBytesToInteger(pack.subList(size - 4, size - 2));
			// 判断数据包的CRC
			if (crc == CRCUtil.CRC16_X25(pack.subList(2, size - 4))) {
				do {
					// 获取模块号第一个字节
					int model1 = pack.get(index);
					// 获取模块号第二个字节
					int model2 = pack.get(index + 1);
					// 获取模块长度
					int length = BytesParser.parseBytesToInteger(pack.subList(index + 2, index + 4));
					// 判断模块类型
					if (model1 == Model.TRANSPORT_TYPE.getNum()[0] && model2 == Model.TRANSPORT_TYPE.getNum()[1]) {
						// 传输类型模块
						// 创建回复包
						List<Byte> reply = new ArrayList<Byte>();
						// 回复包长度
						int len = baseLen + length + 4;
						List<Byte> content = new ArrayList<>();
						List<Byte> lens = BytesParser.parseIntegerToBytes(len);
						if (lens.size() == 1) {
							content.add((byte) 0);
						}
						// 填充回复包
						content.addAll(lens.subList(lens.size() - 1, lens.size()));
						content.addAll(BytesParser.parseIntegerToBytes(Protocol.PROTOCOL_1.getNum()));
						content.addAll(pack.subList(index, index + length + 4));
						content.addAll(pack.subList(size - 6, size - 4));
						byte[] replyCrc = ParserUtil.shortToByteArray(CRC16Util.CRC16_X25(content));
						reply.addAll(BytesParser.parseHexStringToBytes("79 79"));
						reply.addAll(content);
						reply.addAll(BytesParser.cast(replyCrc));
						reply.addAll(BytesParser.parseHexStringToBytes("0D 0A"));
						packSocket.send(reply);
						// 保存发送包，status = 7
						new TcpPack().setImei(imei).setContent(BytesParser.parseBytesToHexString(reply)).setReceiveTime(new Date()).setStatus(7);
						System.out.println(reply.toString() + " | " + count++);
					} else if (model1 == Model.IMEI.getNum()[0] && model2 == Model.IMEI.getNum()[1]) {
						// 解析IMEI
						imei = BytesParser.parseBytesToHexString(pack.subList(index + 4, index + 4 + length)).replace(" ", "");
						PackSocektServer.packSocketMap.put(packSocket, imei);
					} else if (model1 == Model.CARD_ID.getNum()[0] && model2 == Model.CARD_ID.getNum()[1]) {

						// 解析卡ID
						String cardId = BytesParser.parseBytesToHexString(pack.subList(index + 4, index + 4 + length)).replace(" ", "");
						calendar.set(Calendar.HOUR, 0);
						calendar.set(Calendar.MINUTE, 0);
						calendar.set(Calendar.SECOND, 0);
						calendar.set(Calendar.MILLISECOND, 0);
						Date time1 = calendar.getTime();
						calendar.add(Calendar.MONTH, 1);
						Date time2 = calendar.getTime();
						// 查询当天是否第一次打卡
						GisCard gisCard = GisCard.dao.findFirst(SQL.GET_GISCARD_BY_CARDID_AND_TIME, cardId, time1, time2);
						if (gisCard == null) {
							// 第一次打卡，则发送回复包
							gisCard = new GisCard();
							gisCard.setImei(imei).setReceiveTime(receiveTime).setCardId(cardId).save();
							packSocket.send(replyCardIdBytes);
							new TcpPack().setImei(imei).setContent(REPLY_CARDID_PACK).setReceiveTime(new Date()).setStatus(7);
						}
					}
					index = index + 4 + length;
				} while (index < size - 6);

				imei = PackSocektServer.packSocketMap.get(packSocket);
				if (imei.equals("") || imei == null) {
					// 格式正确，校验成功，imei不存在
					status = 2;
				} else {
					// 格式正确，校验成功，imei存在
					status = 1;
				}
			} else {
				imei = PackSocektServer.packSocketMap.get(packSocket);
				if (imei.equals("") || imei == null) {
					// 格式正确，校验不成功，imei不存在
					status = 4;
				} else {
					// 格式正确，校验不成功，imei存在
					status = 3;
				}
			}
		} else {
			imei = PackSocektServer.packSocketMap.get(packSocket);
			if (imei.equals("") || imei == null) {
				// 格式不正确，imei不存在
				status = 6;
			} else {
				// 格式不正确，imei存在
				status = 5;
			}
		}
		if (imei != null && !imei.equals("")) {
			Bus bus = Bus.dao.findFirst(SQL.GET_BUS_BY_IMEI, imei);
			bus.setLastPingTime(receiveTime).update();
		}
		// 存储接收包
		tcpPack.setImei(imei).setStatus(status).setContent(BytesParser.parseBytesToHexString(pack)).save();
	}

}
