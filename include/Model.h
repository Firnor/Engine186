#pragma once

#include "Animator.h"

namespace e186
{
	struct AnimationClip
	{
		unsigned int m_animation_index;
		double m_ticks_per_second;
		double m_start_ticks;
		double m_end_ticks;
		Animator* m_animator;

		AnimationClip(unsigned int animationIndex, double ticksPerSec, double startTicks, double endTicks, Animator* animator)
			: m_animation_index(animationIndex),
			m_ticks_per_second(ticksPerSec),
			m_start_ticks(startTicks),
			m_end_ticks(endTicks),
			m_animator(animator)
		{
		}
	};

	enum ModelLoaderFlags
	{
		// Assimp flags
		MOLF_triangulate = 0x000001,
		MOLF_smoothNormals = 0x000002,
		MOLF_faceNormals = 0x000004,
		MOLF_flipUVs = 0x000008,
		MOLF_limitBoneWeights = 0x000010,
		MOLF_calcTangentSpace = 0x000020,
		// vertex-data flags
		MOLF_loadNormals = 0x010000,
		MOLF_loadColors = 0x020000,
		MOLF_loadTexCoords = 0x040000,
		MOLF_loadTexCoords2 = 0x080000,
		MOLF_loadTexCoords3 = 0x100000,
		MOLF_loadBones = 0x200000,
		// the default flags
		MOLF_default = MOLF_triangulate | MOLF_smoothNormals | MOLF_limitBoneWeights,
	};
	
	struct Mesh
	{
		// Constructor - initialize everything
		Mesh() :
			m_vertex_data_layout(0),
			m_vertex_data_vbo_id(0),
			m_indices_vbo_id(0),
			m_size_one_vertex(0),
			m_position_offset(0),
			m_normal_offset(0),
			m_tex_coords_offset(0),
			m_color_offset(0),
			m_bone_incides_offset(0),
			m_bone_weights_offset(0),
			m_tangent_offset(0),
			m_bitangent_offset(0),
			m_position_size(0),
			m_normal_size(0),
			m_tex_coords_size(0),
			m_color_size(0),
			m_bone_indices_size(0),
			m_bone_weights_size(0),
			m_tangent_size(0),
			m_scene_transformation_matrix(glm::mat4(1.0f)),
			m_bitangent_size(0)
		{}

		unsigned int m_vertex_data_layout;

		GLuint m_vertex_data_vbo_id;
		GLuint m_indices_vbo_id;

		/// stores different VAOs for different VertexData-combinations
		std::unordered_map<unsigned int, GLuint> m_vertex_array_objects;

		std::vector<char> m_vertex_data;
		std::vector<GLuint> m_indices;

		size_t m_size_one_vertex;

		size_t m_position_offset;
		size_t m_normal_offset;
		size_t m_tex_coords_offset;
		size_t m_color_offset;
		size_t m_bone_incides_offset;
		size_t m_bone_weights_offset;
		size_t m_tangent_offset;
		size_t m_bitangent_offset;

		size_t m_position_size;
		size_t m_normal_size;
		size_t m_tex_coords_size;
		size_t m_color_size;
		size_t m_bone_indices_size;
		size_t m_bone_weights_size;
		size_t m_tangent_size;
		size_t m_bitangent_size;

		glm::mat4 m_scene_transformation_matrix;

		std::unique_ptr<MaterialData> m_material_data;
	};
	
	class Model
	{
	private:
		static const int kNumFaceVertices = 3;
		static const int kVertexSize = 3;
		static const int kColorSize = 4;
		static const int kNormalSize = 3;
		static const char* kIndent;
		static const glm::vec4 kDefaultDiffuseColor;
		static const glm::vec4 kDefaultSpecularColor;
		static const glm::vec3 kDefaultAmbientColor;
		static const glm::vec3 kDefaultEmissiveColor;
		static const glm::vec4 kDefaultTransparentColor;
		static const float kDefaultSpecularPower;
		static const float kDefaultOpacity;
		static const float kDefaultBumpScaling;
		static const float kDefaultRefraction;
		static const float kDefaultReflectivity;

		std::vector<Mesh> m_meshes;

#ifdef false
		std::vector<Animator*> m_animators;
		std::map<unsigned int, AnimationClip> m_animation_clips;
#endif

		/// The transformation matrix specified while
		glm::mat4 m_load_transformation_matrix;

	public:
		Model(const glm::mat4& loadTransMatrix = glm::mat4(1.0f));
		Model(const Model& other) = delete;
		Model(Model&& other) noexcept;
		Model& operator=(const Model& other) = delete;
		Model& operator=(Model&& other) noexcept;
		~Model();

	public:
		static std::unique_ptr<Model> LoadFromFile(const std::string& path, const glm::mat4& transform_matrix, const unsigned int model_loader_flags = MOLF_default);

	private:
		bool Load(const std::string& path, const unsigned int modelLoaderFlags = MOLF_default);

		void Dispose();
		bool InitScene(const aiScene* scene, const std::string& path);
		bool InitMesh(const int index, const aiMesh* paiMesh);
		void InitTransformationMatrices(const aiNode* pNode, const aiMatrix4x4& accTrans);

