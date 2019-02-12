package com.jimi.bm.receiver.pack;

public enum Pack {

	START_INDEX(0), PACK_LENGTH_INDEX(2), PROTOCOL_NUM_INDEX(4), STOP_LAST_INDEX(2), CRC_LAST_INDEX(4), SN_LAST_INDEX(6);

	private int num;


	private Pack(int i) {
		this.num = i;
	}


	public int getNum() {
		return this.num;
	}
}
