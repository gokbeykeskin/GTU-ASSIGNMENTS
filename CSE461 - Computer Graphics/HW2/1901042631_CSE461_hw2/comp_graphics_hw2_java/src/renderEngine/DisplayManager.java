package renderEngine;

import org.lwjgl.LWJGLException;
import org.lwjgl.Sys;
import org.lwjgl.opengl.Display;
import org.lwjgl.opengl.DisplayMode;
import org.lwjgl.opengl.GL11;
import org.lwjgl.opengl.ContextAttribs;
import org.lwjgl.opengl.PixelFormat;

public class DisplayManager {
	
	private static final int WIDTH = 1920;
	private static final int HEIGHT = 1080;
	private static final int MAX_FPS = 120;

	private static long lastFrameTime;
	private static float delta;
	
	public static void createDisplay() {
		//use new core profile (programmable pipeline)
		ContextAttribs attribs = new ContextAttribs(3,2).
				withForwardCompatible(true).
				withProfileCore(true);
		
		try {
			Display.setDisplayMode(new DisplayMode(WIDTH, HEIGHT));
			Display.create(new PixelFormat(), attribs);
		} catch (LWJGLException e) {
			e.printStackTrace();
		}
		
		//render whole screen
		GL11.glViewport(0, 0, WIDTH, HEIGHT);
		lastFrameTime = getCurrentTime();
	}
	
	public static void updateDisplay() {
		Display.sync(MAX_FPS);
		Display.update();
		long currentFrameTime = getCurrentTime();
		delta = (currentFrameTime - lastFrameTime)/1000f;
		lastFrameTime = currentFrameTime;
	}
	
	public static float getFrameTimeSeconds() {
		return delta;
	}
	
	public static void closeDisplay() {
		Display.destroy();
	}
	
	//returns the current time in miliseconds
	private static long getCurrentTime() {
		//getTime returns the time in ticks. It should be divided to resolution. *1000 is to get it in miliseconds.
		return Sys.getTime() * 1000 / Sys.getTimerResolution();
	}
}
