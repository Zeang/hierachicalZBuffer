#include "model.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

/*
 * @brief constructor, load info from the file
 * @detail use assimp parse the load format
 * @param filepath the model file path
 */
Model::Model(const std::string& filepath) {
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filepath,
		aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		throw std::runtime_error(importer.GetErrorString());
	}

	_processNode(scene->mRootNode, scene);
}


/*
 * @brief get all triangle faces in vertices - indices format
 * @param vertices of the triangle as output
 * @param vertex indices of the triangle face as output
 */
void Model::getFaces(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices) {
	
	for (const auto& mesh : _meshes) {
		vertices.insert(vertices.end(), mesh.vertices.begin(), mesh.vertices.end());
		indices.insert(indices.end(), mesh.indices.begin(), mesh.indices.end());
	}
}


/*
 * @brief process the current aiNode and recursively process its children
 * @param node assimp node
 * @param scene assimp scene
 */
void Model::_processNode(aiNode* node, const aiScene* scene) {
	for (unsigned int i = 0; i < node->mNumMeshes; ++i) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		_meshes.push_back(_processMesh(mesh, scene));
	}

	for (unsigned int i = 0; i < node->mNumChildren; ++i) {
		_processNode(node->mChildren[i], scene);
	}
}


/*
 * @brief process the mesh to get its vertex and texture
 * @param mesh assimp mesh stored in aiNode
 * @param scene assimp scene
 * @return class Mesh as defined
 */
Mesh Model::_processMesh(aiMesh* mesh, const aiScene* scene) {
	std::vector<Vertex> vertices;
	for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
		Vertex vertex;
		vertex.position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);

		if (mesh->HasNormals()) {
			vertex.normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
		}

		if (mesh->mTextureCoords[0]) {
			vertex.uv = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
		}
		else {
			vertex.uv = glm::vec2(0.0f, 0.0f);
		}

		vertices.push_back(vertex);
	}

	std::vector<uint32_t> indices;
	for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}

	std::vector<Texture> textures;

	return Mesh{ vertices, indices, textures };
}