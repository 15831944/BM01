package com.jimi.bm.server.controller;

import com.jfinal.aop.Inject;
import com.jfinal.core.Controller;
import com.jimi.bm.server.service.StationService;
import com.jimi.bm.server.util.ResultUtil;


public class StationController extends Controller {

	@Inject
	private StationService stationService;


	public void getStations() {
		ResultUtil result = stationService.getStations();
		renderJson(result);
	}
}
