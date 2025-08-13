#include "AssetManager.h"
#include <iostream>
#include <Systems/Logging/Logger.h>
#include <xxHash/xxhash.h>
#include <cereal/cereal.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/types/unordered_map.hpp>
#include <fstream>

namespace Aozora::Resources {

	AssetManager::AssetManager()
	{
		


	}

	bool AssetManager::createProject(std::string name)
	{
		// use serializer class instead for most things but whatever
		m_workingDirectory = "Projects/" + name + "/Assets/";
		m_projectDir = "Projects/" + name;
		

		if (std::filesystem::create_directories(m_workingDirectory)) {
			Log::info("Successfully created project folder");
			// load base assets and serialize to store binary data

			loadAsset("Resources/testcube/testcube.obj");
			return true;
		}

		Log::error("Failed to create project directory. It may already exist.");


		loadAsset("Resources/testcube/testcube.obj");
		return false;
	}


	Asset& AssetManager::getAsset(uint32_t ID)
	{
		if (m_assets.find(ID) != m_assets.end()) {
			return m_assets[ID];
		}
		throw std::out_of_range(std::format("Asset with ID: {} doesn't exist", ID));
	}

	void AssetManager::loadAsset(const std::string& path)
	{
		// This is for prototyping and getting a grasp around the system im designing
		// temp hack before i introduce the filedialogue
		uint32_t extensionIndex = path.find_last_of('.') + 1; // if a file doesnt have an extension we explode like the KUOW Mazda incident
		std::string e = path.substr(extensionIndex);
		std::string filename = path.substr(path.find_last_of('/')+1, path.find_last_of('.'));

		if (e == "obj" || e == "gltf" || e == "fbx") {

			IntermediateModel iModel = m_modelLoader.loadModel(path);
			Model& model = iModel.model;

			// serialize the model
			{
				Asset asset;
				asset.type = AssetType::Model;
				asset.parentDir = "";
				asset.icon = 0;
				asset.name = filename;
				asset.hash = XXH64(path.c_str(), path.length(), 0);
				std::ofstream os(m_workingDirectory + std::to_string(asset.hash) + ".model", std::ios::binary);
				cereal::BinaryOutputArchive archive(os);
				archive(iModel.model);

				m_importRegistry[(path).c_str()] = asset.hash;
				m_assets[m_nextAssetID] = asset;
				m_nextAssetID++;
			}

			// serialize meshes
			for (auto& mesh : iModel.meshes) {
				Asset asset;
				asset.type = AssetType::Mesh;
				asset.parentDir = "";
				asset.icon = 0;
				asset.name = filename;
				asset.hash = mesh.id;
				asset.hidden = true;

				std::ofstream os(m_workingDirectory + std::to_string(mesh.id) + ".mesh", std::ios::binary);
				cereal::BinaryOutputArchive archive(os);
				archive(mesh);

				m_assets[m_nextAssetID] = asset;
				m_nextAssetID++;
			}

			// serialize materials
			for (auto& mat : iModel.materials) {
				Asset asset;
				asset.type = AssetType::Material;
				asset.parentDir = "";
				asset.icon = 0;
				asset.name = mat.name; // take asset name
				asset.hash = mat.ID;
				asset.hidden = true;

				std::ofstream os(m_workingDirectory + std::to_string(mat.ID) + ".material", std::ios::binary);
				cereal::BinaryOutputArchive archive(os);
				archive(mat);

				m_assets[m_nextAssetID] = asset;
				m_nextAssetID++;
			}

			
			// serialize textures
			for (auto& tex : iModel.textures) {
				Asset asset;
				asset.type = AssetType::Texture;
				asset.parentDir = "";
				asset.icon = 0;
				asset.name = filename;
				asset.hash = tex.id;
				asset.hidden = true;

				std::ofstream os(m_workingDirectory + std::to_string(tex.id) + ".texture", std::ios::binary);
				cereal::BinaryOutputArchive archive(os);
				archive(tex);

				m_assets[m_nextAssetID] = asset;
				m_nextAssetID++;
			}


		}

		if (e == "scene") {
			Asset asset;
			asset.type = AssetType::Scene;
			asset.parentDir = "";
			asset.icon = 0;
			asset.name = filename;
			asset.hash = XXH64(filename.c_str(), filename.length(), 0);
			m_assets[m_nextAssetID] = asset;
			m_nextAssetID++;
		}


		//Log::error("Invalid extension or file");

		// update the manifest
		std::ofstream os(m_workingDirectory + "manifest.manifest", std::ios::binary);
		cereal::BinaryOutputArchive archive(os);
		archive(m_importRegistry);

	}

	Model AssetManager::loadModel(uint64_t hash)
	{
		Model model; // parse the file with the hash


		std::ifstream os(m_workingDirectory + std::to_string(hash) +".model", std::ios::binary);
		cereal::BinaryInputArchive archive(os);
		archive(model);

		return model;
	}

	Mesh AssetManager::loadMesh(uint64_t hash)
	{
		Mesh mesh;
		std::ifstream os(m_workingDirectory + std::to_string(hash) + ".mesh", std::ios::binary);
		cereal::BinaryInputArchive archive(os);
		archive(mesh);
		return mesh;
	}

	Texture AssetManager::loadTexture(uint64_t hash)
	{
		Texture tex;
		std::ifstream os(m_workingDirectory + std::to_string(hash) + ".texture", std::ios::binary);
		cereal::BinaryInputArchive archive(os);
		archive(tex);
		return tex;
	}

	Material AssetManager::loadMaterial(uint64_t hash)
	{
		Material mat;
		std::ifstream os(m_workingDirectory + std::to_string(hash) + ".material", std::ios::binary);
		cereal::BinaryInputArchive archive(os);
		archive(mat);
		return mat;
	}


	std::vector<std::reference_wrapper<Asset>> AssetManager::getLoadedAssets()
	{
		std::vector<std::reference_wrapper<Asset>> data;
		data.reserve(m_assets.size());
		for (auto& pair : m_assets) {
			data.push_back(pair.second);
		}
		return data;
	}


}

