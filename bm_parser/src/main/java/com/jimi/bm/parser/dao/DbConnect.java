package com.jimi.bm.parser.dao;

import com.jfinal.kit.PropKit;
import com.jfinal.plugin.activerecord.ActiveRecordPlugin;
import com.jfinal.plugin.activerecord.dialect.MysqlDialect;
import com.jfinal.plugin.druid.DruidPlugin;
import com.jimi.bm.parser.model.Bus;
import com.jimi.bm.parser.model.BusFlow;
import com.jimi.bm.parser.model.GisCard;
import com.jimi.bm.parser.model.Location;
import com.jimi.bm.parser.model.TcpPack;


/**
 * 数据库连接类
 * @type DbConnect
 * @Company 几米物联技术有限公司-自动化部
 * @author 汤如杰
 * @date 2019年1月30日
 */
public class DbConnect {

	public static void startDbConnect() {
		DruidPlugin dp = new DruidPlugin(PropKit.use("config.properties").get("d_jdbcUrl"), PropKit.use("config.properties").get("d_user"), PropKit.use("config.properties").get("d_password"));
		ActiveRecordPlugin arp = new ActiveRecordPlugin(dp);
		arp.setDialect(new MysqlDialect());
		dp.setDriverClass(PropKit.use("config.properties").get("d_driver"));
		arp.addMapping("gis_card", GisCard.class);
		arp.addMapping("tcp_pack", TcpPack.class);
		arp.addMapping("location", Location.class);
		arp.addMapping("bus_flow", BusFlow.class);
		arp.addMapping("bus", Bus.class);
		dp.start();
		arp.start();
	}
}
