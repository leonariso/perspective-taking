# ifdef _WIN32
	#include <windows.h>
# endif
#ifdef __APPLE__
	#include <OpenGL/gl.h>
	#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glut.h>
#endif

#include <cstdlib>
#include <memory>
#include <cstdio>
#include <cmath>
#include <string>
#include <algorithm>
#include <iostream>
#include <vector>

#include "glm.h"	// Library GLM 0.3.2 installed from http://devernay.free.fr/hacks/glm/


#include <XnOS.h>
#include <XnCppWrapper.h>
#include <XnCodecIDs.h>

#include "CRForestEstimator.h"

#define bool int
#define true 1
#define false 0

#define PI 3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679821

using namespace xn;
using namespace std;
using namespace cv;


/* Projection Parameters (Ortho/Perspective) */
double _left = 0.0;
double _right = 0.0;
double _bottom = 0.0;
double _top = 0.0;
double _zNear = 0.1;	// must be positive (and never zero)
double _zFar = 10.0;	// must be positive
double _nearVal= 1.5;	// must be positive (and never zero)
double _farVal= 20.0;	// must be positive
double fovy = 45.0;
double prev_z = 0;
double scale_f = 1.5;	// scale factor when screen is reset

/* Mouse Control Variables  */
int _mouseX = 0;
int _mouseY = 0;
bool _mouseLeft = false;
bool _mouseMiddle = false;
bool _mouseRight = false;

double _dragPosX = 0.0;
double _dragPosY = 0.0;
double _dragPosZ = 0.0;


/* Modelview Matrix */
double _matrix[16];
double _matrixI[16];
double vlen(double x, double y, double z);
void pos(double *px, double *py, double *pz, const int x, const int y,
	 const int *viewport);
void getMatrix();
void invertMatrix(const GLdouble * m, GLdouble * out);

int full_screen = 0;


/* Global Variables */
GLMmodel *pmodel = NULL;	/* the loaded model */
char filename[256];
GLuint mode = 0;			/* viewing mode */

int ww, wh;				/* window width and height */
int wireframe = 0;		/* Draw modes */
int show_axis = 1;
int smooth = 1;
int material = 1;
int textured = 1;
int two_sided = 1;
int show_help = 1;		/* help toggle */
int benchmark = 0;
int benchmark2 = 0;
int centerZ = -3;
float auto_rotate = 0;
double orbitDegrees = 1;
int orbitReverse = 1;
int xrotate = 0;
int yrotate = 1; 		/* orbit the Y axis */
int zrotate = 0;

int lighting = 1;


#ifdef SMOOTH_HINT
int smooth_hint = 0;
#endif
void *Help_Font = GLUT_BITMAP_8_BY_13;
int linestart = 10;		/* start point on y axis for text lines */
int linespace = 20;		/* spac betwwen text lines */


/* Lighting Variables */
GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };

GLfloat mat_ambient[] = { 0.7, 0.7, 0.7, 1.0 };
GLfloat mat_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat high_shininess[] = { 100.0 };

/* Head Control variables */ 
string g_treepath;
int g_ntrees;
float g_maxv = 1000.f;
float g_larger_radius_ratio = 1.f;
float g_smaller_radius_ratio = 6.f;
int g_stride = 5;
int g_max_z = 0;
int g_th = 400;
float g_prob_th = 1.0f;

CRForestEstimator* g_Estimate;

Mat g_im3D;

std::vector< cv::Vec<float,POSE_SIZE> > g_means;
std::vector< std::vector< Vote > > g_clusters;
std::vector< Vote > g_votes;

float g_means_03, g_means_04; 
float queue_03[5], queue_04[5];
float wma_03, wma_04;

double d_pitch_p;
double d_yaw_p;
double dx_p;
double dy_p;


/* Kinect OpenNI variables */ 
XnStatus g_RetVal;
xn::Context g_Context;
xn::DepthGenerator g_DepthGenerator;
XnUInt64 g_focal_length;
XnDouble g_pixel_size;
DepthMetaData g_depthMD;
int g_im_w = 640;
int g_im_h = 480;
int g_fps = 30;


