/*
 * CS455 Objects and View Transforms
 * Project 3
 * Caleb Call
 */
#include "main.h"

using namespace std;

/* Global variables */
char title[] = "Project 3";
WFObject car;
WFObject tire;
WFObject parkingLot;
WFObject goku;
GLuint texNames[3];
Gamepad gamepad;

// Global camera variables
float camx = 0;
float camz = 0;
float camry = 0;
float camrx = 0;
float tireR = 0;

/* Initialize OpenGL Graphics */
void initGL() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
    glClearDepth(1.0f);                   // Set background depth to farthest
    glEnable(GL_DEPTH_TEST);   // Enable depth testing for z-culling
    glDepthFunc(GL_LEQUAL);    // Set the type of depth-test
    glShadeModel(GL_SMOOTH);   // Enable smooth shading
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);  // Nice perspective corrections

	glGenTextures(3, texNames); // Generate texture IDs
	ilInit(); // Initialize devIL for loading textures
}
 
void renderModel(WFObject& model)
{
	// Iterate over each model face in the model
	for(int i = 0; i < model.ModelFaces.size(); i++)
	{
		glBegin(GL_POLYGON);                
		ModelFace* mf = &model.ModelFaces[i];

		for(int j = 0; j < mf->points.size(); j++)
		{
			Vector* vVector = &mf->points[j];
			Vector* tVector = &mf->textureCoords[j];

			if (tVector->x != -1) // a tIndex with -1 means there is no texture coordiant for that vertex
			{
				glTexCoord2f(tVector->x, tVector->y); // Assign the texture image vertex to the vertex we are creating
			}

			glVertex3f(vVector->x, vVector->y, vVector->z); // Create the vertex
		}
		glEnd();  // End of drawing
	}
}

void cameraTransforms()
{
	// Translate and rotate all models to make the camera appear to be moving.
	// Use the negative camera values

	float translateToOrigin[] = { 1, 0, 0, 0, 
								0, 1, 0, 0,
								0, 0, 1, 0,  
								0, -1, -5, 1  };
	// Turn 
	float a = -camry; 
	float rotateY[] = { cos(a), 0, -sin(a), 0, 
					       0,	 1,   0,     0,
					     sin(a), 0, cos(a),  0,
						   0,    0,   0,     1 };

	a = camrx; 
	float rotateX[] = { 1,   0,      0,     0, 
					   0,  cos(a), sin(a), 0,
					   0, -sin(a), cos(a), 0,
					   0,    0,      0,    1 };

	// move x and y portion
	float translate[] = { 1, 0, 0, 0, 
						    0, 1, 0, 0,
						    0, 0, 1, 0,  
						    -camx, 1, -camz + 5, 1  };

	glMultMatrixf(translate);
	glMultMatrixf(rotateX);
	glMultMatrixf(rotateY);
	glMultMatrixf(translateToOrigin);
}

void renderCar()
{
	/* RENDER CAR *****************************
	*******************************************/
	glPushMatrix();
	float a = M_PI / 3; // angle
	float rotate[] = { cos(a), 0, -sin(a), 0,  // rotation matrix about y-axis
					     0,	   1,    0,    0,
					   sin(a), 0, cos(a),  0,
						 0,    0,    0,    1 };
	glMultMatrixf(rotate); // Multiply with ModelView matrix

	// choose texture and draw
	glBindTexture(GL_TEXTURE_2D, texNames[0]); // Bind desired texture to GL_TEXTURE_2D
	renderModel(car);
	glPopMatrix();
}

void tireRotate()
{
	float a = tireR;
	float rotate[] = { cos(a), 0, -sin(a), 0,  // Rotate tires
					       0,	 1,   0,     0,
					     sin(a), 0, cos(a),  0,
						   0,    0,   0,     1 };
	glMultMatrixf(rotate);
}

