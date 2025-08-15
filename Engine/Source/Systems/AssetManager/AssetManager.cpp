#include "AssetManager.h"
#include <iostream>
#include <Systems/Logging/Logger.h>
#include <xxHash/xxhash.h>
#include <cereal/cereal.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/types/unordered_map.hpp>
#include <fstream>
#include <random>

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

			std::ifstream os(m_workingDirectory + "manifest.manifest", std::ios::binary);
			if (os) {

				cereal::BinaryInputArchive archive(os);
				archive(m_importRegistry);
			}
			loadAsset("Resources/testcube/testcube.obj");
			loadAsset("Resources/sponza2/sponza.obj");
			//loadAsset("Resources/main_sponza/NewSponza_Main_glTF_003.gltf");
			loadAsset("Resources/DamagedHelmet/DamagedHelmet.gltf");
			return true;
		}

		Log::error("Failed to create project directory. It may already exist.");


		
		std::ifstream os(m_workingDirectory + "manifest.manifest", std::ios::binary);
		if (os) {

			cereal::BinaryInputArchive archive(os);
			archive(m_importRegistry);
		}

		loadAsset("Resources/testcube/testcube.obj");
		//loadAsset("Resources/main_sponza/NewSponza_Main_glTF_003.gltf");
		loadAsset("Resources/sponza2/sponza.obj");
		loadAsset("Resources/DamagedHelmet/DamagedHelmet.gltf");
		return false;
	}


	Asset& AssetManager::getAsset(uint32_t ID)
	{
		if (m_assets.find(ID) != m_assets.end()) {
			return m_assets[ID];
		}
		throw std::out_of_range(std::format("Asset with ID: {} doesn't exist", ID));
	}

	// intended for non engine file formats
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

	Model AssetManager::loadModelFromDisk(uint64_t hash)
	{
		Log::info(std::format("Loading model: {} from disk", hash));
		Model model; // parse the file with the hash


		std::ifstream os(m_workingDirectory + std::to_string(hash) +".model", std::ios::binary);
		cereal::BinaryInputArchive archive(os);
		archive(model);

		return model;
	}

	Mesh AssetManager::loadMeshFromDisk(uint64_t hash)
	{
		Log::info(std::format("Loading mesh: {} from disk", hash));
		Mesh mesh;
		std::ifstream os(m_workingDirectory + std::to_string(hash) + ".mesh", std::ios::binary);
		cereal::BinaryInputArchive archive(os);
		archive(mesh);
		return mesh;
	}

	Texture AssetManager::loadTextureFromDisk(uint64_t hash)
	{
		Log::info(std::format("Loading texture: {} from disk", hash));
		Texture tex;
		// load from disk
		std::ifstream os(m_workingDirectory + std::to_string(hash) + ".texture", std::ios::binary);
		cereal::BinaryInputArchive archive(os);
		archive(tex);
		return tex;
	}

	Material AssetManager::loadMaterialFromDisk(uint64_t hash)
	{
		Log::info(std::format("Loading material: {} from disk", hash));
		Material mat;
		std::ifstream os(m_workingDirectory + std::to_string(hash) + ".material", std::ios::binary);
		cereal::BinaryInputArchive archive(os);
		archive(mat);
		return mat;
	}

	Skybox AssetManager::loadSkyboxFromDisk(uint64_t hash)
	{
		Log::info(std::format("Loading skybox: {} from disk", hash));
		Skybox skybox;
		std::ifstream os(m_workingDirectory + std::to_string(hash) + ".skybox", std::ios::binary);
		cereal::BinaryInputArchive archive(os);
		archive(skybox);
		return skybox;
	}

	uint64_t AssetManager::createSkybox()
	{
		Asset asset;
		asset.hidden = true;
		asset.icon = 0;
		asset.name = "Skybox";
		asset.parentDir = "";
		asset.type = AssetType::Skybox;
		asset.hash = getUniqueID();

		Skybox skybox;
		skybox.id = asset.hash;
		m_assets[asset.hash] = asset;

		// default skybox textures
		std::vector<std::string> paths = {
		"Resources/cubemap/px.hdr",
		"Resources/cubemap/nx.hdr",
		"Resources/cubemap/py.hdr",
		"Resources/cubemap/ny.hdr",
		"Resources/cubemap/pz.hdr",
		"Resources/cubemap/nz.hdr"
		};

		// this is unironically so bad design
		Texture cubeMapTexture = m_textureLoader.loadCubemap(paths);

		// confusing situation
		//Texture irradienceMapTexture = m_textureLoader.loadCubemap(paths);

		// temp
		skybox.cubeMapTexture = cubeMapTexture.id;
		skybox.irradienceMapTexture = cubeMapTexture.id;

		{

			std::ofstream os(m_workingDirectory + std::to_string(skybox.id) + ".skybox", std::ios::binary);
			cereal::BinaryOutputArchive archive(os);
			archive(skybox);
		}
		{

			std::ofstream os(m_workingDirectory + std::to_string(cubeMapTexture.id) + ".texture", std::ios::binary);
			cereal::BinaryOutputArchive archive(os);
			archive(cubeMapTexture);
		}

		// update the manifest
		std::ofstream os(m_workingDirectory + "manifest.manifest", std::ios::binary);
		cereal::BinaryOutputArchive archive(os);
		archive(m_importRegistry);

		return skybox.id;
	}

	uint64_t AssetManager::createTexture(const std::string& filePath)
	{

		Texture tex = m_textureLoader.loadTexture(filePath);
		// load from disk
		if (tex.id == 0) {
			// if its already loaded return the hash
			return m_importRegistry[filePath];
		}

		// save to disk
		std::ofstream os(m_workingDirectory + std::to_string(tex.id) + ".texture", std::ios::binary);
		cereal::BinaryOutputArchive archive(os);
		archive(tex);

		{
			// update the manifest
			std::ofstream os(m_workingDirectory + "manifest.manifest", std::ios::binary);
			cereal::BinaryOutputArchive archive(os);
			archive(m_importRegistry);
		}
	
		return tex.id;
	}

	uint64_t AssetManager::getUniqueID()
	{
		std::random_device rd;
		std::mt19937_64 gen(rd());
		std::uniform_int_distribution<uint64_t> dis;

		return dis(gen);
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

