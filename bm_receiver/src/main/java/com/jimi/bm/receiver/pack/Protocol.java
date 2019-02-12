package com.jimi.bm.receiver.pack;

public enum Protocol {

	PROTOCOL_1(0x70);

	private int num;


	private Protocol(int i) {
		this.num = i;
	}


	public int getNum() {
		return this.num;
	}
}
