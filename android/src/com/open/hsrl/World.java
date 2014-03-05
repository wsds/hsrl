package com.open.hsrl;

import java.util.Hashtable;
import java.util.Map;

public class World {
	
	public static World world;

	public static World getInstance() {
		if (world == null) {
			world = new World();
		}
		return world;
	}

	public Map<String, Node>children=new Hashtable<String, Node>();
}
