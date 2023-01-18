// Local Headers

// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <assimp/Importer.hpp>      
#include <assimp/scene.h>           
#include <assimp/postprocess.h> 

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Shader.h"


// Standard Headers
#include <iostream>
#include <cstdlib>
#include <vector>

int mWidth = 500;
int mHeight = 500;
float moveX = 0, moveY = 0, moveZ = 0;
glm::mat4 rotation;
double oldxpos, oldypos;
bool lMouse = false;
float LRrotation = 0, oldLRrotation = 0;
float TBrotation = 0, oldTBrotation = 0;
bool addControlPoint = false;
bool animate = false;

//funkcija koja se poziva prilikom mijenjanja velicine prozora, moramo ju poveyati pomocu glfwSetFramebufferSizeCallback
void framebuffer_size_callback(GLFWwindow* window, int Width, int Height)
{
	mWidth = Width;
	mHeight = Height;

	glViewport(0, 0, mWidth, mHeight);

}

void mousePressedOrReleased(GLFWwindow* window, int button, int action, int mods) {
	int height, width;
	glfwGetCursorPos(window, &oldxpos, &oldypos);
	glfwGetWindowSize(window, &width, &height);
	oldLRrotation = LRrotation;
	oldTBrotation = TBrotation;
	if (action == GLFW_PRESS) {
		lMouse = true;
		//std::cout << "A mouse button was pressed at position: " << (oldxpos / width) * -2 + 1 << " " << (oldypos / height) * -2 + 1 << std::endl;

	}
	else
	{
		//std::cout << "A mouse button was released at position: " << (oldxpos / width) * -2 + 1 << " " << (oldypos / height) * -2 + 1 << std::endl;
		lMouse = false;
	}
	//else
		//std::cout << "A mouse button was released" << std::endl;
}

void keyPressed(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS) {
		switch (key) {
		case GLFW_KEY_T:
			//addControlPoint = true;
			break;
		case GLFW_KEY_SPACE:
			animate = true;
			break;
		case GLFW_KEY_W:
			//moveZ += 0.2;
			break;
		case GLFW_KEY_A:
			//moveX += 0.2;
			break;
		case GLFW_KEY_S:
			//moveZ -= 0.2;
			break;
		case GLFW_KEY_D:
			//moveX -= 0.2;
			break;
		case GLFW_KEY_Q:
			//moveY += 0.2;
			break;
		case GLFW_KEY_E:
			//moveY -= 0.2;
			break;
		}
	}
}


float max3(float first, float second, float third) {
	float max = first;
	if (second > max) max = second;
	if (third > max) max = third;
	return max;
}

Shader* loadShader(char* path, char* naziv) {
	std::string sPath(path);
	std::string pathVert;
	std::string pathFrag;

	pathVert.append(path, sPath.find_last_of("\\/") + 1);
	pathFrag.append(path, sPath.find_last_of("\\/") + 1);
	if (pathFrag[pathFrag.size() - 1] == '/') {
		pathVert.append("shaders/");
		pathFrag.append("shaders/");
	}
	else if (pathFrag[pathFrag.size() - 1] == '\\') {
		pathVert.append("shaders\\");
		pathFrag.append("shaders\\");
	}
	else {
		std::cerr << "nepoznat format pozicije shadera";
		exit(1);
	}

	pathVert.append(naziv);
	pathVert.append(".vert");
	pathFrag.append(naziv);
	pathFrag.append(".frag");

	return new Shader(pathVert.c_str(), pathFrag.c_str());
}


