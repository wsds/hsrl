package com.open.hsrl;

import java.util.Hashtable;
import java.util.Map;

public class Node {
	public String key;
	
	public Map<String, String> perprotise=new Hashtable<String, String>();
	public Map<String, Node> children=new Hashtable<String, Node>();
	
	
}