/* Head Control functions */
void HeadControl(void);
void initialize();
void loadConfig(const char* filename);
bool read_data();
bool process();
void idle();

/* Standard Glut interface functions */
void Reshape(int w, int h);
void Mouse(int button, int state, int x, int y);
void Motion(int x, int y);
void Keyboard(unsigned char key, int x, int y);
void Display(void);
void AutoSpin(void);

/* Model and help display functoins */
void DrawModel(void);
void DrawAxis(float scale);
void HelpDisplay(GLint ww, GLint wh);
void HelpRenderBitmapString(float x, float y, void *font, char *string);



/*****************************************************************************/
/*Main Function***************************************************************/
/*****************************************************************************/


int main(int argc, char **argv)
{
	int i;

	if (argc > 1)
	{
		strncpy(filename, argv[argc - 1], sizeof(filename)); 

		for (i = 0; i < argc; i++)
		{
			if (strstr(argv[i], "-f"))
			full_screen = 1;
		}
	}
	else
	{
		printf("Usage: %s [-f] <obj filename>\n", argv[0]);
		exit(0);
	}

	loadConfig("config.txt");

	g_Estimate =  new CRForestEstimator();
	if( !g_Estimate->loadForest(g_treepath.c_str(), g_ntrees) )
	{
		cerr << "could not read forest!" << endl;
		exit(-1);
	}

	initialize();

	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(1024, 768);
	glutInitWindowPosition(50, 50); 
	glutCreateWindow("Perspective Taking");

	if (full_screen)
		glutFullScreen();

	glutDisplayFunc(Display);
	glutKeyboardFunc(Keyboard);
	getMatrix();
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClearAccum(0.0, 0.0, 0.0, 0.0);
	glutReshapeFunc(Reshape);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutIdleFunc(NULL);

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);

	if (lighting)
		glEnable(GL_LIGHTING);

	if (lighting)
		glEnable(GL_LIGHT0);

	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glEnable(GL_TEXTURE_2D);

	#ifdef SMOOTH_HINT
	if (smooth_hint)
	{
		glEnable(GL_LINE_SMOOTH);
		// glEnable (GL_POLYGON_SMOOTH);
		glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
		// glHint (GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	}
	#endif				/*0 */

	if (!pmodel)
	{
		/* load up the model */

		pmodel = glmReadOBJ(filename);
		if (!pmodel)
		{
			printf("\nUsage: objviewV2 <-s> <obj filename>\n");
			exit(0);
		}
		glmUnitize(pmodel);
		glmVertexNormals(pmodel, 90.0, GL_TRUE);
	}

	// glutIdleFunc(idle);

	glutMainLoop();

	return 0;
}



/*****************************************************************************/
/*Mouse Functions*************************************************************/
/*****************************************************************************/


void Reshape(int w, int h)
{
	/* Set the viewport  */
    glViewport(0, 0, w, h);

    ww = w;
    wh = h;

    _top = 1.0;
    _bottom = -1.0;
    _left = -(double) w / (double) h;
    _right = -_left;

    /* Set the projection tranform */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // glOrtho(_left,_right,_bottom,_top,_nearVal,_farVal);		 /* Orthographic projection */
    // glFrustum(_left,_right,_bottom,_top,_nearVal,_farVal);	 /* Perspective projection  */
    gluPerspective(fovy, (double) w / (double) h, _zNear, _zFar); /* Perspective projection  */

    glMatrixMode(GL_MODELVIEW);
}


void Mouse(int button, int state, int x, int y)
{
    int viewport[4];

    _mouseX = x;
    _mouseY = y;

    if (state == GLUT_UP)
		switch (button)
		{
			case GLUT_LEFT_BUTTON:
			    _mouseLeft = false;
			    break;
			case GLUT_MIDDLE_BUTTON:
			    _mouseMiddle = false;
			    break;
			case GLUT_RIGHT_BUTTON:
			    _mouseRight = false;
			    break;
	    }
    else
		switch (button)
		{
			case GLUT_LEFT_BUTTON:
			    _mouseLeft = true;
			    break;
			case GLUT_MIDDLE_BUTTON:
			    _mouseMiddle = true;
			    break;
			case GLUT_RIGHT_BUTTON:
			    _mouseRight = true;
			    break;
		}

    glGetIntegerv(GL_VIEWPORT, viewport);
    pos(&_dragPosX, &_dragPosY, &_dragPosZ, x, y, viewport);
}


