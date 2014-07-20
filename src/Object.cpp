/**
   Object - a storage class for a .obj file
*/

#include "Object.h"

using namespace std;

int Object::init(string filename, Shader* s) {
  if (s == NULL) {
    cerr << "Shader was null in Object::init(). Object not created.\n";
    return 1;
  }

  this->parseObjectFile(filename);
  glm::mat4 id (1.0f); // identity
  this->transformation = id;
  this->shader = s;
  this->vao = 0;

  this->elms = 16; // 3 pos, 3 norm, 3*3 color, 1 shine
  
  // default to white/greyish
  glm::vec3 white (1.0f,1.0f,1.0f);
  this->ambient = white * 0.1f;
  this->diffuse = white * 0.4f;
  this->specular = white * 0.8f;
  this->shininess = 16.0f;

  openglInitialization();
  return 0;
}

Object::Object() {
  init(NULL, NULL);
}

Object::Object(string filename, Shader* s) {
  init(filename, s);
}


/**
   Loads a .obj file and parses it.

   Note: I did not implement the entire .obj specification.
   I only implemented vertices, vertex normals, and faces under the
   assumption that each face is a triangle. 
*/
void Object::parseObjectFile(string filename) {
  if (this->vertices.size() > 0) {
    cerr << "Object has already been loaded from file.\n"
         << "Loading again is undefined and not allowed.\n";
    return;
  }
  
  ifstream file (filename.c_str());
  if (!file) {
    cerr << "Failed to open object file: " << filename << endl;
    return;
  }

  string line;
  while (getline(file,line)) {
    stringstream ss (line);
    
    if (line.size() == 0) {
      continue;
    }

    string op;
    ss >> op;

    // vertices
    if (op == "v") {
      float x,y,z;
      ss >> x >> y >> z;
      glm::vec3 v (x,y,z);
      this->vertices.push_back(v);
    }

    // vertex normals
    if (op == "vn") {
      float x,y,z;
      ss >> x >> y >> z;
      glm::vec3 v (x,y,z);
      this->vertexNormals.push_back(v);
    }

    // parse a triangle face
    if (op == "f") {
      string s1,s2,s3;
      
      // strings should be like
      // "f 6//1 7//1 4//1"
      // or "f 2 3 4"
      ss >> s1 >> s2 >> s3;
      
      int a,b,c;
      int an,bn,cn;
      a = parseOBJVertex(s1);
      b = parseOBJVertex(s2);
      c = parseOBJVertex(s3);
      an = parseOBJVertexNormal(s1);
      bn = parseOBJVertexNormal(s2);
      cn = parseOBJVertexNormal(s3);

      Triangle t;
      if (an != -1 && bn != -1 && cn != -1) {
        Triangle tr (a,b,c,an,bn,cn);
        t = tr;
      } else {
        Triangle tr (a,b,c);
        t = tr;
      }
      this->triangles.push_back(t);
    }

    // I didn't implement the entire .obj specification. The rest is
    // garbled gooky shit. GARBAGE I TELL YOU! GARBAGE!
  }
}

void Object::glDraw() {
  this->applyModelMatrix();

  glBindVertexArray(this->vao);
  glDrawElements(GL_TRIANGLES, this->triangles.size()*3, 
                 GL_UNSIGNED_INT, 0);
}

void Object::setShader(Shader* shader) {
  this->shader = shader;
}

