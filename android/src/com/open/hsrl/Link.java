package com.open.hsrl;

public class Link {
	public String type;// TouchEventMove TouchEventClick.
	public Boolean active = false;

	public class Factor {
		public float x;// left
		public float y;// top
		public float z;
	}

	public Factor factor = new Factor();

}
