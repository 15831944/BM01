package com.jimi.bm.server.service;

import java.util.List;

import com.jfinal.aop.Enhancer;
import com.jfinal.plugin.activerecord.Page;
import com.jfinal.plugin.activerecord.Record;
import com.jimi.bm.server.entity.StationVO;
import com.jimi.bm.server.service.base.SelectService;
import com.jimi.bm.server.util.ResultUtil;


public class StationService extends SelectService {

	private static SelectService selectService = Enhancer.enhance(SelectService.class);


	public ResultUtil getStations() {
		String[] tables = {"station", "bus_group", "station_group_bind"};
		String[] refers = {"station.id = station_group_bind.station_id", "bus_group.id = station_group_bind.group_id"};
		String[] discard = {"station_group_bind.station_id", "station_group_bind.group_id"};
		Page<Record> pageRecord = selectService.select(tables, refers, null, null, null, null, null, discard);
		List<StationVO> stationVOs = StationVO.fillList(pageRecord.getList());
		return ResultUtil.succeed(stationVOs);
	}

}
