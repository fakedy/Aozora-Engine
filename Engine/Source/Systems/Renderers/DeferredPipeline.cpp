#include "DeferredPipeline.h"
#include <Systems/Input.h>
#include <Systems/Logging/Logger.h>

namespace Aozora {

	DeferredPipeline::DeferredPipeline(uint32_t width, uint32_t height) {

		// setting up framebuffers for deferred rendering
		setupGbuffer();
		setupRenderBuffer();
		setupPostfxBuffer();

		m_outputAttachment = postfxBuffer->m_colorAttachments[0];

		// We dont need normals but whatever
		screenQuad.meshData.vertices = {
			{ {-1.0f, -1.0f, 0.0f}, {-1.0f, -1.0f, 0.0f}, {-1.0f, -1.0f, 0.0f}, {0.0f, 0.0f} }, // 0: Bottom-left
			{ { 1.0f, -1.0f, 0.0f}, {-1.0f, -1.0f, 0.0f}, {-1.0f, -1.0f, 0.0f}, {1.0f, 0.0f} }, // 1: Bottom-right
			{ {-1.0f,  1.0f, 0.0f}, {-1.0f, -1.0f, 0.0f}, {-1.0f, -1.0f, 0.0f}, {0.0f, 1.0f} }, // 2: Top-left
			{ { 1.0f,  1.0f, 0.0f}, {-1.0f, -1.0f, 0.0f}, {-1.0f, -1.0f, 0.0f}, {1.0f, 1.0f} }  // 3: Top-right
		};

		screenQuad.meshData.indices = {
			0, 1, 3, // First triangle
			3, 2, 0  // Second triangle
		};

		screenQuad.bufferData();

		// illegal type stuff
		glGenVertexArrays(1, &skybox.VAO);

		PSO gbufferPSO;
		gbufferPSO.stateCommands = []() {
			glEnable(GL_DEPTH_TEST);
			glDepthMask(GL_TRUE);
			glDisable(GL_BLEND);
			};

		glGenBuffers(1, &m_indirectBuffer);
		glGenBuffers(1, &m_VBO);
		glGenBuffers(1, &m_EBO);
		glGenVertexArrays(1, &m_VAO);

		glBindVertexArray(m_VAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
		// vertex positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex), (void*)0);
		// vertex normals
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex), (void*)offsetof(Mesh::Vertex, Normal));
		// tangents
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex), (void*)offsetof(Mesh::Vertex, Tangent));
		// vertex texture coords
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex), (void*)offsetof(Mesh::Vertex, TexCoords));

		glBindVertexArray(0);

		glGenBuffers(1, &objectSSBO);

		// can be set once
		m_defaultShader.setInt("gPosition", 0);
		m_defaultShader.setInt("gNormal", 1);
		m_defaultShader.setInt("gAlbedo", 2);
		m_defaultShader.setInt("gEmissive", 3);
		m_defaultShader.setInt("gProperties", 4);
		m_defaultShader.setInt("gDepth", 5);

	}

	// would be call to setup settings for this to allow customization
	void DeferredPipeline::resize(uint32_t width, uint32_t height)
	{
		gBuffer->updateTexture(width, height);
		renderBuffer->updateTexture(width, height);
		postfxBuffer->updateTexture(width, height);

	}

	void DeferredPipeline::execute(IrenderAPI& renderAPI, ResourceManager& resourceManager,
		Scene& scene, entt::entity camera, uint32_t width, uint32_t height, bool isEditor)
	{

		if (Input::getKeyPressed(Input::Key::F6)) {
			Log::info("Recompiling shaders");
			m_gBufferShader.recompile();
			m_defaultShader.recompile();
			m_postfxShader.recompile();
			m_gridShader.recompile();
		}


		auto MeshTransformEntities = scene.getRegistry().view<const MeshComponent, TransformComponent>(); // register of all mesh components
		auto skyboxes = scene.getRegistry().view<const SkyboxComponent>();
		auto cameraView = scene.getRegistry().view<CameraComponent>();

		// check if viewport have a camera
		if (camera != entt::null) {

			// gBuffer pass
			gBuffer->bind();

			renderAPI.clear(0.0f, 0.0f, 0.0f, 1.0f);
			glViewport(0, 0, width, height);

			// get the camera from the cameraID
			auto& current_camera = cameraView.get<CameraComponent>(camera);
			current_camera.m_viewPortWidth = width;
			current_camera.m_viewPortHeight = height;

			glDisable(GL_BLEND);
			// use shader
			glUseProgram(m_gBufferShader.ID);
			m_gBufferShader.setMat4("view", current_camera.getView());
			m_gBufferShader.setMat4("proj", current_camera.getProjection());

			// bind vao
			glBindVertexArray(m_VAO);

			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, objectSSBO);
			glBindBuffer(GL_DRAW_INDIRECT_BUFFER, m_indirectBuffer);

			glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, (GLvoid*)0, m_commands.size(), 0);

			glBindVertexArray(0);
			glBindBuffer(GL_DRAW_INDIRECT_BUFFER, 0);
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
			
			//glEnable(GL_BLEND);

			gBuffer->unbind();
			
			// copy the depth buffer to the lighting buffer
			glBindFramebuffer(GL_READ_FRAMEBUFFER, gBuffer->framebufferID);
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, renderBuffer->framebufferID);
			glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);


			renderBuffer->bind();
			// light pass

			glDisable(GL_BLEND);
			glClearColor(0.0, 0.0, 0.0, 1.0);
			glClear(GL_COLOR_BUFFER_BIT);
			glUseProgram(m_defaultShader.ID);

			auto& camera_transform = MeshTransformEntities.get<TransformComponent>(camera);
			m_defaultShader.setVec3fv("cameraPos", camera_transform.pos);
			m_defaultShader.setMat4("invView", glm::inverse(current_camera.getView()));

			GLuint gBufferTextures[] = {
				gBuffer->m_colorAttachments[0],
				gBuffer->m_colorAttachments[1],
				gBuffer->m_colorAttachments[2],
				gBuffer->m_colorAttachments[3],
				gBuffer->m_colorAttachments[4],
				gBuffer->m_depthTextureID
			};
			glBindTextures(0, 6, gBufferTextures);


			ResourceManager::ResourceContainer& map = resourceManager.m_containerMap[scene.hash];

			auto skyboxes = scene.getRegistry().view<const SkyboxComponent>();
			auto& skyboxComponent = skyboxes.get<SkyboxComponent>(skyboxes.front()); // hack (crash if we dont have a skybox entity)
			Skybox& skyboxObject = map.m_loadedSkyboxes[skyboxComponent.id];
			Texture& cubeMapTexture = map.m_loadedTextures[skyboxObject.cubeMapTexture];
			Texture& irradienceMapTexture = map.m_loadedTextures[skyboxObject.irradienceMapTexture];

			glActiveTexture(GL_TEXTURE6);
			glBindTexture(GL_TEXTURE_CUBE_MAP, irradienceMapTexture.gpuID);
			m_defaultShader.setInt("irradianceMap", 6);

			glActiveTexture(GL_TEXTURE7);
			glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTexture.gpuID);
			m_defaultShader.setInt("skybox", 7); // temp for calculations

			glDepthMask(GL_FALSE);
			renderLights(scene, current_camera.getView());
			glDepthMask(GL_TRUE);

			// render skybox
			glDepthFunc(GL_LEQUAL);
			glDisable(GL_CULL_FACE);
			glUseProgram(m_skyboxShader.ID);
			glBindVertexArray(skybox.VAO);
			for (const auto skybox : skyboxes) {
				m_skyboxShader.setInt("skybox", 0);
				auto& skyboxComponent = skyboxes.get<SkyboxComponent>(skybox);
				glm::mat4 viewWithoutRotation = glm::mat4(glm::mat3(current_camera.getView()));
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTexture.gpuID);
				m_skyboxShader.setMat4("view", viewWithoutRotation);
				m_skyboxShader.setMat4("proj", current_camera.getProjection());
				glDrawArrays(GL_TRIANGLES, 0, 36);
				glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

			}
			glBindVertexArray(0);
			glDepthFunc(GL_LESS);
			glEnable(GL_CULL_FACE);

			if (isEditor) {
				drawGrid(current_camera, camera_transform.pos, width, height);
			}

			glEnable(GL_CULL_FACE);
			 
			renderBuffer->unbind();

			postfxBuffer->bind();
			postfxPass();
			postfxBuffer->unbind();
			
		}

	}

	void DeferredPipeline::drawGrid(const Aozora::CameraComponent& camera, const glm::vec3& cameraPos, uint32_t width, uint32_t height) {
		// editor grid
		glDisable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_FALSE);

		glUseProgram(m_gridShader.ID);
		m_gridShader.setMat4("view", camera.getView());
		m_gridShader.setMat4("proj", camera.getProjection());
		m_gridShader.setVec3fv("cameraPos", cameraPos);

		const GLint screenSizeLoc = glGetUniformLocation(m_gridShader.ID, "screenSize");
		glUniform2f(screenSizeLoc, static_cast<float>(width), static_cast<float>(height));

		glBindVertexArray(skybox.VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

		glDepthMask(GL_TRUE);
		glDisable(GL_BLEND);
		glEnable(GL_CULL_FACE);
	}

	uint32_t DeferredPipeline::getFinalImage()
	{
		// for debug
		if (Input::getKeyPressed(Input::Key::F1)) {
			m_outputAttachment = postfxBuffer->m_colorAttachments[0]; // postfx
			Log::info("output image: postfx");
		}
		if (Input::getKeyPressed(Input::Key::F2)) {
			m_outputAttachment = renderBuffer->m_colorAttachments[0]; // lighting output
			Log::info("output image: lighting output");
		}
		if (Input::getKeyPressed(Input::Key::F3)) {
			m_outputAttachment = gBuffer->m_colorAttachments[2]; // gbuffer color
			Log::info("output image: gbuffer color");
		}
		if (Input::getKeyPressed(Input::Key::F4)) {
			m_outputAttachment = gBuffer->m_colorAttachments[1]; // normal
			Log::info("output image: gbuffer normal");
		}
		if (Input::getKeyPressed(Input::Key::F5)) {
			m_outputAttachment = gBuffer->m_colorAttachments[4]; // properties
			Log::info("output image: gbuffer properties");
		}

		return m_outputAttachment;
	}
	void DeferredPipeline::setupGbuffer()
	{
		FrameBuffer::FrameBufferSpecification gBufferSpecs;
		FrameBuffer::FrameBufferAttachment positionAttachment;
		FrameBuffer::FrameBufferAttachment normalAttachment;
		FrameBuffer::FrameBufferAttachment albedoAttachment;
		FrameBuffer::FrameBufferAttachment emissiveAttachment;
		FrameBuffer::FrameBufferAttachment propertiesAttachment;
		FrameBuffer::FrameBufferAttachment gBufferDepthAttachment;

		gBufferSpecs.width = 1920;
		gBufferSpecs.height = 1080;

		positionAttachment.textureTarget = FrameBuffer::TextureTarget::TEXTURE_2D;
		positionAttachment.textureFormat = FrameBuffer::TextureFormat::RGBA16F;
		positionAttachment.textureFilter = FrameBuffer::TextureFilter::Nearest;
		positionAttachment.dataType = FrameBuffer::DataType::FLOAT;
		positionAttachment.dataFormat = FrameBuffer::DataFormat::RGBA;

		normalAttachment.textureTarget = FrameBuffer::TextureTarget::TEXTURE_2D;
		normalAttachment.textureFormat = FrameBuffer::TextureFormat::RGBA8;
		normalAttachment.textureFilter = FrameBuffer::TextureFilter::Nearest;
		normalAttachment.dataType = FrameBuffer::DataType::UNSIGNED_BYTE;
		normalAttachment.dataFormat = FrameBuffer::DataFormat::RGBA;

		albedoAttachment.textureTarget = FrameBuffer::TextureTarget::TEXTURE_2D;
		albedoAttachment.textureFormat = FrameBuffer::TextureFormat::RGBA8;
		albedoAttachment.textureFilter = FrameBuffer::TextureFilter::Nearest;
		albedoAttachment.dataType = FrameBuffer::DataType::UNSIGNED_BYTE;
		albedoAttachment.dataFormat = FrameBuffer::DataFormat::RGBA;

		emissiveAttachment.textureTarget = FrameBuffer::TextureTarget::TEXTURE_2D;
		emissiveAttachment.textureFormat = FrameBuffer::TextureFormat::RGBA16F;
		emissiveAttachment.textureFilter = FrameBuffer::TextureFilter::Nearest;
		emissiveAttachment.dataType = FrameBuffer::DataType::UNSIGNED_BYTE;
		emissiveAttachment.dataFormat = FrameBuffer::DataFormat::RGBA;

		propertiesAttachment.textureTarget = FrameBuffer::TextureTarget::TEXTURE_2D;
		propertiesAttachment.textureFormat = FrameBuffer::TextureFormat::RGBA8;
		propertiesAttachment.textureFilter = FrameBuffer::TextureFilter::Nearest;
		propertiesAttachment.dataType = FrameBuffer::DataType::UNSIGNED_BYTE;
		propertiesAttachment.dataFormat = FrameBuffer::DataFormat::RGBA;

		gBufferDepthAttachment.textureTarget = FrameBuffer::TextureTarget::TEXTURE_2D;
		gBufferDepthAttachment.textureFormat = FrameBuffer::TextureFormat::DEPTH24STENCIL8;
		gBufferDepthAttachment.textureFilter = FrameBuffer::TextureFilter::Linear;
		gBufferDepthAttachment.textureWrap = FrameBuffer::TextureWrap::ClampToEdge;
		gBufferDepthAttachment.dataType = FrameBuffer::DataType::UNSIGNED_INT_24_8;
		gBufferDepthAttachment.dataFormat = FrameBuffer::DataFormat::DEPTH_STENCIL;


		gBufferSpecs.attachments.push_back(positionAttachment);
		gBufferSpecs.attachments.push_back(normalAttachment);
		gBufferSpecs.attachments.push_back(albedoAttachment);
		gBufferSpecs.attachments.push_back(emissiveAttachment);
		gBufferSpecs.attachments.push_back(propertiesAttachment);
		gBufferSpecs.attachments.push_back(gBufferDepthAttachment);
		gBuffer = std::make_unique<OpenglFrameBuffer>(gBufferSpecs);

		gBuffer->buffer();
	}
	void DeferredPipeline::setupPostfxBuffer()
	{
		FrameBuffer::FrameBufferSpecification postfxBufferSpecs;
		FrameBuffer::FrameBufferAttachment postfxColorAttachment;
		postfxBufferSpecs.width = 1920;
		postfxBufferSpecs.height = 1080;

		postfxColorAttachment.textureTarget = FrameBuffer::TextureTarget::TEXTURE_2D;
		postfxColorAttachment.textureFormat = FrameBuffer::TextureFormat::RGBA16F;
		postfxColorAttachment.textureFilter = FrameBuffer::TextureFilter::Linear;
		postfxColorAttachment.textureWrap = FrameBuffer::TextureWrap::ClampToEdge;
		postfxColorAttachment.dataType = FrameBuffer::DataType::FLOAT;
		postfxColorAttachment.dataFormat = FrameBuffer::DataFormat::RGBA;

		postfxBufferSpecs.attachments.push_back(postfxColorAttachment);

		postfxBuffer = std::make_unique<OpenglFrameBuffer>(postfxBufferSpecs);
		postfxBuffer->buffer();
	}
	void DeferredPipeline::setupRenderBuffer()
	{
		FrameBuffer::FrameBufferSpecification renderBufferSpecs;
		FrameBuffer::FrameBufferAttachment colorAttachment;
		FrameBuffer::FrameBufferAttachment depthAttachment;

		renderBufferSpecs.width = 1920;
		renderBufferSpecs.height = 1080;

		colorAttachment.textureTarget = FrameBuffer::TextureTarget::TEXTURE_2D;
		colorAttachment.textureFormat = FrameBuffer::TextureFormat::RGBA16F;
		colorAttachment.textureFilter = FrameBuffer::TextureFilter::Linear;
		colorAttachment.textureWrap = FrameBuffer::TextureWrap::ClampToEdge;
		colorAttachment.dataType = FrameBuffer::DataType::FLOAT;
		colorAttachment.dataFormat = FrameBuffer::DataFormat::RGBA;

		depthAttachment.textureTarget = FrameBuffer::TextureTarget::TEXTURE_2D;
		depthAttachment.textureFormat = FrameBuffer::TextureFormat::DEPTH24STENCIL8;
		depthAttachment.textureFilter = FrameBuffer::TextureFilter::Linear;
		depthAttachment.textureWrap = FrameBuffer::TextureWrap::ClampToEdge;
		depthAttachment.dataType = FrameBuffer::DataType::UNSIGNED_INT_24_8;
		depthAttachment.dataFormat = FrameBuffer::DataFormat::DEPTH_STENCIL;

		renderBufferSpecs.attachments.push_back(colorAttachment);
		renderBufferSpecs.attachments.push_back(depthAttachment);

		renderBuffer = std::make_unique<OpenglFrameBuffer>(renderBufferSpecs);
		renderBuffer->buffer();
	}
	void DeferredPipeline::renderLights(Scene& scene, const glm::mat4& view)
	{
		// render lightss
		auto lightView = scene.getRegistry().view<const LightComponent, TransformComponent>();

		int index = 0;
		for (auto entity : lightView) {
			auto& lightComponent = lightView.get<LightComponent>(entity);
			auto& transformComponent = lightView.get<TransformComponent>(entity);
			// kinda whack ngl
			std::string lightPosVar = "lights[" + std::to_string(index) + "].position";
			std::string lightRotVar = "lights[" + std::to_string(index) + "].direction";
			std::string lightColorVar = "lights[" + std::to_string(index) + "].color";
			std::string lightLinearVar = "lights[" + std::to_string(index) + "].linear";
			std::string lightQuadraticVar = "lights[" + std::to_string(index) + "].quadratic";
			std::string lightRadiusVar = "lights[" + std::to_string(index) + "].radius";
			std::string lightPowerVar = "lights[" + std::to_string(index) + "].power";
			std::string lightTypeVar = "lights[" + std::to_string(index) + "].type";


			m_defaultShader.setVec3fv(lightPosVar, glm::mat3(view)*transformComponent.pos); // make this view space
			m_defaultShader.setVec3fv(lightRotVar, glm::mat3(view)*transformComponent.rot); // make this view space
			m_defaultShader.setVec3fv(lightColorVar, lightComponent.color);
			m_defaultShader.setFloat(lightLinearVar, lightComponent.linear);
			m_defaultShader.setFloat(lightQuadraticVar, lightComponent.quadratic);
			m_defaultShader.setFloat(lightRadiusVar, lightComponent.radius);
			m_defaultShader.setFloat(lightPowerVar, lightComponent.power);
			m_defaultShader.setInt(lightTypeVar, lightComponent.type);
			index++;
		}
		m_defaultShader.setInt("activeLights", index);

		screenQuad.drawGeometry();

	}
	void DeferredPipeline::postfxPass()
	{
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(m_postfxShader.ID);

		glActiveTexture(GL_TEXTURE0);
		m_postfxShader.setInt("colorTexture", 0);
		glBindTexture(GL_TEXTURE_2D, renderBuffer->m_colorAttachments[0]);
		screenQuad.drawGeometry();
		glBindTexture(GL_TEXTURE_2D, 0);
	}


	void DeferredPipeline::updateInstanceData(Scene& scene, ResourceManager& resourceManager)
	{
		// transforms, materials etc
		auto MeshTransformEntities = scene.getRegistry().view<const MeshComponent, TransformComponent>();

		uint32_t i = 0;

		m_objectDataVector.resize(MeshTransformEntities.size_hint());
		ResourceManager::ResourceContainer& map = resourceManager.m_containerMap[scene.hash];
		for (const auto entity : MeshTransformEntities) {
			auto& meshComponent = MeshTransformEntities.get<MeshComponent>(entity);
			auto& transformComponent = MeshTransformEntities.get<TransformComponent>(entity);

			// data to be uploaded to gpu so we can access variables inside the shaders
			ObjectData objectData = {}; // set all to 0
			{
				objectData.model = transformComponent.model;
				Material& mat = map.m_loadedmaterials[meshComponent.materialID];

				uint64_t diffuseTextureID = mat.diffuseTexture;
				objectData.diffuseTextureHandle = map.m_loadedTextures[diffuseTextureID].handle;
				objectData.albedo = mat.baseColor;

				uint64_t emissiveTextureID = mat.emissiveTexture;
				objectData.emissiveTextureHandle = map.m_loadedTextures[emissiveTextureID].handle;
				objectData.emissive = mat.emissive;

				uint64_t aoTextureID = mat.aoTexture;
				objectData.aoTextureHandle = map.m_loadedTextures[aoTextureID].handle;
				objectData.ao = mat.ao;

				uint64_t metallicTextureID = mat.metallicTexture;
				objectData.metallicTextureHandle = map.m_loadedTextures[metallicTextureID].handle;
				objectData.metallic = mat.metallic;

				uint64_t roughnessTextureID = mat.roughnessTexture;
				objectData.roughnessTextureHandle = map.m_loadedTextures[roughnessTextureID].handle;
				objectData.roughness = mat.roughness;

				uint64_t normalTextureID = mat.normalTexture;
				objectData.normalTextureHandle = map.m_loadedTextures[normalTextureID].handle;

			}

			m_objectDataVector[i] = objectData;
			i++;
		}
		glBindVertexArray(0);

		m_objectDataVector.resize(i);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, objectSSBO);
		glBufferData(GL_SHADER_STORAGE_BUFFER, m_objectDataVector.size() * sizeof(ObjectData), m_objectDataVector.data(), GL_DYNAMIC_DRAW);
	}
	void DeferredPipeline::updateDrawCommands(Scene& scene, ResourceManager& resourceManager)
	{
		// mesh
		auto MeshTransformEntities = scene.getRegistry().view<const MeshComponent, TransformComponent>();

		// step 1
		// calculate the total size needed and allocate buffers, we will use glBufferSubData to fill the buffers later
		glBindVertexArray(m_VAO);

		uint32_t verticesSize = 0;
		uint32_t indexSize = 0;
		for (const auto entity : MeshTransformEntities) {
			auto& meshComponent = MeshTransformEntities.get<MeshComponent>(entity);
			Mesh::MeshData& data = resourceManager.m_containerMap[scene.hash].m_loadedMeshes[meshComponent.meshID].meshData;
			verticesSize += data.vertices.size() * sizeof(Mesh::Vertex);
			indexSize += data.indices.size() * sizeof(uint32_t);
		}

		// Allocate GPU memory for all vertices and indices of all meshes in the scene
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, verticesSize, nullptr, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexSize, nullptr, GL_STATIC_DRAW);

		m_commands.resize(MeshTransformEntities.size_hint());

		uint32_t i = 0;
		uint32_t baseVertex = 0;
		uint32_t firstIndex = 0;
		uint32_t currentVertexOffsetBytes = 0;
		uint32_t currentIndicesOffsetBytes = 0;

		ResourceManager::ResourceContainer& map = resourceManager.m_containerMap[scene.hash];
		for (const auto entity : MeshTransformEntities) {
			auto& meshComponent = MeshTransformEntities.get<MeshComponent>(entity);
			Mesh::MeshData& data = map.m_loadedMeshes[meshComponent.meshID].meshData;
			uint64_t verticesAmount = data.vertices.size();
			uint64_t indicesAmount = data.indices.size();


			// Upload data to the VBO and EBO with the correct offsets for each mesh
			glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
			glBufferSubData(GL_ARRAY_BUFFER, currentVertexOffsetBytes, verticesAmount * sizeof(Mesh::Vertex), data.vertices.data());

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
			glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, currentIndicesOffsetBytes, indicesAmount * sizeof(uint32_t), data.indices.data());

			// indirect draw command for glMultiDrawElementsIndirect
			DrawElementsIndirectCommand command;
			command.count = indicesAmount;
			command.instanceCount = 1; // draw 1 instance
			command.firstIndex = firstIndex; // draw from index 0
			command.baseVertex = baseVertex; // where the new "object" begins
			command.baseInstance = i; // what object we are on?
			m_commands[i] = command;

			firstIndex += indicesAmount;
			baseVertex += verticesAmount; // add offset
			currentVertexOffsetBytes += verticesAmount * sizeof(Mesh::Vertex);
			currentIndicesOffsetBytes += indicesAmount * sizeof(uint32_t);
			i++;
		}
		glBindVertexArray(0);


		m_commands.resize(i);
		glBindBuffer(GL_DRAW_INDIRECT_BUFFER, m_indirectBuffer);
		glBufferData(GL_DRAW_INDIRECT_BUFFER, m_commands.size() * sizeof(DrawElementsIndirectCommand), m_commands.data(), GL_DYNAMIC_DRAW);
	}


}
