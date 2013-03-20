#include "shadows-common.h" /* pch */

#include "REngine.h"
#include "GL/glew.h"
#include "GL/glu.h"
#include "md3_mesh.h"
#include "PlaneMesh.h"


int SHADER_POS = -1;
int SHADER_COLOR = -1;
int SHADER_NORMAL = -1;
int SHADER_TC = -1;
int SHADER_TANGENT = -1;

int SHADER_AMB = -1;
int SHADER_DIFF = -1;
int SHADER_SPEC = -1;
int SHADER_SHINE = -1;

REngine::REngine(int X, int Y, int W, int H, const char *L) : GlCanvas(X, Y, W, H, L)
{
  lastx = 0;
  lasty = 0;
  m_RenderScene = true;
  m_RenderShadowVolumes = false;
  m_RenderSilhouttes = false;
  m_VolumesCullFront = false;
  m_VolumesCullBack = false;
  m_fov = 90;
  m_znear = 120;
  m_zfar = 9000;
}

REngine::~REngine()
{

}

void REngine::addRenderEntity(Renderable& r)
{
  entities.insert(&r);
}

void REngine::drawPass()
{
  renderProgram.activate();

  glm::vec4 light1_pos = light1->getPosition();
  renderProgram.setUniform4f("light_pos", 
    light1_pos.x, light1_pos.y, light1_pos.z, light1_pos.w);

  glm::vec3 view_pos = c.getWorldPosition();
  cout << "view_pos: " << view_pos.x << ", " << view_pos.y << ", " << view_pos.z << endl;


  for (auto i = entities.begin(); i != entities.end(); i++)
  {
    (*i)->onDraw(view_pos, light1_pos, renderProgram);
  }
}

void REngine::shadowPass()
{
  extrusionProgram.activate();

  glm::vec4 light1_pos = light1->getPosition();

  for (auto i = entities.begin(); i != entities.end(); i++)
  {
    (*i)->onDrawShadowVolumes(light1_pos, extrusionProgram);
  }
}

void REngine::silhouttePass()
{
  renderProgram.activate();

  glm::vec4 light1_pos = light1->getPosition();

  for (auto i = entities.begin(); i != entities.end(); i++)
  {
    (*i)->onDrawSilhoutte(light1_pos);
  }
}

void REngine::depthPass()
{
  /*disable writes to depth/color buffer*/
  glEnable(GL_STENCIL_TEST);
  glStencilFunc(GL_ALWAYS, 0, ~0);
  glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
  glDepthMask(GL_FALSE);
  glEnable(GL_DEPTH_CLAMP);

  /* increment back faces */
  glCullFace(GL_FRONT);
  glStencilOp(GL_KEEP, GL_INCR, GL_KEEP);
  light1->deactivate();
  shadowPass();

  /* decrement front faces */
  glCullFace(GL_BACK);
  glStencilOp(GL_KEEP, GL_DECR, GL_KEEP);
  shadowPass();

  /* turn everything back on */
  glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
  glCullFace(GL_BACK);
  glDepthFunc(GL_EQUAL);
  glStencilFunc(GL_EQUAL, 0, ~0);
  glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
  glEnable(GL_BLEND);
  glBlendFunc(GL_ONE, GL_ONE);
  glDisable(GL_DEPTH_CLAMP);

  light1->activate();
  drawPass();
  light1->deactivate();

  /* cleanup */
  glDepthMask(GL_TRUE);
  glDepthFunc(GL_LESS);
  glDisable(GL_BLEND);
  glStencilFunc(GL_ALWAYS, 0, ~0);
}

void REngine::onDraw()
{
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  //c.lookAt();
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity()
;  gluPerspective(m_fov, w()/h(), m_znear, m_zfar);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  c.setupModelView();

  light1->activate();
  //GLfloat global_ambient[] = { 0 , 0, 0 , 1.0};
  //glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);

  if(m_RenderScene)
  {
    drawPass();
  }

  if (m_RenderSilhouttes)
  {
    glDisable(GL_CULL_FACE);
    silhouttePass();
    glEnable(GL_CULL_FACE);
  }

  /* render the shadow volumes to the color buffer */
  if (m_RenderShadowVolumes)
  {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_BLEND);
    glBlendFunc(GL_CONSTANT_ALPHA, GL_ONE_MINUS_CONSTANT_ALPHA);
    glBlendColor(0.0, 0.0, 0.0, .3);


    glEnable(GL_CULL_FACE);
    if(m_VolumesCullBack && m_VolumesCullFront)
    {
      glCullFace(GL_FRONT_AND_BACK);
    } else if (m_VolumesCullBack)
    {
      glCullFace(GL_BACK);
    } else if (m_VolumesCullFront)
    {
      glCullFace(GL_FRONT);
    } else 
    {
      glDisable(GL_CULL_FACE);
    }

    shadowPass();

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);    
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  }


  //depthPass();

  /* catchall error check */
  checkError();
}

void REngine::checkError()
{
  int err = glGetError();
  if (err != 0)
  {
    std::cout << "ERR" << err << std::endl;
  }
}


