#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <algorithm>

#include "map.hh"
#include "display.hh"
#include "render.hh"

#include "textures.hh"

#include "types.hh"
#include "color.hh"

#include "collision.hh"

/* **********************************************************************
   Configuration stuff that is changable from the outside */
float map_max_steepness = 2.0;

/* This is to avoid the map sinking into total void */
#define MIN_Z -25

/* Some defines from Skjalms weird code! :-) */
#define LS_HEIGHT 12.0
#define LS_HALFHEIGHT 3.0
#define LS_SCALE 12.0

/* A type to use with vector manipulation */
typedef vector<TVector> TVectorVector;
typedef  vector<TVectorVector> TVectorMap;

/* **********************************************************************
 * **********************************************************************
 * Class constructors and init code.
 * **********************************************************************
 * *********************************************************************/

/* ************************************************************
   Initialize map. Set size, etc */
TMap::TMap(int w, int l, float nmapsteepness) {
  // cout << "TMap::TMap entering" << endl;
  width         = w;
  length        = l;
  max_steepness = map_max_steepness;
  has_changed   = true;
  data          = (TMappoint *) malloc ((w+1)*(l+1)*sizeof(TMappoint));
  mapsteepness  = nmapsteepness;
  bumpiness     = 1.2;

  /* Check that width and length is equal */
  if (w != l || !w || !l) {
    cerr << "TMap::TMap() illegal width/length specification " << w << ", " << l << endl;
    exit(1);
  }

  /* Fill some points into - call lsGenerate(); */
  //  lsGenerate();


#define RANDOM_MAP
#ifdef RANDOM_MAP
  /* Fill some points into - call lsGenerate(); */
  lsGenerate();

#else
  int x, y;
  for(x = 0; x <= width; x++) {
    for(y = 0; y <= length; y++) {
      PointAt(x, y)->x = x;
      PointAt(x, y)->y = y;
      
      // A function that sets it to part of some "waves". Uses some constants.
      
      /*PointAt(x, y)->z
	= +1 - (3 * (1.5*sin(((double) x)/(float) 64 * 3.5 * M_PI) 
		     - 2* cos(((double) y)/(float) 64 * 4* M_PI)));
      */
      // Simpler function;
      // map[x][y].height = (GLint) 2 * sin((double) x);
      // Everything at sealevel
      // PointAt(x, y)->z = 0;
      
      // Raising 
      PointAt(x, y)->z = (x+y) / 2.0;
      // printf("xyz : %i, %i %f\n", x, y, map->data[x*map->length+y].height);
    }
  }
  // cout << "TMap::TMap map created" << endl;
#endif

  /* Set up all the normal */
  lsNormals(0, width, 0, length);


  /* Set the display list to something we will never use */
  display_list = 0xFFFFFFFF;
}

/* **********************************************************************
   Map destructor */
TMap::~TMap() {
  if (display_list != 0xFFFFFFFF) {
    glDeleteLists(display_list, 1);
  }
  free(data);
}

/* **********************************************************************
 * **********************************************************************
 * Map generating stuff
 * **********************************************************************
 * *********************************************************************/

/* **********************************************************************
   lsRand - sawtooth random function */
GLfloat TMap::lsRand(int step) {
  GLfloat i, factor = mapsteepness * bumpiness * LS_SCALE * (GLfloat) width/32.0 / pow(2,step);
  i = (GLfloat) rand()/(GLfloat)RAND_MAX;
  i += (GLfloat) rand()/(GLfloat)RAND_MAX;
  i -= 1.0;
  return i * factor;
}

/* **********************************************************************
   lsLowerPoint - fracture and bend the line from a to b */
void TMap::lsLowerPoint(TMappoint * dest, TMappoint * a, TMappoint * b, int step) {
  /* Find the centerpoint */
  dest->x = (a->x+b->x) / 2;
  dest->y = (a->y+b->y) / 2;
  if (!PointAt(dest->x, dest->y)->used) {
    PointAt(dest->x, dest->y)->used = true;
    PointAt(dest->x, dest->y)->z 
      = (PointAt(a->x, a->y)->z + PointAt(b->x, b->y)->z)/2.0 + lsRand(step);
  }
}
/* **********************************************************************
   lsSplitTriangle - splits a triangle in four and recurse.
   Also sets normals */
