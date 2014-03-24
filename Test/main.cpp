#include "../Gliw/gliw.h"

GLuint vbo;

void init()
{
	glClearColor(0.0f, 0.5f, 1.0f, 1.0f);

	static const GLfloat data[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
	};

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);
}

void update()
{
	glViewport(0, 0, gliw::getWidth(), gliw::getHeight());
}

void render()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	glDisableVertexAttribArray(0);

	gliw::swapBuffers();
}

int main(int argc, char** argv)
{
	gliw::init(800, 600);
	init();
	while(gliw::update())
	{
		update();
		render();
	}
	system("pause");
	return 0;
}