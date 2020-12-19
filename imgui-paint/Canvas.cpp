#include "Canvas.h"


using namespace std;



enum stroke_t {
	head = 0,
	back,
	uArmL,
	uArmR,
	lArmL,
	lArmR,
	uLegL,
	uLegR,
	lLegL,
	lLegR
} strokeID = head;


const string Canvas::strokeDict[] = { 
		"head",
		"back",
		"uArmL",
		"uArmR",
		"lArmL",
		"lArmR",
		"uLegL",
		"uLegR",
		"lLegL",
		"lLegR"
	};


vector<Color> Canvas::colors;



int nStrokes = lLegR+1;

float red, green, blue;
int tmpx, tmpy; // store the first point when shape is line, rectangle or circle
int brushSize = 1;


int cur_bin = -1, cur_state = -1;



typedef vector<Dot> Stroke;

map<int, Color> Canvas::createColors() {

	map<int, Color> colorTypes;

	for(int i = 0; i < nStrokes; i++) {

		int h = 360 / nStrokes * i;
		int s = 100;
		int v = 100;
		Color c = Color::ColorFromHSV(h, s, v);
		colors.push_back(c);
		colorTypes[i] = c;
		
	}

	return colorTypes;
}




void Canvas::display(void)
{

	glLineWidth(brushSize);
	glBegin(GL_LINES);

	for(int s = 0; s < nStrokes; s++) {

		Color  color  = colors[s];
		Stroke stroke = frame->getStroke(s);
		
		glColor3f(color.r, color.g, color.b);

		if(stroke.size() > 0) {
			for (uint i = 0; i < stroke.size()-1; i++)
			{
				Vector3 pt1 = Vector3(stroke[i].x, stroke[i].y, 0);
				Vector3 pt2 = Vector3(stroke[i+1].x, stroke[i+1].y, 0);

				int xcenter = (x_max + x_min) / 2;
				int ycenter = (y_max + y_min) / 2;
				float xcenterN = (xcenter / static_cast<float>(hspan)) - 1.f;
				float ycenterN = ((window_h - ycenter) / static_cast<float>(vspan)) - 1.f;

				Vector3 centerN = Vector3(xcenterN, ycenterN, 0);

				pt1 -= centerN;
				pt2 -= centerN;

				Vector3 pt1T = transform.dot(pt1);
				Vector3 pt2T = transform.dot(pt2);
				
				pt1T += centerN;
				pt2T += centerN;

				glVertex2f(pt1T.x, pt1T.y);
				glVertex2f(pt2T.x, pt2T.y);
			}
		}

	}
	glEnd();
	glutSwapBuffers();
}


void Canvas::swatchSwitch(int s) {
	strokeID = static_cast<stroke_t>(s);
}



void Canvas::clear()
{
	frame->clearFrame();
	glClear(GL_COLOR_BUFFER_BIT);
	glutSwapBuffers();
}



void Canvas::drawDot(int mousex, int mousey)
{

	float normX = (mousex / static_cast<float>(hspan)) - 1.f;
	float normY = ((window_h - mousey) / static_cast<float>(vspan)) - 1.f;

	Dot newDot(normX, normY, 0);
	frame->addDot(strokeID, newDot);
}

void Canvas::mouse(int x, int y)
{	
	frame->clearStroke(strokeID);
}

void Canvas::motion(int x, int y)
{
	drawDot(x, y);
}

void Canvas::setTransform(Matrix3x3 t) {
	transform = t;
}