void Object::openglInitialization() {

  // initialize the vao, vbo, and ebo
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  this->vao = vao;

  GLuint vbo;
  glGenBuffers(1, &vbo);
  
  GLuint ebo;
  glGenBuffers(1, &ebo);

  
  // create the vertex array
  GLfloat vertexArray[this->vertices.size()*this->elms];

  /*
    The following for-loop got messy but was necessary because OpenGL
    expects all vertex information (including the vertex normal) to be
    in the vertex array (simply offset by however much we tell
    it). This means we needed to map the vertex to its vertex normal
    using the triangle faces (as they are the only things that have
    that information). Because we were grabbing the vertices out of
    the triangles, we needed new element indices as well, so that's
    where q comes in.
   */
  Triangle* t;
  vector<uint> v, vn;
  map<uint,uint> vertexMap; // mapping of vertex index to element index
  uint q = 0; // element counter for current vertex
  for (uint i = 0; i < this->triangles.size(); i++) {
    t = &this->triangles[i];
    v = t->getVertices();
    vn = t->getVertexNormals();

    for (uint k = 0; k < 3; k++) {
      uint a = q*this->elms;
      uint vi = v[k]-1;
      uint vni = vn[k]-1;
      if (vertexMap.find(vi) == vertexMap.end()) {
        // add the vertex to the set and increment the counter
        // the counter (q) is the element index
        q++;
        vertexMap.insert(pair<uint,uint>(vi,q));

        // vertex position
        glm::vec3 vert = this->vertices[vi];
        for (uint j = 0; j < 3; j++) {
          vertexArray[a+j] = vert[j];
        }
        a += 3;

        // vertex normal
        glm::vec3 vertn = this->vertexNormals[vni];
        for (uint j = 0; j < 3; j++) {
          vertexArray[a+j] = vertn[j];
        }
        a += 3;

        // ambient color
        for (uint j = 0; j < 3; j++) {
          vertexArray[a+j] = this->ambient[j];
        }
        a += 3;

        // diffuse color
        for (uint j = 0; j < 3; j++) {
          vertexArray[a+j] = this->diffuse[j];
        }
        a += 3;

        // specular color
        for (uint j = 0; j < 3; j++) {
          vertexArray[a+j] = this->specular[j];
        }
        a += 3;

        // specular exponent (shininess)
        vertexArray[a] = this->shininess;
        a += 1;
      }
    }
  }

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertexArray), 
               vertexArray, GL_STATIC_DRAW);
  
  /*
    Here we finally create the element array that maps element indices
    to vertices in the vertex array.
   */
  uint elementArray[this->triangles.size()*3];
  for (uint i = 0; i < this->triangles.size(); i++) {
    uint a = i*3;
    vector<uint> v = this->triangles[i].getVertices();
    for (uint j = 0; j < 3; j++) {
      elementArray[a+j] = vertexMap.find(v[j]-1)->second - 1;
    }
  }
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elementArray), 
               elementArray, GL_STATIC_DRAW);


  // setup the shaders
  GLuint shaderProgram = this->shader->getProgram();
  glUseProgram(shaderProgram);

  // shader attributes
  uint offset = 0;
  GLint posAttrib = glGetAttribLocation(shaderProgram, "vPosition");
  glEnableVertexAttribArray(posAttrib);
  // (input, # of values, type, normalize, stride, offset)
  glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE,
                        this->elms*sizeof(float), 
                        (void*)(offset*sizeof(float)));
  offset += 3;

  GLint normalAttrib = glGetAttribLocation(shaderProgram, "vNormal");
  glEnableVertexAttribArray(normalAttrib);
  glVertexAttribPointer(normalAttrib, 3, GL_FLOAT, GL_TRUE,
                        this->elms*sizeof(float), 
                        (void*)(offset*sizeof(float)));
  offset += 3;

  GLint ambientAttrib = glGetAttribLocation(shaderProgram, 
                                            "ambientColor");
  glEnableVertexAttribArray(ambientAttrib);
  glVertexAttribPointer(ambientAttrib, 3, GL_FLOAT, GL_FALSE, 
                        this->elms*sizeof(float), 
                        (void*)(offset*sizeof(float)));
  offset += 3;

  GLint diffuseAttrib = glGetAttribLocation(shaderProgram, 
                                            "diffuseColor");
  glEnableVertexAttribArray(diffuseAttrib);
  glVertexAttribPointer(diffuseAttrib, 3, GL_FLOAT, GL_FALSE, 
                        this->elms*sizeof(float), 
                        (void*)(offset*sizeof(float)));
  offset += 3;

  GLint specularAttrib = glGetAttribLocation(shaderProgram, 
                                             "specularColor");
  glEnableVertexAttribArray(specularAttrib);
  glVertexAttribPointer(specularAttrib, 3, GL_FLOAT, GL_FALSE, 
                        this->elms*sizeof(float), 
                        (void*)(offset*sizeof(float)));
  offset += 3;

  GLint shininessAttrib = glGetAttribLocation(shaderProgram, 
                                              "shininess");
  glEnableVertexAttribArray(shininessAttrib);
  glVertexAttribPointer(shininessAttrib, 1, GL_FLOAT, GL_FALSE, 
                        this->elms*sizeof(float), 
                        (void*)(offset*sizeof(float)));
  offset += 1;
}