class Curve
{
public:
	Curve()
	{
		glGenBuffers(6, VBO);
		glGenVertexArrays(3, VAO);
	}
	void addPoint(glm::vec3 point) {
		controlPoints.push_back(point);
		if (controlPoints.size() < 2)
			return;

		float* positions = new float[controlPoints.size() * 3]{};
		float* colors = new float[controlPoints.size() * 3]{};

		for (size_t i = 0; i < controlPoints.size(); i++)
		{
			positions[i * 3] = controlPoints[i].x;
			positions[i * 3 + 1] = controlPoints[i].y;
			positions[i * 3 + 2] = controlPoints[i].z;

			colors[i * 3] = 1.0f;
			colors[i * 3 + 1] = 1.0f;
			colors[i * 3 + 2] = 1.0f;
		}

		glBindVertexArray(VAO[0]);
		glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * controlPoints.size() * 3, (void*)positions, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * controlPoints.size() * 3, (void*)colors, GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glBindVertexArray(0);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		delete[] positions;
		delete[] colors;


		if (controlPoints.size() < 4)
			return;


		for (size_t i = 0; i <= curvePointCount; i++)
		{
			approxCurvePoint((float)i / curvePointCount, controlPoints.size()-4);
		}

		positions = new float[curvePoints.size() * 3]{};
		colors = new float[curvePoints.size() * 3]{};

		for (size_t i = 0; i < curvePoints.size(); i++)
		{
			positions[i * 3] = curvePoints[i].x;
			positions[i * 3 + 1] = curvePoints[i].y;
			positions[i * 3 + 2] = curvePoints[i].z;

			colors[i * 3] = 1.0f;
			colors[i * 3 + 1] = 1.0f;
			colors[i * 3 + 2] = 0.0f;
		}

		glBindVertexArray(VAO[1]);
		glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * curvePoints.size() * 3, (void*)positions, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * curvePoints.size() * 3, (void*)colors, GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glBindVertexArray(0);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		delete[] positions;
		delete[] colors;

	}
	void draw(Shader* shader, glm::mat4 viewM, glm::mat4 projM) {
		if (controlPoints.size() < 2)
			return;
		glBindVertexArray(VAO[0]);
		GLint uniLocM = glGetUniformLocation(shader->ID, "model");
		GLint uniLocV = glGetUniformLocation(shader->ID, "view");
		GLint uniLocP = glGetUniformLocation(shader->ID, "projection");

		glm::mat4 jedinicna = glm::mat4(1);
		glUniformMatrix4fv(uniLocM, 1, GL_FALSE, &jedinicna[0][0]);
		glUniformMatrix4fv(uniLocV, 1, GL_FALSE, &viewM[0][0]);
		glUniformMatrix4fv(uniLocP, 1, GL_FALSE, &projM[0][0]);

		glDrawArrays(GL_LINE_STRIP, 0, controlPoints.size());
		glBindVertexArray(0);

		if (controlPoints.size() < 4)
			return;
		glBindVertexArray(VAO[1]);
		glDrawArrays(GL_LINE_STRIP, 0, (curvePoints.size()));
		glBindVertexArray(0);


		float* positions = new float[controlPoints.size() * 3]{curvePoints[current].x, curvePoints[current].y, curvePoints[current].z,
			curvePoints[current].x + curveTangents[current].x * 5, curvePoints[current].y + curveTangents[current].y * 5, curvePoints[current].z + curveTangents[current].z * 5 };
		float* colors = new float[controlPoints.size() * 3]{0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f };

		glBindVertexArray(VAO[2]);

		glBindBuffer(GL_ARRAY_BUFFER, VBO[4]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6, (void*)positions, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glBindBuffer(GL_ARRAY_BUFFER, VBO[5]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6, (void*)colors, GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);;

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glDrawArrays(GL_LINE_STRIP, 0, 2);
		glBindVertexArray(0);


		delete[] positions;
		delete[] colors;
	}

	std::vector<glm::vec3> getCurvePoints(){
		return curvePoints;
	};
	std::vector<glm::mat4> getCurveRotation() {
		return curveRotation;
	};
	void setCurrent(int i) {
		current = i;
	}
private:
	void approxCurvePoint(float t, int i) {
		glm::vec4 vecPoint = glm::vec4(pow(t,3), pow(t, 2), t, 1);
		vecPoint *= (float)1 / 6;
		glm::mat4 mat = glm::mat4(
			-1.0f, 3.0f, -3.0f, 1.0f,
			3.0f, -6.0f, 3.0f, 0.0f,
			-3.0f, 0.0f, 3.0f, 0.0f,
			1.0f, 4.0f, 1.0f, 0.0f
		);

		vecPoint = mat * vecPoint;
		float pointX = glm::dot(vecPoint, glm::vec4(controlPoints[i].x, controlPoints[i + 1].x, controlPoints[i + 2].x, controlPoints[i + 3].x));
		float pointY = glm::dot(vecPoint, glm::vec4(controlPoints[i].y, controlPoints[i + 1].y, controlPoints[i + 2].y, controlPoints[i + 3].y));
		float pointZ = glm::dot(vecPoint, glm::vec4(controlPoints[i].z, controlPoints[i + 1].z, controlPoints[i + 2].z, controlPoints[i + 3].z));
		
		curvePoints.push_back(glm::vec3(pointX, pointY, pointZ));

		glm::vec4 vecDer1 = glm::vec4(pow(t, 2) * 3, t * 2, 1, 0);
		vecDer1 *= (float)1 / 6;
		vecDer1 = mat * vecDer1;

		glm::vec3 tang = glm::normalize(glm::vec3(glm::dot(vecDer1, glm::vec4(controlPoints[i].x, controlPoints[i + 1].x, controlPoints[i + 2].x, controlPoints[i + 3].x)),
			glm::dot(vecDer1, glm::vec4(controlPoints[i].y, controlPoints[i + 1].y, controlPoints[i + 2].y, controlPoints[i + 3].y)),
			glm::dot(vecDer1, glm::vec4(controlPoints[i].z, controlPoints[i + 1].z, controlPoints[i + 2].z, controlPoints[i + 3].z))));

		curveTangents.push_back(tang);

		glm::vec4 vecDer2 = glm::vec4(t * 6, 2, 0, 0);
		vecDer2 *= (float)1 / 6;
		vecDer2 = mat * vecDer2;

		glm::vec3 dev2LookAt = glm::normalize(glm::vec3(glm::dot(vecDer2, glm::vec4(controlPoints[i].x, controlPoints[i + 1].x, controlPoints[i + 2].x, controlPoints[i + 3].x)),
			glm::dot(vecDer2, glm::vec4(controlPoints[i].y, controlPoints[i + 1].y, controlPoints[i + 2].y, controlPoints[i + 3].y)),
			glm::dot(vecDer2, glm::vec4(controlPoints[i].z, controlPoints[i + 1].z, controlPoints[i + 2].z, controlPoints[i + 3].z))));

		glm::vec3 norm = glm::cross(tang, dev2LookAt);

		glm::vec3 biNorm = glm::cross(tang, norm);

		curveRotation.push_back(glm::transpose(glm::mat4(
			biNorm.x, -tang.x, norm.x, 0.0f,
			biNorm.y, -tang.y, norm.y, 0.0f,
			biNorm.z, -tang.z, norm.z, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		)));


	}
private:
	std::vector<glm::vec3> controlPoints;
	std::vector<glm::vec3> curvePoints;
	std::vector<glm::vec3> curveTangents;
	std::vector<glm::mat4> curveRotation;
	unsigned int VBO[6];
	unsigned int VAO[3];
	unsigned int curvePointCount = 128;
	int current = 0;
};


class Transform {
public:
	Transform() {
		matrix = glm::mat4(1);
	}
	void move(float x, float y, float z) {
		matrix = glm::translate(matrix, glm::vec3(x, y, z));
	}
	void scale(float x, float y, float z) {
		matrix = glm::scale(matrix, glm::vec3(x, y, z));
	}
	void rotate(float alpha, float x, float y, float z) {
		matrix = glm::rotate(matrix, alpha, glm::vec3(x, y, z));
	}
	glm::mat4 getMatrix() {
		return matrix;
	}

protected:
	glm::mat4 matrix;
};

class Camera : Transform {
public:
	void setViewMatrix(glm::vec3 eye, glm::vec3 center, glm::vec3 up) {
		matrix = glm::lookAt(eye, center, up);
	}
	glm::mat4 getViewMatrix() {
		return matrix;
	}
	void setProjMatrix(double left, double right, double bottom, double top, double n, double f) {
		projMatrix = glm::frustum(left, right, bottom, top, n, f);
	}
	glm::mat4 getProjMatrix() {
		return projMatrix;
	}



private:
	glm::mat4 projMatrix;
};


class Mesh {
public:

