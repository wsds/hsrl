package com.open.hsrl;

import java.util.Hashtable;
import java.util.Map;

public class Node {
	public String key;

	// public Map<String, String> perprotise = new Hashtable<String, String>();
	public Map<String, Node> children = new Hashtable<String, Node>();

	public String image;

	public class Position {
		public float x;// left
		public float y;// top
		public float z;
	}

	public Position position = new Position();

	public class Size {
		public int w;// width
		public int h;// height
		public int d;// depth
	}

	public Size size = new Size();

	String mode = "NONE";// FIT || FILL || FIT_W || FIT_H

	public Map<String, Link> links = new Hashtable<String, Link>();
	public Map<String, Interpolator> interpolators = new Hashtable<String, Interpolator>();
	
	public Boolean shown = true;
}