GLuint Object::getVao() const {
  return this->vao;
}

void Object::applyModelMatrix() const {
  GLint uniModel = glGetUniformLocation(this->shader->getProgram(),
                                        "model");
  glUniformMatrix4fv(uniModel, 1, GL_FALSE,
                     glm::value_ptr(this->transformation));
}

void Object::setTransform(glm::mat4 t) {
  this->transformation = t;
}

glm::mat4 Object::getTransform() const {
  return this->transformation;
}

void Object::setAmbientColor(glm::vec3 c) {
  this->ambient = c;
}

void Object::setDiffuseColor(glm::vec3 c) {
  this->diffuse = c;
}

void Object::setSpecularColor(glm::vec3 c) {
  this->specular = c;
}

void Object::setShininess(float s) {
  this->shininess = s;
}


/**
   This outputToObj function was part a school project on Bezier
   Surfaces, so I kept it in here just because. Kinda useless for this
   program though...
 */
void Object::outputToObj(string filename) const {
  ofstream file;
  file.open(filename.c_str());
  file << "# " << filename 
       << "\n\n";

  file << "# vertices: should have " << this->vertices.size() << "\n";
  // vertices
  for (uint i = 0; i < this->vertices.size(); i++) {
    glm::vec3 vert = this->vertices[i];
    file << "v " 
         << vert[0] << " " << vert[1] << " " << vert[2] << " " << "\n";
  }
  
  file << "\n\n\n";
  
  file << "# vertex normals: " << this->vertexNormals.size() << "\n";
  // vertex normals
  for (uint i = 0; i < this->vertexNormals.size(); i++) {
    glm::vec3 vert = this->vertexNormals[i];
    file << "vn " 
         << vert[0] << " " << vert[1] << " " << vert[2] << " " << "\n";
  }

  file << "\n\n\n";
  
  file << "# triangle faces: should have " << this->triangles.size() 
       << "\n";
  // faces
  for (uint i = 0; i < this->triangles.size(); i++) {
    vector<uint> v = this->triangles[i].getVertices();
    vector<uint> vn = this->triangles[i].getVertexNormals();
    file << "f " 
         << v[0] << "//" << vn[0] << " "
         << v[1] << "//" << vn[1] << " " 
         << v[2] << "//" << vn[2] << "\n";
  }
  
  file << "\n\n# May the Llamas spit upon ye.";  
}


/**
   Private
*/

int Object::parseOBJVertex(string s) const {
  string split = "/";
  int slash = s.find(split);
  if (!slash) {
    return atoi(s.c_str());
  }
  string v = s.substr(0,slash);
  return atoi(v.c_str());
}

int Object::parseOBJVertexNormal(string s) const {
  string split = "//";
  int slashes = s.find(split);
  if (!slashes) {
    return -1;
  }
  string n = s.substr(slashes + split.length());
  return atoi(n.c_str());
}

glm::vec3 Object::surfaceNormal(Triangle t) const {
  vector<uint> v = t.getVertices();
  glm::vec3 a = this->vertices[v[0]];
  glm::vec3 b = this->vertices[v[1]];
  glm::vec3 c = this->vertices[v[2]];

  glm::vec3 p = a - b;
  glm::vec3 q = a - c;

  return glm::cross(p,q);
}
