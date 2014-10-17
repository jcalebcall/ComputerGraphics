#ifndef _WAVEFRONT_LOADER_H_
#define _WAVEFRONT_LOADER_H_

#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <regex>
#include <sstream>

// Serialization
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/vector.hpp>

using namespace std;

class Vector
{
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & x;
		ar & y;
		ar & z;
	}

public:
	float x;
	float y;
	float z;
};

class Face
{
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & vIndecies;
		ar & tIndecies;
		ar & nIndecies;
	}

public:
	vector<int> vIndecies;
	vector<int> tIndecies;
	vector<int> nIndecies;
};

class ModelFace
{
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & points;
		ar & textureCoords;
	}

public:
	vector<Vector> points;
	vector<Vector> textureCoords;
};

class WFObject
{
private:
	friend class boost::serialization::access;
	// When the class Archive corresponds to an output archive, the
	// & operator is defined similar to <<.  Likewise, when the class Archive
	// is a type of input archive the & operator is defined similar to >>.
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & vertices;
		ar & normals;
		ar & textures;
		ar & faces;
		ar & ModelFaces;
	}

	void parseLine(char *line);
	void parseVertex(char *line);
	void parseNormal(char *line);
	void parseTexture(char *line);
	void parseFace(char *line);
	void modelRepresentation();

public:
	WFObject();
	~WFObject();

	int load(char *filename);

	// Dynamic variables to keep our object data in
	vector<Vector> vertices;
	vector<Vector> normals;
	vector<Vector> textures;
	vector<Face> faces;

	// Model representation of data
	vector<ModelFace> ModelFaces;
};

#endif
