#include <GL/glut.h>
#include <algorithm>

#include "types.hh"
#include "object.hh"
#include "point.hh"
#include "triangle.hh"
#include "collision.hh"

// Default constructor, how many points and how many triangles
TObject::TObject(int numPts, int numTris) {

  // Allocate mem for points array
  maxPoints = numPts;
  numPoints = 0;
  points = new Point[maxPoints];

  // Add mem for triangles array
  maxTriangles = numTris;
  numTriangles = 0;
  triangles = new Triangle[maxTriangles];

  // Add mem for normals
  normals = new TVector[maxTriangles];
  ptsnormals = new TVector[maxPoints];

  // Add an empty list to each points neighbour list
  ptsNbour.resize(maxPoints);

  bboxused = false;
}

// Retrieve the velocity vector
/*
TVector TObject::getVelocity() {
  return vel;
}
*/
// Set the velocity
void TObject::setVelocity(TVector v) {
  vel[0] = v.x;
  vel[1] = v.y;
  vel[2] = v.z;
}

// Add to the velocity
void TObject::addVelocity(TVector v) {
  vel[0] += v.x;
  vel[1] += v.y;
  vel[2] += v.z;
}

// Add the velocity to each point
void TObject::move(float s) {
  int i;

  for (i = 0 ; i < numPoints ; i++) {
    points[i].coords.x += s * vel[0];
    points[i].coords.y += s * vel[1];
    points[i].coords.z += s * vel[2];
  }
  for (i = 0 ; i < 2 ; i++) {
    bbox[0][i] += s * vel[0];
    bbox[1][i] += s * vel[1];
    bbox[2][i] += s * vel[2];
  }
}

// Add a new point to the point list
void TObject::addPoint(float p0, float p1, float p2) {
  Point pt;
  
  if (numPoints<maxPoints) {
    points[numPoints] = Point(p0,p1,p2);
    pt = points[numPoints];
    if (bboxused) {
      // Check if current point is "lower" than previous min
      bbox[0][0] = (pt.coords.x < bbox[0][0])?pt.coords.x:bbox[0][0];
      bbox[1][0] = (pt.coords.y < bbox[1][0])?pt.coords.y:bbox[1][0];
      bbox[2][0] = (pt.coords.z < bbox[2][0])?pt.coords.z:bbox[2][0];
      
      // Check if current point is "higher" than previous min
      bbox[0][1] = (pt.coords.x > bbox[0][1])?pt.coords.x:bbox[0][1];
      bbox[1][1] = (pt.coords.y > bbox[1][1])?pt.coords.y:bbox[1][1];
      bbox[2][1] = (pt.coords.z > bbox[2][1])?pt.coords.z:bbox[2][1];
    } else {
      bbox[0][0] = bbox[0][1] = pt.coords.x;
      bbox[1][0] = bbox[1][1] = pt.coords.y;
      bbox[2][0] = bbox[2][1] = pt.coords.z;
      bboxused = true;
    }

    numPoints++;
  }
  else {
    cerr << "One too many points added, numPoints = "
	 << numPoints
	 << "\n";
  }
}

// Add a new point
void TObject::addPoint(float pts[]) {
  addPoint(pts[0],pts[1],pts[2]);

}

// Add a new point
void TObject::addPoint(float p0, float p1, float p2, float *col) {
  Point pt;
  
  if (numPoints<maxPoints) {
    points[numPoints] = Point(p0,p1,p2,col);
    pt = points[numPoints];
    if (bboxused) {
      // Check if current point is "lower" than previous min
      bbox[0][0] = (pt.coords.x < bbox[0][0])?pt.coords.x:bbox[0][0];
      bbox[1][0] = (pt.coords.y < bbox[1][0])?pt.coords.y:bbox[1][0];
      bbox[2][0] = (pt.coords.z < bbox[2][0])?pt.coords.z:bbox[2][0];
      
      // Check if current point is "higher" than previous min
      bbox[0][1] = (pt.coords.x > bbox[0][1])?pt.coords.x:bbox[0][1];
      bbox[1][1] = (pt.coords.y > bbox[1][1])?pt.coords.y:bbox[1][1];
      bbox[2][1] = (pt.coords.z > bbox[2][1])?pt.coords.z:bbox[2][1];
    } else {
      bbox[0][0] = bbox[0][1] = pt.coords.x;
      bbox[1][0] = bbox[1][1] = pt.coords.y;
      bbox[2][0] = bbox[2][1] = pt.coords.z;
      bboxused = true;
    }

    numPoints++;
  }
  else {
    cerr << "One too many points added, numPoints = "
	 << numPoints
	 << "\n";
  }
}

