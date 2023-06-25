package renderEngine;

import org.lwjgl.opengl.GL11;
import org.lwjgl.opengl.GL13;
import org.lwjgl.opengl.GL20;
import org.lwjgl.opengl.GL30;
import org.lwjgl.util.vector.Matrix4f;

import entities.Entity;
import models.RawModel;
import models.TexturedModel;
import shaders.StaticShader;
import textures.ModelTexture;
import utilities.Maths;

import java.util.List;
import java.util.Map;

public class EntityRenderer {
	
	private StaticShader shader;
	
	public EntityRenderer(StaticShader shader, Matrix4f projectionMatrix) {
		this.shader = shader;		
		setProjectionMatrix(projectionMatrix);
	}
	
	//clear screen, set background color and do z-buffering (depth-buffering)

	public void setProjectionMatrix(Matrix4f projectionMatrix) {
		shader.start();
		shader.loadProjectionMatrix(projectionMatrix);
		shader.stop();
	}
	
	public void render(Map<TexturedModel, List<Entity>> entities) {
		
		/*
		 * Some entities use the same texture. Instead of binding and unbinding
		 * same texture again and again before rendering them, I keep these entities in maps
		 * (entities with same Model)to render them in batches.
		 * */
		for(TexturedModel model:entities.keySet()) {
			prepareTexturedModel(model);
			List<Entity> batch = entities.get(model);
			for(Entity entity : batch) {
				prepareInstance(entity);
				GL11.glDrawElements(GL11.GL_TRIANGLES, model.getRawModel().getVertexCount(), GL11.GL_UNSIGNED_INT, 0);

			}
			unbindTexturedModel();
		}
	}
	
	private void prepareTexturedModel(TexturedModel model) {
		RawModel rawModel = model.getRawModel();
		GL30.glBindVertexArray(rawModel.getVaoID());
		GL20.glEnableVertexAttribArray(0); //vertex positions
		GL20.glEnableVertexAttribArray(1); //texture coords
		GL20.glEnableVertexAttribArray(2); //normals
		ModelTexture texture = model.getTexture();
		if(texture.getHasTransparency()) {
			MasterRenderer.disableCulling();
		}
		shader.loadFakeLightingVariable(texture.getUseFakeLighting());
		shader.loadShineVariables(texture.getShineDamper(), texture.getReflectivity());
		GL13.glActiveTexture(GL13.GL_TEXTURE0);
		GL11.glBindTexture(GL11.GL_TEXTURE_2D, model.getTexture().getID());
	}
	
	private void unbindTexturedModel() {
		MasterRenderer.enableCulling();
		GL20.glDisableVertexAttribArray(0);
		GL20.glDisableVertexAttribArray(1);
		GL20.glDisableVertexAttribArray(2);
		GL30.glBindVertexArray(0);

	}
	
	private void prepareInstance(Entity entity) {
		Matrix4f transformationMatrix = Maths.createTransformationMatrix(
				entity.getPosition(),
				entity.getRotation(),
				entity.getScale()
		); 
		shader.loadTransformationMatrix(transformationMatrix);
	}
}
