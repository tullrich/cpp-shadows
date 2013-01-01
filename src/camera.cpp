#include "shadows-common.h" /* pch */

#include "camera.h"


Camera::Camera() {
		modelView = Matrix();

		znear = 0.1;
        zfar  = 3000;

		fieldofviewY = 90;

        u.x = 1; u.y = 0; u.z = 0;
        v.x = 0; v.y = 1; v.z = 0;
        n.x = 0; n.y = 0; n.z = 1;

		eye.x = eye.y = eye.z = 0;
		center.x = center.y = center.z = 0;
		distance = 0.0;

		up.x = up.z = 0;
		up.y = 1;
}

Camera::~Camera() {}

void Camera::place()
{
	distance = 5000;

	eye.x = 0;
	eye.y = 0;
	eye.z = -5000;


	//modelView.setComponents(u, v, n);
    modelView.vals[12] = -eye.x;
    modelView.vals[13] = -eye.y;
    modelView.vals[14] = eye.z;
}

void Camera::place(Vector max, Vector min, int w, int h) {
	printf("placing max.x %f  max.y %f   max.z %f   min.x %f  min.y %f   min.z %f  w:%i h:%i\n", max.x, max.y, max.z, min.x, min.y, min.z, w, h);
	float hy=0, hx=0, ph=0;

	center.x = (max.x + min.x)/2; 
    center.y = (max.y + min.y)/2;

    hy = ((max.y - min.y)/2) / tan((fieldofviewY/2.0) * RADIAN_PER_DEGREE);
    ph = (h/2) / tan((fieldofviewY/2) * RADIAN_PER_DEGREE);
        
    fieldofviewX = atan((w/2)/ph) * 2.0;
	hx = ((max.x - min.x)/2) / tan(fieldofviewX/2.0);

	float zstart;
	if (abs(hx)>abs(hy)) {
		zstart = hx;
	} else {
		zstart = hy;
	}

    center.z = (zstart + fabs(max.z));
     
	//printf("gluePersepective(fovy=%3f, aspect=%3f, znear=%3f, zfar=%3f", fovy, (float)w/(float)h, znear, zfar)

	eye.x = center.x;
	eye.y = center.y;
	eye.z = -center.z;

	distance = fabs(eye.z);

	n = center - eye;
	//printf("n(%.0f, %.0f, %.0f)\n", n.x, n.y, n.z);
	n.normalize();
	//printf("normal n(%.0f, %.0f, %.0f)\n", n.x, n.y, n.z);

	u = up.cross(n);
	v = n.cross(u);

	this->w = w;
	this->h = h;

	modelView.setComponents(u, v, n);
    modelView.vals[12] = -eye.x;
    modelView.vals[13] = -eye.y;
    modelView.vals[14] = eye.z;
}

void Camera::translate(Vector translation) {
	//eye.x += translation.x * u.x + translation.y * v.x + translation.z * n.x;
	//eye.y += translation.x * u.y + translation.y * v.y + translation.z * n.y;
	//eye.z += translation.x * u.z + translation.y * v.z + translation.z * n.z;
	//printf("u(%.0f, %.0f, %.0f) v(%.0f, %.0f, %.0f)\n", u.x, u.y, u.z, v.x, v.y, v.z);
	//printf("eye(%.0f, %.0f, %.0f) center(%.0f, %.0f, %.0f)\n", eye.x, eye.y, eye.z, center.x, center.y, center.z);
	//drawCameraSightLine();

	modelView.translate(translation);
}

void Camera::rotate(Vector rotation) {
	/*Matrix rotationX;
    rotationX.rotate(rotation.x, u);
    rotationX.apply(&n);
    rotationX.apply(&v);
                
    Matrix rotationY;
    rotationY.rotate(rotation.y, v);
    rotationY.apply(&n);
    rotationY.apply(&u);

    Matrix rotationZ;
    rotationZ.rotate(rotation.z, n);
    rotationZ.apply(&v);
    rotationZ.apply(&u);*/
	
	Matrix rotationMatrix = Matrix();
	rotationMatrix.loadRotation(rotation);
	//printf("rotationMatrix: \n%s", rotationMatrix.toString());

	modelView = rotationMatrix * modelView;
}

