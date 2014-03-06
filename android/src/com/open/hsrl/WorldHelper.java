package com.open.hsrl;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.util.Iterator;

import org.json.JSONException;
import org.json.JSONObject;

import android.content.Context;

public class WorldHelper {

	public WorldHelper(Context context) {
		this.context = context;
	}

	public static WorldHelper helper;

	public static WorldHelper getInstance() {
		if (helper == null) {
			helper = new WorldHelper(null);
		}
		return helper;
	}

	public Context context;

	World parseWorld(JSONObject jWorld) {
		World world = new World();

		try {

			@SuppressWarnings("unchecked")
			Iterator<String> keys = jWorld.keys();

			String key;
			JSONObject jNode;
			Node node;

			while (keys.hasNext()) {
				key = keys.next();
				if (key.equals("properties") || key.equals("keys")) {
					continue;
				}
				jNode = jWorld.getJSONObject(key);
				node = parseNode(key, jNode);
				world.children.put(key, node);
			}
		} catch (JSONException e) {
			e.printStackTrace();
		}

		return world;
	}

	Node parseNode(String selfKey, JSONObject jNode) {
		Node node = new Node();
		node.key = selfKey;

		try {

			@SuppressWarnings("unchecked")
			Iterator<String> keys = jNode.keys();

			String key;
			JSONObject jChildNode;
			Node childNode;

			while (keys.hasNext()) {
				key = keys.next();
				if (key.equals("keys")) {
					continue;
				}
				if (key.equals("properties")) {
					JSONObject jPropertiesNode;
					jPropertiesNode = jNode.getJSONObject(key);
					parseProperties(jPropertiesNode, node);
					continue;
				}
				jChildNode = jNode.getJSONObject(key);
				childNode = parseNode(key, jChildNode);
				node.children.put(key, childNode);
			}
		} catch (JSONException e) {
			e.printStackTrace();
		}

		return node;
	}

	void parseProperties(JSONObject jPropertiesNode, Node node) {
		try {
			node.image = jPropertiesNode.getString("image");
		} catch (JSONException e) {
		}
		try {
			String size = jPropertiesNode.getString("size");
			String[] sizeSplit = new String[3];
			sizeSplit = size.split("\\*");
			node.size.w = Integer.valueOf(sizeSplit[0]).intValue();
			node.size.h = Integer.valueOf(sizeSplit[1]).intValue();
			node.size.d = Integer.valueOf(sizeSplit[2]).intValue();
		} catch (JSONException e) {
		}
		try {
			String position = jPropertiesNode.getString("position");
			String[] positionSplit;
			positionSplit = position.split(",");
			node.position.x = Integer.valueOf(positionSplit[0]).intValue();
			node.position.y = Integer.valueOf(positionSplit[1]).intValue();
			node.position.z = Integer.valueOf(positionSplit[2]).intValue();
		} catch (JSONException e) {
		}
		try {
			node.mode = jPropertiesNode.getString("mode");
		} catch (JSONException e) {
		}
		try {
			JSONObject jLinksNode;
			jLinksNode = jPropertiesNode.getJSONObject("links");
			parseLinks(jLinksNode, node);
		} catch (JSONException e) {
		}
		try {
			JSONObject jInterpolatorsNode;
			jInterpolatorsNode = jPropertiesNode.getJSONObject("interpolators");
			parseInterpolators(jInterpolatorsNode, node);
		} catch (JSONException e) {
		}
		try {
			String isShownStr = jPropertiesNode.getString("shown");
			if (isShownStr.equals("false")) {
				node.shown = false;
			}
		} catch (JSONException e) {
		}
	}

	void parseLinks(JSONObject jLinksNode, Node node) {
		try {
			JSONObject jLinkNode;
			jLinkNode = jLinksNode.getJSONObject("TouchEventMove");
			Link link = new Link();
			link.type = "TouchEventMove";

			String isActiveStr = jLinkNode.getString("active");
			if (isActiveStr.equals("true")) {
				link.active = true;
			}

			String factorStr = jLinkNode.getString("factor");
			String[] factorSplit;
			factorSplit = factorStr.split("\\*");
			link.factor.x = Float.valueOf(factorSplit[0]);
			link.factor.y = Float.valueOf(factorSplit[1]);
			link.factor.z = Float.valueOf(factorSplit[2]);

			node.links.put("TouchEventMove", link);

		} catch (JSONException e) {
		}
	}

	void parseInterpolators(JSONObject jInterpolatorsNode, Node node) {
		try {
			JSONObject jInterpolatorNode;
			jInterpolatorNode = jInterpolatorsNode.getJSONObject("deceleration");
			Interpolator interpolator = new Interpolator();
			interpolator.type = "deceleration";

			String isActiveStr = jInterpolatorNode.getString("active");
			if (isActiveStr.equals("true")) {
				interpolator.active = true;
			}

			String factorStr = jInterpolatorNode.getString("factor");
			String[] factorSplit;
			factorSplit = factorStr.split("\\*");
			interpolator.factor.x = Float.valueOf(factorSplit[0]);
			interpolator.factor.y = Float.valueOf(factorSplit[1]);
			interpolator.factor.z = Float.valueOf(factorSplit[2]);

			node.interpolators.put("deceleration", interpolator);

		} catch (JSONException e) {
		}
	}

	public String getFromAssets(String fileName) {
		String result = null;
		try {
			InputStreamReader inputReader = new InputStreamReader(context.getResources().getAssets().open(fileName));
			BufferedReader bufReader = new BufferedReader(inputReader);
			String line = "";
			result = "";
			while ((line = bufReader.readLine()) != null) {
				result += line;
			}
			return result;
		} catch (Exception e) {
			e.printStackTrace();
		}
		return result;
	}

	public void initializeWorld() {
		// String worldStr = getFromAssets("world.js");
		String worldStr = getFromAssets("hot.js");
		try {
			JSONObject jWorld = new JSONObject(worldStr);
			World world = this.parseWorld(jWorld);
			World.world = world;
		} catch (JSONException e) {
			e.printStackTrace();
		}
	}

	public Node $(String selector) {
		Node node = null;
		World world = World.getInstance();

		String[] selectorSplit;
		selectorSplit = selector.split(" ");

		node = world.children.get(selectorSplit[0]);

		for (int i = 1; i < selectorSplit.length; i++) {
			node = node.children.get(selectorSplit[i]);
		}

		return node;
	}

}
