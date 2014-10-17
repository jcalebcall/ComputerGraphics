#include "wavefrontLoader.h"

WFObject::WFObject()
{

}

WFObject::~WFObject()
{

}

int WFObject::load(char *filename)
{
	fstream objFile;
	objFile.open(filename);

	if(objFile.is_open())
	{
		char line[255];

		// Parse object file line by line
		while(objFile.good())
		{
			objFile.getline(line, 255);
			parseLine(line);
		}

		objFile.close();
	}
	else
	{
		cout << "Could not open WFObject file '" << filename << "'\n";
		return false;
	}

	modelRepresentation();

	return true;
}

void WFObject::parseLine(char *line)
{
	if(!strlen(line))
	{
		return;
	}

	char *lineType;
	lineType = strtok(_strdup(line), " ");

	// Decide what to do
	if(!strcmp(lineType, "v"))		// Vertex
	{
		parseVertex(line);
	}
	else if (!strcmp(lineType, "vt"))
	{
		parseTexture(line);
	}
	else if(!strcmp(lineType, "vn"))	// Normal
	{
		parseNormal(line);
	}
	else if(!strcmp(lineType, "f"))	// Face
	{
		parseFace(line);
	}

	return;
}

// Parse a "v" vertex line of the file into our vertices array
void WFObject::parseVertex(char *line)
{
	vertices.push_back(Vector());		// Add a new element to the vertices array

	sscanf(line, "v %f %f %f", &vertices.back().x, &vertices.back().y, &vertices.back().z);

	return;
}

// Parse a "vn" normal line of the file into the normals array
void WFObject::parseNormal(char *line)
{
	normals.push_back(Vector());

	sscanf(line, "vn %f %f %f", &normals.back().x, &normals.back().y, &normals.back().z);

	return;
}

void WFObject::parseTexture(char *line)
{
	textures.push_back(Vector());

	sscanf(line, "vt %f %f", &textures.back().x, &textures.back().y);

	return;
}

// Parse a "f" face line into the faces array. This gets complex due to there being different line formats.
void WFObject::parseFace(char *line)
{
	faces.push_back(Face());
	if (regex_match (line, regex("f (\\d+/\\d+/\\d+\\s*)*")))
	{
		string word;
		stringstream ss(line);
		while (ss >> word)
		{
			if (word == "f")
				continue;

			int v, t, n;
			v = 0;
			t = 0;
			n = 0;
			sscanf(word.c_str(), "%d/%d/%d", &v, &t, &n);
			faces.back().vIndecies.push_back(v);
			faces.back().tIndecies.push_back(t);
			faces.back().nIndecies.push_back(n);
		}
	}
	else
	{
		string word;
		stringstream ss(line);
		while (ss >> word)
		{
			if (word == "f")
				continue;

			int v, n;
			v = 0;
			n = 0;
			sscanf(word.c_str(), "%d//%d", &v, &n);
			faces.back().vIndecies.push_back(v);
			faces.back().tIndecies.push_back(-1);
			faces.back().nIndecies.push_back(n);
		}
	}

	return;
}

void WFObject::modelRepresentation()
{
	// Iterate over each face in the model
	for(int i = 0; i < faces.size(); i++)
	{              
		// Create a model face for model representation
		ModelFace mf = ModelFace();

		Face f = faces[i];

		// Iterate over the v/t/n tuples that make up the face 
		for(int j = 0; j < f.vIndecies.size(); j++)
		{
			int vIndex = f.vIndecies[j];
			int tIndex = f.tIndecies[j];

			// Add vertex vector to model face
			Vector vVector = vertices[vIndex - 1];
			mf.points.push_back(vVector);

			// Add texture coordinate to model face
			if (tIndex == -1) // a tIndex with -1 means there is no texture coordiant for that vertex
			{
				Vector tVector = Vector();
				tVector.x = -1;
				tVector.y = -1;
				mf.textureCoords.push_back(tVector);
			}
			else 
			{
				Vector tVector = textures[tIndex - 1]; // Remember .obj files start counting at 1
				mf.textureCoords.push_back(tVector);
			}
		}
		ModelFaces.push_back(mf);
	}
}
