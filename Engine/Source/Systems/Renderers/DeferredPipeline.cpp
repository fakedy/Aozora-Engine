#include "DeferredPipeline.h"
#include "Application.h"

namespace Aozora {

	DeferredPipeline::DeferredPipeline(uint32_t width, uint32_t height) {

		// setting up framebuffers for deferred rendering
		setupGbuffer();
		setupRenderBuffer();
		setupPostfxBuffer();

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
	}




	void DeferredPipeline::resize(uint32_t width, uint32_t height)
	{
		gBuffer->updateTexture(width, height);
		renderBuffer->updateTexture(width, height);
		postfxBuffer->updateTexture(width, height);

	}






	void DeferredPipeline::execute(IrenderAPI& renderAPI, Scene& scene, entt::entity camera, uint32_t width, uint32_t height)
	{


		auto MeshTransformEntities = scene.getRegistry().view<const MeshComponent, TransformComponent>(); // register of all mesh components
		auto skyboxes = scene.getRegistry().view<const SkyboxComponent>();
		auto cameraView = scene.getRegistry().view<CameraComponent>();

		// check if viewport have a camera
		if (camera != entt::null) {
			// gBuffer pass
			gBuffer->bind();
			ResourceManager& resourceManager = Application::getApplication().getResourceManager();


			renderAPI.clear(0.0f, 0.0f, 0.0f, 1.0f);
			glViewport(0, 0, width, height);

			// get the camera from the cameraID
			auto& current_camera = cameraView.get<CameraComponent>(camera);
			current_camera.m_viewPortWidth = width;
			current_camera.m_viewPortHeight = height;


			// submit render command
			//draw commands can be generated on another thread
			std::vector<DrawElementsIndirectCommand> commands;
			std::vector<ObjectData> objectDataVector;

			glDisable(GL_BLEND);
			// use shader
			glUseProgram(m_gBufferShader.ID);
			// bind vao
			glBindVertexArray(m_VAO);

			// create commands
			uint32_t i = 0;
			uint32_t baseVertex = 0;
			uint32_t firstIndex = 0;

			m_gBufferShader.setMat4("view", current_camera.getView());
			m_gBufferShader.setMat4("proj", current_camera.getProjection());
			// assume the EBO/VBO have the exact same order as this loop
			// it can be wrong and screw things up
			commands.resize(MeshTransformEntities.size_hint());
			objectDataVector.resize(MeshTransformEntities.size_hint());
			for (const auto entity : MeshTransformEntities) {
				auto& meshComponent = MeshTransformEntities.get<MeshComponent>(entity);

				Mesh::MeshData& data = resourceManager.m_loadedMeshes[meshComponent.meshID].meshData;
				uint64_t verticesAmount = data.vertices.size();
				uint64_t indicesAmount = data.indices.size();

				DrawElementsIndirectCommand command;
				command.count = indicesAmount; 
				command.instanceCount = 1; // draw 1 instance
				command.firstIndex = firstIndex; // draw from index 0
				command.baseVertex = baseVertex; // where the new "object" begins
				command.baseInstance = i; // what object we are on?

				commands[i] = command;
				auto& transformComponent = MeshTransformEntities.get<TransformComponent>(entity);
				ObjectData objectData;
				objectData.model = transformComponent.model;

				uint64_t diffuseTextureID = resourceManager.m_loadedmaterials[meshComponent.materialID].diffuseTexture;
				objectData.diffuseTextureHandle = resourceManager.m_loadedTextures[diffuseTextureID].handle;

				uint64_t emissiveTextureID = resourceManager.m_loadedmaterials[meshComponent.materialID].diffuseTexture;
				objectData.emissiveTextureHandle = resourceManager.m_loadedTextures[emissiveTextureID].handle;

				uint64_t aoTextureID = resourceManager.m_loadedmaterials[meshComponent.materialID].diffuseTexture;
				objectData.aoTextureHandle = resourceManager.m_loadedTextures[aoTextureID].handle;

				uint64_t metallicTextureID = resourceManager.m_loadedmaterials[meshComponent.materialID].diffuseTexture;
				objectData.metallicTextureHandle = resourceManager.m_loadedTextures[metallicTextureID].handle;

				uint64_t roughnessTextureID = resourceManager.m_loadedmaterials[meshComponent.materialID].diffuseTexture;
				objectData.roughnessTextureHandle = resourceManager.m_loadedTextures[roughnessTextureID].handle;

				uint64_t normalTextureID = resourceManager.m_loadedmaterials[meshComponent.materialID].diffuseTexture;
				objectData.normalTextureHandle = resourceManager.m_loadedTextures[normalTextureID].handle;

				objectDataVector[i] = objectData;
				firstIndex += indicesAmount;
				baseVertex += verticesAmount; // add offset
				i++;
			}

			// upload the data to the gpu
			glBindBuffer(GL_DRAW_INDIRECT_BUFFER, m_indirectBuffer);
			glBufferData(GL_DRAW_INDIRECT_BUFFER, commands.size() * sizeof(DrawElementsIndirectCommand), commands.data(), GL_DYNAMIC_DRAW);

			glBindBuffer(GL_SHADER_STORAGE_BUFFER, objectSSBO);
			glBufferData(GL_SHADER_STORAGE_BUFFER, objectDataVector.size() * sizeof(ObjectData), objectDataVector.data(), GL_DYNAMIC_DRAW);
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, objectSSBO);



			glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, (GLvoid*)0, commands.size(), 0);
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
			m_defaultShader.setInt("gPosition", 0);
			m_defaultShader.setInt("gNormal", 1);
			m_defaultShader.setInt("gAlbedo", 2);
			m_defaultShader.setInt("gEmissive", 3);
			m_defaultShader.setInt("gProperties", 4);
			m_defaultShader.setInt("gDepth", 5);

