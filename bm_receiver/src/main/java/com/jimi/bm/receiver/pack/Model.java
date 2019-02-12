package com.jimi.bm.receiver.pack;

public enum Model {

	TRANSPORT_TYPE(0x00, 0x00), LOCATION(0x00, 0x33), EVENT(0x00, 0x1F), IMEI(0x00, 0x01), CARD_ID(0x00, 0x37);

	private int[] num;


	private Model(int i, int j) {
		this.num = new int[] {i, j};
	}


	public int[] getNum() {
		return this.num;
	}
}
