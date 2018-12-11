package com.jimi.bm.server.entity;

import java.util.List;

import com.jfinal.plugin.activerecord.Page;
import com.jfinal.plugin.activerecord.Record;


public class PageUtil<T> {

	private Integer totalPage;

	private Integer totalRow;

	private Integer pageNumber;

	private Integer pageSize;

	private Boolean lastPage;

	private Boolean firstPage;

	private List<T> list;


	public Integer getPageNumber() {
		return pageNumber;
	}


	public void setPageNumber(Integer pageNumber) {
		this.pageNumber = pageNumber;
	}


	public Integer getPageSize() {
		return pageSize;
	}


	public void setPageSize(Integer pageSize) {
		this.pageSize = pageSize;
	}


	public Integer getTotalPage() {
		return totalPage;
	}


	public void setTotalPage(Integer totallyPage) {
		this.totalPage = totallyPage;
	}


	public Integer getTotalRow() {
		return totalRow;
	}


	public void setTotalRow(Integer totalRow) {
		this.totalRow = totalRow;
	}


	public List<T> getList() {
		return list;
	}


	public void setList(List<T> list) {
		this.list = list;
	}


	public Boolean getLastPage() {
		return lastPage;
	}


	public void setLastPage(Boolean lastPage) {
		this.lastPage = lastPage;
	}


	public Boolean getFirstPage() {
		return firstPage;
	}


	public void setFirstPage(Boolean firstPage) {
		this.firstPage = firstPage;
	}


	public void fill(Page<Record> pageRecord, List<T> list) {
		this.setPageNumber(pageRecord.getPageNumber());
		this.setPageSize(pageRecord.getPageSize());
		this.setTotalRow(pageRecord.getTotalRow());
		this.setTotalPage(pageRecord.getTotalPage());
		this.setFirstPage(pageRecord.isFirstPage());
		this.setLastPage(pageRecord.isLastPage());
		this.setList(list);
	}
}