void Camera::rotateAboutCenter(int horizontalDeg, int verticalDeg) {
	Matrix translationIn;
	translationIn.loadTranslation(Vector(0.0, 0.0, distance));
	//printf("translationIn: \n%s", translationIn.toString());


	Matrix rotation;
	rotation.loadRotation(Vector(verticalDeg, horizontalDeg, 0.0));
	//printf("rotation: \n%s", rotation.toString());

	Matrix translationOut;
	translationOut.loadTranslation(Vector(0.0, 0.0, -distance));
	//printf("translationOut: \n%s", translationOut.toString());

	modelView = translationOut * rotation * translationIn * modelView;
	//printf("total: \n%s", modelView.toString());
}

void Camera::setupModelView() {
	//gluLookAt (eye.x, eye.y, eye.z, eye.x + n.x, eye.y + n.y, eye.z + n.z, v.x, v.y, v.z); 

	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

	//printf("modelView: \n%s", modelView.toString());

	glLoadMatrixf(modelView.vals);

}

void Camera::perspectiveProjection() {
	/* clear the matrix */ 
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

	projection.loadFrustum(w/2, h/2, znear, zfar);
	
	glLoadMatrixf(projection.vals);
	/*
	glFrustum(-((GLdouble)w)/2, ((GLdouble)w)/2, -((GLdouble)h)/2, ((GLdouble)h)/2, znear, zfar);
	*/ 
	//printf("FRUSTUM: \n%s", frustum.toString());
}

void Camera::lookAt() {
	/* clear the matrix */ 

	//gluPerspective(fieldofviewY, (float)w/(float)h, znear, zfar);
	perspectiveProjection();
	setupModelView();
	

	//drawViewCenter();

}

static void drawCube(int sideLength) {
	float r = sideLength/2;

	glColor3ub(0, 61, 245);

	glBegin(GL_LINE_LOOP);
	glVertex3f(r, -r, r); 
	glVertex3f(r, -r, -r); 
	glVertex3f(-r, -r, -r); 
	glVertex3f(-r, -r, r);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex3f(r, r, r); 
	glVertex3f(r, r, -r); 
	glVertex3f(-r, r, -r); 
	glVertex3f(-r, r, r);
	glEnd();

}

static void drawCube2(Vector loc, int sideLength) {
	float r = sideLength/2;

	glColor3ub(245, 61, 0);

	glBegin(GL_LINE_LOOP);
	glVertex3f(loc.x + r, loc.y - r, loc.z + r); 
	glVertex3f(loc.x + r, loc.y - r, loc.z - r); 
	glVertex3f(loc.x - r, loc.y - r, loc.z - r); 
	glVertex3f(loc.x - r, loc.y - r, loc.z + r);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex3f(loc.x + r, loc.y + r, loc.z + r); 
	glVertex3f(loc.x + r, loc.y + r, loc.z - r); 
	glVertex3f(loc.x - r, loc.y + r, loc.z - r); 
	glVertex3f(loc.x - r, loc.y + r, loc.z + r);
	glEnd();
}

void Camera::drawViewCenter() {
	Matrix centerz;
	centerz.loadTranslation(Vector(0.0, 0.0, distance));

	Matrix test = centerz * modelView;
	//printf("modelView: \n%s", modelView.toString());
	//printf("Test before: \n%s", test.toString());
	test.invert2();
	//printf("Test: \n%s", test.toString());

	/*Vector worldCenter = test * Vector(1.0, 1.0, 1.0);
	worldCenter.x = -worldCenter.x;
	worldCenter.y = -worldCenter.y;
	worldCenter.z = -worldCenter.z;
	printf("worldCenter: (%f, %f, %f)\n", worldCenter.x, worldCenter.y, worldCenter.z);

	drawCube2(worldCenter, 5);
	drawCube2(worldCenter, 100);
	drawCube2(worldCenter, 500);
	*/
	glPushMatrix();
	glLoadMatrixf(test.vals);

	drawCube(5);
	drawCube(100);
	drawCube(500);

	glPopMatrix();
	
}

