// This is a sample OpenGL/CLUT program for SC208 course.
// Developed by Alexei Sourin, October 2003.
// The students are to study this code and use it 
// as a template for their assignment.
// Open a new project "Win32 Console Application" and add
// sample.c to Source Files 

#include <stdio.h>
#include <GL/glut.h>

// The unit size used in this application's World Coordinate System. 
// All other dimensions and parameters are made proportional to it.
#define	SIZE	10.0

int RendMode; /* May take values 1 or 2: for wireframe and polygon modes. */
double alpha, beta;  /* Angles of rotation about vertical and horizontal axes. */

// Defining a point light source parameters
GLfloat light_position1[] = {   0.0,   10.0*SIZE, 100.0*SIZE, 1.0 };
GLfloat light_ambient[]   = {   0.1,   0.1,   0.1, 1.0 };
GLfloat light_diffuse[]   = {   1.0,   1.0,   1.0, 1.0 };
GLfloat light_specular[]  = {   1.0,   1.0,   1.0, 1.0 };

//============================================================
static void Init(void)
{
// This function is called only once before any other functions are called
	alpha=-20.0; beta=20.0; RendMode=1;

// Setting up a point light source
	glLightfv(GL_LIGHT1, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT1, GL_POSITION, light_position1);

// Enabling lighting with the light source #1
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT1);

// enabling both side illumination for the polygons and hidden surface/line removal
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    glEnable(GL_DEPTH_TEST);
}


//============================================================
static void reshape( int width, int height )
{

// This function is called every time the window is to be resized or exposed
    glViewport(0, 0, (GLint)width, (GLint)height);
    glMatrixMode (GL_PROJECTION);	
    glLoadIdentity ();
    glFrustum(-3*SIZE, 3*SIZE, -3*SIZE, 3*SIZE, 3*SIZE, 40*SIZE);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity ();
}

//============================================================
void MakeShapes(void)
{
// Geometric objects (axes and surfaces) are defined here. 
// If the shaopes change in time, the time parameter controllling these changes can be
// defined and changed in function draw()

// Since the axes are to be drawn as solid lines, we disable lighting for them.
    glDisable(GL_LIGHTING);
	glBegin(GL_LINES);

	glColor4f(1.0, 0.0, 0.0, 1.0);
	glVertex3f(0.0,0.0,0.0);
	glVertex3f(2*SIZE,0.0,0.0);

	glColor4f(0.0, 1.0, 0.0, 1.0);
	glVertex3f(0.0,0.0,0.0);
	glVertex3f(0.0,2*SIZE,0.0);

	glColor4f(0.0, 0.0, 1.0, 1.0);
	glVertex3f(0.0,0.0,0.0);
	glVertex3f(0.0,0.0,2*SIZE);
	      
	glEnd();
	        
// The polygons are rendered differently depending on the selected rendering modes:
// which are wireframe, flat shading, and smooth shading.
// Notice how the edges visually disappear when smooth rendering is selected.
// Default material settings are used. You may define your own material properties.

	if (RendMode==1){
		glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);	
		glColor4f(1.0, 1.0, 1.0, 1.0);
	}
	else {
		glEnable(GL_LIGHTING);
		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	}

	glBegin(GL_POLYGON);
 
      // Right face (x = 10.0f)
	  glNormal3f(1.0f,0.0,0.0);
      glVertex3f(10.0f,  10.0f, -10.0f);
      glVertex3f(10.0f,  10.0f,  10.0f);
      glVertex3f(10.0f, -10.0f,  10.0f);
      glVertex3f(10.0f, -10.0f, -10.0f);
	glEnd();

	glBegin(GL_POLYGON);
      // Left face (x = -10.0f) D
	  glNormal3f(1.0f,0.0,0.0);
      glVertex3f(-10.0f,  10.0f,  10.0f);
      glVertex3f(-10.0f,  10.0f, -10.0f);
      glVertex3f(-10.0f, -10.0f, -10.0f);
      glVertex3f(-10.0f, -10.0f,  10.0f);
	glEnd();
		  
	glBegin(GL_POLYGON);
		glNormal3f(0.0,0.0,1.0);
	  // Top face (y = 1.0f)
      glVertex3f( 10.0f, 10.0f, -10.0f);
      glVertex3f(-10.0f, 10.0f, -10.0f);
      glVertex3f(-10.0f, 10.0f,  10.0f);
      glVertex3f( 10.0f, 10.0f,  10.0f);
	glEnd();
		  
	glBegin(GL_POLYGON);
	  glNormal3f(0.0,0.0,1.0);
      // Bottom face (y = -10.0f)
      glColor3f(0.0f, 1.0f, 0.0f);
      glVertex3f( 10.0f, -10.0f,  10.0f);
      glVertex3f(-10.0f, -10.0f,  10.0f);
      glVertex3f(-10.0f, -10.0f, -10.0f);
      glVertex3f( 10.0f, -10.0f, -10.0f);
	glEnd();

	
	glBegin(GL_POLYGON);
	  glNormal3f(0.0,1.0,0.0);
      // Front face  (z = 10.0f)
      glVertex3f( 10.0f,  10.0f, 10.0f);
      glVertex3f(-10.0f,  10.0f, 10.0f);
      glVertex3f(-10.0f, -10.0f, 10.0f);
      glVertex3f( 10.0f, -10.0f, 10.0f);
	glEnd();
 	
	glBegin(GL_POLYGON);
	  glNormal3f(0.0,1.0,0.0);
      // Back face (z = -10.0f)
      glVertex3f( 10.0f, -10.0f, -10.0f);
      glVertex3f(-10.0f, -10.0f, -10.0f);
      glVertex3f(-10.0f,  10.0f, -10.0f);
      glVertex3f( 10.0f,  10.0f, -10.0f);
	glEnd();

}

