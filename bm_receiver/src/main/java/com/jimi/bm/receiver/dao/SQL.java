package com.jimi.bm.receiver.dao;

public class SQL {

	// 根据cardID和时间查询记录
	public static String GET_GISCARD_BY_CARDID_AND_TIME = "SELECT * FROM gis_card WHERE card_id = ? AND (receive_time >= ? and receive_time < ?)";
	//根据imei查询公交车ID
		public static String GET_BUS_BY_IMEI = "SELECT * FROM bus WHERE imei = ?";
}
