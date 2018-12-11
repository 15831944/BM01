package com.jimi.bm.server.entity;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import com.jfinal.plugin.activerecord.Record;


public class StationVO {

	private int stationId;

	private String stationName;

	private double lat;

	private double lng;

	private List<BusGroupVO> list;


	public int getStationId() {
		return stationId;
	}


	public void setStationId(int stationId) {
		this.stationId = stationId;
	}


	public String getStationName() {
		return stationName;
	}


	public void setStationName(String stationName) {
		this.stationName = stationName;
	}


	public double getLat() {
		return lat;
	}


	public void setLat(double lat) {
		this.lat = lat;
	}


	public double getLng() {
		return lng;
	}


	public void setLng(double lng) {
		this.lng = lng;
	}


	public List<BusGroupVO> getList() {
		return list;
	}


	public void setList(List<BusGroupVO> list) {
		this.list = list;
	}


	public static List<StationVO> fillList(List<Record> records) {
		Map<Integer, StationVO> map = new HashMap<>();
		List<StationVO> stationVOs = new ArrayList<StationVO>();
		for (Record record : records) {
			if (map.get(record.getInt("Station_Id")) == null) {
				StationVO stationVO = new StationVO();
				map.put(record.getInt("Station_Id"), stationVO);
			}
			StationVO stationVO = map.get(record.getInt("Station_Id"));
			if (stationVO.getList() == null) {
				stationVO.setStationId(record.getInt("Station_Id"));
				stationVO.setStationName(record.getStr("Station_Name"));
				stationVO.setLat(Double.valueOf(record.getStr("Station_Latitude")));
				stationVO.setLng(Double.valueOf(record.getStr("Station_Longitude")));
				List<BusGroupVO> busGroupVOs = new ArrayList<>();
				stationVO.setList(busGroupVOs);
				BusGroupVO busGroupVO = new BusGroupVO();
				busGroupVO.setGroupId(record.getInt("BusGroup_Id"));
				busGroupVO.setGroupName(record.getStr("BusGroup_Name"));
				busGroupVOs.add(busGroupVO);
			} else {
				BusGroupVO busGroupVO = new BusGroupVO();
				busGroupVO.setGroupId(record.getInt("BusGroup_Id"));
				busGroupVO.setGroupName(record.getStr("BusGroup_Name"));
				stationVO.getList().add(busGroupVO);
			}
		}
		stationVOs.addAll(map.values());
		return stationVOs;
	}
}
