package renderEngine;
import shaders.StaticShader;
import entities.Camera;
import entities.Entity;
import entities.Light;

import java.util.List;
import java.util.ArrayList;
import java.util.Map;

import org.lwjgl.opengl.Display;
import org.lwjgl.opengl.GL11;
import org.lwjgl.util.vector.Matrix4f;

import java.util.HashMap;
import models.TexturedModel;
import shaders.TerrainShader;
import terrains.Terrain;

public class MasterRenderer {
	
	private static float FOV = 70;
	private static final float NEAR_PLANE = 0.1f;
	private static final float FAR_PLANE = 1000f;
	
	private static final float SKY_RED = 0.7f;
	private static final float SKY_GREEN = 0.7f;
	private static final float SKY_BLUE = 0.85f;
	
	private static Matrix4f projectionMatrix;
	
	private StaticShader shader = new StaticShader();
	private static EntityRenderer renderer;
	
	private TerrainShader terrainShader = new TerrainShader();
	private static TerrainRenderer terrainRenderer;
	
	
	private Map<TexturedModel, List<Entity>> entities = new HashMap<TexturedModel, List<Entity>>();
	private List<Terrain> terrains = new ArrayList<Terrain>();
	
	public MasterRenderer() {
		//don't render the parts which are not visible to camera (inside faces).
		enableCulling();
		
		createProjectionMatrix();
		renderer = new EntityRenderer(shader, projectionMatrix);
		terrainRenderer = new TerrainRenderer(terrainShader, projectionMatrix);
	}
	
	public void prepare() {
		GL11.glEnable(GL11.GL_DEPTH_TEST);
		//clear the color from previous frame
		GL11.glClear(GL11.GL_COLOR_BUFFER_BIT|GL11.GL_DEPTH_BUFFER_BIT);
		GL11.glClearColor(SKY_RED, SKY_GREEN, SKY_BLUE, 1);
	}
	
	public static void enableCulling() {
		GL11.glEnable(GL11.GL_CULL_FACE);
		GL11.glCullFace(GL11.GL_BACK);
	}
	
	//Back culling is disabled on semi-transparent objects.
	public static void disableCulling() {
		GL11.glDisable(GL11.GL_CULL_FACE);
		GL11.glCullFace(GL11.GL_BACK);
	}
	
	public void render(List<Light> lights, Camera camera) {
		prepare();
		shader.start();
		shader.loadSkyColor(SKY_RED, SKY_GREEN, SKY_BLUE);
		shader.loadLights(lights);
		shader.loadViewMatrix(camera);
		renderer.render(entities);
		shader.stop();
		terrainShader.start();
		terrainShader.loadSkyColor(SKY_RED, SKY_GREEN, SKY_BLUE);
		terrainShader.loadLights(lights);
		terrainShader.loadViewMatrix(camera);
		terrainRenderer.render(terrains);
		terrainShader.stop();
		
		terrains.clear();
		entities.clear();
	}
	
	public void processTerrain(Terrain terrain) {
		terrains.add(terrain);
	}
	
	public void processEntity(Entity entity) {
		TexturedModel entityModel = entity.getModel();
		List<Entity> batch = entities.get(entityModel);
		if(batch!=null) batch.add(entity);
		else {
			batch = new ArrayList<Entity>();
			batch.add(entity);
			entities.put(entityModel, batch);
		}
	}
	
	public void cleanUp() {
		shader.cleanUp();
		terrainShader.cleanUp();
	}
	
	private void createProjectionMatrix() {
		float aspectRatio = (float) Display.getWidth() / (float) Display.getHeight();
		float y_scale = (float) (1f / Math.tan(Math.toRadians(FOV / 2f)) * aspectRatio);
		float x_scale = y_scale/aspectRatio;
		float frustum_length = FAR_PLANE - NEAR_PLANE;
		
		projectionMatrix = new Matrix4f();
		projectionMatrix.m00 = x_scale;
		projectionMatrix.m11 = y_scale;
		projectionMatrix.m22 = -((FAR_PLANE + NEAR_PLANE) / frustum_length);
		projectionMatrix.m23 = -1;
		projectionMatrix.m32 = -((2 *FAR_PLANE * NEAR_PLANE) / frustum_length);
		projectionMatrix.m33 = 0;
	}
	
	public static void updateFOV(float newFOV) {
		if(newFOV < 10 || newFOV > 100) return;
		FOV = newFOV;
		float aspectRatio = (float) Display.getWidth() / (float) Display.getHeight();
		float y_scale = (float) (1f / Math.tan(Math.toRadians(FOV / 2f)) * aspectRatio);
		float x_scale = y_scale/aspectRatio;
		projectionMatrix.m00 = x_scale;
		projectionMatrix.m11 = y_scale;
		renderer.setProjectionMatrix(projectionMatrix);
		terrainRenderer.setProjectionMatrix(projectionMatrix);

	}
}
