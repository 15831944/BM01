package com.jimi.bm.parser.dao;


/**
 * sql语句
 * @type SQL
 * @Company 几米物联技术有限公司-自动化部
 * @author 汤如杰
 * @date 2019年1月30日
 */
public class SQL {

	//根据cardID和时间查询记录
	public static String GET_GISCARD_BY_CARDID_AND_TIME = "SELECT * FROM gis_card WHERE card_id = ? AND (receive_time >= ? and receive_time < ?)";

	//获取未解析的包
	public static String GET_UNSOLVE_TCPPACK = "SELECT * FROM tcp_pack WHERE status = 1 LIMIT 0, 10";

	//根据imei查询公交车ID
	public static String GET_BUS_BY_IMEI = "SELECT * FROM bus WHERE imei = ?";
}
