
#include <GL/glut.h>
#include <stdlib.h> 
#include <stdio.h>   
#include <math.h>


#define	SIZE	10.0

int RendMode = GL_LINE; 

float ang_rot = 05.0; 

float vrot_x, vrot_y, vrot_z; 

float local_scale = 0.12f;

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

	// propriedades da luz
float luz_ambiente[] = {0.2, 0.2, 0.0, 0.0};  // ambiente
float luz_difusa[] = {1.0, 1.0, 1.0, 0.0};  // cor luz
float movLuz = 10.0;   // angulo inicial da luz
   
   // propriedades material
float mat_ambiente[] = {0.3, 0.0, 0.0, 1.0};
float mat_difuso[] = {0.5, 0.0, 0.0, 1.0};
float mat_especular[] = {1.0, 1.0, 1.0, 1.0};
float mat_emissao[] = {0, 0, 0, 1};
float mat_brilho[] = {50.0};

void calculaNormalFace(vector_int *face, vector_f4d *vertices)
{
	f4d a, b, vetorNormal;
	float s;
	int nn, io, ia, ib;

	nn = face->n - 1;
	io = face->indice[0];
	ia = face->indice[nn];
	ib = face->indice[1];

	a[0] = vertices->vPoint[ia][0] - vertices->vPoint[io][0];
	a[1] = vertices->vPoint[ia][1] - vertices->vPoint[io][1];
	a[2] = vertices->vPoint[ia][2] - vertices->vPoint[io][2];

	b[0] = vertices->vPoint[ib][0] - vertices->vPoint[io][0];
	b[1] = vertices->vPoint[ib][1] - vertices->vPoint[io][1];
	b[2] = vertices->vPoint[ib][2] - vertices->vPoint[io][2];
	
	vetorNormal[0] = a[1]*b[2] - a[2]*b[1];
	vetorNormal[1] = a[2]*b[0] - a[0]*b[2];
	vetorNormal[2] = a[0]*b[1] - a[1]*b[0];

	s = sqrt(vetorNormal[0]*vetorNormal[0]+vetorNormal[1]*vetorNormal[1]+vetorNormal[2]*vetorNormal[2]);

	face->vNormal[0] = vetorNormal[0] / s;
	face->vNormal[1] = vetorNormal[1] / s;
	face->vNormal[2] = vetorNormal[2] / s;
}

void DisenaObjeto(un_objeto *obj)
{
	int i, j, h;

	if(!obj)
		return;

	glPolygonMode(GL_FRONT_AND_BACK, RendMode);
glColor3f(0.0f, 0.0f, 1.0f);
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

void initlights(void) 
{
  
//   float position[] = {0.0, 0.0, 2.0, 1.0};

   glLightfv(GL_LIGHT0, GL_AMBIENT, luz_ambiente);
   glLightfv(GL_LIGHT0, GL_DIFFUSE, luz_difusa);
   
   glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.3);
   
   // habilitar ilumicao
   glEnable(GL_LIGHT0);
   glEnable(GL_NORMALIZE);
}

static void init(void)
{
   vrot_x = 0.0;
   vrot_y = 0.0;
   vrot_z = 1.0;
   
   glClearColor(1.0, 1.0, 1.0, 0.0);
   glEnable(GL_DEPTH_TEST);
   glEnable(GL_MAP2_VERTEX_3);
//   glEnable(GL_AUTO_NORMAL);
   glMapGrid2f(20, 0.0, 1.0, 20, 0.0, 1.0);
 //  initlights();       /* for lighted version only */
}

void display(void)
{
    float luz_Posicao[] = {movLuz, 2, 3, 2};
    glLightfv(GL_LIGHT0, GL_POSITION, luz_Posicao);

   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


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


void reshape(int w, int h)
{
   glViewport(0, 0, w, h);    // area de visao
   glClearColor(1, 1, 1, 1);  // fundo preto
   
   initlights(); 
   
   glMatrixMode(GL_PROJECTION); // matriz de projecao para ejecucao
   glLoadIdentity();            // reinicia com identdade
   if (w <= h)
      glOrtho(-5.0, 5.0, -5.0*(GLfloat)h/(GLfloat)w,
              5.0*(GLfloat)h/(GLfloat)w, -5.0, 5.0);  
            // alto, baixo, frente, fundo, esquerda, direita
   else
      glOrtho(-5.0*(GLfloat)w/(GLfloat)h,
              5.0*(GLfloat)w/(GLfloat)h, -5.0, 5.0, -5.0, 5.0);

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

void keyboard(int key, int x, int y)
{
   ang_rot +=1.0; 

   switch (key) {
	case GLUT_KEY_LEFT:
		    vrot_x +=1.0;
			break;
	case GLUT_KEY_RIGHT:
			vrot_x -=1.0;
			break;
	case GLUT_KEY_UP:
			vrot_y +=1.0;
			break;
	case GLUT_KEY_DOWN:
			vrot_y -=1.0;
			break;
    case 27:
         exit(0);
         break;
	default:
		break;
   }
   	glutPostRedisplay();
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


static void idle( void )
{
// This function will call draw() as frequent as possible thus enabling us to make 
// interaction and animation.

   draw();
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

	glutPostRedisplay();
}

void createGLUTMenus() 
{
	int menu, submenu1, submenu2;
	
	submenu1 = glutCreateMenu(processMenuEvents);
	glutAddMenuEntry("Objeto aberto",1);
	glutAddMenuEntry("Objeto Fechado",2);

	submenu2 = glutCreateMenu(processMenuEvents);
	glutAddMenuEntry("Aramado",3);
	glutAddMenuEntry("Realismo",4);

	menu = glutCreateMenu(processMenuEvents);
	glutAddSubMenu("Tipo de Objeto",submenu1);
	glutAddSubMenu("Exibe objeto",submenu2);

	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

int main(int argc, char** argv)
{
   glutInit(&argc, argv);

   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
   
   glutInitWindowPosition(100, 100);    // posicao da janela na tela
   glutInitWindowSize(650, 650);      // tamanho da janela (horiznotal e vertical)
   
   glutCreateWindow("Objetos 3D BREP - Comandos:  ->,  <-,  <f>,  and  <Esc> keys.");

   init();
   
   glutReshapeFunc(reshape);   // funcao de variacao de janela
   glutIdleFunc(idle);
   glutDisplayFunc(display);   // funcao de mostrar objetos
   glutSpecialFunc(keyboard);

   createGLUTMenus();  
   glutMainLoop();
   return 0;
}