void Motion(int x, int y)
{
    bool changed = false;

    const int dx = x - _mouseX;
    const int dy = y - _mouseY;

    int viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    if (dx == 0 && dy == 0)
		return;

    if (_mouseMiddle || (_mouseLeft && _mouseRight))
    {
    	/* Zoom */

		double s = exp((double)dy*0.01); 
		glScalef(s,s,s); 
		if(abs(prev_z) <= 1.0) 

		glLoadIdentity();
		glTranslatef(0, 0, dy * 0.01);
		glMultMatrixd(_matrix);

		changed = true;

		// printf ("\n s = %f ", s);
    }
    else if (_mouseLeft)
    {
    	/* Rotate */

		double ax, ay, az;
		double bx, by, bz;
		double angle;

		ax = dy;
		ay = dx;
		az = 0.0;
		angle = vlen(ax, ay, az) / (double) (viewport[2] + 1) * 180.0;

		/* Use inverse matrix to determine local axis of rotation */

		bx = _matrixI[0] * ax + _matrixI[4] * ay + _matrixI[8] * az;
		by = _matrixI[1] * ax + _matrixI[5] * ay + _matrixI[9] * az;
		bz = _matrixI[2] * ax + _matrixI[6] * ay + _matrixI[10] * az;

		glRotatef(angle, bx, by, bz);

		changed = true;
    }
    else if (_mouseRight)
    {
    	/* Translate */

		double px, py, pz;

		pos(&px, &py, &pz, x, y, viewport);

		glLoadIdentity();
		glTranslatef(px - _dragPosX, py - _dragPosY, pz - _dragPosZ);
		glMultMatrixd(_matrix);

		_dragPosX = px;
		_dragPosY = py;
		_dragPosZ = pz;

		changed = true;
    }

    _mouseX = x;
    _mouseY = y;

    if (changed)
    {
		getMatrix();
		glutPostRedisplay();
    }
}


void AutoSpin(void)
{
    if (xrotate || yrotate || zrotate)
    {
		// printf ("\nXrot =%d, yRot = %d, ZRot = %d", xrotate, yrotate, zrotate);

		glRotatef(orbitDegrees*orbitReverse, xrotate, yrotate, zrotate);
		getMatrix();
		glutPostRedisplay();
    }
}



/*****************************************************************************/
/*Head Control Functions******************************************************/
/*****************************************************************************/


void initialize()
{
	std::cout << endl << "Initializing Kinect... " << endl << endl;

	g_RetVal = g_Context.Init();

	g_RetVal = g_DepthGenerator.Create(g_Context);
	if (g_RetVal != XN_STATUS_OK)
		printf("Failed creating DEPTH generator %s\n", xnGetStatusString(g_RetVal));

	XnMapOutputMode outputMode;
	outputMode.nXRes = g_im_w;
	outputMode.nYRes = g_im_h;
	outputMode.nFPS = g_fps;
	g_RetVal = g_DepthGenerator.SetMapOutputMode(outputMode);
	if (g_RetVal != XN_STATUS_OK)
		printf("Failed setting the DEPTH output mode %s\n", xnGetStatusString(g_RetVal));

	g_RetVal = g_Context.StartGeneratingAll();
	if (g_RetVal != XN_STATUS_OK)
		printf("Failed starting generating all %s\n", xnGetStatusString(g_RetVal));

	g_DepthGenerator.GetIntProperty ("ZPD", g_focal_length);
	g_DepthGenerator.GetRealProperty ("ZPPS", g_pixel_size);
	g_pixel_size *= 2.f;

	g_im3D.create(g_im_h,g_im_w,CV_32FC3);
}


