#include <iostream>

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Value_Input.H>
#include <FL/Fl_Hor_Value_Slider.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Pack.H>
#include <FL/Fl_Round_Button.H>
#include <FL/Fl_Button.H>

#include "GL/glew.h"
#include "REngine.h"

REngine *canvas;
Fl_Hor_Value_Slider  *light_x, *light_y, *light_z;
Fl_Hor_Value_Slider  *block_x, *block_y, *block_z;
Fl_Check_Button *shadowVolumesOn, *silhouttesOn, *sceneOn;
Fl_Check_Button *volumes_CullFront, *volumes_CullBack;

Fl_Pack *slider_g;
Fl_Pack *light_type_rg;
Fl_Round_Button *point_l, *infinite_l;
Fl_Button *spawn_b;
Fl_Hor_Value_Slider *scale, *fov, *znear, *zfar;

void fov_callback(Fl_Widget *w, void *data) {
  canvas->setFoV(fov->value(), znear->value(), zfar->value());
}

void block_callback(Fl_Widget *w, void *data) {
  canvas->setBlockPosition(block_x->value(), block_y->value(), block_z->value(), scale->value());
}

void spawnblock_callback(Fl_Widget *w, void *data)
{
  canvas->spawnBlockAt(block_x->value(), block_y->value(), block_z->value(), scale->value());
}

void xyz_callback(Fl_Widget *w, void *data) {
	canvas->setLightPosition(light_x->value(), light_y->value(), light_z->value());
}

void renderScene_callback(Fl_Widget *w, void *data) {
  canvas->setRenderScene(sceneOn->value());
}

void renderSV_callback(Fl_Widget *w, void *data) {
  canvas->setRenderShadowVolumes(shadowVolumesOn->value());
  if(shadowVolumesOn->value())
  {
    volumes_CullBack->activate();
    volumes_CullFront->activate();
  } else
  {
    volumes_CullBack->deactivate();
    volumes_CullFront->deactivate();
  }
}

void volumesCullFront_callback(Fl_Widget *w, void *data) {
  canvas->setVolumesCullFront(volumes_CullFront->value());
}

void volumesCullBack_callback(Fl_Widget *w, void *data) {
  canvas->setVolumesCullBack(volumes_CullBack->value());
}

void renderSilhouttes_callback(Fl_Widget *w, void *data) {
  canvas->setRenderSilhouttes(silhouttesOn->value());
}

void lightType_callback(Fl_Widget *w, void *data) {
  if (w == point_l)
  {
    point_l->set();
    infinite_l->value(0);
    canvas->setLightType(true);
  } else
  {
    infinite_l->set();
    point_l->value(0);
    canvas->setLightType(false);
  }
}


int main(int argc, char **argv)
{

  Fl_Window *window = new Fl_Window(25, 50, 800,600, "OpenGL Depth-Fail Test");
   canvas = new REngine(0,0, 800, 600, "TEST");

  window->end();
  window->show(argc, argv);
  canvas->show();

  Fl_Window *window2 = new Fl_Window( 850, 50, 200,600, "Variables");

  slider_g = new Fl_Pack(5, 5, 190, 90);

  light_x = new Fl_Hor_Value_Slider(5, 5, 190, 25, "");
  light_x->bounds(-5000, 5000);
  light_x->value(1000);
  light_x->step(15);
  light_x->callback(xyz_callback);
  slider_g->add(light_x);

  light_y = new Fl_Hor_Value_Slider(5, 5, 190, 25, "");
  light_y->bounds(-5000, 5000);
  light_y->value(1000);
  light_y->step(15);
  light_y->callback(xyz_callback);
  slider_g->add(light_y);


  light_z = new Fl_Hor_Value_Slider(5, 5, 190, 25, "");
  light_z->bounds(-5000, 5000);
  light_z->value(400);
  light_z->step(15);
  light_z->callback(xyz_callback);
  slider_g->add(light_z);

  sceneOn = new Fl_Check_Button(5, 95, 190, 30, "Render Scene");
  sceneOn->set();
  sceneOn->callback(renderScene_callback);
  slider_g->add(sceneOn);

  shadowVolumesOn = new Fl_Check_Button(5, 95, 190, 30, "Render Shadow Volumes");
  shadowVolumesOn->callback(renderSV_callback);
  slider_g->add(shadowVolumesOn);

  volumes_CullFront = new Fl_Check_Button(20, 95, 190, 30, "Cull Front");
  volumes_CullFront->callback(volumesCullFront_callback);
  volumes_CullFront->deactivate();

  volumes_CullBack = new Fl_Check_Button(20, 95, 190, 30, "Cull Back");
  volumes_CullBack->callback(volumesCullBack_callback);
  volumes_CullBack->deactivate();

  silhouttesOn = new Fl_Check_Button(5, 95, 190, 30, "Render Silhouettes");
  silhouttesOn->callback(renderSilhouttes_callback);
  slider_g->add(silhouttesOn);


  light_type_rg = new Fl_Pack(5, 5, 190, 25);
  light_type_rg->type(Fl_Pack::HORIZONTAL);
  light_type_rg->box(FL_THIN_UP_FRAME);
  point_l = new Fl_Round_Button(5, 5, 95, 25, "Point");
  point_l->callback(lightType_callback);
  point_l->setonly();
  light_type_rg->add(point_l);
  infinite_l = new Fl_Round_Button(5, 5, 95, 25, "Infinite");
  infinite_l->callback(lightType_callback);
  light_type_rg->add(infinite_l);
  light_type_rg->end();


  block_x = new Fl_Hor_Value_Slider(5, 5, 190, 25, "");
  block_x->bounds(-5000, 5000);
  block_x->value(0);
  block_x->step(15);
  block_x->callback(block_callback);
  slider_g->add(block_x);

  block_y = new Fl_Hor_Value_Slider(5, 5, 190, 25, "");
  block_y->bounds(-5000, 5000);
  block_y->value(400);
  block_y->step(15);
  block_y->callback(block_callback);
  slider_g->add(block_y);


  block_z = new Fl_Hor_Value_Slider(5, 5, 190, 25, "");
  block_z->bounds(-5000, 5000);
  block_z->value(0);
  block_z->step(15);
  block_z->callback(block_callback);
  slider_g->add(block_z);


  spawn_b = new Fl_Button(5, 5, 190, 25, "Spawn Block");
  spawn_b->callback(spawnblock_callback);
  slider_g->add(spawn_b);

  scale = new Fl_Hor_Value_Slider(5, 5, 190, 25, "Scale");
  scale->bounds(1, 50);
  scale->value(1);
  scale->step(.1);
  scale->callback(block_callback);
  slider_g->add(scale);


  fov = new Fl_Hor_Value_Slider(5, 5, 190, 25, "FoV");
  fov->bounds(-180, 360);
  fov->value(90);
  fov->step(1);
  fov->callback(fov_callback);
  slider_g->add(fov);

  znear = new Fl_Hor_Value_Slider(5, 5, 190, 25, "ZNear");
  znear->bounds(0, 1000);
  znear->value(120);
  znear->step(1);
  znear->callback(fov_callback);
  slider_g->add(znear);

  zfar = new Fl_Hor_Value_Slider(5, 5, 190, 25, "ZFar");
  zfar->bounds(0, 10000);
  zfar->value(9000);
  zfar->step(1);
  zfar->callback(fov_callback);
  slider_g->add(zfar);
  

  slider_g->end();
  window2->end();
  window2->show(argc, argv);

  return Fl::run();
}