void TMap::lsSplitTriangle(TMappoint * a, TMappoint * b,TMappoint * c, 
			   int step, int max_steps) {
  TMappoint d, e, f;
  if (step < max_steps) {
    /* Split each line in triangle abc and split
       the resulting four triangles */
    step++;
    lsLowerPoint(&d, a, b, step);
    lsLowerPoint(&e, b, c, step);
    lsLowerPoint(&f, a, c, step);

    lsSplitTriangle(a,  &d, &f, step, max_steps);
    lsSplitTriangle(b,  &e, &d, step, max_steps);
    lsSplitTriangle(&e, c,  &f, step, max_steps);
    lsSplitTriangle(&e, &f, &d, step, max_steps);
  } 
}

/* **********************************************************************
   lsGenerate - Starts the landscape generation 
   NOTE: Only handles width == length*/
void TMap::lsGenerate() {
  int i,j;

  /* Check that w is 2^x */
  int tmp = width;
  int max_steps = 0;
  /* While bit 0 != 1 */
  while(0 == (1 & tmp)) {
    tmp = tmp >> 1;
    max_steps++;
  };
  /* At most one bit is allowed */
  if (tmp != 1) {
    cerr << "TMap::lsGenerate() illegal width/length specification " 
	 << width << ", " << length << endl;
    exit(1);
  }
  /* Initialize some data structures */
  for (i = 0; i <= width; i++) {
    for (j = 0; j <= length; j++) {
      // lsPointUsed[i][j] = 0;
      PointAt(i, j)->used = false;
      // lsPoints[i][j] = 0.0;
      PointAt(i, j)->x = i;
      PointAt(i, j)->y = j;
      PointAt(i, j)->z = 30.0;
    }
  }

  /* Set some selected points to random heights 
   +-----+
   |*   *|
   |  *  |
   | *** |
   |  *  |
   |*   *|
   +-----+
  */
  GLfloat factor = (GLfloat) mapsteepness * LS_HEIGHT * (GLfloat) width/32.0;
  GLfloat pushdown1 = mapsteepness * LS_HALFHEIGHT * (GLfloat) width/32.0;
  GLfloat pushdown = mapsteepness * LS_HALFHEIGHT * (GLfloat) width/32.0 + 0.5 * LS_HALFHEIGHT;
  PointAt(0,         0)->z             = rand()/(GLfloat)RAND_MAX * factor - pushdown;
  PointAt(0,         0)->used          = true;
  PointAt(0,         length)->z        = rand()/(GLfloat)RAND_MAX * factor - pushdown;
  PointAt(0,         length)->used     = true;
  PointAt(width,     length)->z        = rand()/(GLfloat)RAND_MAX * factor - pushdown;
  PointAt(width,     length)->used     = true;
  PointAt(width,     0)->z             = rand()/(GLfloat)RAND_MAX * factor - pushdown;
  PointAt(width,     0)->used          = true;
  PointAt(width/2,   length/4)->z      = rand()/(GLfloat)RAND_MAX * factor - pushdown1;
  PointAt(width/2,   length/4)->used   = true;
  PointAt(width/4,   length/2)->z      = rand()/(GLfloat)RAND_MAX * factor - pushdown1;
  PointAt(width/4,   length/2)->used   = true;
  PointAt(width/2,   length/4*3)->z    = rand()/(GLfloat)RAND_MAX * factor - pushdown1;
  PointAt(width/2,   length/4*3)->used = true;
  PointAt(width/4*3, length/2)->z      = rand()/(GLfloat)RAND_MAX * factor - pushdown1;
  PointAt(width/4*3, length/2)->used   = true;
  PointAt(width/2,   length/2)->z      = rand()/(GLfloat)RAND_MAX * factor - pushdown;
  PointAt(width/2,   length/2)->used   = true;

  /* "Start" variables - one in each corner */
  TMappoint ul, ll, lr, ur;
  ul.x = 0;     ul.y = length; 
  ll.x = 0;     ll.y = 0; 
  lr.x = width; lr.y = 0; 
  ur.x = width; ur.y = length;

  /* Start splitting lower left triangle */
  lsSplitTriangle(&ul, &ll, &lr, 0, max_steps);

  /* Split upper right triangle */
  lsSplitTriangle(&ul, &lr, &ur, 0, max_steps);
}


