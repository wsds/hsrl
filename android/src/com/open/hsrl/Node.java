package com.open.hsrl;

import java.util.Hashtable;
import java.util.Map;

public class Node {
	public String key;

	public Map<String, String> perprotise = new Hashtable<String, String>();
	public Map<String, Node> children = new Hashtable<String, Node>();

	public String image;

	public class Position {
		public int x;//left
		public int y;//top
		public int z;
	}
	public Position position=new Position();

	public class Size {
		public int w;// width
		public int h;// height
		public int d;// depth
	}
	public Size size=new Size();
	
	String mode="NONE";//FIT || FILL || FIT_W || FIT_H

}
