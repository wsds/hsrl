package com.test.view20;

import java.util.Set;

import android.opengl.GLES20;
import android.opengl.Matrix;

import com.open.hsrl.Interpolator;
import com.open.hsrl.Link;
import com.open.hsrl.Node;
import com.open.hsrl.World;
import com.open.hsrl.WorldHelper;

public class SpaceHolder {

	ImagePool imagePool;

	public float width;
	public float height;
	public float ratio;
	public float Pi;

	public float screen_offset_x;
	public float screen_offset_y;

	public float x0 = 0;// touch_down
	public float y0 = 0;

	public float x = 0;// touch_move
	public float y = 0;

	public float x1 = 0;// touch_up
	public float y1 = 0;

	public float ax = 0;// Acceleration
	public float ay = 0;

	public float dx = 0;
	public float dy = 0;

	public void initialize(float ratio, float width, float height, int mModelMatrixHandle, ImagePool imagePool) {
		this.width = width;
		this.height = height;
		this.ratio = ratio;
		this.Pi = 2.0f / width;

		this.screen_offset_x = -1.0f;
		this.screen_offset_y = 1.0f / ratio;

		this.mModelMatrixHandle = mModelMatrixHandle;
		this.imagePool = imagePool;

		worldHelper.context = this.imagePool.mContext;

		worldHelper.initializeWorld();
	}

	void drawImage(String key, float left, float top, float z, float width, float height) {
		drawImage(key, left, top, z, width, height, null);
	}

	private float[] mModelMatrixpreMove = new float[16];

	private float[] mModelMatrix = new float[16];
	private int mModelMatrixHandle;

	float angle = 0;
	float offset_x = 0;
	float offset_y = 0;
	float offset_z = 0;
	float offset_w = 0;
	float offset_h = 0;
	float[] modelMatrix = null;

	void drawImage(String key, float left, float top, float z, float width, float height, float[] modelMatrixMove) {
		int textureID = imagePool.getImage(key);
		GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, textureID);

		offset_x = this.screen_offset_x + left * this.Pi;
		offset_y = this.screen_offset_y - top * this.Pi;
		offset_z = -z / 10.0f;

		offset_w = 2 * width / this.width;
		offset_h = 2 * height / this.width;

		Matrix.setRotateM(mModelMatrixpreMove, 0, angle, 0, 0, 1.0f);
		if (modelMatrixMove != null) {
			Matrix.multiplyMM(mModelMatrix, 0, mModelMatrixpreMove, 0, modelMatrixMove, 0);
			modelMatrix = mModelMatrix;
		} else {
			Matrix.translateM(mModelMatrixpreMove, 0, offset_x, offset_y, offset_z);
			modelMatrix = mModelMatrixpreMove;
		}

		Matrix.scaleM(modelMatrix, 0, offset_w, offset_h, 1.0f);

		GLES20.glUniformMatrix4fv(mModelMatrixHandle, 1, false, modelMatrix, 0);
		GLES20.glDrawArrays(GLES20.GL_TRIANGLE_STRIP, 0, 4);
	}

	World world;
	WorldHelper worldHelper = WorldHelper.getInstance();

	WorldHelper w = worldHelper;

	void renderWorld() {
		currentMillis = System.currentTimeMillis();

		world = World.getInstance();
		Set<String> keys = world.children.keySet();
		for (String key : keys) {
			Node node = world.children.get(key);
			renderNode(node, 0, 0, 0);
		}

		lastMillis = currentMillis;
	}

	long lastMillis = 0;
	long currentMillis = 0;

	void renderNode(Node node, float parent_x, float parent_y, float parent_z) {
		// render this node
		// resolve links
		float x_link = 0;
		float y_link = 0;

		Set<String> linkKeys = node.links.keySet();
		for (String linkKey : linkKeys) {
			Link link = node.links.get(linkKey);
			if (link.active == false) {
				continue;
			}
			if (link.type == "TouchEventMove") {
				x_link = (this.x - this.x0) * link.factor.x;
				y_link = (this.y - this.y0) * link.factor.y;
			}
		}

		float x = parent_x + node.position.x + x_link;
		float y = parent_y + node.position.y + y_link;
		float z = parent_z + node.position.z;

		// resolve interpolators
		if (lastMillis != 0) {
			long deltaMillis = currentMillis - lastMillis;

			Set<String> interpolatorKeys = node.interpolators.keySet();
			for (String interpolatorKey : interpolatorKeys) {
				Interpolator interpolators = node.interpolators.get(interpolatorKey);
				if (interpolators.active == false) {
					continue;
				}
				if (interpolators.type == "deceleration") {
					interpolators.deceleration(node.position, deltaMillis);
				}
			}
		}

		// check if the node is shown
		if (node.shown == false) {
			return;
		}
		// draw the image
		if (node.image != null && !node.image.equals("")) {
			this.drawImage(node.image, x, y, z, node.size.w, node.size.h);
		}
		// render Children
		Set<String> keys = node.children.keySet();
		for (String key : keys) {
			Node childNode = node.children.get(key);
			renderNode(childNode, x, y, z);
		}
	}

	void resolveWorld() {
		world = World.getInstance();
		Set<String> keys = world.children.keySet();
		for (String key : keys) {
			Node node = world.children.get(key);
			resolveNode(node, 0, 0);
		}
		this.x = 0;
		this.x0 = 0;
		this.y = 0;
		this.y0 = 0;
	}

	void resolveNode(Node node, float parent_x, float parent_y) {
		// resolve this node
		// resolve links
		float x_link = 0;
		float y_link = 0;

		Set<String> linkKeys = node.links.keySet();
		for (String linkKey : linkKeys) {
			Link link = node.links.get(linkKey);
			if (link.active == false) {
				continue;
			}
			if (link.type == "TouchEventMove") {
				x_link = (this.x1 - this.x0) * link.factor.x;
				y_link = (this.y1 - this.y0) * link.factor.y;
			}
		}

		node.position.x = node.position.x + x_link;
		node.position.y = node.position.y + y_link;

		// resolve Children
		Set<String> keys = node.children.keySet();
		for (String key : keys) {
			Node childNode = node.children.get(key);
			resolveNode(childNode, x, y);
		}
	}

}
