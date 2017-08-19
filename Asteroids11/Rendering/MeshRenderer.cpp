#include "MeshRenderer.h"

#include "Graphics.h"
#include "Light.h"
#include "Mesh.h"
#include "Shader.h"

void MeshRenderer::Render(const Camera& camera, const glm::mat4& modelMatrix, Graphics* graphics, Light* ambientLight, Light* directionalLight)
{
	if(_mesh == nullptr)
	{
		return;
	}

	graphics->SetShader(_shader->GetProgramID());
	graphics->DrawIndexed(camera, ambientLight, directionalLight, modelMatrix, _color, _mesh->_vertexBuffer, _mesh->_indexBuffer, _mesh->_normalBuffer, _mesh->_indexBufferSize);
}
