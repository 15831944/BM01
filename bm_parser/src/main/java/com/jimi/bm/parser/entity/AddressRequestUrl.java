package com.jimi.bm.parser.entity;

import java.util.ArrayList;
import java.util.List;


public class AddressRequestUrl {

	// 非线程安全
	private StringBuilder URL = new StringBuilder();

	List<String> params = new ArrayList<String>();


	public AddressRequestUrl() {
		URL.append("https://restapi.amap.com/v3/geocode/regeo");
	}


	public void add(String key, String value) {
		String param = key + "=" + value;
		params.add(param);
	}


	@Override
	public String toString() {
		URL.append("?");
		for (String param : params) {
			URL.append(param);
			URL.append("&");
		}
		URL.deleteCharAt(URL.length() - 1);
		return URL.toString();

	}
}
