// This is a sample OpenGL/CLUT program for SC208 course.
// Developed by Alexei Sourin, October 2003.
// The students are to study this code and use it 
// as a template for their assignment.
// Open a new project "Win32 Console Application" and add
// sample.c to Source Files 

#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>

// The unit size used in this application's World Coordinate System. 
// All other dimensions and parameters are made proportional to it.
#define	SIZE	10.0


int RendMode = GL_LINE;
double alpha, beta;
float ang_rot = 05.0; 
float vrot_x, vrot_y, vrot_z; 
float local_scale = 0.9f;
typedef float f4d[4];


typedef struct st_vector_int
{
	int n;			// numero de elementos
	int *indice;	// elemento - indice del vertice
	f4d vNormal;    // vetor normal da face
} vector_int;


typedef struct st_vector_f4d
{
	int n;				// numero de vertices
	f4d *vPoint;	// coordenadas de vertice
} vector_f4d;

typedef struct st_vector_faces
{
	int n;						// numero de faces
	vector_int *faceVertice;	// faces por indice vertices
} vector_faces;

typedef struct st_un_objeto
{
	vector_f4d *vertices;		// vector de vertices
	vector_faces *faces;		// vector de faces
} un_objeto;

typedef struct st_objetos
{
	int n;                      // numero de objetos
	un_objeto *obs;
} objetos;                      // n objetos

// ===========================================

un_objeto *objeto = NULL;		// un objeto

// ===========================================
   // propriedades material
float mat_ambiente[] = {0.3, 0.0, 0.0, 1.0};
float mat_difuso[] = {0.5, 0.0, 0.0, 1.0};
float mat_especular[] = {1.0, 1.0, 1.0, 1.0};
float mat_emissao[] = {0, 0, 0, 1};
float mat_brilho[] = {50.0};

// Defining a point light source parameters
GLfloat light_position1[] = {   0.0,   10.0*SIZE, 100.0*SIZE, 1.0 };
GLfloat light_ambient[]   = {   0.1,   0.1,   0.1, 1.0 };
GLfloat light_diffuse[]   = {   1.0,   1.0,   1.0, 1.0 };
GLfloat light_specular[]  = {   1.0,   1.0,   1.0, 1.0 };
float movLuz = 10.0;   // angulo inicial da luz

//============================================================
static void Init(void)
{
// This function is called only once before any other functions are called
	alpha=-20.0;
	beta=20.0;
	RendMode=1;

   vrot_x = 0.0;
   vrot_y = 0.0;
   vrot_z = 1.0;
   
 glClearColor(1.0, 1.0, 1.0, 0.0);

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
  glEnable(GL_MAP2_VERTEX_3);
  glMapGrid2f(20, 0.0, 1.0, 20, 0.0, 1.0);
  void CreateLineXYZ();
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
  
    float luz_Posicao[] = {movLuz, 2, 3, 2};
    glLightfv(GL_LIGHT0, GL_POSITION, luz_Posicao);

   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  CreateLineXYZ();

glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambiente);
glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_difuso);
glMaterialfv(GL_FRONT, GL_SPECULAR, mat_especular);

glMaterialfv(GL_FRONT, GL_SHININESS, mat_brilho);

glEnable(GL_COLOR_MATERIAL);

   glPushMatrix();
   glRotatef(ang_rot, vrot_x, vrot_y, vrot_z);
   DisenaObjeto(objeto);    // disenhando un objeto
   glPopMatrix();

   glutSwapBuffers();

}


un_objeto* liberaObjeto(un_objeto* obj)
{
	int i, j;

	if(obj)
	{
		if(obj->vertices)
		{
			if(obj->vertices->vPoint)
				free(obj->vertices->vPoint);
			free(obj->vertices);
		}
		if(obj->faces)
		{
			if(obj->faces->faceVertice)
			{
				for(j=0; j<objeto->faces->n; j++)
					free(objeto->faces->faceVertice[j].indice);

				free(obj->faces->faceVertice);
			}
			free(objeto->faces);
		}

		free(obj);
	}
	return NULL;
}

void CreateLineXYZ(void)
{

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
}