void REngine::addMd3FromFile(const char* filepath)
{
  MD3Reader reader;
  reader.open(filepath);
  MD3Mesh *mesh = new MD3Mesh();
  reader.readMesh(mesh);
  reader.close();

  MeshRenderable *renderEntity = new MeshRenderable(mesh);
  renderEntity->castsShadows = true;
  addRenderEntity(*renderEntity);
}


void REngine::init()
{
  /* general */
	glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
  //glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
  glDepthFunc(GL_LESS);
  glCullFace(GL_BACK);

  /* lighting */
  glEnable(GL_LIGHTING);
  glShadeModel(GL_FLAT);
  GLfloat global_ambient[] = { .1 , .1, .1 , 1.0};
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);

  glClearColor(0.75, 0.75, 0.75, 1.0); 
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  renderProgram.start();
  glBindAttribLocation(renderProgram.program, 7, "amb_in");
  glBindAttribLocation(renderProgram.program, 9, "normalzz");
  glBindAttribLocation(renderProgram.program, 14, "diff_in");
  glBindAttribLocation(renderProgram.program, 15, "spec_in");
  glBindAttribLocation(renderProgram.program, 13, "shine_in");
  renderProgram.attachFileAs(GL_VERTEX_SHADER, string("vertexshader.txt"));
  renderProgram.attachFileAs(GL_FRAGMENT_SHADER, string("fragmentshader.txt"));
  renderProgram.link();

  extrusionProgram.start();
  extrusionProgram.attachFileAs(GL_VERTEX_SHADER, string("extrusion_vertexshader.txt"));
  extrusionProgram.attachFileAs(GL_FRAGMENT_SHADER, string("fragmentshader.txt"));
  extrusionProgram.link();

  SHADER_POS = renderProgram.getAttributeIndex("positionzz");
  SHADER_COLOR =  renderProgram.getAttributeIndex("colorzz");
  SHADER_NORMAL =  renderProgram.getAttributeIndex("normalzz");
  SHADER_TC = renderProgram.getAttributeIndex("texcoords");
  SHADER_TANGENT = renderProgram.getAttributeIndex("tangent");
  SHADER_AMB =  renderProgram.getAttributeIndex("amb_in");
  SHADER_DIFF =  renderProgram.getAttributeIndex("diff_in");
  SHADER_SPEC =  renderProgram.getAttributeIndex("spec_in");
  SHADER_SHINE =  renderProgram.getAttributeIndex("shine_in");

  cout << "SHADER_TANGENT" << SHADER_TANGENT << endl;

  renderProgram.setUniform1i("diff_texture", 0);
  renderProgram.setUniform1i("normal_texture", 1);
  renderProgram.setUniform1i("spec_texture", 2);


  cube = new CubeMesh();
  //block1 = new MeshRenderable(cube,
  //  glm::vec3(0.0, 400, 0.0), 1.0);
  //block1->castsShadows = true;
  //addRenderEntity(*block1);


  MeshRenderable *floor = new MeshRenderable(new PlaneMesh(), 
    glm::vec3(0.0, 0.0, 0.0));
  addRenderEntity(*floor);


  block1 = new MeshRenderable(
    new TexturedMesh(
      "uvmaps/temple-diffuse.png", 
      "uvmaps/temple-normal.png",
      "uvmaps/temple-specular.png"));
  addRenderEntity(*block1);

  light1 = new Light(1000, 1000, 400, true);
  addRenderEntity(*light1);

  c.place();
}

void REngine::mouseDown(int x, int y)
{
  lastx = x;
  lasty = y;
}

void REngine::mouseDrag(int x, int y)
{
  int motionx = lastx - x;
  int motiony = lasty - y;

  lastx = x;
  lasty = y;

  c.rotateAboutCenter(-motionx, -motiony);

  redraw();
}

void REngine::setLightPosition(int x, int y, int z)
{
  light1->setPosition(glm::vec4(x, y, z, light1->getPosition().w));
  redraw();
}

void REngine::setRenderScene(bool doRender)
{
  m_RenderScene = doRender;
  redraw();
}


void REngine::setRenderShadowVolumes(bool doRender)
{
  m_RenderShadowVolumes = doRender;
  redraw();
}

void REngine::setRenderSilhouttes(bool doRender)
{
  m_RenderSilhouttes = doRender;
  redraw();
}


void REngine::setLightType(bool isPoint)
{
  light1->setType(isPoint);
  redraw();
}

void REngine::setBlockPosition(int x, int y, int z, float scale)
{
  block1->setPosition(glm::vec3(x, y, z), scale);
  redraw();
}


void REngine::spawnBlockAt(int x, int y, int z, float scale)
{

  MeshRenderable *cubeEntity = new MeshRenderable(cube, 
    glm::vec3(x, y, z), scale);
  cubeEntity->castsShadows = true;
  addRenderEntity(*cubeEntity);
  redraw();
}

void REngine::setVolumesCullFront(bool doCull)
{
  m_VolumesCullFront = doCull;
  redraw();
}

void REngine::setVolumesCullBack(bool doCull)
{
  m_VolumesCullBack = doCull;
  redraw();
}

void REngine::setFoV(double fov, double znear, double zfar)
{
  m_fov = fov;
  m_znear = znear;
  m_zfar = zfar;
  redraw();
}