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

  for (i=0;i<numTriangles;i++) {
    triangles[i].getPoints(idx);
    for (j=0;j<3;j++) {
      pts[j] = points[idx[j]];
    }


    foo = pts[1].coords - pts[0].coords;
    bar = pts[2].coords - pts[0].coords;
    normal = foo.cross(bar);
    normal.Normalize();

    glBegin(GL_TRIANGLES);

    glColor4fv(pts[0].color);
    glNormal3f(normal.x, normal.y, normal.z);
    glVertex3f(pts[0].coords.x , pts[0].coords.y , pts[0].coords.z);

    glColor4fv(pts[1].color);
    glNormal3f(normal.x, normal.y, normal.z);
    glVertex3f(pts[1].coords.x , pts[1].coords.y , pts[1].coords.z);

    glColor4fv(pts[2].color);
    glNormal3f(normal.x, normal.y, normal.z);
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


// Destructor
TObject::~TObject() {

}
