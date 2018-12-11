package com.jimi.bm.server.service;

import java.util.List;

import com.jimi.bm.server.model.BusGroup;
import com.jimi.bm.server.util.ResultUtil;


public class BusGroupService {

	private final static String SELECT_BUS_GROUP_INFO = "select bus_group.id as groupId, bus_group.name as groupName, count(1) as `count` from bus_group INNER JOIN bus on bus.group_id = bus_group.id GROUP BY group_id";


	public ResultUtil getGroups() {
		List<BusGroup> busGroups = BusGroup.dao.find(SELECT_BUS_GROUP_INFO);
		return ResultUtil.succeed(busGroups);
	}
}