	void LoadMesh(const aiScene* scene) {
		aiMesh* mesh = scene->mMeshes[0];
		glGenVertexArrays(1, &VAO);
		glGenBuffers(2, VBO);
		glGenBuffers(1, &EBO);
		calculateBoundingBox(mesh);

		indeksiraniVrhovi = new float[mesh->mNumVertices * 3]{};
		indeksiraneNormale = new float[mesh->mNumVertices * 3]{};
		indeksi = new unsigned int[mesh->mNumFaces * 3]{};

		numVert = mesh->mNumVertices;
		numFaces = mesh->mNumFaces;

		for (int i = 0; i < mesh->mNumVertices; i++) {
			indeksiraniVrhovi[i * 3] = (float)mesh->mVertices[i].x;
			indeksiraniVrhovi[i * 3 + 1] = (float)mesh->mVertices[i].y;
			indeksiraniVrhovi[i * 3 + 2] = (float)mesh->mVertices[i].z;
			indeksiraneNormale[i * 3] = (float)1;
			indeksiraneNormale[i * 3 + 1] = (float)1;
			indeksiraneNormale[i * 3 + 2] = (float)1;
		}

		for (int i = 0; i < mesh->mNumFaces; i++) {
			for (int j = 0; j < mesh->mFaces[i].mNumIndices; j++) {
				indeksi[i * 3 + j] = mesh->mFaces[i].mIndices[j];
			}
		}

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * numVert * 3, (void*)(indeksiraniVrhovi), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * numVert * 3, (void*)(indeksiraneNormale), GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * numFaces * 3, (void*)(indeksi), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		glBindVertexArray(0);
	}

