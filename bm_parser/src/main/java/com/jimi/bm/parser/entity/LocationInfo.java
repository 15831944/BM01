package com.jimi.bm.parser.entity;

/**
 * 定位信息
 * @type LocationInfo
 * @Company 几米物联技术有限公司-自动化部
 * @author 汤如杰
 * @date 2019年1月30日
 */
public class LocationInfo {

	private Gps gps;

	private int locationTime;


	public LocationInfo(Gps gps, int locationTime) {
		this.gps = gps;
		this.locationTime = locationTime;
	}


	public Gps getGps() {
		return gps;
	}


	public void setGps(Gps gps) {
		this.gps = gps;
	}


	public int getLocationTime() {
		return locationTime;
	}


	public void setLocationTime(int locationTime) {
		this.locationTime = locationTime;
	}
}