void loadConfig(const char* filename)
{

	ifstream in(filename);
	string dummy;

	if(in.is_open()) {

		in >> dummy;
		in >> g_treepath;

		in >> dummy;
		in >> g_ntrees;

		in >> dummy;
		in >> g_maxv;

		in >> dummy;
		in >> g_larger_radius_ratio;

		in >> dummy;
		in >> g_smaller_radius_ratio;

		in >> dummy;
		in >> g_stride;

		in >> dummy;
		in >> g_max_z;

		in >> dummy;
		in >> g_th;

	}
	else
	{
		cerr << "File not found " << filename << endl;
		exit(-1);
	}

	in.close();

	cout << endl << "------------------------------------" << endl << endl;
	cout << "Estimation:       " << endl;
	cout << "Trees:            " << g_ntrees << " " << g_treepath << endl;
	cout << "Stride:           " << g_stride << endl;
	cout << "Max Variance:     " << g_maxv << endl;
	cout << "Max Distance:     " << g_max_z << endl;
	cout << "Head Threshold:   " << g_th << endl;

	cout << endl << "------------------------------------" << endl << endl;
}


bool read_data()
{
	g_RetVal = g_Context.WaitAndUpdateAll();
	if (g_RetVal != XN_STATUS_OK)
	{
		printf("Failed updating data: %s\n", xnGetStatusString(g_RetVal));
		return false;
	}

	g_DepthGenerator.GetMetaData(g_depthMD);

	float f = g_focal_length/g_pixel_size;
	int valid_pixels = 0;

	for(int y = 0; y < g_im3D.rows; y++)
	{
		Vec3f* Mi = g_im3D.ptr<Vec3f>(y);
		for(int x = 0; x < g_im3D.cols; x++){

			float d = (float)g_depthMD(x,y);

			if ( d < g_max_z && d > 0 ){

				valid_pixels++;

				Mi[x][0] = ( float(d * (x - 320)) / f );
				Mi[x][1] = ( float(d * (y - 240)) / f );
				Mi[x][2] = d;

			}
			else
				Mi[x] = 0;

		}
	}

	return true;
}


bool process()
{
	read_data();

	g_means.clear();
	g_clusters.clear();
	g_votes.clear();

	g_Estimate->estimate( 	g_im3D,
							g_means,
							g_clusters,
							g_votes,
							g_stride,
							g_maxv,
							g_prob_th,
							g_larger_radius_ratio,
							g_smaller_radius_ratio,
							false,
							g_th
						);

	if(g_means.size()>0)
	{
		// cout << "Nose Coord (mm): "  << g_means[0][0] << " \t" << g_means[0][1] << "\t" << g_means[0][2] <<endl;
		cout << "Rot Angles (deg): " << g_means[0][3] << " \t" << g_means[0][4] << "   \t" << g_means[0][5] <<endl;
		
		g_means_03 = g_means[0][3];
		g_means_04 = g_means[0][4];
	}
	else
	{
		g_means_03 = 0.0;
		g_means_04 = 0.0;
	}

	/* FIFO queue for storing last five values */
	for (int i=0; i<4; i++)
	{
		queue_03[i] = queue_03[i+1];
		queue_04[i] = queue_04[i+1];
	}
	queue_03[4] = g_means_03;
	queue_04[4] = g_means_04;

	/* Calculate WMA (Weighted Mean Average) */
	wma_03 = 0.0;
	wma_04 = 0.0;
	for (int i=0; i<5; i++)
	{
		wma_03 += queue_03[i]*(i+1.0)/15.0;
		wma_04 += queue_04[i]*(i+1.0)/15.0;
	}

	return true;
}


void idle()
{
	process();
}


void HeadControl(void)
{
	/* Basic camera with Pitch and Yaw (omitting Roll, i.e. angle psi) */

	process();

	double d_pitch 	= - wma_03;		// change of angle theta in deg 
	double d_yaw 	= - wma_04;		// change of angle phi in deg

	std::cout << endl << "      yaw:    " << d_yaw   << endl << endl;
	std::cout << endl << "      pitch:  " << d_pitch << endl << endl;

	double Sp = sin(d_pitch / 180 * PI );
	double Sy = sin(d_yaw   / 180 * PI );

	double cam_dist = 3.0;

	double dx = cam_dist * Sy;
	double dy = - cam_dist * Sp; 		// mind the minus sign

	glLoadIdentity();

	/* Reset (Invert) Previous Transformation */
	glTranslated(- dx_p, 0.0, 0.0);
	glRotated(d_yaw_p,0,1,0);
	glTranslated(0.0, - dy_p, 0.0);
	glRotated(d_pitch_p,1,0,0);

	/* Set Current Transformation */
	glRotated(- d_pitch,1,0,0);
	glTranslated(0.0, dy, 0.0);
	glRotated(- d_yaw,0,1,0);
	glTranslated(dx, 0.0, 0.0);

	glMultMatrixd(_matrix);
	getMatrix();

	d_pitch_p = d_pitch;
	d_yaw_p = d_yaw;
	dx_p = dx;
	dy_p = dy;

	glutPostRedisplay();
}



