package textures;

public class ModelTexture {

	private int textureID;
	
	private float shineDamper = 1;
	private float reflectivity=0;
	
	private boolean hasTransparency=false;
	
	//some objects (e.g grass) only consists of 2 quads and normals are facing in wrong directions.
	//so, lighting on them should be simulated.
	private boolean useFakeLighting = false;
	public ModelTexture(int id) {
		this.textureID = id;
	}
	
	public float getShineDamper() {
		return shineDamper;
	}

	public float getReflectivity() {
		return reflectivity;
	}

	public int getID() {
		return this.textureID;
	}
	
	public void setShineDamper(float shineDamper) {
		this.shineDamper = shineDamper;
	}

	public void setReflectivity(float reflectivity) {
		this.reflectivity = reflectivity;
	}

	public boolean getHasTransparency() {
		return hasTransparency;
	}

	public void setHasTransparency(boolean hasTransparency) {
		this.hasTransparency = hasTransparency;
	}

	public boolean getUseFakeLighting() {
		return useFakeLighting;
	}
	
	public void setUseFakeLighting(boolean fakeLighting) {
		useFakeLighting = fakeLighting;
	}
	

}
