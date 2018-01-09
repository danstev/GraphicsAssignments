/*
Dan Ward 28/11/17
Particle, blue for rain.
*/

using namespace std;
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include "wrapper_glfw.h"
#include "particle.h"


	Particle::Particle()
	{
		attribute_v_coord = 0;
		attribute_v_colours = 1;
		pos = glm::vec3(0,0,0);
	}

	Particle:: ~Particle()
	{

	}

	//Slightly moves down the particle by a slightly random amount.
	 void Particle::decreaseParticle()
	 {
		 pos.y = pos.y - ((float)(rand() % 100) / 1000); //-= did not work here for some reason.
	 }

	 //Input is normally the eye position. (i.e your viewpoint.)
	 void Particle::randomPosition(glm::vec3 curpos)
	 {
		 float xpos = ((float)(rand() % 100) / 20) + curpos.x - 2;
		 float ypos = ((float)(rand() % 100) / 20) + curpos.z - 2;
		 pos = glm::vec3(xpos, curpos.y + 3, ypos);
		 
	 }

	 void Particle::makeParticle()
	 {
		 GLfloat vertexPositions[] =
		 {
			 0.0f, 0.0f, 0.0f, 0.0f,
		 };

		 float vertexColours[] = {
			 0.0f, 0.0f, 1.0f, 1.0f,
		 };

		 glGenBuffers(1, &positionBufferObject);
		 glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
		 glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
		 glBindBuffer(GL_ARRAY_BUFFER, 0);

		 glGenBuffers(1, &colourObject);
		 glBindBuffer(GL_ARRAY_BUFFER, colourObject);
		 glBufferData(GL_ARRAY_BUFFER, sizeof(vertexColours), vertexColours, GL_STATIC_DRAW);
		 glBindBuffer(GL_ARRAY_BUFFER, 0);

	 }

	void Particle::drawParticle()
	{
		glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
		glEnableVertexAttribArray(attribute_v_coord);
		glVertexAttribPointer(attribute_v_coord, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindBuffer(GL_ARRAY_BUFFER, colourObject);
		glEnableVertexAttribArray(attribute_v_colours);
		glVertexAttribPointer(attribute_v_colours, 4, GL_FLOAT, GL_FALSE, 0, 0);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDrawArrays(GL_POINTS, 0, 4);
	}