void renderTireFL()
{
	/* RENDER FRONT LEFT TIRE *****************************
	*******************************************/
	glPushMatrix();
	
	float scaleFL[] = { .25,  0,  0, 0,    // Scale tire
						 0, .25,  0, 0,
						 0,  0, .25, 0,
						 0,  0,  0, 1 };

	float a = 4 * M_PI / 3;
	float rotateFL[] = { cos(a), 0, -sin(a), 0,  // Rotate tire about y-axis
					       0,	 1,   0,     0,
					     sin(a), 0, cos(a),  0,
						   0,    0,   0,     1 };

	float translateFL[] = { 1, 0, 0, 0,    // Translate tire onto front left tire well
						    0, 1, 0, 0,
						    0, 0, 1, 0,  
						    -.7, .15, .1, 1  };

	// Multiply the transforms in opposite order
	glMultMatrixf(translateFL);
	glMultMatrixf(scaleFL);
	glMultMatrixf(rotateFL);
	tireRotate();

	// choose texture and draw
	glBindTexture(GL_TEXTURE_2D, texNames[1]); // Bind desired texture to GL_TEXTURE_2D
	renderModel(tire);

	glPopMatrix();
}

void renderTireBL()
{
	/* RENDER BACK LEFT TIRE *****************************
	*******************************************/
	glPushMatrix();

	float scaleBL[] = { .25,  0,  0, 0,    // Scale tire
						 0, .25,  0, 0,
						 0,  0, .25, 0,
						 0,  0,  0, 1 };

	float a = 4 * M_PI / 3;
	float rotateBL[] = { cos(a), 0, -sin(a), 0,  // Rotate tire about y-axis
					       0,	 1,   0,     0,
					     sin(a), 0, cos(a),  0,
						   0,    0,   0,     1 };

	float translateBL[] = { 1, 0, 0, 0,    // Translate tire onto back left tire well
						    0, 1, 0, 0,
						    0, 0, 1, 0,  
						    .175, .15, .65, 1  };

	// Multiply the transforms in opposite order
	glMultMatrixf(translateBL);
	glMultMatrixf(scaleBL);
	glMultMatrixf(rotateBL);

	// choose texture and draw
	glBindTexture(GL_TEXTURE_2D, texNames[1]); // Bind desired texture to GL_TEXTURE_2D
	renderModel(tire);

	glPopMatrix();
}

void renderTireBR()
{
	/* RENDER BACK RIGHT TIRE *****************************
	*******************************************/
	glPushMatrix();
	
	float scaleBR[] = { .25,  0,  0, 0,    // Scale tire
						 0, .25,  0, 0,
						 0,  0, .25, 0,
						 0,  0,  0, 1 };

	float a = M_PI / 3;
	float rotateBR[] = { cos(a), 0, -sin(a), 0,  // Rotate tire about y-axis
					       0,	 1,   0,     0,
					     sin(a), 0, cos(a),  0,
						   0,    0,   0,     1 };

	float translateBR[] = { 1, 0, 0, 0,    // Translate tire onto back right tire well
						    0, 1, 0, 0,
						    0, 0, 1, 0,  
						    .65, .15, -.15, 1  };

	// Multiply the transforms in opposite order
	glMultMatrixf(translateBR);
	glMultMatrixf(scaleBR);
	glMultMatrixf(rotateBR);

	// choose texture and draw
	glBindTexture(GL_TEXTURE_2D, texNames[1]); // Bind desired texture to GL_TEXTURE_2D
	renderModel(tire);

	glPopMatrix();
}

