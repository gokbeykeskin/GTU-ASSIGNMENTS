package entities;

import org.lwjgl.util.vector.Vector3f;
public class Camera {
	

	private Vector3f position = new Vector3f(0,0,0);
	private float pitch = 20;
	private float yaw;
	private float roll;
	
	private Player player;

	public Camera(Player player) {
		this.player = player;
	}
	
	
	public Vector3f getPosition() {
		return position;
	}

	public float getPitch() {
		return pitch;
	}

	public float getYaw() {
		return yaw;
	}

	public float getRoll() {
		return roll;
	}
	
	public void setPosition(Vector3f position) {
		this.position = position;
	}

	public void setPitch(float pitch) {
		this.pitch = pitch;
	}

	public void setYaw(float yaw) {
		this.yaw = yaw;
	}

	public void setRoll(float roll) {
		this.roll = roll;
	}
	
	
	public void move()
	 {
		Vector3f camPos = new Vector3f(player.getPosition().x, player.getPosition().y + 2f, player.getPosition().z);
		position = camPos;
		pitch = -player.getRotation().x;
		yaw = -player.getRotation().y + 180f;
		roll = -player.getRotation().z;
	 }
	
	
}
