package com.jimi.bm.server.entity;

import java.util.ArrayList;
import java.util.Calendar;
import java.util.Date;
import java.util.List;

import com.jfinal.plugin.activerecord.Record;


public class BusVO {

	private Integer id;

	private String imei;

	private String numPlate;

	private Integer groupId;

	private String groupName;

	private Integer status;

	private Date lastPingTime;


	public Integer getId() {
		return id;
	}


	public void setId(Integer id) {
		this.id = id;
	}


	public String getImei() {
		return imei;
	}


	public void setImei(String imei) {
		this.imei = imei;
	}


	public String getNumPlate() {
		return numPlate;
	}


	public void setNumPlate(String numPlate) {
		this.numPlate = numPlate;
	}


	public Integer getGroupId() {
		return groupId;
	}


	public void setGroupId(Integer groupId) {
		this.groupId = groupId;
	}


	public String getGroupName() {
		return groupName;
	}


	public void setGroupName(String groupName) {
		this.groupName = groupName;
	}


	public Integer getStatus() {
		return status;
	}


	public void setStatus(Integer status) {
		this.status = status;
	}


	public Date getLastPingTime() {
		return lastPingTime;
	}


	public void setLastPingTime(Date lastPingTime) {
		this.lastPingTime = lastPingTime;
	}


	public static List<BusVO> fillList(List<Record> records) {
		Calendar calendar = Calendar.getInstance();
		List<BusVO> busVOs = new ArrayList<BusVO>();
		for (Record record : records) {
			BusVO busVO = new BusVO();
			busVO.setId(record.getInt("Bus_Id"));
			busVO.setImei(record.getStr("Bus_Imei"));
			busVO.setNumPlate(record.getStr("Bus_Plate"));
			busVO.setGroupId(record.getInt("BusGroup_Id"));
			busVO.setGroupName(record.getStr("BusGroup_Name"));
			Date time = record.getDate("Bus_LastPingTime");
			busVO.setLastPingTime(time);
			if (time != null) {
				Date currentTime = new Date(System.currentTimeMillis());
				calendar.setTime(time);
				int min = calendar.get(Calendar.MINUTE) + 2;
				calendar.set(Calendar.MINUTE, min);
				Date tempTime = calendar.getTime();
				if (tempTime.after(currentTime) || tempTime.equals(currentTime)) {
					busVO.setStatus(1);
				} else {
					busVO.setStatus(0);
				}
			} else {
				busVO.setStatus(0);
			}
			busVOs.add(busVO);
		}
		return busVOs;
	}
}