void renderTireFR()
{
	/* RENDER FRONT RIGHT TIRE *****************************
	*******************************************/
	glPushMatrix();

	float scaleFR[] = { .25,  0,  0, 0,    // Scale tire
						 0, .25,  0, 0,
						 0,  0, .25, 0,
						 0,  0,  0, 1 };

	float a = M_PI / 3;
	float rotateFR[] = { cos(a), 0, -sin(a), 0,  // Rotate tire about y-axis
					       0,	 1,   0,     0,
					     sin(a), 0, cos(a),  0,
						   0,    0,   0,     1 };

	float translateFR[] = { 1, 0, 0, 0,    // Translate tire onto back left tire well
						    0, 1, 0, 0,
						    0, 0, 1, 0,  
						    -.25, .16, -.65, 1  };

	// Multiply the transforms in opposite order
	glMultMatrixf(translateFR);
	glMultMatrixf(scaleFR);
	glMultMatrixf(rotateFR);
	tireRotate();

	// choose texture and draw
	glBindTexture(GL_TEXTURE_2D, texNames[1]); // Bind desired texture to GL_TEXTURE_2D
	renderModel(tire);

	glPopMatrix();
}


void renderParkingLot()
{
	/* RENDER PARKING LOT *****************************
	*******************************************/
	glPushMatrix();

	float scale[] = { 1, 0,  0, 0,    // Scale tire
					  0, 1,  0, 0,
					  0,  0, 1, 0,
					  0,  0, 0, 1 };

	float translate[] = { 1, 0, 0, 0,    // Translate tire onto back left tire well
						  0, 1, 0, 0,
						  0, 0, 1, 0,  
						  2.5, 0, 7.5, 1  };

	// Multiply the transforms in opposite order
	glMultMatrixf(translate);
	glMultMatrixf(scale);

	// choose texture and draw
	glBindTexture(GL_TEXTURE_2D, texNames[2]); // Bind desired texture to GL_TEXTURE_2D
	renderModel(parkingLot);

	glPopMatrix();
}

void renderGoku()
{
	/* RENDER GOKU *****************************
	*******************************************/
	glPushMatrix();

	float scale[] = { .15, 0,  0, 0,    // Scale
					  0, .15,  0, 0,
					  0,  0, .15, 0,
					  0,  0, 0, 1 };

	float a = 4 * M_PI / 3;
	float rotate[] = { cos(a), 0, -sin(a), 0,  // Rotate
					       0,	 1,   0,     0,
					     sin(a), 0, cos(a),  0,
						   0,    0,   0,     1 };

	float translate[] = { 1, 0, 0, 0,    // Translate
						  0, 1, 0, 0,
						  0, 0, 1, 0,  
						  .1, .75, 0, 1  };

	// Multiply the transforms in opposite order
	glMultMatrixf(translate);
	glMultMatrixf(scale);
	glMultMatrixf(rotate);

	glDisable (GL_TEXTURE_2D); // Disable texturing
	glColor3f(1.0, 0.0, 0.0);
	renderModel(goku);

	glPopMatrix();
}

void renderModels()
{
	renderCar();
	renderTireFL();
	renderTireBL();
	renderTireBR();
	renderTireFR();
	renderParkingLot();
	renderGoku();
}

/* Handler for window-repaint event. Called back when the window first appears and
   whenever the window needs to be re-painted. */
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffers 

	glEnable(GL_TEXTURE_2D); // Enable texturing
	glMatrixMode(GL_MODELVIEW);     // To operate on model-view matrix
	glLoadIdentity();
	cameraTransforms();
	renderModels(); // NOTE: OPENGL READS MATRICIES IN COLUMN-MAJOR ORDERING
	glDisable (GL_TEXTURE_2D); // Disable texturing

    glutSwapBuffers();  // Swap the front and back frame buffers (double buffering)
}
 
/* Handler for window re-size event. Called back when the window first appears and
   whenever the window is re-sized with its new width and height */
