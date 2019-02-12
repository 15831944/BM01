package com.jimi.bm.parser.entity;

/**
 * Gps定位经纬度
 * @type Gps
 * @Company 几米物联技术有限公司-自动化部
 * @author 汤如杰
 * @date 2019年1月30日
 */
public class Gps {

	private double wgLat;
	private double wgLon;


	public Gps(double wgLat, double wgLon) {
		setWgLat(wgLat);
		setWgLon(wgLon);
	}


	public double getWgLat() {
		return wgLat;
	}


	public void setWgLat(double wgLat) {
		this.wgLat = wgLat;
	}


	public double getWgLon() {
		return wgLon;
	}


	public void setWgLon(double wgLon) {
		this.wgLon = wgLon;
	}


	@Override
	public String toString() {
		return wgLat + "," + wgLon;
	}
}