/*****************************************************************************/
/*Keyboard input**************************************************************/
/*****************************************************************************/


void Keyboard(unsigned char key, int x, int y)
{

    switch (key)
    {
	    case 'a':
	    case 'A':
	    {
		    show_axis = !show_axis;
		    break;
		}
		case 'b':
	    case 'B':
		{
		    benchmark = !benchmark;
		    if (benchmark)
		    {
				glutIdleFunc(AutoSpin);
			}
		    else
		    {
				glutIdleFunc(NULL);
			}
		    break;
		}
		case 'c':
	    case 'C':
	    {
	    	benchmark2 = !benchmark2;
		    if (benchmark2)
		    {
				glutIdleFunc(HeadControl);
			}
		    else
			{
				glutIdleFunc(NULL);
			}
		    break;
		}
	    case 'd':
	    case 'D':
	    {
		    two_sided = !two_sided;
		    break;
		}
	#ifdef AVL
	    case 'f':
	    case 'F':
	    {
		    glmFlipModelTextures(pmodel);
		    break;
		}
	#endif
		case 'h':
	    case 'H':{
		    show_help = !show_help;
		    break;
		}
	    case 'l':
	    case 'L':
	    {
	    	lighting = !lighting;
			break;
	    }
	#ifdef SMOOTH_HINT
	    case 'l':
	    case 'L':
	    {
		    smooth_hint = !smooth_hint;
		    if (smooth_hint)
		    {
				glEnable(GL_LINE_SMOOTH);
				// glEnable (GL_POLYGON_SMOOTH);
				glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
				// glHint (GL_POLYGON_SMOOTH_HINT, GL_NICEST);
		    }
		    else
		    {
				glDisable(GL_LINE_SMOOTH);
				// glDisable (GL_POLYGON_SMOOTH);
		    }
		    break;
		}
	#endif
	    case 'm':
	    case 'M':
	    {
		    material = !material;
		    if (!material && textured)
			textured = 0;
		    break;
		}
		case 'O':
	    case 'o':
		{
		    orbitReverse = orbitReverse * -1;
		    break;
		}
	    case 'r':
	    case 'R':{	    
		    /* Resetting Scene */

			glLoadIdentity();
			gluLookAt(	0.0,	0.0,	-0.05,		// eye (camera location)
						0.0,	0.0,	0.0,		// centre (lookint at)
						0.0, 	-1.0, 	0.0		);	// up (orientation vector)
			getMatrix();

			glScalef(scale_f,scale_f,scale_f);
			getMatrix();

		    break;
		}
	    case 's':
	    case 'S':
	    {
		    smooth = !smooth;
		    break;
		}
	    case 't':
	    case 'T':
	    {
		    textured = !textured;
		    if (!material && textured)
			material = 1;
		    break;
		}
	    case 'w':
	    case 'W':
	    {
		    wireframe = !wireframe;
		    break;
		}
	    case 'x':
	    case 'X':
		{
		    xrotate = !xrotate;
		    break;
		}
	    case 'y':
	    case 'Y':
		{
		    yrotate = !yrotate;
		    break;
		}
	    case 'z':
	    case 'Z':
		{
		    zrotate = !zrotate;
		    break;
		}
		case '+':
		case '=':
		{

			g_stride++;
			cout << "stride : " << g_stride << endl;
			break;

		}
		case '-':
		case '_':
		{

			g_stride = MAX(0, g_stride-1);
			cout << "stride : " << g_stride << endl;
			break;

		}
		case '*':
		case '8':
		{

			g_th += 20;
			cout << "head threshold : " << g_th << endl;
			break;
		}

		case '/':
		case '?':
		{

			g_th -= 20;
			cout << "head threshold : " << g_th << endl;
			break;
		}
	    case 9:				/* TAB */
		{
		    if (!full_screen)
		    {
				glutFullScreen();
				full_screen = 1;
		    }

		    break;
		}
	    case 27:			/* Esc */
	    {
			exit(0);
			break;
		}
	    default:
	    {
		    break;
		}			
    }

    glutPostRedisplay();
}



