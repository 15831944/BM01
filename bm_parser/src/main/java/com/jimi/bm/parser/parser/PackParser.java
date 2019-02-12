package com.jimi.bm.parser.parser;

import java.io.IOException;
import java.util.Calendar;
import java.util.Date;
import java.util.List;

import cc.darhao.dautils.api.BytesParser;

import com.alibaba.fastjson.JSONObject;
import com.jimi.bm.parser.App;
import com.jimi.bm.parser.dao.SQL;
import com.jimi.bm.parser.entity.AddressRequestUrl;
import com.jimi.bm.parser.entity.Gps;
import com.jimi.bm.parser.entity.LocationInfo;
import com.jimi.bm.parser.model.Bus;
import com.jimi.bm.parser.model.BusFlow;
import com.jimi.bm.parser.model.Location;
import com.jimi.bm.parser.model.TcpPack;
import com.jimi.bm.parser.pack.Model;
import com.jimi.bm.parser.util.HttpHelper;
import com.jimi.bm.parser.util.LocationUtil;
import com.jimi.bm.parser.util.PositionUtil;


/**
 * 包解析类
 * @type PackParser
 * @Company 几米物联技术有限公司-自动化部
 * @author 汤如杰
 * @date 2019年1月30日
 */
public class PackParser {

	public static void parserPack(TcpPack tcpPack) throws IOException {
		List<Byte> pack = BytesParser.parseHexStringToBytes(tcpPack.getContent());
		// 获取IMEI
		String imei = tcpPack.getImei();
		// 获取公交车ID
		Bus bus = Bus.dao.findFirst(SQL.GET_BUS_BY_IMEI, imei);
		Integer busId = bus.getId();
		// 获取整个包长度
		int size = pack.size();
		// 第一个模块的索引值
		int index = 5;
		// 插入数据库的定位包ID
		int locationId = 0;
		Calendar calendar = Calendar.getInstance();
		// 判断数据包的包头，包尾
		do {
			// 获取模块号第一个字节
			int model1 = pack.get(index);
			// 获取模块号第二个字节
			int model2 = pack.get(index + 1);
			// 获取模块长度
			int length = BytesParser.parseBytesToInteger(pack.subList(index + 2, index + 4));
			// 判断模块类型
			if (model1 == Model.LOCATION.getNum()[0] && model2 == Model.LOCATION.getNum()[1]) {
				// 定位模块
				List<Byte> locationPack = pack.subList(index + 4, index + 4 + length);
				Integer timeInt = BytesParser.parseBytesToInteger(locationPack.subList(0, 4));
				calendar.setTimeInMillis(timeInt);
				Date time = calendar.getTime();
				// 解析经纬度
				Double latitude = BytesParser.parseBytesToInteger(locationPack.subList(7, 11)) / 1800000.0;
				Double longitude = BytesParser.parseBytesToInteger(locationPack.subList(11, 15)) / 1800000.0;
				// 解析速度
				int speed = BytesParser.parseBytesToInteger(locationPack.subList(15, 16));
				// 经纬度转码
				Gps gps = PositionUtil.gps84_To_Gcj02(latitude.doubleValue(), longitude.doubleValue());
				if (gps != null) {
					// 如果为第一次解析定位包，则将当前定位信息存入缓存中
					if (App.locationMap.get(imei) == null) {
						LocationInfo info = new LocationInfo(gps, timeInt);
						App.locationMap.put(imei, info);
					}
					// 计算上次定位包和这次定位包的两点间的平均速度
					double calSpeed = LocationUtil.getSpeed(App.locationMap.get(imei).getGps(), gps, App.locationMap.get(imei).getLocationTime(), timeInt);
					if (calSpeed > 120) {
						// 速度大于120km/h，跳过该定位包
						break;
					}
					// 将本次定位信息写入缓存中
					LocationInfo info = new LocationInfo(gps, timeInt);
					App.locationMap.put(imei, info);
					// 使用高德API将经纬度转化成中文地址
					AddressRequestUrl requestUrl = new AddressRequestUrl();
					requestUrl.add("key", "27639182fe1124cdc87198b5e6b7940f");
					requestUrl.add("location", longitude.toString() + "," + latitude.toString());
					requestUrl.add("radius", "100");
					requestUrl.add("output", "JSON");
					String result = HttpHelper.getRequestHttp(requestUrl.toString());
					JSONObject jsonObject = JSONObject.parseObject(result);
					JSONObject regeocode = (JSONObject) jsonObject.get("regeocode");
					// 获取格式化地址
					String formatAddress = regeocode.getString("formatted_address");
					// 写入数据库
					Location location = new Location().setBusId(busId).setSpeed(speed).setAddress("").setLatitude(latitude.toString()).setLongitude(longitude.toString()).setLocatedTime(time).setAmapLatitude(String.valueOf(gps.getWgLat())).setAmapLongitude(String.valueOf(gps.getWgLon())).setAmapAddress(formatAddress).setRecvTime(tcpPack.getReceiveTime()).setLocationTypeId(1);
					location.save();
					// 将定位id赋值
					locationId = location.getId();
					System.out.println(latitude + "|" + longitude);
					System.out.println(time + " | " + "lat: " + gps.getWgLat() + " | long: " + gps.getWgLon() + " | speed: " + speed + "| formatAddress: " + formatAddress);
				}
			} else if (model1 == Model.FLOW.getNum()[0] && model2 == Model.FLOW.getNum()[1]) {
				// 人流包
				List<Byte> flowPack = pack.subList(index + 4, index + 4 + length);
				// 解析门编号
				int gateNum = flowPack.get(0).intValue();
				// 解析剩余人数
				int sum = BytesParser.parseBytesToInteger(flowPack.subList(1, 3));
				// 解析上车人数
				int incomeNum = BytesParser.parseBytesToInteger(flowPack.subList(3, 5));
				// 解析下车人数
				int outcomeNum = BytesParser.parseBytesToInteger(flowPack.subList(5, 7));
				// 写入数据库
				BusFlow busFlow = new BusFlow();
				busFlow.setBusId(busId).setIncomeNum(incomeNum).setOutcomeNum(outcomeNum).setLocationId(locationId).setSumNum(sum).setGateNum(gateNum).save();
				System.out.println("gateNum: " + gateNum + " | " + "sum: " + sum + " | income: " + incomeNum + " | outcome: " + outcomeNum);
			}
			index = index + 4 + length;
		} while (index < size - 6);
		//设置tcp包为已解析状态
		tcpPack.setStatus(0).update();
	}

}