/* **********************************************************************
   lsNormals - set up normals for ths points within the area */
void TMap::lsNormals(int xlow, int xhigh, int ylow, int yhigh) {
  /* TODO: Does not handle border very well - if at all */
  // cout << "lsNormals(" << xlow << ", " << xhigh << ", " 
  // << ylow << ", " << yhigh << ")" << endl;
  
  /* Declares space to hold our normals */
  TVectorMap normals;
  int x, y;
  int xdiff, ydiff;
  /* The normals are 2 to each point, because of to triangles to each square */
  xdiff = (xhigh - xlow);
  ydiff = 2*(yhigh - ylow);

  /* Presize normals */
  vector<TVector> tmp;
  TVector tmp2;
  normals.insert(0, xdiff, tmp);
  for (x = 0; x < xdiff; x++) {
    normals[x].insert(0, ydiff, tmp2);
  }
  /* Make normals for all triangles in area (2 for each square) */
  for(x = xlow; x < xhigh; x++) {
    for(y = ylow; y < yhigh; y++) {
      /* Lower left triangle (dont try this at home, kids) */
      normals[x-xlow][2*(y-ylow)].x = PointAt(x, y)->z - PointAt(x+1, y)->z;
      normals[x-xlow][2*(y-ylow)].y = PointAt(x, y)->z - PointAt(x, y+1)->z;
      normals[x-xlow][2*(y-ylow)].z = 1;
      (normals[x-xlow][2*(y-ylow)]).Normalize();

      /* Upper rigth triangle */
      normals[x-xlow][2*(y-ylow)+1].x = PointAt(x, y+1)->z - PointAt(x+1, y+1)->z;
      normals[x-xlow][2*(y-ylow)+1].y = PointAt(x+1, y)->z - PointAt(x+1, y+1)->z;
      normals[x-xlow][2*(y-ylow)+1].z = 1;
      normals[x-xlow][2*(y-ylow)+1].Normalize();
#define NORMAL_AVG
#ifndef NORMAL_AVG
      PointAt(x, y)->normal = normals[x-xlow][2*(y-ylow)+1];
#endif

    }
  }
#ifdef NORMAL_AVG
  /* Set all point normals to average of surrounding normals */
  TVector * tmp_normal;

  /* Only the inside is calculated and drawn */
  for(x = xlow+1; x < xhigh-1; x++) {
    for(y = ylow+1; y < yhigh-1; y++) {
      tmp_normal = &(PointAt(x, y)->normal);
      /* Set tmp_normal to average of 6 neighbours */
      tmp_normal->x 
	= normals[x-xlow-1][2*(y-ylow)-1].x
	+ normals[x-xlow-1][2*(y-ylow)].x
	+ normals[x-xlow-1][2*(y-ylow)+1].x
	+ normals[x-xlow][2*(y-ylow)-2].x
	+ normals[x-xlow][2*(y-ylow)-1].x
	+ normals[x-xlow][2*(y-ylow)].x;
      tmp_normal->y
	= normals[x-xlow-1][2*(y-ylow)-1].y
	+ normals[x-xlow-1][2*(y-ylow)].y
	+ normals[x-xlow-1][2*(y-ylow)+1].y
	+ normals[x-xlow][2*(y-ylow)-2].y
	+ normals[x-xlow][2*(y-ylow)-1].y
	+ normals[x-xlow][2*(y-ylow)].y;
      tmp_normal->z
	= normals[x-xlow-1][2*(y-ylow)-1].z
	+ normals[x-xlow-1][2*(y-ylow)].z
	+ normals[x-xlow-1][2*(y-ylow)+1].z
	+ normals[x-xlow][2*(y-ylow)-2].z
	+ normals[x-xlow][2*(y-ylow)-1].z
	+ normals[x-xlow][2*(y-ylow)].z;
      tmp_normal->Normalize();
    }
  }
#endif
}

/* **********************************************************************
 * **********************************************************************
 * Get random points from the map
 * **********************************************************************
 * *********************************************************************/