/*****************************************************************************/
/*Matrix and Vector Utility functions ****************************************/
/*****************************************************************************/


double vlen(double x, double y, double z)
{
    return sqrt(x * x + y * y + z * z);
}


void pos(double *px, double *py, double *pz, const int x, const int y,
	 const int *viewport)
{
    /*
       Use the ortho projection and viewport information
       to map from mouse co-ordinates back into world 
       co-ordinates
     */

    *px = (double) (x - viewport[0]) / (double) (viewport[2]);
    *py = (double) (y - viewport[1]) / (double) (viewport[3]);

    *px = _left + (*px) * (_right - _left);
    *py = _top + (*py) * (_bottom - _top);
    *pz = _zNear;
}


void getMatrix()
{
    glGetDoublev(GL_MODELVIEW_MATRIX, _matrix);
    invertMatrix(_matrix, _matrixI);
}


void invertMatrix(const GLdouble * m, GLdouble * out)
{

/* NB. OpenGL Matrices are COLUMN major!! */
#define MAT(m,r,c) (m)[(c)*4+(r)]

/* Here's some shorthand converting standard (row,column) to index. */
#define m11 MAT(m,0,0)
#define m12 MAT(m,0,1)
#define m13 MAT(m,0,2)
#define m14 MAT(m,0,3)
#define m21 MAT(m,1,0)
#define m22 MAT(m,1,1)
#define m23 MAT(m,1,2)
#define m24 MAT(m,1,3)
#define m31 MAT(m,2,0)
#define m32 MAT(m,2,1)
#define m33 MAT(m,2,2)
#define m34 MAT(m,2,3)
#define m41 MAT(m,3,0)
#define m42 MAT(m,3,1)
#define m43 MAT(m,3,2)
#define m44 MAT(m,3,3)

    GLdouble det;
    GLdouble d12, d13, d23, d24, d34, d41;
    GLdouble tmp[16];		/* Allow out == in. */

    /* Inverse = adjoint / det. (See linear algebra texts.) */

    /* pre-compute 2x2 dets for last two rows when computing */
    /* cofactors of first two rows. */
    d12 = (m31 * m42 - m41 * m32);
    d13 = (m31 * m43 - m41 * m33);
    d23 = (m32 * m43 - m42 * m33);
    d24 = (m32 * m44 - m42 * m34);
    d34 = (m33 * m44 - m43 * m34);
    d41 = (m34 * m41 - m44 * m31);

    tmp[0] = (m22 * d34 - m23 * d24 + m24 * d23);
    tmp[1] = -(m21 * d34 + m23 * d41 + m24 * d13);
    tmp[2] = (m21 * d24 + m22 * d41 + m24 * d12);
    tmp[3] = -(m21 * d23 - m22 * d13 + m23 * d12);

    /* Compute determinant as early as possible using these cofactors. */
    det = m11 * tmp[0] + m12 * tmp[1] + m13 * tmp[2] + m14 * tmp[3];

    /* Run singularity test. */
    if (det == 0.0)
    {
		// printf("invert_matrix: Warning: Singular matrix.\n");
		// memcpy(out,_identity,16*sizeof(double));
    }
    else
    {
		GLdouble invDet = 1.0 / det;
		/* Compute rest of inverse. */
		tmp[0] *= invDet;
		tmp[1] *= invDet;
		tmp[2] *= invDet;
		tmp[3] *= invDet;

		tmp[4] = -(m12 * d34 - m13 * d24 + m14 * d23) * invDet;
		tmp[5] = (m11 * d34 + m13 * d41 + m14 * d13) * invDet;
		tmp[6] = -(m11 * d24 + m12 * d41 + m14 * d12) * invDet;
		tmp[7] = (m11 * d23 - m12 * d13 + m13 * d12) * invDet;

		/* Pre-compute 2x2 dets for first two rows when computing */
		/* cofactors of last two rows. */
		d12 = m11 * m22 - m21 * m12;
		d13 = m11 * m23 - m21 * m13;
		d23 = m12 * m23 - m22 * m13;
		d24 = m12 * m24 - m22 * m14;
		d34 = m13 * m24 - m23 * m14;
		d41 = m14 * m21 - m24 * m11;

		tmp[8] = (m42 * d34 - m43 * d24 + m44 * d23) * invDet;
		tmp[9] = -(m41 * d34 + m43 * d41 + m44 * d13) * invDet;
		tmp[10] = (m41 * d24 + m42 * d41 + m44 * d12) * invDet;
		tmp[11] = -(m41 * d23 - m42 * d13 + m43 * d12) * invDet;
		tmp[12] = -(m32 * d34 - m33 * d24 + m34 * d23) * invDet;
		tmp[13] = (m31 * d34 + m33 * d41 + m34 * d13) * invDet;
		tmp[14] = -(m31 * d24 + m32 * d41 + m34 * d12) * invDet;
		tmp[15] = (m31 * d23 - m32 * d13 + m33 * d12) * invDet;

		memcpy(out, tmp, 16 * sizeof(GLdouble));
    }

#undef m11
#undef m12
#undef m13
#undef m14
#undef m21
#undef m22
#undef m23
#undef m24
#undef m31
#undef m32
#undef m33
#undef m34
#undef m41
#undef m42
#undef m43
#undef m44
#undef MAT
}



