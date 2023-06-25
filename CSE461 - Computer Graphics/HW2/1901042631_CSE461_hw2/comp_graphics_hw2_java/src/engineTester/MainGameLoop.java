package engineTester;

import java.util.ArrayList;
import java.util.List;
import java.util.Random;

import models.RawModel;
import models.TexturedModel;
import objConverter.ModelData;
import objConverter.OBJFileLoader;

import org.lwjgl.opengl.Display;
import org.lwjgl.util.vector.Vector3f;

import renderEngine.DisplayManager;
import renderEngine.Loader;
import renderEngine.MasterRenderer;
import terrains.Terrain;
import textures.ModelTexture;
import textures.TerrainTexture;
import textures.TerrainTexturePack;

import entities.Camera;
import entities.Entity;
import entities.Light;
import entities.Player;

public class MainGameLoop {

    private static final Random random = new Random();




	public static void main(String[] args) {

		DisplayManager.createDisplay();
		Loader loader = new Loader();
		
		ModelData treeData = OBJFileLoader.loadOBJ("tree");
		RawModel treeModel = loader.loadToVAO(treeData.getVertices(), treeData.getTextureCoords(),treeData.getNormals(),treeData.getIndices());
		TexturedModel texturedTreeModel = new TexturedModel(treeModel,new ModelTexture(loader.loadTexture("tree")));
		
		ModelData tree2Data = OBJFileLoader.loadOBJ("lowPolyTree");
		RawModel tree2Model = loader.loadToVAO(tree2Data.getVertices(), tree2Data.getTextureCoords(),tree2Data.getNormals(),tree2Data.getIndices());
		TexturedModel texturedTree2Model = new TexturedModel(tree2Model,new ModelTexture(loader.loadTexture("lowPolyTree")));
		
		ModelData grassData = OBJFileLoader.loadOBJ("grassModel");
		RawModel grassModel = loader.loadToVAO(grassData.getVertices(), grassData.getTextureCoords(),grassData.getNormals(),grassData.getIndices());
		TexturedModel texturedGrassModel = new TexturedModel(grassModel,new ModelTexture(loader.loadTexture("grassTexture")));
		texturedGrassModel.getTexture().setHasTransparency(true);
		texturedGrassModel.getTexture().setUseFakeLighting(true);
		
		ModelData fernData = OBJFileLoader.loadOBJ("fern");
		RawModel fernModel = loader.loadToVAO(fernData.getVertices(), fernData.getTextureCoords(),fernData.getNormals(),fernData.getIndices());
		TexturedModel texturedFernModel = new TexturedModel(fernModel,new ModelTexture(loader.loadTexture("fern")));
		texturedFernModel.getTexture().setHasTransparency(true);
		texturedFernModel.getTexture().setUseFakeLighting(true);

		ModelData flowerData = OBJFileLoader.loadOBJ("grassModel");
		RawModel flowerModel = loader.loadToVAO(flowerData.getVertices(), flowerData.getTextureCoords(),flowerData.getNormals(),flowerData.getIndices());
		TexturedModel texturedFlowerModel = new TexturedModel(flowerModel,new ModelTexture(loader.loadTexture("flower")));
		texturedFlowerModel.getTexture().setHasTransparency(true);
		texturedFlowerModel.getTexture().setUseFakeLighting(true);
		
		ModelData stallData = OBJFileLoader.loadOBJ("stall");
		RawModel stallModel = loader.loadToVAO(stallData.getVertices(), stallData.getTextureCoords(),stallData.getNormals(),stallData.getIndices());
		TexturedModel texturedStallModel = new TexturedModel(stallModel,new ModelTexture(loader.loadTexture("stallTexture")));
		
		ModelData lampData = OBJFileLoader.loadOBJ("lamp");
		RawModel lampModel = loader.loadToVAO(lampData.getVertices(), lampData.getTextureCoords(),lampData.getNormals(),lampData.getIndices());
		TexturedModel texturedLampModel = new TexturedModel(lampModel,new ModelTexture(loader.loadTexture("lamp")));
		texturedLampModel.getTexture().setUseFakeLighting(true);
		
		ModelData dragonData = OBJFileLoader.loadOBJ("dragon");
		RawModel dragonModel = loader.loadToVAO(dragonData.getVertices(), dragonData.getTextureCoords(),dragonData.getNormals(),dragonData.getIndices());
		TexturedModel textureddragonModel = new TexturedModel(dragonModel,new ModelTexture(loader.loadTexture("stallTexture")));
		textureddragonModel.getTexture().setUseFakeLighting(true);
		List<Entity> entities = new ArrayList<Entity>();
		for(int i=0;i<200;i++){
			entities.add(new Entity(texturedTreeModel, 
				generateRandomEntityPos(),
				new Vector3f(0,0,0),
				new Vector3f(6,6,6))
			);
			entities.add(new Entity(texturedTree2Model, 
					generateRandomEntityPos(),
					new Vector3f(0,0,0),
					new Vector3f(1,1,1))
				);
		}
		for(int i=0;i<500;i++) {
			entities.add(new Entity(texturedGrassModel, 
					generateRandomEntityPos(),
					new Vector3f(0,0,0),
					new Vector3f(1,1,1))
			);
			entities.add(new Entity(texturedFernModel, 
					generateRandomEntityPos(),
					new Vector3f(0,0,0),
					new Vector3f(0.7f,0.7f,0.7f))
			);
			entities.add(new Entity(texturedFlowerModel, 
					generateRandomEntityPos(),
					new Vector3f(0,0,0),
					new Vector3f(2f,2f,2f))
			);
		}
		
		
		List<Light> lights = new ArrayList<Light>();
		lights.add(new Light(new Vector3f(0,10000,-7000),new Vector3f(0.6f,0.6f,0.6f))); // directional light
		//point lights
		lights.add(new Light(new Vector3f(360, 12.4f, 220),new Vector3f(2,0,0), new Vector3f(1,0.01f,0.002f)));
		entities.add(new Entity(texturedLampModel, new Vector3f(360, 0, 220), new Vector3f(0,0,0), new Vector3f(1,1,1)));
		lights.add(new Light(new Vector3f(390, 12.4f, 290),new Vector3f(0,2,2), new Vector3f(1,0.01f,0.002f)));
		entities.add(new Entity(texturedLampModel, new Vector3f(390, 0, 290), new Vector3f(0,0,0), new Vector3f(1,1,1)));
		lights.add(new Light(new Vector3f(450, 12.4f, 250),new Vector3f(2,2,0), new Vector3f(1,0.01f,0.002f)));
		entities.add(new Entity(texturedLampModel, new Vector3f(450, 0, 250), new Vector3f(0,0,0), new Vector3f(1,1,1)));

		Vector3f center = new Vector3f(410, 0, 250); // Center of the bounded area
		// Scale factor of 3 for all stalls
		Vector3f scale = new Vector3f(3, 3, 3);
		
		Entity dragon = new Entity(textureddragonModel, new Vector3f(410, 100, 250), new Vector3f(0,0,0), new Vector3f(10,10,10));
		entities.add(dragon);
		
		float radius = 80; // Radius of the circle
		float angleOffset = (float) (2 * Math.PI / 24); // Angle offset for evenly distributing the stalls

		for (int i = 0; i <24; i++) {
		    float angle = angleOffset * i;
		    float x = center.x + radius * (float) Math.cos(angle);
		    float z = center.z + radius * (float) Math.sin(angle);
		    entities.add(new Entity(texturedStallModel, new Vector3f(x, 0, z), calculateRotation(new Vector3f(x, 0, z), center), scale));
		}
			
		
		
		TerrainTexture backgroundTexture = new TerrainTexture(loader.loadTexture("grassy"));
		TerrainTexture rTexture = new TerrainTexture(loader.loadTexture("dirt"));
		TerrainTexture gTexture = new TerrainTexture(loader.loadTexture("pinkFlowers"));
		TerrainTexture bTexture = new TerrainTexture(loader.loadTexture("path"));
		TerrainTexture blendMap = new TerrainTexture(loader.loadTexture("blendMap"));
		TerrainTexturePack texturePack = new TerrainTexturePack(backgroundTexture, rTexture, gTexture, bTexture);
		
		Terrain terrain = new Terrain(0,0,loader, texturePack, blendMap);
		
		MasterRenderer renderer = new MasterRenderer();
		

		Player player = new Player(new Vector3f(420,1,250),new Vector3f(0,0,0));
		Camera camera = new Camera(player);

		
		while(!Display.isCloseRequested()){
			camera.move();
			player.move();
			
			renderer.processTerrain(terrain);
			dragon.increaseRotation(0.1f, 0.1f, 0.1f);
			for(Entity entity:entities){
				renderer.processEntity(entity);
			}
			renderer.render(lights, camera);
			DisplayManager.updateDisplay();
		}

		renderer.cleanUp();
		loader.cleanUp();
		DisplayManager.closeDisplay();

	}
	
	
    public static Vector3f generateRandomEntityPos() {
        float x = random.nextFloat() * 1000;
        float z = random.nextFloat() * 1000;

        return new Vector3f(x, 0, z);
    }
	private static Vector3f calculateRotation(Vector3f position, Vector3f center) {
	    Vector3f direction = new Vector3f();
	    Vector3f.sub(center, position, direction);
	    direction.normalise();
	    float angle = (float) Math.toDegrees(Math.atan2(-direction.getX(), -direction.getZ()));
	    return new Vector3f(0, angle, 0);
	}

}