	void calculateBoundingBox(aiMesh* mesh) {
		minX = mesh->mVertices[0].x;
		maxX = mesh->mVertices[0].x;
		minY = mesh->mVertices[0].y;
		maxY = mesh->mVertices[0].y;
		minZ = mesh->mVertices[0].z;
		maxZ = mesh->mVertices[0].z;
		for (int i = 0; i < mesh->mNumVertices; i++) {
			if (mesh->mVertices[i].x < minX) minX = mesh->mVertices[i].x;
			if (mesh->mVertices[i].x > maxX) maxX = mesh->mVertices[i].x;
			if (mesh->mVertices[i].y < minY) minY = mesh->mVertices[i].y;
			if (mesh->mVertices[i].y > maxY) maxY = mesh->mVertices[i].y;
			if (mesh->mVertices[i].z < minZ) minZ = mesh->mVertices[i].z;
			if (mesh->mVertices[i].z > maxZ) maxZ = mesh->mVertices[i].z;
		}

	}

	std::vector<std::pair<float, float>> getBoundingBox() {
		return { {minX, maxX}, {minY, maxY}, {minZ, maxZ} };
	}
	
	void applyTransform(glm::mat4 transformMatrix, glm::mat4 viewMatrix, glm::mat4 projMatrix, Shader* sjencar) {
		glBindVertexArray(VAO);
		GLuint lokacijaUniformVarijable = glGetUniformLocation(sjencar->ID, "model");
		GLuint lokacijaViewVarijable = glGetUniformLocation(sjencar->ID, "view");
		GLuint lokacijaProjVarijable = glGetUniformLocation(sjencar->ID, "projection");
		glUniformMatrix4fv(lokacijaUniformVarijable, 1, GL_FALSE, &transformMatrix[0][0]);
		glUniformMatrix4fv(lokacijaViewVarijable, 1, GL_FALSE, &viewMatrix[0][0]);
		glUniformMatrix4fv(lokacijaProjVarijable, 1, GL_FALSE, &projMatrix[0][0]);
		glDrawElements(GL_TRIANGLES, (getNumFaces()) * 3, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	GLuint getVAO() {
		return VAO;
	}

	unsigned int getNumFaces() {
		return numFaces;
	}

private:
	float* indeksiraniVrhovi;
	float* indeksiraneNormale;
	unsigned int* indeksi;
	float minX;
	float maxX;
	float minY;
	float maxY;
	float minZ;
	float maxZ;
	GLuint VAO;
	GLuint VBO[2];
	GLuint EBO;
	unsigned int numVert;
	unsigned int numFaces;
};

class Object {

public:
	Object(std::string pathName, std::string file, Shader* loadSjencar) {

		tMatrix = Transform();
		sjencar = loadSjencar;
		

		Assimp::Importer importer;

		std::string path(pathName);
		std::string dirPath(path, 0, path.find_last_of("\\/"));
		std::string resPath(dirPath);
		resPath.append("\\resources"); //za linux pretvoriti u forwardslash
		std::string objPath(resPath);
		objPath.append(file); //za linux pretvoriti u forwardslash

		const aiScene* scene = importer.ReadFile(objPath.c_str(),
			aiProcess_CalcTangentSpace |
			aiProcess_Triangulate |
			aiProcess_JoinIdenticalVertices |
			aiProcess_SortByPType);

		mesh.LoadMesh(scene);

		projectionMatrix = glm::frustum(-0.5, 0.5, -0.5, 0.5, 1.0, 100.0);
		viewMatrix = glm::lookAt(glm::vec3(3, 4, 1), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
	}

	void drawObject(glm::mat4 viewMatrix, glm::mat4 projMatrix) {
		//glUseProgram(sjencar->ID);
		//GLuint VAO = mesh.getVAO();
		//glBindVertexArray(VAO);
		//glDrawElements(GL_TRIANGLES, (mesh.getNumFaces()) * 3, GL_UNSIGNED_INT, 0);
		//glBindVertexArray(0);
		
		//matrix = glm::rotate(matrix, (float)0.0004, glm::vec3(1, 0.2, 0.8));
		
		//viewMatrix = glm::rotate(viewMatrix, 0.0001f ,glm::vec3(0, 1, 0));

		mesh.applyTransform(glm::translate(glm::mat4(1), glm::vec3(moveX, moveY, moveZ)) * rotation * tMatrix.getMatrix(), viewMatrix, projMatrix, sjencar);
		//mesh.applyTransform(glm::translate(glm::mat4(1), glm::vec3(moveX, moveY, moveZ)) * glm::rotate(glm::translate(glm::mat4(1), glm::vec3(3, 0, 1)), 1.f, glm::vec3(0, 1, 0)) * tMatrix.getMatrix(), viewMatrix, projMatrix, sjencar);
		//mesh.applyTransform(glm::translate(glm::mat4(1), glm::vec3(moveX, moveY, moveZ)) * glm::rotate(glm::translate(glm::mat4(1), glm::vec3(2, 0, 3)), 2.f, glm::vec3(0, 1, 0)) * tMatrix.getMatrix(), viewMatrix, projMatrix, sjencar);
	}

	void normalize() {
		std::vector<std::pair<float, float>> bBox = mesh.getBoundingBox();
		float x = (bBox[0].first + bBox[0].second) / 2;
		float y = (bBox[1].first + bBox[1].second) / 2;
		float z = (bBox[2].first + bBox[2].second) / 2;

		float max = max3(bBox[0].second - bBox[0].first, bBox[1].second - bBox[1].first, bBox[2].second - bBox[2].first);
		float scale = 2 / max;
		//tMatrix.rotate(1.5, 1, 0.2, 0.95);
		//tMatrix.scale(0.5, 0.5, 0.5);
		
		tMatrix.scale(scale, scale, scale);
		tMatrix.move(-x, -y, -z);

		
		//mesh.applyTransform((projectionMatrix * viewMatrix) * tMatrix.getMatrix(), viewMatrix, projMatrix, sjencar);
	}

private:
	Mesh mesh;
	Transform tMatrix;
	Shader* sjencar;
	GLint lokacijaUniformVarijable;
	glm::mat4 viewMatrix = glm::mat4(1); 
	glm::mat4 projectionMatrix = glm::mat4(1);
};

class Renderer {
public:
	Renderer(Shader* loadSjencar) {
		sjencar = loadSjencar;
		camera.setViewMatrix(eye, center, lookUp);
		camera.setProjMatrix(-0.5, 0.5, -0.5, 0.5, 1.0, 100.0);
		calculateVecs();
		updateView();
	}
	void addObject(std::string pathName, std::string file) {
		objects.push_back({ pathName, file, sjencar });
		objects.back().normalize();
	}
	void addCurve(Curve* curve) {
		this->curve = curve;
	}

	void calculateVecs() {
		glm::vec3 norm = eye - center;
		glm::vec3 point = eye + lookUp;
		float t = (norm.x * (eye.x - point.x) + norm.y * (eye.y - point.y) + norm.z * (eye.z - point.z)) / (std::pow(norm.x, 2) + std::pow(norm.y, 2) + std::pow(norm.z, 2));
		glm::vec3 projPoint = point + t * norm;
		localLookUp = eye - projPoint;
		localLookRight = glm::cross(norm, localLookUp);
	}

	glm::vec3 getEye() {
		glm::vec4 e = glm::vec4(eye, 1.0f);
		e = e * glm::rotate(glm::rotate(glm::mat4(1), TBrotation, localLookRight), LRrotation, glm::vec3(0, 1, 0));
		return glm::vec3(e.x, e.y, e.z) * (1.0f / e.w);;
	}

	void draw() {
		for (auto& obj : objects) {
			obj.drawObject(currentView, camera.getProjMatrix());
		}
		curve->draw(sjencar, currentView, camera.getProjMatrix());
	}

	void updateView() {
		currentView = glm::rotate(glm::rotate(camera.getViewMatrix(), TBrotation, localLookRight), LRrotation, glm::vec3(0, 1, 0));
	}

	void setView(glm::mat4 view) {
		currentView = view;
	}

	glm::mat4 getView() {
		return currentView;
	}


private:
	std::vector<Object> objects;
	Curve* curve;
	glm::mat4 currentView;
	Camera camera;
	Shader* sjencar;
	glm::vec3 eye = glm::vec3(30, 40, 10);
	glm::vec3 center = glm::vec3(0.0, 0.0, 0.0);
	glm::vec3 lookUp = glm::vec3(0.0, 1.0, 0.0);
	glm::vec3 localLookUp;
	glm::vec3 localLookRight;
};

// Test error function
void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	std::cout << "[OpenGL Error](" << type << ") " << message << std::endl;
}


int main(int argc, char* argv[]) {
	std::cout << argv[0] << std::endl;
	/*********************************************************************************************/
	//postavljanje OpenGL konteksta, dohvacanje dostupnih OpenGL naredbi
	GLFWwindow* window;

	glfwInit();
	gladLoadGL();

	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);


	window = glfwCreateWindow(mWidth, mHeight, "Zadatak X", nullptr, nullptr);
	// provjeri je li se uspio napraviti prozor
	if (window == nullptr) {
		fprintf(stderr, "Failed to Create OpenGL Context");
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);

	// dohvati sve dostupne OpenGL funkcije
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		fprintf(stderr, "Failed to initialize GLAD");
		exit(-1);
	}
	fprintf(stderr, "OpenGL %s\n", glGetString(GL_VERSION));


