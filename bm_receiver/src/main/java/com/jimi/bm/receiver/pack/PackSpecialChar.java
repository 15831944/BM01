package com.jimi.bm.receiver.pack;

public enum PackSpecialChar {

	CILENT_START(0x79, 0x79), SERVER_START(0x78, 0x78), END(0x0D, 0x0A);

	private int[] num;


	private PackSpecialChar(int i, int j) {
		this.num = new int[] {i, j};
	}


	public int[] getNum() {
		return this.num;
	}
}
