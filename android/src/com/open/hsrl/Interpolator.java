package com.open.hsrl;

import com.open.hsrl.Node.Position;

public class Interpolator {
	public String type;// deceleration acceleration
	public Boolean active = false;
	public float dxSpeed = 0.0f;
	public float dySpeed = 0.0f;

	public void deceleration(Position position, long deltaMillis) {

		position.x += dxSpeed * deltaMillis;
		position.y += dySpeed * deltaMillis;

		dxSpeed *= (1.0f - 0.001f * deltaMillis);
		dySpeed *= (1.0f - 0.001f * deltaMillis);

		if (Math.abs(dxSpeed) < 0.001f) {
			dxSpeed = 0.0f;
		}
		if (Math.abs(dySpeed) < 0.001f) {
			dySpeed = 0.0f;
		}
		if (dxSpeed == 0.0f && dySpeed == 0.0f) {
			this.active = false;
		}
	}

}
