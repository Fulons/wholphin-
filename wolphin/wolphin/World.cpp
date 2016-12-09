#include "World.h"


namespace wholphin{

	void Renderer::Present(GLuint modelMatrixIndex) {
		for (auto renderable : renderables) {
			renderable.first->BindBuffers();
			for (auto instance : renderable.second) {
				glUniformMatrix4fv(modelMatrixIndex, 1, GL_FALSE, &instance[0][0]);
				renderable.first->DrawInitialized();
			}
			renderable.first->UnbindBuffers();
			renderable.second.clear();
		}
	}

}