void Camera::drawCameraSightLine() {
	glColor3ub(0, 61, 245);   

	glBegin(GL_LINES);
	glVertex3f(center.x, center.y, center.z); 
	glVertex3f(eye.x-1, eye.y, eye.z); 
	glEnd();
}

static void drawArrow(int sideLength) {
	float r = sideLength/2;

	glColor3ub(245, 61, 0);

	glBegin(GL_LINE_LOOP);
	glVertex3f(r, -r, 2 * r); 
	glVertex3f(r, -r, 2 * -r); 
	glVertex3f(-r, -r, 2 * -r); 
	glVertex3f(-r, -r, 2 * r);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex3f(r, r, 2 * r); 
	glVertex3f(r, r, 2 * -r); 
	glVertex3f(-r, r, 2 * -r); 
	glVertex3f(-r, r, 2 * r);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(r, -r, 2 * r); 
	glVertex3f(r, r, 2 * r); 

	glVertex3f(r, -r, 2 * -r); 
	glVertex3f(r, r, 2 * -r);

	glVertex3f(-r, -r, 2 * -r); 
	glVertex3f(-r, r, 2 * -r); 

	glVertex3f(-r, -r, 2 * r);
	glVertex3f(-r, r, 2 * r);
	glEnd();

	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0, 0, 4 * -r); 
	glVertex3f(r, -r, 2 * -r); 
	glVertex3f(r, +r, 2 * -r); 
	glVertex3f(-r, +r, 2 * -r); 
	glVertex3f(-r, -r, 2 * -r); 
	glEnd();
}

static void drawFrustum() {
	/* draw the viewing frustum */
	glColor3ub(61, 245, 0);
    glBegin(GL_QUADS);
    glVertex3i(1, 1, 1);
    glVertex3i(-1, 1, 1);
    glVertex3i(-1, -1, 1);
    glVertex3i(1, -1, 1);
    glEnd();
    
	glColor3ub(61, 245, 0);
    glBegin(GL_LINES);
    glVertex3i(1, 1, -1);
    glVertex3i(1, 1, 1);
    glVertex3i(-1, 1, -1);
    glVertex3i(-1, 1, 1);
    glVertex3i(-1, -1, -1);
    glVertex3i(-1, -1, 1);
    glVertex3i(1, -1, -1);
    glVertex3i(1, -1, 1);
    glEnd();
    
    //glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor3ub(61, 245, 245);
    glBegin(GL_QUADS);
    glVertex3i(1, 1, -1);
    glVertex3i(-1, 1, -1);
    glVertex3i(-1, -1, -1);
    glVertex3i(1, -1, -1);
    glEnd();
    //glDisable(GL_BLEND);
}

void Camera::drawSelf(Matrix topMatrix, GLuint matrixVar) {
	Matrix centerz;
	centerz.loadTranslation(Vector(0.0, 0.0, distance));
	Matrix test = topMatrix * centerz * modelView;
	test.invert2();


	//glPushMatrix();
	//glMultMatrixf(test.vals);

	glUniformMatrix4fv(matrixVar, 1, GL_FALSE, test.vals);

	drawCube(50);

	Matrix inv = modelView;
	inv.invert2();
	inv = topMatrix * inv;
	//glPushMatrix();
	//glMultMatrixf(inv.vals);
	glUniformMatrix4fv(matrixVar, 1, GL_FALSE, inv.vals);
	drawArrow(50);

	Matrix inv2 = projection;
	inv2.invert2();
	//glMultMatrixf(inv.vals);
	glUniformMatrix4fv(matrixVar, 1, GL_FALSE, (inv * inv2).vals);
	drawFrustum();
	//glPopMatrix();
	
	//glPopMatrix();
}

Matrix Camera::getModelViewProjectionMatrix() {
	return projection * modelView;
}