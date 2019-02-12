package com.jimi.bm.parser.util;

import com.jimi.bm.parser.entity.Gps;


public class LocationUtil {

	private static double EARTH_RADIUS = 6378.137;


	private static double rad(double d) {
		return d * Math.PI / 180.0;
	}


	/** 
	 * 通过经纬度获取距离(单位：千米) 
	 * @param lat1 
	 * @param lng1 
	 * @param lat2 
	 * @param lng2 
	 * @return 
	 */
	public static double getDistance(Gps gps1, Gps gps2) {
		double radLat1 = rad(gps1.getWgLat());
		double radLat2 = rad(gps2.getWgLat());
		double a = radLat1 - radLat2;
		double b = rad(gps1.getWgLon()) - rad(gps2.getWgLon());
		double s = 2 * Math.asin(Math.sqrt(Math.pow(Math.sin(a / 2), 2) + Math.cos(radLat1) * Math.cos(radLat2) * Math.pow(Math.sin(b / 2), 2)));
		s = s * EARTH_RADIUS;
		s = Math.round(s * 10000d) / 10000d;
		return s;
	}


	/**
	 * 根据经纬度和时间获取速度
	 * @param gps1
	 * @param gps2
	 * @param time1
	 * @param time2
	 * @return
	 */
	public static double getSpeed(Gps gps1, Gps gps2, int time1, int time2) {
		double dis = getDistance(gps1, gps2);
		int timeInt = time2 - time1;
		if (timeInt == 0) {
			return 0;
		}
		double time = Math.abs(timeInt) / ((60 * 60 * 1000) * 1.0);
		double speed = dis / time;
		return speed;
	}
}