		static void PrintIndent(std::ostream& stream, int indent);
		static void PrintMatrix(std::ostream& stream, const aiMatrix4x4& mat, int indent);
		static void PrintMesh(const aiScene* scene, std::ostream& stream, unsigned int meshIndex, int indent);
		static void PrintNodeRecursively(const aiScene* scene, std::ostream& stream, const aiNode* pNode, const aiMatrix4x4 accTranse, int indent);
		static void PrintVector3D(std::ostream& stream, const aiVector3D& vec);
		static void PrintQuaternion(std::ostream& stream, const aiQuaternion& quat);
		static void PrintVectorKey(std::ostream& stream, const aiVectorKey& key, int indent);
		static void PrintRotationKey(std::ostream& stream, const aiQuatKey& key, int indent);
		static void PrintAnimationChannel(std::ostream& stream, const aiNodeAnim* channel, int indent);
		static void PrintMeshKey(std::ostream& stream, const aiMeshKey& key, int indent);
		static void PrintMeshChannel(std::ostream& stream, const aiMeshAnim* channel, int indent);
		static void PrintBone(std::ostream& stream, const aiBone* bone, int indent);
		static void PrintAnimMesh(std::ostream& stream, const aiAnimMesh* am, int indent);

		static std::string GetTextureNameFromMaterials(const aiScene* scene, unsigned int meshIndex, aiTextureType type);

	public:
		bool BindVAOForMesh(const unsigned int meshIndex, const unsigned int vertexDataConfig) const;
		void UnbindVAO() const;

		bool GenerateVAOsWithVertexAttribConfig(const unsigned int vertexDataConfig);
		bool GenerateVAOForMeshWithVertexAttribConfig(const unsigned int index, const unsigned int vertexDataConfig);
		void RenderForVertexAttribConfig(unsigned int meshIndex, const unsigned int vertexDataConfig, GLenum mode) const;
		void RenderForVertexAttribConfig(const unsigned int vertexDataConfig, GLenum mode) const;
		void RenderWithShader(unsigned int meshIndex, const Shader& shader) const;
		void RenderWithShader(const Shader& shader) const;
		void RenderForVertexDataConfigGenerateMissingVAO(unsigned int meshIndex, const unsigned int vertexDataConfig, GLenum mode);
		void RenderForVertexDataConfigGenerateMissingVAO(const unsigned int vertexDataConfig, GLenum mode);
		void RenderWithShaderGenerateMissingVAO(Shader& shader);

		const glm::mat4& transformation_matrix() const;
		const glm::mat4 transformation_matrix(unsigned int meshIndex) const;

		static void PrintNodeTree(const aiScene* scene, std::ostream& stream);
		static void PrintAnimationTree(const aiScene* scene, std::ostream& stream);
		static void PrintMeshes(const aiScene* scene, std::ostream& stream);

		size_t num_meshes() const;
		size_t num_vertices(unsigned int meshIndex) const;
		size_t indices_length(unsigned int meshIndex) const;

		std::string GetMeshName(unsigned int meshIndex) const;
		static std::string GetMeshName(const aiScene* scene, unsigned int meshIndex);
		int Model::GetIndexOfMeshWithName(const aiScene* scene, const std::string& meshName);
		static std::string GetDiffuseTextureName(const aiScene* scene, unsigned int meshIndex);
		static std::string GetSpecularTextureName(const aiScene* scene, unsigned int meshIndex);
		static std::string GetAmbientTextureName(const aiScene* scene, unsigned int meshIndex);
		static std::string GetEmissiveTextureName(const aiScene* scene, unsigned int meshIndex);
		static std::string GetHeightTextureName(const aiScene* scene, unsigned int meshIndex);
		static std::string GetNormalsTextureName(const aiScene* scene, unsigned int meshIndex);
		static std::string GetShininessTextureName(const aiScene* scene, unsigned int meshIndex);
		static std::string GetOpacityTextureName(const aiScene* scene, unsigned int meshIndex);
		static std::string GetDisplacementTextureName(const aiScene* scene, unsigned int meshIndex);
		static std::string GetReflectionTextureName(const aiScene* scene, unsigned int meshIndex);
		static std::string GetLightmapTextureName(const aiScene* scene, unsigned int meshIndex);

		void GatherMaterialData(const aiScene* scene, const std::string& model_path);
		static aiMaterial* GetAssimpMaterialPtr(const aiScene* scene, unsigned int meshIndex);

		void PrepareForShader(Shader& shader);
		MaterialData GetMaterialData(unsigned int meshIndex);

		template<typename Func>
		std::vector<std::reference_wrapper<Mesh>> SelectMeshes(Func predicate)
		{ // looki looki: http://en.cppreference.com/w/cpp/utility/functional/reference_wrapper
			std::vector<std::reference_wrapper<Mesh>> selection;
			int n = m_meshes.size();
			for (int i = 0; i < n; ++i)
			{
				if (predicate(i, m_meshes[i]))
				{
					selection.push_back(m_meshes[i]);
				}
			}
			return selection;
		}

		Mesh& mesh_at(unsigned int meshIndex);

#ifdef false
		bool HasAnimationWithIndex(const unsigned int animationIndex);
		double GetTicksPerSecondOfAnimation(const unsigned int animationIndex);
		double GetDurationOfAnimationTicks(const unsigned int animationIndex);
		bool AddAnimationClip(const unsigned int name, const unsigned int animationIndex, const double defaultTicksPerSecond = 0.0);
		bool AddAnimationClip(const unsigned int name, const unsigned int animationIndex, const double startTimeTicks, const double endTimeTicks, const double defaultTicksPerSecond = 0.0);
		bool HasAnimationClips() const;
		const AnimationClip* GetAnimationClip(const unsigned int name);
		Animator* GetAnimatorForAnimationIndex(const unsigned int animationIndex);
#endif

		static bool GetBoneWeightsAndIndicesForMeshVertex(const aiScene* scene, const unsigned int meshIndex, const unsigned int vertexId, glm::uvec4& outBoneIndices, glm::vec4& outBoneWeights);
		glm::vec3 GetVertexPosition(const unsigned meshIndex, const unsigned int vertexId) const;
		glm::vec3 GetVertexNormal(const unsigned meshIndex, const unsigned int vertexId) const;
	};

}