// Add a new point
void TObject::addPoint(float pts[], float col[]) {
  addPoint(pts[0],pts[1],pts[2],col);
}

// Add a triangle spanned by q0, q1, q2 (indices from points[])
void TObject::addTriangle(int q0, int q1, int q2) {

  if (numTriangles<maxTriangles) {
    triangles[numTriangles] = Triangle(q0,q1,q2);
    ptsNbour[q0].push_front(&triangles[numTriangles]);
    ptsNbour[q1].push_front(&triangles[numTriangles]);
    ptsNbour[q2].push_front(&triangles[numTriangles]);
    numTriangles++;
  }
  else {
    cerr << "One too many triangle added, numTriangles = "
	 << numTriangles 
	 << "\n";
  }
}


// Draw the TObject using OpenGL functions
void TObject::draw() {
  int i, j, idx[3];
  Point pts[3];
  TVector normal,foo,bar;
  GLfloat shininess[] = { 100.0 };

  glMaterialfv(GL_FRONT, GL_SPECULAR, points[0].color);
  glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, points[0].color);

  for (i=0;i<numTriangles;i++) {
    triangles[i].getPoints(idx);
    for (j=0;j<3;j++) {
      pts[j] = points[idx[j]];
    }

    /*
      foo = pts[1].coords - pts[0].coords;
      bar = pts[2].coords - pts[0].coords;
      normal = foo.cross(bar);
      normal.Normalize();
    */

    glBegin(GL_TRIANGLES);

    glColor4fv(pts[0].color);
    glNormal3f(normals[i].x, normals[i].y, normals[i].z);
    glVertex3f(pts[0].coords.x , pts[0].coords.y , pts[0].coords.z);

    glColor4fv(pts[1].color);
    glNormal3f(normals[i].x, normals[i].y, normals[i].z);
    glVertex3f(pts[1].coords.x , pts[1].coords.y , pts[1].coords.z);

    glColor4fv(pts[2].color);
    glNormal3f(normals[i].x, normals[i].y, normals[i].z);
    glVertex3f(pts[2].coords.x , pts[2].coords.y , pts[2].coords.z);

    glEnd();

  }
}

void TObject::print() {
  int i, j, idx[3];
  Point pts[3];

  for (i=0;i<numTriangles;i++) {
    triangles[i].getPoints(idx);
    for (j=0;j<3;j++) {
      pts[j] = points[idx[j]];
    }
    
    cout << "Drawing triangle:\n"
	 << "  ( " << pts[0].coords.x << " , " << pts[0].coords.y << " , " << pts[0].coords.z << " )\n"
	 << "  ( " << pts[1].coords.x << " , " << pts[1].coords.y << " , " << pts[1].coords.z << " )\n"
	 << "  ( " << pts[2].coords.x << " , " << pts[2].coords.y << " , " << pts[2].coords.z << " )\n"
	 << "\n";

  }

}

// Draw the TObject's bounding box
void TObject::drawBbox() {

  if (collision)
    glColor4f(1.0, 0.0, 0.0, 1.0);
  else 
    glColor4f(0.0, 1.0, 0.0, 1.0);

  // Line strip covering back and left sides of bbox
  glBegin(GL_LINE_STRIP);
  glVertex3f(bbox[0][0], bbox[1][0], bbox[2][0]);
  glVertex3f(bbox[0][1], bbox[1][0], bbox[2][0]);
  glVertex3f(bbox[0][1], bbox[1][1], bbox[2][0]);
  glVertex3f(bbox[0][0], bbox[1][1], bbox[2][0]);
  glVertex3f(bbox[0][0], bbox[1][0], bbox[2][0]);
  glVertex3f(bbox[0][0], bbox[1][0], bbox[2][1]);
  glVertex3f(bbox[0][0], bbox[1][1], bbox[2][1]);
  glVertex3f(bbox[0][0], bbox[1][1], bbox[2][0]);
  glEnd();

  // Line strip covering front and right sides of bbox
  glBegin(GL_LINE_STRIP);
  glVertex3f(bbox[0][1], bbox[1][0], bbox[2][1]);
  glVertex3f(bbox[0][1], bbox[1][0], bbox[2][0]);
  glVertex3f(bbox[0][1], bbox[1][1], bbox[2][0]);
  glVertex3f(bbox[0][1], bbox[1][1], bbox[2][1]);
  glVertex3f(bbox[0][1], bbox[1][0], bbox[2][1]);
  glVertex3f(bbox[0][0], bbox[1][0], bbox[2][1]);
  glVertex3f(bbox[0][0], bbox[1][1], bbox[2][1]);
  glVertex3f(bbox[0][1], bbox[1][1], bbox[2][1]);
  glEnd();

}