	glEnable(GL_DEPTH_TEST); //ukljuci z spremnik (prikazuju se oni fragmenti koji su najblizi promatracu)
	glDepthFunc(GL_LESS);

	glEnable(GL_CULL_FACE); //ukljuci uklanjanje straznjih poligona -- za ovaj primjer je iskljuceno
	glCullFace(GL_BACK); 

	glClearColor(0.15, 0.1, 0.1, 1); //boja brisanja platna izmedu iscrtavanja dva okvira


	glfwSwapInterval(0); //ne cekaj nakon iscrtavanja (vsync

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); //funkcija koja se poziva prilikom mijenjanja velicine prozora

	glfwSetMouseButtonCallback(window, mousePressedOrReleased);

	glfwSetKeyCallback(window, keyPressed);

	Shader* sjencar = loadShader(argv[0], "shader");

	Renderer r(sjencar);
	Curve c = Curve();

	r.addObject(argv[0], "\\galleon\\galleon.obj");
	r.addCurve(&c);
	//r.addObject(argv[0], "\\kocka\\kocka.obj");

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Enable debug output
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(MessageCallback, 0);


	c.addPoint({ 0.0f, 0.0f, -30.0f });
	c.addPoint({ 0.0f, 10.0f, -25.0f });
	c.addPoint({ 10.0f, 10.0f, -20.0f });
	c.addPoint({ 10.0f, 0.0f, -15.0f });
	c.addPoint({ 0.0f, 0.0f, -10.0f });
	c.addPoint({ 0.0f, 10.0f, -5.0f });
	c.addPoint({ 10.0f, 10.0f, 0.0f });
	c.addPoint({ 10.0f, 0.0f, 5.0f });
	c.addPoint({ 0.0f, 0.0f, 10.0f });
	c.addPoint({ 0.0f, 10.0f, 15.0f });
	c.addPoint({ 10.0f, 10.0f, 20.0f });
	c.addPoint({ 10.0f, 0.0f, 25.0f });