int procSolido(char *arch)
{
  FILE *fobj;
  char token[40];
  float px, py, pz;
  int i, j, n, m;

  printf(" \n ler  %s  \n", arch);

  if((fobj=fopen(arch,"rt"))==NULL)
  {
     printf("Error en la apertura del archivo %s \n", arch);
     return 0; 
  }
   
  if (objeto) objeto = liberaObjeto(objeto);

  if((objeto = (un_objeto*) malloc(sizeof(un_objeto)))==NULL)
  {
		printf("\n Error en alocacion de memoria para un_objeto");
		return 0;
  }

  fgets(token, 40, fobj);  /* #cabecalho */ 
  fscanf(fobj,"%s %d", token, &n);

  objeto->vertices = (vector_f4d*) malloc(sizeof(vector_f4d));

  objeto->vertices->n = n;
  objeto->vertices->vPoint = (f4d*) calloc(n, sizeof(f4d));
  for(j=0; j<objeto->vertices->n; j++)
  {
	 fscanf(fobj, "%s %f %f %f", token, &px, &py, &pz);
	 
     objeto->vertices->vPoint[j][0] = px * local_scale;     
     objeto->vertices->vPoint[j][1] = py * local_scale;
     objeto->vertices->vPoint[j][2] = pz * local_scale;     
	 objeto->vertices->vPoint[j][3] = 0.0f;
  }

  fscanf(fobj,"%s %i", token, &n);

  objeto->faces = (vector_faces*) malloc(sizeof(vector_faces));

  objeto->faces->n = n;
  objeto->faces->faceVertice = (vector_int*) calloc(n, sizeof(vector_int));
  for(j=0; j<objeto->faces->n; j++)
  {
	 fscanf(fobj, "%s %i", token, &m);
	 objeto->faces->faceVertice[j].n = m;
	 objeto->faces->faceVertice[j].indice = (int*) calloc(m, sizeof(int));

	 for(i=0; i<objeto->faces->faceVertice[j].n; i++)
	 {
		fscanf(fobj, "%i", &m);
		objeto->faces->faceVertice[j].indice[i] = m;
	 }
  }

  fclose(fobj);
  return 1;
}

void processMenuEvents(int option) 
{
	if (option == 1)
		procSolido("data/dadosObjPlanar.txt");
	else if (option == 2)
		procSolido("data/dadosObj1.txt");
	else if(option == 3)
		RendMode = GL_LINE;
	else if (option == 4)
		RendMode = GL_FILL;
	else if (option == 5)
		procSolido("data/dadosObjPlanar2.txt");
	else if (option == 6)
		procSolido("data/dadosObj2.txt");

	glutPostRedisplay();
}

void createGLUTMenus() 
{
	int menu, submenu1, submenu2;
	
	submenu1 = glutCreateMenu(processMenuEvents);
	glutAddMenuEntry("Objeto aberto 1",1);
	glutAddMenuEntry("Objeto Fechado 1",2);
	glutAddMenuEntry("Objeto aberto 2",5);
	glutAddMenuEntry("Objeto Fechado 2",6);

	submenu2 = glutCreateMenu(processMenuEvents);
	glutAddMenuEntry("Aramado",3);
	glutAddMenuEntry("Realismo",4);

	menu = glutCreateMenu(processMenuEvents);
	glutAddSubMenu("Tipo de Objeto",submenu1);
	glutAddSubMenu("Exibe objeto",submenu2);

	glutAttachMenu(GLUT_RIGHT_BUTTON);
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


void calculaNormalFace(vector_int *fac, vector_f4d *vert)
{
	f4d a, b, vNorm;
	float s;
	int nn, io, ia, ib;

	nn = fac->n - 1;
	io = fac->indice[0];
	ia = fac->indice[nn];
	ib = fac->indice[1];

	a[0] = vert->vPoint[ia][0] - vert->vPoint[io][0];
	a[1] = vert->vPoint[ia][1] - vert->vPoint[io][1];
	a[2] = vert->vPoint[ia][2] - vert->vPoint[io][2];

	b[0] = vert->vPoint[ib][0] - vert->vPoint[io][0];
	b[1] = vert->vPoint[ib][1] - vert->vPoint[io][1];
	b[2] = vert->vPoint[ib][2] - vert->vPoint[io][2];
	
	vNorm[0] = a[1]*b[2] - a[2]*b[1];
	vNorm[1] = a[2]*b[0] - a[0]*b[2];
	vNorm[2] = a[0]*b[1] - a[1]*b[0];

	s = sqrt(vNorm[0]*vNorm[0]+vNorm[1]*vNorm[1]+vNorm[2]*vNorm[2]);

	fac->vNormal[0] = vNorm[0] / s;
	fac->vNormal[1] = vNorm[1] / s;
	fac->vNormal[2] = vNorm[2] / s;
}


void DisenaObjeto(un_objeto *obj)
{
	int i, j, h;

	if(!obj)
		return;

	glPolygonMode(GL_FRONT_AND_BACK, RendMode);
glColor3f(0.9f, 1.0f, 0.1f); // troca cor
	for(i=0; i<obj->faces->n; i++)
	{
		glBegin(GL_POLYGON);
		   	if(RendMode>1) {
		   		calculaNormalFace(&(obj->faces->faceVertice[i]), obj->vertices); // computa normal da face
				glNormal3fv(obj->faces->faceVertice[i].vNormal);
		   	}
			for(j=0; j<obj->faces->faceVertice[i].n; j++)
			{
				h = obj->faces->faceVertice[i].indice[j];
				glVertex3fv(obj->vertices->vPoint[h]);
			}
		glEnd();	
	}
}  


//============================================================
void main( int argc, char *argv[] )
{

// This is the main program where glut functions are invoked to set up 
// the graphics window as well as to define the callback functions.
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH );
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

   createGLUTMenus(); 

// Refer to GLUT manual on how to set up and use pop-up menus.

// The main event loop is started here.
   glutMainLoop();
}