void reshape(GLsizei width, GLsizei height) {  // GLsizei for non-negative integer
   // Compute aspect ratio of the new window
   if (height == 0) height = 1;                // To prevent divide by 0
   GLfloat aspect = (GLfloat)width / (GLfloat)height;
 
   // Set the viewport to cover the new window
   glViewport(0, 0, width, height);
 
   // Set the aspect ratio of the clipping volume to match the viewport
   glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
   glLoadIdentity();             // Reset
   // Enable perspective projection with fovy, aspect, zNear and zFar
   gluPerspective(45.0f, aspect, 0.1f, 100.0f);

   	gluLookAt(0, 1, 5, 0, 0, 0, 0, 1, 0); // Looking at origin from +z axis
	//gluLookAt(-2, 0, 4, 0, 0, 0, 0, 1, 0); // Left sideways view
	//gluLookAt(0, 15, 0, 0, 0, 0, -1, 0, 0); // Top view
	//gluLookAt(0, 1, -5, 0, 0, 0, 0, 1, 0); // Looking at origin from -z axis
}

bool file_exists (const std::string& name) {
  struct stat buffer;   
  return (stat (name.c_str(), &buffer) == 0); 
}

void loadBinaryModel(WFObject& model, string filename)
{
	// create and open an archive for input
	std::ifstream ifs(filename, std::ios::binary);
	boost::archive::binary_iarchive ia(ifs);
    // read class state from archive
    ia >> model;
    // archive and stream closed when destructors are called
}

// For each file, check if the dat file exists. If it does, load the model from the file.
// If not, read the model in from the text .obj file
void loadModels()
{
	// Load car
	string filename = "Model Binaries//car.dat";
	if (file_exists(filename))
		loadBinaryModel(car, filename);
	else
		car.load("Object Files//car.obj");

	// Load tire
	filename = "Model Binaries//tire.dat";
	if (file_exists(filename))
		loadBinaryModel(tire, filename);
	else
		tire.load("Object Files//tire.obj");

	// Load parking lot
	filename = "Model Binaries//ParkingLot.dat";
	if (file_exists(filename))
		loadBinaryModel(parkingLot, filename);
	else
		parkingLot.load("Object Files//ParkingLot.obj");

	// Load goku
	filename = "Model Binaries//goku.dat";
	if (file_exists(filename))
		loadBinaryModel(goku, filename);
	else
		goku.load("Object Files//goku.obj");
}

void createTexture()
{	
	// Create the new texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, ilGetData());
	
	// Set texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
}
 
void loadTextures()
{
	// Fill vector with file names
	vector<string> fileNamesVector;

	// Car, tire, parking lot, and humanoid
	fileNamesVector.push_back("Textures\\car.bmp");
	fileNamesVector.push_back("Textures\\tire.bmp");
	fileNamesVector.push_back("Textures\\ParkingLot.bmp");

	ILboolean success;
	unsigned int imageID[3];

	// generate image names
	ilGenImages(fileNamesVector.size(), imageID); 
	
	for (int i = 0; i < fileNamesVector.size(); i++)
	{
		// bind image ID
		ilBindImage(imageID[i]); 

		string fileName = fileNamesVector[i];

		// load the car
		success = ilLoadImage((ILstring)fileName.c_str());

		if (!success)
		{
			cout << "Unable to load image\n";
			return;
		}

		printf("Width: %d, Height %d, Bytes per Pixel %d \n", 
        ilGetInteger(IL_IMAGE_WIDTH),
        ilGetInteger(IL_IMAGE_HEIGHT),
        ilGetInteger(IL_IMAGE_BYTES_PER_PIXEL));

		glBindTexture(GL_TEXTURE_2D, texNames[i]); // Bind the texture ID we want for the new texture
		createTexture();
	}
}

void saveBinaryModel(WFObject model, string filename)
{
	std::ofstream ofs(filename, std::ios::binary);

    // save data to archive
    {
        boost::archive::binary_oarchive oa(ofs);
        // write class instance to archive
        oa << model;
        // archive and stream closed when destructors are called
    }
}

