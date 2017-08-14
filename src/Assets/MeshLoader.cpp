
#include "MeshLoader.hpp"

#include <Log.hpp>
#include <vector>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

namespace Loader {

std::vector<Mesh*> m_meshes;

Mesh* GetLoadedMesh (const std::string filename) {
	for (Mesh* const mesh : m_meshes) {
		if (mesh->m_filename == filename) {
			return mesh;
		}
	}
	return nullptr;
}

Mesh* LoadMesh (const std::string filename) {
	// Check if mesh is already loaded
	Mesh* loadedMesh = GetLoadedMesh(filename);
	if (loadedMesh) {
		return loadedMesh;
	}

	// If mesh wasn't loaded, load it
	std::string filepath = "data/models/" + filename;
	tinyobj::attrib_t                attrib;
	std::vector<tinyobj::shape_t>    shapes;
	std::vector<tinyobj::material_t> materials;
	std::string error_message;
	bool result = tinyobj::LoadObj(&attrib,
	                               &shapes,
	                               &materials,
	                               &error_message,
	                               filepath.c_str());

	if (!error_message.empty()) {
		LOG_ERROR(error_message);
	}

	if (attrib.vertices.size() == 0) {
		LOG_ERROR("No positions in " + filename);
	}

	if (attrib.texcoords.size() == 0) {
		LOG_ERROR("No texture coordinates in " + filename);
	}

	if (attrib.normals.size() == 0) {
		LOG_ERROR("No normals in " + filename);
	}

	if (!result) {
		return nullptr;
	}

  Mesh* mesh = new Mesh();
	// Loop over shapes
	for (const auto& shape : shapes) {
		// Loop over triangles
		for (const auto& index : shape.mesh.indices) {
			glm::vec3 position = {
				attrib.vertices[3 * index.vertex_index + 0],
				attrib.vertices[3 * index.vertex_index + 1],
				attrib.vertices[3 * index.vertex_index + 2]
			};

			glm::vec2 texCoord = {
				attrib.texcoords[2 * index.texcoord_index + 0],
				attrib.texcoords[2 * index.texcoord_index + 1]
			};

			glm::vec3 normal = {
				attrib.normals[3 * index.normal_index + 0],
				attrib.normals[3 * index.normal_index + 1],
				attrib.normals[3 * index.normal_index + 2]
			};

			glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
			// Check for duplicates, currently only comparing by position
			int duplicateIndex = -1;
			for (size_t i = 0; i < mesh->m_vertices.positions.size(); i++) {
				if (position == mesh->m_vertices.positions[i] &&
				    texCoord == mesh->m_vertices.texCoords[i] &&
				    normal   == mesh->m_vertices.normals  [i]) {
					duplicateIndex = i;
					break;
				}
			}

			if (duplicateIndex == -1) {
				mesh->m_indices           .push_back(mesh->m_vertices.positions.size());
				mesh->m_vertices.positions.push_back(position);
				mesh->m_vertices.texCoords.push_back(texCoord);
				mesh->m_vertices.normals  .push_back(normal);
				mesh->m_vertices.colors   .push_back(color);
			} else {
				mesh->m_indices.push_back(duplicateIndex);
			}
		}
	}

	mesh->m_filename = filename;
	mesh->Setup();
	m_meshes.push_back(mesh);
	return mesh;
}
}
