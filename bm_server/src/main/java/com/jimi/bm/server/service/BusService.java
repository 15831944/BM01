package com.jimi.bm.server.service;

import java.io.IOException;
import java.io.OutputStream;
import java.util.List;

import com.jfinal.aop.Enhancer;
import com.jfinal.kit.PropKit;
import com.jfinal.plugin.activerecord.Db;
import com.jfinal.plugin.activerecord.Page;
import com.jfinal.plugin.activerecord.Record;
import com.jfinal.plugin.activerecord.SqlPara;
import com.jimi.bm.server.entity.BusAndLoactionVO;
import com.jimi.bm.server.entity.BusVO;
import com.jimi.bm.server.entity.PageUtil;
import com.jimi.bm.server.exception.OperationException;
import com.jimi.bm.server.service.base.SelectService;
import com.jimi.bm.server.util.ExcelWritter;
import com.jimi.bm.server.util.ResultUtil;


public class BusService {

	private static SelectService selectService = Enhancer.enhance(SelectService.class);

	private static String SELECT_LOCATION_INFO = "SELECT a.*, bus_flow.id AS BusFlow_Id, bus_flow.income_num AS BusFlow_IncomeNum, bus_flow.outcome_num AS BusFlow_OutcomeNum, bus_flow.sum_num AS BusFlow_SumNum FROM (SELECT location.recv_time AS Location_RecvTime,location.located_time AS Location_LocatedTime,location.report_mode_id AS Location_ReportModeId,location.latitude AS Location_Latitude,location.longitude AS Location_Longitude,location.speed AS Location_Speed,location.address AS Location_Address,location.id AS Location_Id,bus.id AS Bus_Id,bus.imei AS Bus_Imei,bus.last_ping_time AS Bus_LastPingTime,bus.plate AS Bus_Plate,bus_group.name AS BusGroup_Name,bus_group.id AS BusGroup_Id,location_type.name AS LocationType_Name,location_type.id AS LocationType_Id FROM location JOIN bus JOIN bus_group JOIN location_type ON bus.group_id = bus_group.id AND location.bus_id = bus.id AND location.location_type_id = location_type.id  WHERE bus.imei = ? AND (location.located_time BETWEEN ? AND ?)) AS a LEFT JOIN bus_flow ON a.Location_Id = bus_flow.location_id ORDER BY a.Location_LocatedTime DESC";


	public ResultUtil select(Integer groupId) {
		String[] tables = {"bus", "bus_group"};
		String[] refers = {"bus.group_id = bus_group.id"};
		String[] discards = {"bus.group_id"};
		String filter = "bus_group.id #=#" + groupId;
		Page<Record> pageRecord = selectService.select(tables, refers, null, null, null, null, filter, discards);
		List<BusVO> busVOs = BusVO.fillList(pageRecord.getList());
		return ResultUtil.succeed(busVOs);
	}


	public ResultUtil getRealtimeData(String imei) {
		String[] tables = {"bus", "bus_group", "location", "location_type"};
		String[] refers = {"bus.group_id = bus_group.id", "location.bus_id = bus.id", "location.location_type_id = location_type.id"};
		String[] discards = {"bus.group_id", "location.bus_id", "location.location_type_id"};
		String filter = "bus.imei #=#" + imei;
		String descBy = "location.located_time";
		Page<Record> pageRecord = selectService.select(tables, refers, 1, 1, null, descBy, filter, discards);
		List<BusAndLoactionVO> busAndLoactionVOs = BusAndLoactionVO.fillList(pageRecord.getList());
		if (busAndLoactionVOs.isEmpty()) {
			throw new OperationException("该车不存在定位信息");
		}
		return ResultUtil.succeed(busAndLoactionVOs.get(0));
	}


	public ResultUtil getDeviceTrack(String imei, String beginTime, String endTime, Integer pageNo, Integer pageSize) {
		SqlPara sql = new SqlPara();
		sql.setSql(SELECT_LOCATION_INFO);
		sql.addPara(imei);
		sql.addPara(beginTime);
		sql.addPara(endTime);
		Page<Record> pageRecord = Db.paginate(pageNo, pageSize, sql);
		List<BusAndLoactionVO> busAndLoactionVOs = BusAndLoactionVO.fillList(pageRecord.getList());
		PageUtil<BusAndLoactionVO> pageUtil = new PageUtil<BusAndLoactionVO>();
		pageUtil.fill(pageRecord, busAndLoactionVOs);
		return ResultUtil.succeed(pageUtil);
	}


	public void exportReport(String imei, String beginTime, String endTime, String fileName, OutputStream output) throws IOException {
		SqlPara sql = new SqlPara();
		sql.setSql(SELECT_LOCATION_INFO);
		sql.addPara(imei);
		sql.addPara(beginTime);
		sql.addPara(endTime);
		Page<Record> pageRecord = Db.paginate(1, PropKit.use("config.properties").getInt("defaultPageSize"), sql);
		String[] field = null;
		String[] head = null;
		field = new String[] {"Bus_Id", "Bus_Imei", "Bus_Plate", "BusGroup_Name", "Location_Speed", "Location_Address", "LocationType_Name", "ReportMode_Name", "Location_LocatedTime", "Location_RecvTime", "BusFlow_Id", "BusFlow_IncomeNum", "BusFlow_OutcomeNum", "BusFlow_SumNum", "Bus_LastPingTime"};
		head = new String[] {"车辆ID", "设备IMEI", "车牌号", "车辆路数", "速度", "地址", "定位类型", "上传类型", "定位时间", "接收时间", "人流量ID", "上车人数", "下车人数", "总人数", "最后通讯时间"};
		ExcelWritter writter = ExcelWritter.create(true);
		writter.fill(pageRecord.getList(), fileName, field, head);
		writter.write(output, true);
	}
	/*
	 * public static void main(String[] args) { PropKit.use("config.properties");
	 * DruidPlugin dp = new DruidPlugin(PropKit.get("d_jdbcUrl"),
	 * PropKit.get("d_user"), PropKit.get("d_password"), PropKit.get("d_driver"));
	 * ActiveRecordPlugin arp = new ActiveRecordPlugin(dp); arp.setDialect(new
	 * MysqlDialect()); // 用什么数据库，就设置什么Dialect arp.setShowSql(true);
	 * _MappingKit.mapping(arp); dp.start(); arp.start();
	 * getDeviceTrack("980182301249"); }
	 */
}
