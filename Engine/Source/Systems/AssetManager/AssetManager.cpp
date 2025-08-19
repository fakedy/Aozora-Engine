#include "AssetManager.h"
#include <iostream>
#include <Systems/Logging/Logger.h>
#include <xxHash/xxhash.h>
#include <cereal/cereal.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/types/unordered_map.hpp>
#include <fstream>
#include <random>

#ifdef _WIN32
#include <Windows.h>
#endif // _WIN32



namespace Aozora::Resources {

	AssetManager::AssetManager()
	{
	}

	bool AssetManager::createProject(std::string name)
	{
		wchar_t exePath[512] = {0};
		GetModuleFileNameW(NULL, exePath, 512);
		
		std::filesystem::path exedir = std::filesystem::path(exePath).parent_path();
		// use serializer class instead for most things but whatever
		m_workingDirectory = (exedir / "Projects" / name / "Assets/").string();
		m_projectDir = (exedir / "Projects" / name).string();

		std::error_code errorC;
		std::filesystem::create_directories(m_workingDirectory, errorC);
		// if we get an error
		if (errorC) {
			Log::error(std::format("Failed to create project directory. Reason: {}", errorC.message()));
			return false;
		}


		Log::info(std::format("Successfully created project folder at: {} ", m_projectDir));

		if (std::filesystem::exists((exedir / "Projects" / name / "Assets" / "project.project"))) {
			loadProject();
			std::ifstream os(m_workingDirectory + "manifest.manifest", std::ios::binary);
			if (os) {
				cereal::BinaryInputArchive archive(os);
				archive(m_importRegistry);
			}
		}

		loadAsset("Resources/testcube/testcube.obj");
		//loadAsset("Resources/main_sponza/NewSponza_Main_glTF_003.gltf");
		//loadAsset("Resources/sponza2/sponza.obj");
		loadAsset("Resources/DamagedHelmet/DamagedHelmet.gltf");
		//loadAsset("Resources/survival-guitar-backpack/source/Survival_BackPack_2.fbx");
		return false;
	}


	Asset& AssetManager::getAsset(uint64_t ID)
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

				model.hash = asset.hash;
				m_importRegistry[(path).c_str()] = asset.hash;
				m_assets[asset.hash] = asset;
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

				m_assets[asset.hash] = asset;
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

				m_assets[asset.hash] = asset;
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

				m_assets[asset.hash] = asset;
			}
		}

		if (e == "scene") {
			Asset asset;
			asset.type = AssetType::Scene;
			asset.parentDir = "";
			asset.icon = 0;
			asset.name = filename;
			asset.hash = XXH64(filename.c_str(), filename.length(), 0);
			m_assets[asset.hash] = asset;
		}


		//Log::error("Invalid extension or file");

		saveManifest();

	}

	Model AssetManager::loadModelFromDisk(uint64_t hash)
	{
		Log::info(std::format("Loading model: {} from disk", hash));
		Model model; // parse the file with the hash
		model.hash = hash;

		std::ifstream os(m_workingDirectory + std::to_string(hash) +".model", std::ios::binary);
		cereal::BinaryInputArchive archive(os);
		archive(model);

		return model;
	}

	Mesh AssetManager::loadMeshFromDisk(uint64_t hash)
	{
		Log::info(std::format("Loading mesh: {} from disk", hash));
		Mesh mesh;
		std::ifstream is(m_workingDirectory + std::to_string(hash) + ".mesh", std::ios::binary);
		cereal::BinaryInputArchive archive(is);
		archive(mesh);
		return mesh;
	}

	Texture AssetManager::loadTextureFromDisk(uint64_t hash)
	{
		Log::info(std::format("Loading texture: {} from disk", hash));
		Texture tex;
		// load from disk
		std::ifstream is(m_workingDirectory + std::to_string(hash) + ".texture", std::ios::binary);
		cereal::BinaryInputArchive archive(is);
		archive(tex);
		return tex;
	}

	Material AssetManager::loadMaterialFromDisk(uint64_t hash)
	{
		Log::info(std::format("Loading material: {} from disk", hash));
		Material mat;
		std::ifstream is(m_workingDirectory + std::to_string(hash) + ".material", std::ios::binary);
		cereal::BinaryInputArchive archive(is);
		archive(mat);
		return mat;
	}

	Skybox AssetManager::loadSkyboxFromDisk(uint64_t hash)
	{
		Log::info(std::format("Loading skybox: {} from disk", hash));
		Skybox skybox;
		std::ifstream is(m_workingDirectory + std::to_string(hash) + ".skybox", std::ios::binary);
		cereal::BinaryInputArchive archive(is);
		archive(skybox);
		return skybox;
	}

	Scene AssetManager::loadSceneFromDisk(uint64_t hash)
	{

		Log::info(std::format("Loading scene: {} from disk", hash));
		Scene scene;
		std::ifstream is(m_workingDirectory + std::to_string(hash) + ".scene", std::ios::binary);
		cereal::BinaryInputArchive archive(is);
		archive(scene);
		return scene;

	}

	uint64_t AssetManager::saveSceneToDisk(Scene& scene)
	{
		Log::info(std::format("Saving scene to disk"));
		// if scene is not loaded
		if (m_assets.find(scene.hash) == m_assets.end()) {

			Asset asset;
			asset.type = AssetType::Scene;
			asset.parentDir = "";
			asset.icon = 0;
			asset.name = scene.m_sceneName;
			asset.hidden = false;
			asset.hash = scene.hash;
			m_assets[asset.hash] = asset;

		}
		{
			scene.takeSnapshot();
			std::ofstream os(m_workingDirectory + std::to_string(scene.hash) + ".scene", std::ios::binary);
			cereal::BinaryOutputArchive archive(os);
			archive(scene);
		}

		m_importRegistry[scene.m_sceneName] = scene.hash;

		saveManifest();
		return scene.hash;
	}

	void AssetManager::saveManifest()
	{
		// update the manifest
		std::ofstream os(m_workingDirectory + "manifest.manifest", std::ios::binary);
		cereal::BinaryOutputArchive archive(os);
		archive(m_importRegistry);
	}

	void AssetManager::saveProject()
	{
		Log::info(std::format("Saving project to disk"));
		std::ofstream os(m_workingDirectory + "project.project", std::ios::binary);
		cereal::BinaryOutputArchive archive(os);
		archive(m_assets);
		saveManifest();
	}

	void AssetManager::loadProject()
	{
		Log::info(std::format("Reading scene from disk"));
		std::ifstream is(m_workingDirectory + "project.project", std::ios::binary);
		cereal::BinaryInputArchive archive(is);
		archive(m_assets);
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

		Texture cubeMapTexture = m_textureLoader.loadCubemap(paths);
		skybox.cubeMapTexture = cubeMapTexture.id;

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

		saveManifest();

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