/* ************************************************************
   Return a random spot within the map, at least border from 
   the edge  */
TVector TMap::RandomSpot(int border) {
  TVector tmp;
  tmp.x      = rint(border + ((double) width-(2*border))*rand()/(RAND_MAX+1.0));
  tmp.y      = rint(border + ((double) length-(2*border))*rand()/(RAND_MAX+1.0));
  tmp.z      = PointAt(tmp.x, tmp.y)->z;
  return tmp;
}

/* **********************************************************************
   Return a list of random points that are no closer then border 
   (gives up after 10 tries), and accept close points */
void TMap::DoRandomSpots(TVectors * spots, int remaining, int border) {
  if (0 >= remaining) {
    return;
  }
  // cout << "TMap::DoRandomSpots() " << endl;
  /* Get a candidate, check it against the others */
  TVector candidate;
  int count = 50;
  while(count > 0) {
    /* Get candidate */
    candidate = RandomSpot(5);
    /* Check candidate against the others */
    bool OK = true;
    TVectorsIterator end = spots->end();
    for (TVectorsIterator i = spots->begin(); i != end; i++) {
      if ((*i).Distance(candidate) == 0) {
	/* Do not count this, zero is unacceptable */
	// cout << "TMap::DoRandomSpots: zero distance" << endl;
	OK = false;
	count++;
	break;
      }
      if ((*i).Distance(candidate) < border) {
	// cout << "TMap::DoRandomSpots: distance less then" << border << endl;
	OK = false;
	break;
      }
    }
    if (OK) {
      break;
    }
    count--;
  }
  if (0 == count) {
    // cout << "Failed to find suitable" << endl;
  }
  /* If we have not found one, we use the last anyway, and recurse */
  spots->push_back(candidate);
  DoRandomSpots(spots, remaining-1, border);
}

/* **********************************************************************
   Start DoRandomSpots */
TVectors * TMap::RandomSpots(int n, int border) {
  TVectors * res = new TVectors;
  DoRandomSpots(res, n, border);
  return res;
}

/* **********************************************************************
 * **********************************************************************
 * Impact checking and taking stuff.
 * **********************************************************************
 * *********************************************************************/

/* ************************************************************ 
   This function lowers x,y 1.
   It then checks if any of the neighbours are too high, and if 
   they are, call recursivly */
void TMap::Slide(int x, int y) {
  int i, j;
  //  printf("map_slide called, adjusting %i, %i from %f\n",
  // x, y, PointAt(x, y).height);
  PointAt(x, y)->z = PointAt(x, y)->z - 1;
  //  printf("map_slide called, adjusting %i, %i to %f\n",
  //	 x, y, PointAt(x, y).z);
  // printf("map_slide Looping i over %i, %i\n",
  //  	 max(0,x-1), min(map->width-1, x+1));
  for(i = max(0,x-1); i <= min(width-1, x+1); i++) {
    // printf("map_slide Looping j over %i, %i\n",
    // max(0,y-1), min(map->length-1, y+1));
    for(j = max(0,y-1); j <= min(length-1, y+1); j++) {
      // printf("map_slide checking %i, %i\n", i, j);
      while ( ((PointAt(i, j)->z - PointAt(x, y)->z) 
	       > max_steepness)
	      && (PointAt(i, j)->z> MIN_Z)) {
	// printf("map_slide Adjusting %i, %i\n", i, j);
	/* Too steep, adjust it */
	Slide(i, j);
      }
    }
  }
}

/* **********************************************************************
   Lowers z at this point, so that it has the radius distance to "loc" */
void TMap::LowerPoint(int x, int y, TVector * location, float radius) {
  /* First, figure out if x,y is to far away from the location to be 
     touched */
  float dx2 = (x - location->x) * (x - location->x); 
  float dy2 = (y - location->y) * (y - location->y); 
  /* Use <= since that will give 2 z values always */
  if (sqrt(dx2 + dy2) >= radius) { 
    // cout << "ignoring " << x << ", " << y << endl;
    return; 
  };
  // cout << "lowering " << x << ", " << y << endl;
  /* We now must find 2 z values where the radius matches */
  float zm = sqrt(radius*radius - dx2 - dy2);
  // cout << "sqrt(" << radius*radius - dx2 - dy2 << ")" << endl;
  float z1 = location->z + zm;
  float z2 = location->z - zm;
  // cout << "Found " << zm << ", " << z1 << ", " << z2 << endl;
  float zo = PointAt(x, y)->z;
  // cout << "Lowering first max " << mymax(0, zo-z2) << " second "
  // << mymax(0, zo-z1) << endl;
  // cout << "Lowering " << zo << " to " << zo -  mymax(0, zo-z2) + mymax(0, zo-z1) << endl;
  PointAt(x, y)->z = zo -  mymax(0, zo-z2) + mymax(0, zo-z1);
}