void TObject::printBbox() {
  cout << "Drawing bounding box:\n"
       << "  ( " << bbox[0][0]
       << " , " << bbox[1][0]
       << " , " << bbox[2][0]
       << " ) - ( " << bbox[0][1]
       << " , " << bbox[1][1]
       << " , " << bbox[2][1]
       << " )\n";

}


// Check points for the lower and upper x, y and z bounds and
// define the bounding box
void TObject::calcBbox() {

}


void TObject::setColor(float *col) {
  int i;


  for (i = 0; i < numPoints; i++) {
    points[i].setColor(col);
  }

}

bool TObject::CollisionDetect(TVector * old_location, TVector * new_location) {

  int i;
  vector<float> times;
  float t;
  TVector P0, P1, P2;
  Triangle T;

  for ( i = 0 ; i < numTriangles ; i++) {

    T = triangles[i];

    /* Create vector from triangle coordinates */
    P0 = TVector(points[T.p0].coords.x,points[T.p0].coords.y,points[T.p0].coords.z);
    P1 = TVector(points[T.p1].coords.x,points[T.p1].coords.y,points[T.p1].coords.z);
    P2 = TVector(points[T.p2].coords.x,points[T.p2].coords.y,points[T.p2].coords.z);
    
    /* Store collision-time in a vector<int> */
    t = checkPointPoly(old_location, new_location, &P0, &P1, &P2);
    times.push_back(t);
    
  }

  /* Sort the found collision times
   * Mental note: checkPointPoly returns 2.0 if no
   * collision is found!
   */
  sort(times.begin(),times.end());

  if ( (times.size() != 0) && (times[0] <= 1.0) ) {
    /* The point of collision is: old + (new - old) * time */
    (*new_location) = (*old_location) + 
      ((*new_location) - (*old_location)) * t;
    return true;

  } else {
    /* Well, duuuuh... */
    return false;
  }
}


void TObject::calcNormals() {
  //  TVector Normals[numTriangles];
  TVector P0, P1, P2, u, v;
  TrianglePtrList::iterator ite, end;

  int i,idx[3];
assert(numTriangles<=maxTriangles);
  for (i = 0; i < numTriangles; i++) {
    triangles[i].getPoints(idx);
    P0 = TVector(points[idx[0]].coords.x,points[idx[0]].coords.y,points[idx[0]].coords.z);
    P1 = TVector(points[idx[1]].coords.x,points[idx[1]].coords.y,points[idx[1]].coords.z);
    P2 = TVector(points[idx[2]].coords.x,points[idx[2]].coords.y,points[idx[2]].coords.z);
    u = P1 - P0;
    v = P2 - P0;
    normals[i] = u.cross(v);
    normals[i].Normalize();
  }
  /*
    for (i = 0; i < numPoints; i++) {
    ite = ptsNbour[i].begin();
    end = ptsNbour[i].end();
    u = TVector(0.0, 0.0, 0.0);
    while (ite != end) {
      u = u + normals[(*ite)];
      ite++;
    }
    u.Normalize();
    ptsnormals[i] = u;
  }
  */
}


/* Transformation functions */

