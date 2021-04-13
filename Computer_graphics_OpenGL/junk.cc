struct pinfo
{
	float width;
	float x, y, z;
	float v_x, v_y, v_z;
	float r, g, b;

} particles[MAXPARTICLES];

void drawOneParticle()
{
	glutSolidSphere(0.1, 15, 15);

	glPushMatrix();
	glTranslatef(lx,ly,lz);

	glMaterialfv(GL_FRONT, GL_AMBIENT, default_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);

	glPopMatrix();
}

void drawParticles()
{
	for(unsigned int i = 0; i < MAXPARTICLES; i++)
	{
		glPushMatrix();
		glTranslatef(particles[i].x, particles[i].y, particles[i].z);
		glScalef(particles[i].width, particles[i].width, particles[i].width);
		glColor3f(particles[i].r, particles[i].g, particles[i].b);
		drawOneParticle();
		glPopMatrix();
	}
}

void fireworks()
{
	for(unsigned int i = 0; i < MAXPARTICLES; i++)
	{
		particles[i].width = 3.0 * (rand() / (float)RAND_MAX) + 1.0;
		particles[i].x = 0.0;
		particles[i].y = 15.0;
		particles[i].z = 0.0;
		particles[i].v_x = 5.0 * (rand() / (float)RAND_MAX) - 2.5;
		particles[i].v_y = 15.0 * (rand() / (float)RAND_MAX) + 10.0; // always upwards
		particles[i].v_z = 5.0 * (rand() / (float)RAND_MAX) - 2.5;
		particles[i].r = 0.0;
		particles[i].g = 0.5;
		particles[i].b = 0.9;
	}
	glutGet(GLUT_ELAPSED_TIME);
}	

	for(unsigned int i = 0; i < MAXPARTICLES; i++)
	{
		particles[i].v_y = particles[i].v_y - a*time;

		if (particles[i].y <= 0 && particles[i].v_y < 0)
		{
			particles[i].v_x = 0.0;
			particles[i].v_y = 0.0;
			particles[i].v_z = 0.0;
		} 

		particles[i].x = particles[i].x + particles[i].v_x*time;
		particles[i].y = particles[i].y + particles[i].v_y*time;
		particles[i].z = particles[i].z + particles[i].v_z*time;
	}
