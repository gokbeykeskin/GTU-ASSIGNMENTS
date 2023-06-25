package entities;

import org.lwjgl.input.Keyboard;
import org.lwjgl.input.Mouse;
import org.lwjgl.util.vector.Vector3f;

import renderEngine.DisplayManager;
import renderEngine.MasterRenderer;

public class Player {
	Vector3f position;
	Vector3f rotation;
    public Player(Vector3f position, Vector3f rotation) {
        this.position = position;
        this.rotation = rotation;
        Mouse.setGrabbed(true);
        
    }
    
    private static final float WALK_SPEED = 20;
    private static final float RUN_SPEED = 40;
    private static final float TURN_SPEED = 0.1f;
    private static final float GRAVITY = -50;
    private static final float JUMP_POWER = 20;

    private static final float PLAYER_HEIGHT = 8;

    private float currentSpeed = 0;
    private float currentSidewaySpeed = 0;
    private float currentTurnSpeed = 0;
    private float currentUpSpeed = 0;
    
    private float currentZoom = 70;
    
    private boolean isInAir = false;
    private boolean godMode = false;
    
    //these are used for detecting initial key strokes instead of keyDown.
    private boolean keyLocked = false;
    private boolean keyLocked2 = false;

    
    public void move() {
        checkInputs();

        increaseRotation(0, currentTurnSpeed * DisplayManager.getFrameTimeSeconds(), 0);
        float distance = currentSpeed * DisplayManager.getFrameTimeSeconds();
        float sidewayDistance = currentSidewaySpeed * DisplayManager.getFrameTimeSeconds();
        float dx = (float) (distance * Math.sin(Math.toRadians(rotation.y))) +
                   (float) (sidewayDistance * Math.sin(Math.toRadians(rotation.y + 90)));
        float dz = (float) (distance * Math.cos(Math.toRadians(rotation.y))) +
                   (float) (sidewayDistance * Math.cos(Math.toRadians(rotation.y + 90)));
        increasePosition(dx, 0, dz);
        
        currentUpSpeed += GRAVITY * DisplayManager.getFrameTimeSeconds();
        if(godMode) isInAir = false;
        increasePosition(0, currentUpSpeed * DisplayManager.getFrameTimeSeconds(), 0);
        
        //simple terrain collision
        if (position.y < PLAYER_HEIGHT) {
            currentUpSpeed = 0;
            position.y = PLAYER_HEIGHT;
            isInAir = false;
        }
        MasterRenderer.updateFOV(currentZoom);
    }

    private void jump() {
        if (!isInAir) {
            this.currentUpSpeed = JUMP_POWER;
            isInAir = true;
        }
    }

    private void checkInputs() {
        // Mouse rotation
        float dx = -Mouse.getDX();
        float dy = -Mouse.getDY();
        increaseRotation(-dy * TURN_SPEED, dx * TURN_SPEED, 0);

        // Sideways movement
        if (Keyboard.isKeyDown(Keyboard.KEY_A)) {
        	if(Keyboard.isKeyDown(Keyboard.KEY_LSHIFT))
        		this.currentSidewaySpeed = RUN_SPEED;
        	else this.currentSidewaySpeed = WALK_SPEED;
        } else if (Keyboard.isKeyDown(Keyboard.KEY_D)) {
        	if(Keyboard.isKeyDown(Keyboard.KEY_LSHIFT))
        		this.currentSidewaySpeed = -RUN_SPEED;
        	else this.currentSidewaySpeed = -WALK_SPEED;
        } else {
            this.currentSidewaySpeed = 0;
        }

        // Forward/backward movement
        if (Keyboard.isKeyDown(Keyboard.KEY_W)) {
        	if(Keyboard.isKeyDown(Keyboard.KEY_LSHIFT))
        		this.currentSpeed = RUN_SPEED;
        	else this.currentSpeed = WALK_SPEED;
        } else if (Keyboard.isKeyDown(Keyboard.KEY_S)) {
        	if(Keyboard.isKeyDown(Keyboard.KEY_LSHIFT))
        		this.currentSpeed = -RUN_SPEED;
        	else this.currentSpeed= -WALK_SPEED;
        } else {
            this.currentSpeed = 0;
        }

        // Jumping
        if (Keyboard.isKeyDown(Keyboard.KEY_SPACE)) {
            jump();
        }
        
        //Zoom
        currentZoom -= Mouse.getDWheel() * 0.05;
        
        if(Keyboard.isKeyDown(Keyboard.KEY_ESCAPE)) {
        	if(!keyLocked) {
            	Mouse.setGrabbed(!Mouse.isGrabbed());
            	keyLocked = true;

        	}
        } else keyLocked = false;
        
        if(Keyboard.isKeyDown(Keyboard.KEY_G)) {
        	if(!keyLocked2) {
            	godMode = !godMode;
            	keyLocked2 = true;
        	}
        } else keyLocked2 = false;
    }
    
    public void increasePosition(float dx, float dy, float dz) {
        float newX = this.position.x + dx;
        float newY = this.position.y + dy;
        float newZ = this.position.z + dz;

        // Calculate the distance between the new position and the center of the circle
        float distance = (float) Math.sqrt(Math.pow(newX - 410, 2) + Math.pow(newY - 0, 2) + Math.pow(newZ - 250, 2));

        // Check if the distance is within the radius of the circle
        if (distance <= 80 || godMode) {
            this.position.x = newX;
            this.position.y = newY;
            this.position.z = newZ;
        }
    }

	
	public void increaseRotation(float dx, float dy, float dz) {
		this.rotation.x += dx;
		this.rotation.y += dy;
		this.rotation.z += dz;
	}
	
	public Vector3f getPosition() {
		return position;
	}

	public void setPosition(Vector3f position) {
		this.position = position;
	}

	public Vector3f getRotation() {
		return rotation;
	}

	public void setRotation(Vector3f rotation) {
		this.rotation = rotation;
	}
}