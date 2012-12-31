#ifndef _GL_CANVAS_H
#define _GL_CANVAS_H

#include <FL/Fl.H>
#include <FL/Fl_Gl_Window.H>

#include "GL/glew.h"
#include "shadows-common.h"
#include "Shader.h"

class GlCanvas : public Fl_Gl_Window {

	void draw();
	int handle(int);

protected:
  virtual void onDraw() {};
  virtual void init() {};
  virtual void mouseDown(int x, int y) {};
  virtual void mouseDrag(int x, int y) {};

public:

  GlCanvas(int X, int Y, int W, int H, const char *L);
};


#endif /* _GL_CANVAS_H */