/* **********************************************************************
   Calls lower for each point with in the radius.
   Actually it is called in a square, but it just wont matter. */
  /* Lowers a lot of map points */
void TMap::LowerAll(TVector * location, float radius) {
  // cout << "Lowering all" << endl;
  int x_min = (int) floor(mymax(0, location->x-radius));
  /* Higher goes a little longer */
  int x_max = (int) floor(mymin(width-1, location->x+radius+1));
  int y_min  = (int) floor(mymax(0, location->y-radius));
  /* Higher goes a little longer */
  int y_max = (int) floor(mymin(width-1, location->y+radius+1));
  for(int i = x_min; i <= x_max; i++) {
    for(int j = y_min; j <= y_max; j++) {
      // cout << "Lowering " << i << ", " << j << endl;
      LowerPoint(i, j, location, radius);
    }
  }
  /* TODO: Now slide the same points. I know this is not really effecient, but
     it will probably change when the map format change anyway */

  /* Recalc point/lighting normals */
  lsNormals(x_min, x_max, y_min, y_max);
}


/* ********************************************************************************
   Take a hit from an explosion 
   TODO: Different projectile types should have different impact functions? */
/* Take a hit a x, y */
void TMap::Explosion(TExplosion * Explosion) {
  /* Check that the steepness is sane! */
  max_steepness = mymax(max_steepness, 0.5);
  /* Actually take damage */
  LowerAll(&(Explosion->location), Explosion->maxradius);
  /* Map was changed. Displays etc can use this. */
  has_changed = true;
}

/* **********************************************************************
 * **********************************************************************
 * Collision and within check code
 * **********************************************************************
 * *********************************************************************/

/* **********************************************************************
   Check for a collision with the map. 
   TODO: Implement proper collision detecting. How the hell do I do that?
   We probably need to look at all the triangles we pass between old and 
   new, and check if we went through any of them.
   For now, we simply check if we are below the nearest corner, and in that
   case, return that corner as the point of collision */
