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
			JSONObject jSpace;
			Space space;

			while (keys.hasNext()) {
				key = keys.next();
				if (key.equals("properties")  || key.equals("keys")) {
					continue;
				}
				jSpace = jWorld.getJSONObject(key);
				space = parseSpace(key, jSpace);
				world.spaces.put(key, space);
			}
		} catch (JSONException e) {
			e.printStackTrace();
		}
		return world;
	}

	Space parseSpace(String selfKey, JSONObject jSpace) {
		Space space = new Space();
		space.key = selfKey;

		try {

			@SuppressWarnings("unchecked")
			Iterator<String> keys = jSpace.keys();

			String key;
			JSONObject jNode;
			Node node;

			while (keys.hasNext()) {
				key = keys.next();
				if (key.equals("properties")  || key.equals("keys") ) {
					continue;
				}
				jNode = jSpace.getJSONObject(key);
				node = parseNode(key, jNode);
				space.children.put(key, node);
			}
		} catch (JSONException e) {
			e.printStackTrace();
		}

		return space;
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
				if (key.equals("properties")  || key.equals("keys")) {
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

	public Node generateUserFromNode(JSONObject jNode) {
		Node node = new Node();
		try {
			node.key = jNode.getString("key");
		} catch (JSONException e) {
		}

		return node;
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
		String worldStr = getFromAssets("world.js");
		try {
			JSONObject jWorld = new JSONObject(worldStr);
			World world = this.parseWorld(jWorld);
			World.world = world;
		} catch (JSONException e) {
			e.printStackTrace();
		}
	}

}
