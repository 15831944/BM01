package com.jimi.bm.server.controller;

import java.io.IOException;
import java.io.OutputStream;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Date;

import javax.servlet.http.HttpServletResponse;

import com.jfinal.aop.Inject;
import com.jfinal.core.Controller;
import com.jfinal.kit.PropKit;
import com.jimi.bm.server.exception.ParameterException;
import com.jimi.bm.server.service.BusGroupService;
import com.jimi.bm.server.service.BusService;
import com.jimi.bm.server.util.ResultUtil;


public class DeviceController extends Controller {

	@Inject
	private BusService busService;

	@Inject
	private BusGroupService busGroupService;


	public void getGroups() {
		ResultUtil result = busGroupService.getGroups();
		renderJson(result);
	}


	public void getGroupDetails(Integer groupId) {
		if (groupId == null) {
			throw new ParameterException("GroupId 不能为空");
		}
		ResultUtil result = busService.getGroupDetails(groupId);
		renderJson(result);
	}


	public void getRealtimeData(String imei) {
		if (imei == null) {
			throw new ParameterException("IMEI 不能为空");
		}
		ResultUtil result = busService.getRealtimeData(imei);
		renderJson(result);
	}


	public void getIOStats(String imei, String beginTime, String endTime) {
		if (imei == null) {
			throw new ParameterException("IMEI 不能为空");
		}
		if (!isDate(beginTime) || !isDate(endTime)) {
			throw new ParameterException("日期格式不正确");
		}
		if (!checkDateRange(beginTime, endTime, Calendar.HOUR_OF_DAY, 24)) {
			throw new ParameterException("最多取一天内的客流数据，范围不正确");
		}
		ResultUtil result = busService.getIOStats(imei, beginTime, endTime);
		renderJson(result);
	}


	public void getDeviceTrack(String imei, String beginTime, String endTime, Integer pageNo, Integer pageSize) {
		if (imei == null || beginTime == null || endTime == null) {
			throw new ParameterException("参数不能为空");
		}
		if (!isDate(beginTime) || !isDate(endTime)) {
			throw new ParameterException("日期格式不正确");
		}
		if (!checkDateRange(beginTime, endTime, Calendar.MONTH, 6)) {
			throw new ParameterException("日期范围不正确");
		}
		if ((pageNo != null && pageSize == null) || (pageNo == null && pageSize != null)) {
			throw new ParameterException("pageNo和pageSize仅可同时为空或同时不为空");
		}
		if (pageNo == null && pageSize == null) {
			pageNo = 1;
			pageSize = PropKit.use("config.properties").getInt("defaultPageSize");
		}
		if (pageNo <= 0 || pageSize <= 0) {
			throw new ParameterException("pageNo和pageSize均需大于0");
		}
		ResultUtil result = busService.getDeviceTrack(imei, beginTime, endTime, pageNo, pageSize);
		renderJson(result);
	}


	private boolean isDate(String time) {
		SimpleDateFormat format = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
		try {
			format.parse(time);
			return true;
		} catch (Exception e) {
			return false;
		}
	}


	private boolean checkDateRange(String beginTime, String endTime, Integer timeUnit,Integer range) {
		SimpleDateFormat format = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
		Calendar calendar = Calendar.getInstance();
		try {
			Date time1 = format.parse(beginTime);
			Date time2 = format.parse(endTime);
			calendar.setTime(time1);
			int timeRange = calendar.get(timeUnit) + range;
			calendar.set(timeUnit, timeRange);
			int flag1 = calendar.getTime().compareTo(time2);
			int flag2 = time1.compareTo(time2);
			if (flag1 >= 0 && flag2 <= 0) {
				return true;
			} else {
				return false;
			}
		} catch (ParseException e) {
			return false;
		}

	}


	public void exportReport(String imei, String beginTime, String endTime) {
		if (imei == null || beginTime == null || endTime == null) {
			throw new ParameterException("参数不能为空");
		}
		if (!isDate(beginTime) || !isDate(endTime)) {
			throw new ParameterException("日期格式不正确");
		}
		if (!checkDateRange(beginTime, endTime, Calendar.MONTH, 6)) {
			throw new ParameterException("日期范围不正确");
		}
		OutputStream output = null;
		try {
			// 设置响应，只能在controller层设置，因为getResponse()方法只能在controller层调用
			String fileName = "location.xlsx";
			HttpServletResponse response = getResponse();
			response.reset();
			response.setHeader("Content-Disposition", "attachment; filename=" + new String((fileName).getBytes("utf-8"), "utf-8"));
			response.setContentType("application/vnd.ms-excel");
			output = response.getOutputStream();
			busService.exportReport(imei, beginTime, endTime, fileName, output);
		} catch (Exception e) {
			e.printStackTrace();
			renderJson(ResultUtil.failed());
		} finally {
			try {
				if (output != null) {
					output.close();
				}
			} catch (IOException e) {
				renderJson(ResultUtil.failed());
			}
		}
		renderNull();
	}
}