// Rotate theta degrees around Q-P
void TObject::rotate(TVector P, TVector Q, float theta) {
  int i,j;
  float rotation[16],newtransform[16];
  TVector v;
  float xx,xy,xz,yy,yz,zz;
  float cost, sint;

  // We translate back to "origo" to rotate around the
  // objects center/origo
  translate(TVector(-P.x, -P.y, -P.z));

  // Find the axis we're rotating around
  v = Q - P;
  v.Normalize();

  // Elements in v*v'
  xx = v.x * v.x;
  xy = v.x * v.y;
  xz = v.x * v.z;
  yy = v.y * v.y;
  yz = v.y * v.z;
  zz = v.z * v.z;

  // cos(theta) and sin(theta)
  cost = cos(M_PI * theta / 180.0);
  if (fabs(cost) < 1e-6) {
    cost = 0.0;
  }
  sint = sin(M_PI * theta / 180.0);
  if (fabs(sint) < 1e-6) {
    sint = 0.0;
  }

  // First column in R
  rotation[ 0] = xx + cost * (1.0 - xx);
  rotation[ 1] = xy + cost * (0.0 - xy) + sint * v.z;
  rotation[ 2] = xz + cost * (0.0 - xz) - sint * v.y;
  rotation[ 3] = 0.0;

  // Second column in R
  rotation[ 4] = xy + cost * (0.0 - xy) - sint * v.z;
  rotation[ 5] = yy + cost * (1.0 - yy);
  rotation[ 6] = yz + cost * (0.0 - yz) + sint * v.x;
  rotation[ 7] = 0.0;

  // Third column in R
  rotation[ 8] = xz + cost * (0.0 - xz) + sint * v.y;
  rotation[ 9] = yz + cost * (0.0 - yz) - sint * v.x;
  rotation[10] = zz + cost * (1.0 - zz);
  rotation[11] = 0.0;

  // Fourth column in R
  rotation[12] = 0.0;
  rotation[13] = 0.0;
  rotation[14] = 0.0;
  rotation[15] = 1.0;

  // Compute new transformation matrix
  matrixmult(transform,rotation,newtransform);

  // Assign new transformation matrix
  for (i = 0; i < 4; i++) {
    for (j = 0; j < 4; j++) {
      transform[i*4 + j] = newtransform[i*4 + j];
    }
  }  

  // Move the object back in it place
  translate(TVector(P.x, P.y, P.z));
}


// Translate with vector T
void TObject::translate(TVector T) {
  int i,j;
  float translate[16],newtransform[16];

  // Translation matrix
  translate[0] = 1.0; translate[4] = 0.0; translate[8]  = 0.0; translate[12] = T.x;
  translate[1] = 0.0; translate[5] = 1.0; translate[9]  = 0.0; translate[13] = T.y;
  translate[2] = 0.0; translate[6] = 0.0; translate[10] = 1.0; translate[14] = T.z;
  translate[3] = 0.0; translate[7] = 0.0; translate[11] = 0.0; translate[15] = 1.0;
  // Calculate new transformation matrix
  matrixmult(transform,translate,newtransform);
  for (i = 0; i < 4; i++) {
    for (j = 0; j < 4; j++) {
      transform[i*4 + j] = newtransform[i*4 + j];
    }
  }  
  // Update bounding box
  /*
  matrixmult(origbbox,transform,newtransform);
  bbox[0][0] = newtransform[0];
  bbox[1][0] = newtransform[1];
  bbox[2][0] = newtransform[2];
  bbox[0][1] = newtransform[4];
  bbox[1][1] = newtransform[5];
  bbox[2][1] = newtransform[6];
  */
}

// Scale around origo, S holds scalefactors for each axis
void TObject::scale(TVector S) {
  int i,j;
  float scale[16],newtransform[16];

  // Translation matrix
  scale[0] = S.x; scale[4] = 0.0; scale[8]  = 0.0; scale[12] = 0.0;
  scale[1] = 0.0; scale[5] = S.y; scale[9]  = 0.0; scale[13] = 0.0;
  scale[2] = 0.0; scale[6] = 0.0; scale[10] = S.z; scale[14] = 0.0;
  scale[3] = 0.0; scale[7] = 0.0; scale[11] = 0.0; scale[15] = 1.0;
  // Calculate new transformation matrix
  matrixmult(scale,transform,newtransform);
  for (i = 0; i < 4; i++) {
    for (j = 0; j < 4; j++) {
      transform[i*4 + j] = newtransform[i*4 + j];
    }
  }  
  // Update bounding box
  /*
  matrixmult(origbbox,transform,newtransform);
  bbox[0][0] = newtransform[0];
  bbox[1][0] = newtransform[1];
  bbox[2][0] = newtransform[2];
  bbox[0][1] = newtransform[4];
  bbox[1][1] = newtransform[5];
  bbox[2][1] = newtransform[6];
  */
}

void TObject::matrixmult(float *A, float *B, float *C) {
  int i,j;

  for (i = 0; i < 4; i++) {
    for (j = 0; j < 4; j++) {
      C[i*4 + j] =
	A[i*4] * B[j]
	+ A[i*4 + 1] * B[j + 4]
	+ A[i*4 + 2] * B[j + 8]
	+ A[i*4 + 3] * B[j + 12];
    }
  }

}

// Destructor
TObject::~TObject() {

}
