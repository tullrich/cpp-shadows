#include <iostream>
using std::endl;
using std::cout;

#include <fstream>
#include <sstream>

#include <string>
using std::string;

#include "GlCanvas.h"
#include "shaderutils.h"
#include "GL/glu.h"


GlCanvas::GlCanvas(int X, int Y, int W, int H, const char *L) : Fl_Gl_Window(X, Y, W, H, L)
{

}

void GlCanvas::draw() 
{

	if (!valid()) {
    	make_current();

    	glewExperimental = GL_TRUE;
    	glewInit();
    	glViewport(0, 0 ,w(), h());

    	init();
	}

  onDraw();
}


int GlCanvas::handle(int event) 
{
  switch(event) {
  case FL_PUSH:
    mouseDown(Fl::event_x(),  Fl::event_y());
    return 1;
  case FL_DRAG:
    mouseDrag( Fl::event_x(),  Fl::event_y());
    return 1;
  case FL_RELEASE:   
    return 1;
  case FL_FOCUS :
  case FL_UNFOCUS :
    return 1;
  case FL_KEYBOARD:
    return 1;
  case FL_SHORTCUT:
    return 1;
  default:
    // pass other events to the base class...
    return Fl_Gl_Window::handle(event);
  }
}