/*****************************************************************************/
/*OpenGL Display Functions****************************************************/
/*****************************************************************************/


void DrawModel(void)
{

    mode = GLM_NONE;		/* reset mode */

    if (smooth)
		mode = mode | GLM_SMOOTH;
    else
		mode = mode | GLM_FLAT;

    if (two_sided)
		mode = mode | GLM_2_SIDED;

    if (material)
		mode = mode | GLM_MATERIAL;
    else
		mode = mode | GLM_COLOR;

    if (textured && material)
		mode = mode | GLM_TEXTURE;

    glPushMatrix();

    if (pmodel)
		glmDraw(pmodel, mode);

    glPopMatrix();
}


void DrawAxis(float scale)
{
    glPushMatrix();
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glScalef(scale, scale, scale);	/* modeling transformation */ 

    glBegin(GL_LINES);

    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(.8f, 0.05f, 0.0);
    glVertex3f(1.0, 0.25f, 0.0);	/*  Letter X */
    glVertex3f(0.8f, .25f, 0.0);
    glVertex3f(1.0, 0.05f, 0.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(1.0, 0.0, 0.0);		/*  X axis */

    glColor3f(0.0, 1.0, 0.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 1.0, 0.0);		/*  Y axis */

    glColor3f(0.0, 0.0, 1.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 0.0, 1.0);		/*  Z axis */
    glEnd();
    if (lighting)
	glEnable(GL_LIGHTING);
    if (lighting)
	glEnable(GL_TEXTURE_2D);
    glColor3f(1.0, 1.0, 1.0);
    glPopMatrix();
}


void HelpDisplay(GLint ww, GLint wh)
{
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glColor3f(1.0f, 1.0f, 1.0f);


    /*  switch to projection mode */
    glMatrixMode(GL_PROJECTION);

    /*  save previous matrix which contains the  */
    /* settings for the perspective projection */
    glPushMatrix();

    /*  reset matrix */
    glLoadIdentity();

    /*  set a 2D orthographic projection */
    gluOrtho2D(0, ww, 0, wh);

    /*  invert the y axis, down is positive */
    glScalef(1, -1, 1);	

    /*  move the origin from the bottom left corner */
    /*  to the upper left corner */
    glTranslatef(0, -wh, 0);
    glMatrixMode(GL_MODELVIEW);


    glPushMatrix();
    glLoadIdentity();
    linestart = 10;

    HelpRenderBitmapString(30, linestart +=
			   linespace, Help_Font,
			   (char *) "Help Menu");
    HelpRenderBitmapString(30, linestart +=
			   linespace, Help_Font,
			   (char *)"---------");
    HelpRenderBitmapString(30, linestart +=
			   linespace, Help_Font,
			   (char *) "H/h = Toggle Help Menu");
    if (!full_screen)
	HelpRenderBitmapString(30, linestart +=
			    linespace, Help_Font,
			    (char *) "TAB = Activate Full Screen");
    HelpRenderBitmapString(30, linestart +=
			   linespace, Help_Font,
			   (char *) "Esc = Exit Program");
    HelpRenderBitmapString(30, linestart +=
			   linespace, Help_Font,
			   (char *) "R/r = Reset Position");
    HelpRenderBitmapString(30, linestart +=
			   linespace, Help_Font,
			   (char *) "C/c = Toggle Axis");
    HelpRenderBitmapString(30, linestart +=
			   linespace, Help_Font,
			   (char *) "A/a = Show/Hide Axis");
    HelpRenderBitmapString(30, linestart +=
			   linespace, Help_Font,
			   (char *) "W/w = Toggle Wireframe");
    HelpRenderBitmapString(30, linestart +=
			   linespace, Help_Font,
			   (char *) "D/d = Toggle Double Sided Polygons");
    HelpRenderBitmapString(30, linestart +=
			   linespace, Help_Font,
			   (char *) "S/s = Toggle Smooth Shading");
    HelpRenderBitmapString(30, linestart +=
			   linespace, Help_Font,
			   (char *) "L/l = Toggle Lighting");
    HelpRenderBitmapString(30, linestart +=
			   linespace, Help_Font,
			   (char *) "M/m = Toggle Materials");
    HelpRenderBitmapString(30, linestart +=
			   linespace, Help_Font,
			   (char *) "T/t = Toggle Textures");
    HelpRenderBitmapString(30, linestart +=
			   linespace, Help_Font,
			   (char *) "B/b = Toggle Auto Rotate");
    HelpRenderBitmapString(30, linestart +=
			   linespace, Help_Font,
			   (char *) "O/o = Reverse Auto Rotate");
    HelpRenderBitmapString(30, linestart +=
			   linespace, Help_Font,
			   (char *) "+ - = Incr/Decr Stride");
    HelpRenderBitmapString(30, linestart +=
			   linespace, Help_Font,
			   (char *) "* / = Incr/Decr Head Threshold");
    // HelpRenderBitmapString(30,linestart+=
    			// linespace,(void *)Help_Font,
    			// (char *) "L/l = Toggle Line Smoothing");
	#ifdef AVL
    HelpRenderBitmapString(30, linestart +=
			   linespace, Help_Font,
			   (char *) "F/f = Flip Textures");
	#endif

	glPopMatrix();

	/*  set the current matrix to GL_PROJECTION */
	glMatrixMode(GL_PROJECTION);
	/*  restore previous settings */
	glPopMatrix();
	/*  get back to GL_MODELVIEW matrix */
	glMatrixMode(GL_MODELVIEW);

	if (lighting)
	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
}


void HelpRenderBitmapString(float x, float y, void *font, char *string)
{
	char *c;
	/*  set position to start drawing fonts */
	glRasterPos2f(x, y);
	/*  loop all the characters in the string */
	for (c = string; *c != '\0'; c++)
	{
		glutBitmapCharacter(font, *c);
	}
}


void Display(void)
{
	if (lighting)
	{
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
	}
	else
	{
		glDisable(GL_LIGHTING);
		glDisable(GL_LIGHT0);
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();

	glLoadIdentity();
	glTranslatef(0, 0, centerZ);	/* to center object down Z */
	glMultMatrixd(_matrix);

	if (show_axis)
	    DrawAxis(1.0f);

	if (wireframe)		/* if Wireframe is checked */
	    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);	/* draw wireframe */
	else			/* else */
	    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);	/* draw filled polygons */

	DrawModel();
	glTranslatef(0, 0, -centerZ);	/* to center object down Z */
	glPopMatrix();

	if (show_help)
		HelpDisplay(ww, wh);

	glutSwapBuffers();
}