			auto skyboxes = scene.getRegistry().view<const SkyboxComponent>();
			auto& skyboxComponent = skyboxes.get<SkyboxComponent>(skyboxes.front()); // hack
			Skybox& skyboxObject = resourceManager.m_loadedSkyboxes[skyboxComponent.id];
			Texture& cubeMapTexture = resourceManager.m_loadedTextures[skyboxObject.cubeMapTexture];
			Texture& irradienceMapTexture = resourceManager.m_loadedTextures[skyboxObject.irradienceMapTexture];

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, gBuffer->m_colorAttachments[0]);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, gBuffer->m_colorAttachments[1]);
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, gBuffer->m_colorAttachments[2]);
			glActiveTexture(GL_TEXTURE3);
			glBindTexture(GL_TEXTURE_2D, gBuffer->m_colorAttachments[3]);
			glActiveTexture(GL_TEXTURE4);
			glBindTexture(GL_TEXTURE_2D, gBuffer->m_colorAttachments[4]);
			glActiveTexture(GL_TEXTURE5);
			glBindTexture(GL_TEXTURE_2D, gBuffer->m_depthTextureID);
			glActiveTexture(GL_TEXTURE6);
			glBindTexture(GL_TEXTURE_CUBE_MAP, irradienceMapTexture.gpuID);
			m_defaultShader.setInt("irradianceMap", 6);

			glDepthMask(GL_FALSE);
			renderLights(scene);
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
			glEnable(GL_BLEND);
			renderBuffer->unbind();

			postfxBuffer->bind();
			postfxPass();
			postfxBuffer->unbind();
			
		}

	}

	uint32_t DeferredPipeline::getFinalImage()
	{
		return postfxBuffer->m_colorAttachments[0];
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
		normalAttachment.textureFormat = FrameBuffer::TextureFormat::RGBA16F;
		normalAttachment.textureFilter = FrameBuffer::TextureFilter::Nearest;
		normalAttachment.dataType = FrameBuffer::DataType::FLOAT;
		normalAttachment.dataFormat = FrameBuffer::DataFormat::RGBA;

		albedoAttachment.textureTarget = FrameBuffer::TextureTarget::TEXTURE_2D;
		albedoAttachment.textureFormat = FrameBuffer::TextureFormat::RGBA8;
		albedoAttachment.textureFilter = FrameBuffer::TextureFilter::Nearest;
		albedoAttachment.dataType = FrameBuffer::DataType::FLOAT;
		albedoAttachment.dataFormat = FrameBuffer::DataFormat::RGBA;

		emissiveAttachment.textureTarget = FrameBuffer::TextureTarget::TEXTURE_2D;
		emissiveAttachment.textureFormat = FrameBuffer::TextureFormat::RGBA8;
		emissiveAttachment.textureFilter = FrameBuffer::TextureFilter::Nearest;
		emissiveAttachment.dataType = FrameBuffer::DataType::FLOAT;
		emissiveAttachment.dataFormat = FrameBuffer::DataFormat::RGBA;

		propertiesAttachment.textureTarget = FrameBuffer::TextureTarget::TEXTURE_2D;
		propertiesAttachment.textureFormat = FrameBuffer::TextureFormat::RGBA8;
		propertiesAttachment.textureFilter = FrameBuffer::TextureFilter::Nearest;
		propertiesAttachment.dataType = FrameBuffer::DataType::FLOAT;
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
	void DeferredPipeline::renderLights(Scene& scene)
	{
		// render lightss
		auto lightView = scene.getRegistry().view<const LightComponent, TransformComponent>();

		int index = 0;
		for (auto entity : lightView) {
			auto& lightComponent = lightView.get<LightComponent>(entity);
			auto& transformComponent = lightView.get<TransformComponent>(entity);
			// kinda whack ngl
			std::string lightPosVar = "lights[" + std::to_string(index) + "].position";
			std::string lightColorVar = "lights[" + std::to_string(index) + "].color";
			std::string lightLinearVar = "lights[" + std::to_string(index) + "].linear";
			std::string lightQuadraticVar = "lights[" + std::to_string(index) + "].quadratic";
			std::string lightRadiusVar = "lights[" + std::to_string(index) + "].radius";
			std::string lightPowerVar = "lights[" + std::to_string(index) + "].power";
			m_defaultShader.setVec3fv(lightPosVar, transformComponent.pos);
			m_defaultShader.setVec3fv(lightColorVar, lightComponent.color);
			m_defaultShader.setFloat(lightLinearVar, lightComponent.linear);
			m_defaultShader.setFloat(lightQuadraticVar, lightComponent.quadratic);
			m_defaultShader.setFloat(lightRadiusVar, lightComponent.radius);
			m_defaultShader.setFloat(lightPowerVar, lightComponent.power);
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
	void DeferredPipeline::genMegaBuffer(Scene& scene)
	{
		// called everytime a new mesh is added to the scene, unoptimal
		ResourceManager& resourceManager = Application::getApplication().getResourceManager();

		auto MeshTransformEntities = scene.getRegistry().view<const MeshComponent, TransformComponent>(); // register of all mesh components

		// bind vao
		glBindVertexArray(m_VAO);

		uint32_t verticesSize = 0;
		uint32_t indexSize = 0;
		for (const auto entity : MeshTransformEntities) {
			auto& meshComponent = MeshTransformEntities.get<MeshComponent>(entity);
			Mesh::MeshData& data = resourceManager.m_loadedMeshes[meshComponent.meshID].meshData;
			verticesSize += data.vertices.size() * sizeof(Mesh::Vertex);
			indexSize += data.indices.size() * sizeof(uint64_t);

		}


		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, verticesSize, nullptr, GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexSize, nullptr, GL_DYNAMIC_DRAW);

		uint32_t baseVertex = 0;
		uint32_t firstIndex = 0;
		uint32_t currentVertexOffsetBytes = 0;
		uint32_t currentIndicesOffsetBytes = 0;
		for (const auto entity : MeshTransformEntities) {
			auto& meshComponent = MeshTransformEntities.get<MeshComponent>(entity);

			Mesh::MeshData& data = resourceManager.m_loadedMeshes[meshComponent.meshID].meshData;
			uint32_t verticesAmount = data.vertices.size();
			uint32_t indicesAmount = data.indices.size();
			glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
			glBufferSubData(GL_ARRAY_BUFFER, currentVertexOffsetBytes, verticesAmount * sizeof(Mesh::Vertex), data.vertices.data());

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
			glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, currentIndicesOffsetBytes, indicesAmount * sizeof(unsigned int), data.indices.data());

			firstIndex += indicesAmount;
			baseVertex += verticesAmount; // add offset
			currentVertexOffsetBytes += verticesAmount * sizeof(Mesh::Vertex);
			currentIndicesOffsetBytes += indicesAmount * sizeof(unsigned int);
		}
		glBindVertexArray(0);
	}
	void DeferredPipeline::updateMegaBuffer(Scene& scene)
	{


	}
}