//============================================================
static void draw( void )
{
	glClearColor (0.0, 0.0, 0.2, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	glPushMatrix();

// Placement and rotation of the scene.
	glTranslatef(0.0,0.0,-5*SIZE);
	glRotatef(beta, 1.0, 0.0, 0.0);
	glRotatef(alpha, 0.0, 1.0, 0.0);
	MakeShapes();
	glFlush();			         
	glPopMatrix(); 

// This command will swap animation buffers to display the current frame.
    glutSwapBuffers();
}

//============================================================
static void idle( void )
{
// This function will call draw() as frequent as possible thus enabling us to make 
// interaction and animation.

   draw();
}

//============================================================
static void hotkey(unsigned char k, int x, int y)
{
// Here we are processing keyboard events.
   switch (k) 
   {
      case 27:
	  exit (0);
	  break;

// Switch to wireframe rendering
	  case 'w':
	  RendMode=1;
      break;

// Switch to flat shading
	  case 'f':
	  RendMode=2;
	  glShadeModel(GL_FLAT);
	  break;		  
   }
}

//============================================================
static void sfunc(int k, int x, int y)
{
// Here we can process function keys and other special key events
   switch (k) 
   {

// Rotate to the left
	  case GLUT_KEY_LEFT:
	  alpha-=3.0;
	  break;

// Rotate to the right
	  case GLUT_KEY_RIGHT:
	  alpha+=3.0;
	  break;
   }
}

//============================================================
void main( int argc, char *argv[] )
{

// This is the main program where glut functions are invoked to set up 
// the graphics window as well as to define the callback functions.
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB );
   glutInitWindowSize (650, 650); 
   glutInitWindowPosition (100, 100);
   glutCreateWindow ("Sample OpenGL program.          \
   Use  -->,  <--,  <f>,  and  <Esc> keys.");

   Init();
   glutReshapeFunc(reshape);
   glutIdleFunc(idle);
   glutDisplayFunc(draw);
   glutKeyboardFunc(hotkey);
   glutSpecialFunc(sfunc);

// Refer to GLUT manual on how to set up and use pop-up menus.

// The main event loop is started here.
   glutMainLoop();
}