void serializeModels()
{
	// Save car
	string filename = "Model Binaries//car.dat";
	if (!file_exists(filename))
		saveBinaryModel(car, filename);

	// Save tire
	filename = "Model Binaries//tire.dat";
	if (!file_exists(filename))
		saveBinaryModel(tire, filename);

	// Save parking lot
	filename = "Model Binaries//ParkingLot.dat";
	if (!file_exists(filename))
		saveBinaryModel(parkingLot, filename);

	// Save parking lot
	filename = "Model Binaries//goku.dat";
	if (!file_exists(filename))
		saveBinaryModel(goku, filename);
}

/* Callback handler for special-key event */
void specialKeys(int key, int x, int y) {
   switch (key) {
      case GLUT_KEY_RIGHT:    // Right: move camera right, move models left
		 camx += .3; break;
      case GLUT_KEY_LEFT:     
         camx -= .3; break;
      case GLUT_KEY_UP:       
         camz -= .3; break;
      case GLUT_KEY_DOWN:     
         camz += .3; break;
   }

   display();
}
 
/* Callback handler for mouse event */
void mouse(int button, int state, int x, int y) {
   //if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
	  // camrx -= .3;
   //}
   //else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
   //{
	  // camrx += .3;
   //}

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		tireR += .05;
		if (tireR >= M_PI / 6)
		{
			tireR = M_PI / 6;
		}
	}
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) 
	{
		tireR -= .05;
		if (tireR <= -M_PI / 6)
		{
			tireR = -M_PI / 6;
		}
	}

   display();
}

void joystick(unsigned int buttonMask, int x, int y, int z){
	if (!gamepad.Refresh())
	{
		cout << "Please connect an Xbox 360 controller." << endl;
	}
	else
	{
	
		if (gamepad.IsPressed(XINPUT_GAMEPAD_DPAD_LEFT))
		{
			tireR += .05;
			if (tireR >= M_PI / 6)
			{
				tireR = M_PI / 6;
			}
		}
		if (gamepad.IsPressed(XINPUT_GAMEPAD_DPAD_RIGHT)) 
		{
			tireR -= .05;
			if (tireR <= -M_PI / 6)
			{
				tireR = -M_PI / 6;
			}
		}

		// Process translation using left thumbstick	 	
		if (abs(gamepad.leftStickY) > 0.1)
		{
			camz += .3 * -gamepad.leftStickY; // z position
			cout << "Leftstick Y : " << gamepad.leftStickY << endl;
		}
		
		if (abs(gamepad.leftStickX) > 0.1)
		{
			camx += .3 * gamepad.leftStickX; // z position
			cout << "Leftstick x: " << gamepad.leftStickX << endl;
		}

		// Process translation using left thumbstick	 
		if (abs(gamepad.rightStickY) > 0.1)
		{
			camrx += .2 * -gamepad.rightStickY;
		}

			
		if (abs(gamepad.rightStickX) > 0.1)
		{
			camry += .2 * -gamepad.rightStickX;
		}

	}

	// Redraw is a button was clicked
	display();
}

/* Main function: GLUT runs as a console application starting at main() */
int main(int argc, char** argv) {
   glutInit(&argc, argv);            // Initialize GLUT
   glutInitDisplayMode(GLUT_DOUBLE); // Enable double buffered mode
   glutInitWindowSize(640, 480);   // Set the window's initial width & height
   glutInitWindowPosition(50, 50); // Position the window's initial top-left corner
   glutCreateWindow(title);          // Create window with the given title
   glutDisplayFunc(display);       // Register callback handler for window re-paint event
   glutReshapeFunc(reshape);       // Register callback handler for window re-size event
   glutSpecialFunc(specialKeys); // Register callback handler for special-key event
   glutMouseFunc(mouse);   // Register callback handler for mouse event
   glutJoystickFunc(joystick, 20);
   initGL();                       // Our own OpenGL initialization
   loadModels();
   serializeModels();
   loadTextures();
   glutMainLoop();                 // Enter the infinite event-processing loop
   return 0;
}