bool TMap::CollisionDetect(TVector * old_location, TVector * new_location) {

  int xmin, xmax, ymin, ymax;
  int i,j;
  vector<float> times;
  float t;
  TVector P0, P1, P2;

  /* Find outher bounds of old and new locations */
  if ( old_location->x < new_location->x ) {
    xmin = (int) floor(old_location->x);
    xmax = (int) floor(new_location->x) + 1;
  } else {
    xmin = (int) floor(new_location->x);
    xmax = (int) floor(old_location->x) + 1;
  }

  if ( old_location->y < new_location->y ) {
    ymin = (int) floor(old_location->y);
    ymax = (int) floor(new_location->y) + 1;
  } else {
    ymin = (int) floor(new_location->y);
    ymax = (int) floor(old_location->y) + 1;
  }

  /* Check all triangles between (xmin,ymin) and (xmax,ymax) */
  for ( i = xmin ; i < xmax ; i++ ) {
    for ( j = ymin ; j < ymax ; j++) {

      /* Lower-left triangle */
      P0 = TVector(i     , j    , PointAt(i     , j    )->z);
      P1 = TVector(i + 1 , j    , PointAt(i + 1 , j    )->z);
      P2 = TVector(i     , j + 1, PointAt(i     , j + 1)->z);

      /* Store collision-time in a vector<int> */
      t = checkPointPoly(old_location, new_location, &P0, &P1, &P2);
      times.push_back(t);

      /* Upper-right triangle */
      P0 = TVector(i + 1 , j + 1, PointAt(i + 1 , j + 1)->z);
      P1 = TVector(i     , j + 1, PointAt(i     , j + 1)->z);
      P2 = TVector(i + 1 , j    , PointAt(i + 1 , j    )->z);

      /* Store collision-time in a vector<int> */
      t = checkPointPoly(old_location, new_location, &P0, &P1, &P2);
      times.push_back(t);
    }
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

  /* Old stuff... really crappy... doesn't work as it should...
     but errmmm... it might work better than what I'm gonna do next... ;-)
     / Skjalm, 2000-07-15
  if (new_location->z <= PointAt(rint(new_location->x), 
				 rint(new_location->y))->z) {
    new_location->x = rint(new_location->x);
    new_location->y = rint(new_location->y);
    new_location->z = PointAt(rint(new_location->x), 
			      rint(new_location->y))->z;
    return true;
  } else {
    return false;
  }
  */
}

/* **********************************************************************
   Check if an location is within the map */
bool TMap::Within(TVector * location) {
  /* Offsets are due to the "sloppy" way we render the map */
  if (location->x >= 1 && location->x < width-3 
      && location->y >= 1 && location->y < length-3) {
    return true;
  } else {
    return false;
  }
}


/* **********************************************************************
 * **********************************************************************
 * Stuff related to rendering
 * **********************************************************************
 * *********************************************************************/

/* **********************************************************************
   InitRender - set up some variables, load some texture */

static bool textures_loaded = false;

void TMap::InitRender() {
  /* Initialize the displaylist */
  display_list = glGenLists(1);


  if (textures_loaded) return;
  /* Set up the texture management */
  /* Initialize texture system 
     TODO: Texture management of maps should be better */
  // cout << "TMap::TMap displaylist set" << endl;
  texture_init(3);
  if (GL_NO_ERROR != glGetError()) {
    cerr << "TMap::InitRender : GL was in error condition before texture_load" << endl;
  } 
  texture_load("data/graphics/textures/snow.rgb", 64, 64);
  texture_load("data/graphics/textures/ice.rgb", 64, 64);
  // texture_load("data/maps/textures/rock_real.rgb", 64, 64);
  texture_load("data/graphics/textures/snow_cliff5_128.rgb", 128, 128);
  textures_loaded = true;
  if (GL_NO_ERROR != glGetError()) {
    cerr << "TMap::InitRender : GL was in error condition after texture_load" << endl;
  } 

  // cout << "TMap::TMap textures loaded" << endl;
}

/* **********************************************************************
   Set the texture */
void TMap::SetTexture(float h) {
  if (h < -4.0) { 
    texture_set(2);
  } else {
    if (h > 3) {
      texture_set(0);
    } else {
      texture_set(1);
    }
  }
}

/* ************************************************************
   This function renders the map. I am not sure this is the
   way to do it, but... */
void TMap::Render(TViewpoint * viewpoint) {
  int x, y;
  GLfloat mat_specular[] = { 0.0, 0.0, 0.0, 1.0 };
  GLfloat mat_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
  GLfloat mat_shininess[] = { 10.0 };

  if (GL_NO_ERROR != glGetError()) {
    cerr << "TMap::Render: GL was in error condition" << endl;
  } 
  
  /* Draw landscape - a simple height map */
  glColor3f(1.0,1.0,1.0);
  /* Check if dirty. If dirty, redo list otherwise display old */
  if (!has_changed) {
    // printf("render_map : Reusing display list\n");
    glCallList(display_list);
  } else {
    /* Generate display list */
    //    printf("render_map: Generating new display list\n");
    if (GL_NO_ERROR != glGetError()) {
      cerr << "GL was in error condition before glNewList()" << endl;
    } 
    glNewList(display_list, GL_COMPILE_AND_EXECUTE);

    /* Shade: flat or smooth */
    if (render_shade_smooth) {
      glShadeModel(GL_SMOOTH);
    } else {
      glShadeModel(GL_FLAT);
    }
    
    /* Light or not */
    if (render_light) {
      glEnable(GL_LIGHTING);
  /***********************************************
   ** WARNING
   ** This kind of changing the material properties
   ** are BAD - and not only for the I
   ***********************************************/
      glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
      glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
      glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    } else {
      glDisable(GL_LIGHTING);
    }

    switch (render_type) {
    case render_type_lines: {
      glDisable(GL_TEXTURE_2D);
      for(y = 1; y < length-3; y++) {
	for(x = 1; x < width-3; x++) {
	  glColor3f(1.0,0.0,0.0);
	  glBegin(GL_LINES);
	  //	  cout << "1: x, y, z " << x << "," << y << ", " << PointAt(x, y)->z << endl;
	  glVertex3f(x, y, PointAt(x, y)->z);
	  //	  cout << "1: x+1, y, z " << x+1 << "," << y << ", " << PointAt(x+1, y)->z << endl;
	  glVertex3f(x+1, y, PointAt(x+1, y)->z);
	  //	  cout << "1: x, y+1, z " << x << "," << y+1 << ", " << PointAt(x, y+1)->z << endl;
	  glVertex3f(x, y+1, PointAt(x, y+1)->z);
	  glEnd();
	  
	  glColor3f(0.0,0.0,1.0);
	  glBegin(GL_LINES);
	  //	  cout << "2: a" << endl;
	  //	  cout << "2: x, y+1, z " << x << "," << y+1 << ", " << PointAt(x, y+1)->z << endl;
	  glVertex3f(x, y+1, PointAt(x, y+1)->z);
	  //	  cout << "2: b" << endl;
	  //	  cout << "2: x+1, y, z " << x+1 << "," << y << ", " << PointAt(x+1, y)->z << endl;
	  glVertex3f(x+1, y, PointAt(x+1, y)->z);
	  //	  cout << "2: c" << endl;
	  //	  cout << "2: x+1, y+1, z " << x+1 << "," << y+1 << ", " << PointAt(x+1, y+1)->z << endl;
	  glVertex3f(x+1, y+1, PointAt(x+1, y+1)->z);
	  glEnd();
	}
      }
      break;
    } /* render_type_lines */
    case render_type_triangles: {
      glDisable(GL_TEXTURE_2D);
      for(y = 1; y < length-3; y++) {
	for(x = 1; x < width-3; x++) {
	  glBegin(GL_TRIANGLES);

	  glColor3f(1.0,0.0,0.0);
	  glNormal3fv((float *) (&PointAt(x, y)->normal));
	  glVertex3f(x, y, PointAt(x, y)->z);
	  glNormal3fv((float *) (&PointAt(x+1, y)->normal));
	  glVertex3f(x+1, y, PointAt(x+1, y)->z);
	  glNormal3fv((float *) (&PointAt(x, y+1)->normal));
	  glVertex3f(x, y+1, PointAt(x, y+1)->z);
	  

	  glColor3f(0.0,0.0,1.0);
	  glNormal3fv((float *) (&PointAt(x, y+1)->normal));
	  glVertex3f(x, y+1, PointAt(x, y+1)->z);
	  glNormal3fv((float *) (&PointAt(x+1, y)->normal));
	  glVertex3f(x+1, y, PointAt(x+1, y)->z);
	  glNormal3fv((float *) (&PointAt(x+1, y+1)->normal));
	  glVertex3f(x+1, y+1, PointAt(x+1, y+1)->z);

	  glEnd();
	}
      }
      break;
    } /* render_type_triangles */
    case render_type_textures: {
      glColor4f(1.0, 1.0, 1.0, 1.0);
      glEnable(GL_TEXTURE_2D);
      /* Make sure that we do not assume anything about the texture in use */
      texture_reset();
      for(y = 1; y < length-3; y++) {
	for(x = 1; x < width-3; x++) {
	  SetTexture(mymax3(PointAt(x, y)->z, 
			  PointAt(x+1, y)->z, 
			  PointAt(x, y+1)->z));

	  // glColor3f(1.0,0.0,0.0);
	  glBegin(GL_TRIANGLES);
	  glTexCoord2f(TEXCOORD(x), TEXCOORD(y));
	  /*      printf("tex : [%3i, %3i] = [%3f, %3f]\n",
		  x+1, y+1, TEXCOORD(x+1), TEXCOORD(y+1)); */
	  glNormal3fv((float *) (&PointAt(x, y)->normal));
	  glVertex3f(x, y, PointAt(x, y)->z);
	  //	printf("render_map %i, %i, %f\n", x, y, PointAt(x, y)->z);
	  glTexCoord2f(TEXCOORD(x+1), TEXCOORD(y));
	  /*      printf("tex : [%3i, %3i] = [%3f, %3f]\n",
		  x+1, y, TEXCOORD(x+1), TEXCOORD(y)); */
	  glNormal3fv((float *) (&PointAt(x+1, y)->normal));
	  glVertex3f(x+1, y, PointAt(x+1, y)->z);
	  //	printf("render_map %i, %i, %f\n", x+1, y, PointAt(x+1, y)->z);
	  glTexCoord2f(TEXCOORD(x), TEXCOORD(y+1));
	  /*      printf("tex : [%3i, %3i] = [%3f, %3f]\n",
		  x, y+1, TEXCOORD(x), TEXCOORD(y+1)); */
	  glNormal3fv((float *) (&PointAt(x, y+1)->normal));
	  glVertex3f(x, y+1, PointAt(x, y+1)->z);
	  //	printf("render_map %i, %i, %f\n", x, y+1, PointAt(x, y+1)->z);
	  glEnd();

	  SetTexture(mymax3(PointAt(x+1, y+1)->z, 
			    PointAt(x, y+1)->z, 
			    PointAt(x+1, y)->z));
	  // glColor3f(0.0,0.0,1.0);
	  glBegin(GL_TRIANGLES); 
	  glTexCoord2f(TEXCOORD(x), TEXCOORD(y+1));
	  /*      printf("tex : [%3i, %3i] = [%3f, %3f]\n",
		  x, y+1, TEXCOORD(x), TEXCOORD(y+1)); */
	  glNormal3fv((float *) (&PointAt(x, y+1)->normal));
	  glVertex3f(x, y+1, PointAt(x, y+1)->z);
	  //	printf("render_map %i, %i, %f\n", x, y+1, PointAt(x, y+1)->z);
	  glTexCoord2f(TEXCOORD(x+1), TEXCOORD(y));
	  /*      printf("tex : [%3i, %3i] = [%3f, %3f]\n",
		  x+1, y, TEXCOORD(x+1), TEXCOORD(y)); */
	  glNormal3fv((float *) (&PointAt(x+1, y)->normal));
	  glVertex3f(x+1, y, PointAt(x+1, y)->z);
	  //	printf("render_map %i, %i, %f\n", x+1, y, PointAt(x+1, y)->z);
	  glTexCoord2f(TEXCOORD(x+1), TEXCOORD(y+1));
	  /*      printf("tex : [%3i, %3i] = [%3f, %3f]\n",
		  x+1, y+1, TEXCOORD(x+1), TEXCOORD(y+1)); */
	  glNormal3fv((float *) (&PointAt(x+1, y+1)->normal));
	  glVertex3f(x+1, y+1, PointAt(x+1, y+1)->z);
	  //	printf("render_map %i, %i, %f\n", x+1, y+1, PointAt(x+1, y+1)->z);
	  glEnd();
	}
      }
      
      break;
    } /* render_type_textures */
    } /* switch render_type */

    /* render normals or not*/
    if (render_normals) {
      glDisable(GL_LIGHTING);
      glDisable(GL_TEXTURE_2D);
      glColor4fv(ColorDefinitions.Colors["yellow"].data);
      glBegin(GL_LINES);
      for(y = 1; y < length-2; y++) {
	for(x = 1; x < width-2; x++) {
	  /* From the point to the end of the normal */
	  glVertex3f(x, y, PointAt(x, y)->z);
	  glVertex3f(x + PointAt(x, y)->normal.x, 
		     y + PointAt(x, y)->normal.y, 
		     PointAt(x, y)->z + PointAt(x, y)->normal.z); 
	}
      }
      glEnd();
      /* Light or not (Could be changed by normal rendering*/
      if (render_light) {
	glEnable(GL_LIGHTING);
      } else {
	glDisable(GL_LIGHTING);
      }    
    }
    
    /* End the list! */
    glEndList();
    if (GL_NO_ERROR != glGetError()) {
      cerr << "GL was in error condition after glEndList()" << endl;
    } 
    /* set map to not changed */
    has_changed = false;
  }
}

