package com.jimi.bm.server.entity;

import java.util.ArrayList;
import java.util.Calendar;
import java.util.Date;
import java.util.List;

import com.jfinal.plugin.activerecord.Record;


public class BusAndLoactionVO {

	private Integer id;

	private String imei;

	private String numPlate;

	private Integer groupId;

	private String groupName;

	private Integer status;

	private Date communicatedTime;

	private Integer speed;

	private String statusString;

	private Date locatedTime;

	private Integer locationType;

	private String locationTypeString;

	private String location;

	private String type;

	private Integer reportMode;

	private String reportModeString;

	private Date recvTime;

	private Integer upCount;

	private Integer downCount;

	private Integer total;

	private Double lng;
	
	private Double lat;
	
	
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
	

	public Integer getSpeed() {
		return speed;
	}


	public void setSpeed(Integer speed) {
		this.speed = speed;
	}


	public String getStatusString() {
		return statusString;
	}


	public void setStatusString(String statusString) {
		this.statusString = statusString;
	}


	public Integer getLocationType() {
		return locationType;
	}


	public void setLocationType(Integer locationType) {
		this.locationType = locationType;
	}


	public String getLocationTypeString() {
		return locationTypeString;
	}


	public void setLocationTypeString(String locationTypeString) {
		this.locationTypeString = locationTypeString;
	}

	
	public String getLocation() {
		return location;
	}


	
	public void setLocation(String location) {
		this.location = location;
	}


	public String getType() {
		return type;
	}


	public void setType(String type) {
		this.type = type;
	}


	public Integer getReportMode() {
		return reportMode;
	}


	public void setReportMode(Integer reportMode) {
		this.reportMode = reportMode;
	}


	public String getReportModeString() {
		return reportModeString;
	}


	public void setReportModeString(String reportModeString) {
		this.reportModeString = reportModeString;
	}


	public Date getRecvTime() {
		return recvTime;
	}


	public void setRecvTime(Date recvTime) {
		this.recvTime = recvTime;
	}


	public Integer getUpCount() {
		return upCount;
	}


	public void setUpCount(Integer upCount) {
		this.upCount = upCount;
	}


	public Integer getDownCount() {
		return downCount;
	}


	public void setDownCount(Integer downCount) {
		this.downCount = downCount;
	}


	public Integer getTotal() {
		return total;
	}


	public void setTotal(Integer total) {
		this.total = total;
	}


	
	public Date getCommunicatedTime() {
		return communicatedTime;
	}


	
	public void setCommunicatedTime(Date communicatedTime) {
		this.communicatedTime = communicatedTime;
	}


	
	public Date getLocatedTime() {
		return locatedTime;
	}


	
	public void setLocatedTime(Date locatedTime) {
		this.locatedTime = locatedTime;
	}
	
	
	public Double getLng() {
		return lng;
	}


	
	public void setLng(Double lng) {
		this.lng = lng;
	}


	
	public Double getLat() {
		return lat;
	}


	
	public void setLat(Double lat) {
		this.lat = lat;
	}


	public static List<BusAndLoactionVO> fillList(List<Record> records) {
		Calendar calendar = Calendar.getInstance();
		List<BusAndLoactionVO> busAndLoactionVOs = new ArrayList<BusAndLoactionVO>();
		for (Record record : records) {
			BusAndLoactionVO busAndLoactionVO = new BusAndLoactionVO();
			busAndLoactionVO.setId(record.getInt("Bus_Id"));
			busAndLoactionVO.setImei(record.getStr("Bus_Imei"));
			busAndLoactionVO.setNumPlate(record.getStr("Bus_Plate"));
			busAndLoactionVO.setGroupId(record.getInt("BusGroup_Id"));
			busAndLoactionVO.setGroupName(record.getStr("BusGroup_Name"));
			busAndLoactionVO.setSpeed(record.getInt("Location_Speed"));
			busAndLoactionVO.setLocation(record.getStr("Location_Address"));
			busAndLoactionVO.setLocationType(record.getInt("LocationType_Id"));
			busAndLoactionVO.setLocationTypeString(record.getStr("LocationType_Name"));
			busAndLoactionVO.setLocatedTime(record.getDate("Location_LocatedTime"));
			busAndLoactionVO.setReportMode(record.getInt("ReportMode_Id"));
			busAndLoactionVO.setReportModeString(record.getStr("ReportMode_Name"));
			busAndLoactionVO.setRecvTime(record.getDate("Location_RecvTime"));
			busAndLoactionVO.setLat(Double.valueOf(record.getStr("Location_Latitude")));
			busAndLoactionVO.setLng(Double.valueOf(record.getStr("Location_Longitude")));
			if (record.getInt("BusFlow_Id") == null) {
				busAndLoactionVO.setType("normal");
			} else {
				busAndLoactionVO.setType("busStop");
				busAndLoactionVO.setUpCount(record.getInt("BusFlow_IncomeNum"));
				busAndLoactionVO.setDownCount(record.getInt("BusFlow_OutcomeNum"));
				busAndLoactionVO.setTotal(record.getInt("BusFlow_SumNum"));
			}
			Date time = record.getDate("Bus_LastPingTime");
			busAndLoactionVO.setCommunicatedTime(time);
			if (time != null) {
				Date currentTime = new Date(System.currentTimeMillis());
				calendar.setTime(time);
				int min = calendar.get(Calendar.MINUTE) + 2;
				calendar.set(Calendar.MINUTE, min);
				Date tempTime = calendar.getTime();
				if (tempTime.after(currentTime) || tempTime.equals(currentTime)) {
					busAndLoactionVO.setStatus(1);
					busAndLoactionVO.setStatusString("正常运行");
				} else {
					busAndLoactionVO.setStatus(0);
					busAndLoactionVO.setStatusString("静止");
				}
			} else {
				busAndLoactionVO.setStatus(0);
				busAndLoactionVO.setStatusString("静止");
			}
			busAndLoactionVOs.add(busAndLoactionVO);
		}
		return busAndLoactionVOs;
	}
}
