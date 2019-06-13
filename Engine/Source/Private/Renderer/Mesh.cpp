#include <Renderer/Mesh.h>
#include <Renderer/Renderer.h>
#include <Logger.h>

#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
#include <assimp/DefaultLogger.hpp>
#include <assimp/LogStream.hpp>

#include <glad/glad.h>

/**
 * @brief Import flags passed to Assimp.
 */
const uint32_t AssimpImportFlags = 
	aiProcess_CalcTangentSpace |
	aiProcess_Triangulate |
	aiProcess_SortByPType |
	aiProcess_PreTransformVertices |
	aiProcess_GenNormals |
	aiProcess_GenUVCoords |
	aiProcess_OptimizeMeshes |
	aiProcess_Debone |
	aiProcess_ValidateDataStructure;

/**
 * @brief Assimp LogStream implementation that routes messages to
 *		  the engine Logger.
 */
struct LogStream : public Assimp::LogStream
{
	/**
	 * @brief Initialize this LogStream.
	 */
	static void Initialize()
	{
		if(Assimp::DefaultLogger::isNullLogger())
		{
			Assimp::DefaultLogger::create("", Assimp::Logger::VERBOSE);
			Assimp::DefaultLogger::get()->attachStream(new LogStream, Assimp::Logger::Err | Assimp::Logger::Warn);
		}
	}

	/**
	 * @brief Route the @p message to the engine Logger.
	 */
	void write(const char* message) override
	{
		Logger::Log("Renderer", LoggerVerbosity::Error, "Assimp Error: {0}", message);
	}
};

Mesh::Mesh(const std::string& filepath) : m_Filepath(filepath)
{
	LogStream::Initialize();

	Logger::Log("Renderer", LoggerVerbosity::Info, "Loading mesh: {0}", filepath);

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filepath, AssimpImportFlags);

	if (!scene || !scene->HasMeshes())
	{
		Logger::Log("Renderer", LoggerVerbosity::Error, "Assimp failed to load mesh file: {0}", filepath);
	}

	aiMesh* mesh = scene->mMeshes[0];

	LOG_CATEGORY_ASSERT(mesh->HasPositions(), "Renderer", "Meshes require positions.");
	LOG_CATEGORY_ASSERT(mesh->HasNormals(), "Renderer", "Meshes require normals.");

	m_Vertices.reserve(mesh->mNumVertices);

	// Extract vertices from model
	for (size_t i = 0; i < m_Vertices.capacity(); i++)
	{
		Vertex vertex;
		vertex.Position = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };
		vertex.Normal = { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z };

		if (mesh->HasTangentsAndBitangents())
		{
			vertex.Tangent = { mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z };
			vertex.Binormal = { mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z };
		}

		if (mesh->HasTextureCoords(0))
		{
			vertex.TextureCoordinate = { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y };
		}

		m_Vertices.push_back(vertex);
	}

	m_VertexBuffer.reset(VertexBuffer::Create());
	m_VertexBuffer->SetData(m_Vertices.data(), m_Vertices.size() * sizeof(Vertex));

	// Extract indices from model
	m_Indices.reserve(mesh->mNumFaces);
	for (size_t i = 0; i < m_Indices.capacity(); i++)
	{
		LOG_CATEGORY_ASSERT(mesh->mFaces[i].mNumIndices == 3, "Renderer", "Must have 3 indices.");
		m_Indices.push_back({ mesh->mFaces[i].mIndices[0], mesh->mFaces[i].mIndices[1], mesh->mFaces[i].mIndices[2] });
	}

	m_IndexBuffer.reset(IndexBuffer::Create());
	m_IndexBuffer->SetData(m_Indices.data(), m_Indices.size() * sizeof(Index));
}

Mesh::~Mesh() = default;

void Mesh::Render()
{
	m_VertexBuffer->Bind();
	m_IndexBuffer->Bind();
	ENGINE_RENDER_S({
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Position));

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Normal));

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Tangent));

		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Binormal));

		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, TextureCoordinate));
	});

	Renderer::DrawIndexed(m_IndexBuffer->GetCount());
}
