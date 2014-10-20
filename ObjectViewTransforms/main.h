#define _USE_MATH_DEFINES
#include <windows.h>  // for MS Windows
#include <iostream>
#include <string>
#include <vector>
#include <math.h>
#include <GL/glut.h>  // GLUT, include glu.h and gl.h
#include <IL/il.h>
#include <sys/stat.h>
#include "2DVector.h"

// Serialization
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/vector.hpp>

#include "wavefrontLoader.h"
#include "gamepad.h"

using namespace std;