	std::vector<glm::vec3> curvePoints = c.getCurvePoints();
	std::vector<glm::mat4> curveRotation = c.getCurveRotation();
	float x = curvePoints.size();

	glm::vec3 move = curvePoints[0];
	rotation = curveRotation[0];

	moveX = move.x;
	moveY = move.y;
	moveZ = move.z;

	while (glfwWindowShouldClose(window) == false) {
		
		glUseProgram(sjencar->ID);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (lMouse) {
			double xpos, ypos;
			glfwGetCursorPos(window, &xpos, &ypos);
			LRrotation = oldLRrotation + (xpos - oldxpos) * 0.002;
			TBrotation = oldTBrotation + (ypos - oldypos) * 0.002;
			r.updateView();
		}

		if (addControlPoint)
		{
			c.addPoint(r.getEye());
			addControlPoint = false;
		}
		if (animate) {
			x = 0;
		}
		if (x < curvePoints.size()-1)
		{

			//r.setView(glm::lookAt(c.interCurvePoint(x), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)));

			glm::vec3 move = curvePoints[(int)x];
			rotation = curveRotation[(int)x];
			c.setCurrent((int)x);

			moveX = move.x;
			moveY = move.y;
			moveZ = move.z;

			x += 0.02;

			animate = false;
		}

		r.draw();
		glfwSwapBuffers(window);
		glfwPollEvents();

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);
	}
	glfwTerminate();

	return EXIT_SUCCESS;
}