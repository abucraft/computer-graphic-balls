#include "3DObject.h"

void getPointIdx(std::string &buffer, int&v, int&uv, int &nm);

std::string fillPath(const char* parentName, const char* filename) {
	std::string fullpath(parentName);
	size_t pathpt = fullpath.rfind('/');
	std::string fpath = fullpath.substr(0, pathpt + 1);
	fpath += filename;
	return fpath;
}

void SubMesh::draw()
{
}

SubMesh::SubMesh() {

	ptBuffer = 0;
	vNum = 0;
	uvBuffer = 0;
	nmBuffer = 0;
	ptIndices = 0;
}

SubMesh::~SubMesh()
{
	if (ptBuffer != 0) {
		free(ptBuffer);
	}
	if (uvBuffer != 0) {
		free(uvBuffer);
	}
	if (nmBuffer != 0) {
		free(nmBuffer);
	}
	if (ptIndices != NULL) {
		free(ptIndices);
	}
}

void Object::draw()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnable(GL_NORMALIZE);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPushMatrix();
	glTranslatef(position.x, position.y, position.z);
	for (std::list<SubMesh*>::iterator i = children.begin();i != children.end();i++) {
		Material mt = material_map[(*i)->mtName];
		//glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0);
		glColor4f(mt.kd.red, mt.kd.green, mt.kd.blue, mt.kd.alpha);
		SetMaterialColor(mt.ka,GL_FRONT_AND_BACK, GL_AMBIENT);
		SetMaterialColor(mt.kd,GL_FRONT_AND_BACK, GL_DIFFUSE);
		SetMaterialColor(mt.ks, GL_FRONT_AND_BACK, GL_SPECULAR);
		glBindTexture(GL_TEXTURE_2D, mt.kd_texid);
		glVertexPointer(3, GL_FLOAT, 0, (*i)->ptBuffer);
		glTexCoordPointer(2, GL_FLOAT, 0, (*i)->uvBuffer);
		glNormalPointer(GL_FLOAT, 0, (*i)->nmBuffer);
		glDrawArrays(GL_TRIANGLES, 0, (*i)->vNum);
	}
	glPopMatrix();
	glDisable(GL_NORMALIZE);
	glDisable(GL_TEXTURE_2D);
}

void Object::parsePolygon(std::ifstream & ifs,std::string &name)
{
	int totalFace = countFaceNumber(ifs);
	SubMesh* pMesh = new SubMesh;
	std::string curline;
	pMesh->mtName = name;
	pMesh->fcNum = totalFace;
	pMesh->vNum = totalFace * 3;
	pMesh->ptBuffer = (Point*)malloc(sizeof(Point)*pMesh->vNum);
	pMesh->uvBuffer = (UVPoint*)malloc(sizeof(UVPoint)*pMesh->vNum);
	pMesh->nmBuffer = (NMPoint*)malloc(sizeof(NMPoint)*pMesh->vNum);
	pMesh->ptIndices = (GLuint*)malloc(sizeof(GLuint)*pMesh->vNum);
	int cur_pt = 0;
	int cur_fc = 0;
		while (std::getline(ifs, curline)) {
			std::stringstream strstm(curline);
			std::string head;
			strstm >> head;
			if (head == "f") {
				std::string pt;
				while (!strstm.eof()) {
					if (!(strstm >> pt)) {
						break;
					}
					int v, uv, nm;
					getPointIdx(pt, v, uv, nm);
					memcpy(pMesh->ptBuffer[cur_pt], ptBuffer[v -1], sizeof(Point));
					memcpy(pMesh->uvBuffer[cur_pt], uvBuffer[uv -1], sizeof(UVPoint));
					memcpy(pMesh->nmBuffer[cur_pt], nmBuffer[nm -1], sizeof(NMPoint));
					pMesh->ptIndices[cur_pt] = v - 1;
					cur_pt = cur_pt+1;
				}
				cur_fc= cur_fc+1;
				if (cur_fc == totalFace) {
					break;
				}
			}
			if (head == "g") {
				std::string fcname;
				strstm >> fcname;
				pMesh->fcName = fcname;
				
			}
		}
	
	children.push_back(pMesh);
}

void getPointIdx(std::string & buffer, int & v, int & uv, int & nm)
{
	std::string vbuf;
	std::string uvbuf;
	std::string nmbuf;
	v = -1;
	uv = -1;
	nm = -1;
	int idx = 0;
	for (int i = 0;i < buffer.size();i++) {
		if (buffer[i] == '/') {
			idx++;
			continue;
		}
		switch (idx)
		{
		case 0:
			vbuf += buffer[i];
			break;
		case 1:
			uvbuf += buffer[i];
			break;
		case 2:
			nmbuf += buffer[i];
			break;
		default:
			break;
		}
	}
	v = atoi(vbuf.c_str());
	uv = atoi(uvbuf.c_str());
	nm = atoi(nmbuf.c_str());
}

int Object::countFaceNumber(std::ifstream & ifs)
{
	std::streampos cur = ifs.tellg();
	std::string curline;
	int totalFace = 0;
	while (std::getline(ifs, curline)) {
		if (curline[0] == 'f') {
			totalFace++;
		}
		if (curline[0] != '#'&&curline[0] != 'f'&&curline[0] != 'g' && curline[0]!='s') {
			break;
		}
	}
	ifs.clear();
	ifs.seekg(cur);
	return totalFace;
}

void Object::countNumber(std::ifstream& ifs)
{
	std::streampos cur = ifs.tellg();
	std::string curline;
	while (std::getline(ifs, curline)) {
		if (curline[0] == 'v') {
			if (curline[1] == 't') {
				vtNum ++;
			}
			else if (curline[1] == 'n') {
				vnNum++;
			}
			else {
				vNum++;
			}
		}
	}
	ifs.clear();
	ifs.seekg(cur);
}

void Object::loadMtl(const char * filename)
{
	std::ifstream ifs(filename);
	if (!ifs) {
		std::cout << "找不到材质文件:" << filename << '\n';
		std::cout << "任意键退出\n";
		std::cin.get();
		exit(0);
	}
	std::string curline;
	while (std::getline(ifs, curline)) {
		std::stringstream strstm(curline);
		std::string head;
		strstm >> head;
		if (head == "newmtl") {
			std::string name;
			strstm >> name;
			while(std::getline(ifs,curline)){
				if (curline == "") {
					break;
				}
				std::stringstream curstm(curline);
				curstm >> head;
				if (head == "illum") {
					curstm >> material_map[name].illum;
				}
				if (head == "Kd") {
					curstm >> material_map[name].kd.red;
					curstm >> material_map[name].kd.green;
					curstm >> material_map[name].kd.blue;
				}
				if (head == "Ka") {
					curstm >> material_map[name].ka.red;
					curstm >> material_map[name].ka.green;
					curstm >> material_map[name].ka.blue;
				}
				if (head == "Ks") {
					curstm >> material_map[name].ks.red;
					curstm >> material_map[name].ks.green;
					curstm >> material_map[name].ks.blue;
				}
				if (head == "Ke") {
					curstm >> material_map[name].ke.red;
					curstm >> material_map[name].ke.green;
					curstm >> material_map[name].ke.blue;
				}
				if (head == "Ns") {
					curstm >> material_map[name].ns;
				}
				if (head == "map_Kd") {
					std::string texname;
					curstm >> texname;
					std::string fpath = fillPath(filename, texname.c_str());
					if (texname[texname.size() - 1] == 'p') {
						LoadTexture(fpath.c_str(), material_map[name].kd_texid);
					}
					else if (!LoadTexture(fpath.c_str(), material_map[name].kd_texid,GL_BGRA,GL_RGBA)) {
						std::cout << "无法加载纹理:" << fpath<<'\n';
					}
				}
			}
		}
	}
}

void Object::loadFromObj(const char * filename)
{
	std::ifstream ifs(filename);
	if (!ifs) {
		std::cout << "找不到模型文件:" << filename << '\n';
		std::cout << "任意键退出\n";
		std::cin.get();
		exit(0);
	}
	countNumber(ifs);
	std::string curline;
	ptBuffer = new Point[vNum];
	nmBuffer = new NMPoint[vnNum];
	uvBuffer = new UVPoint[vtNum];
	GLuint cur_vp = 0;
	GLuint cur_vnp = 0;
	GLuint cur_vtp = 0;
	while (std::getline(ifs, curline)) {
		std::stringstream strstm(curline);
		std::string head;
		strstm >> head;
		if (head == "mtllib") {
			std::string fname;
			strstm >> fname;
			std::string fpath = fillPath(filename, fname.c_str());
			loadMtl(fpath.c_str());
		}
		if (head == "v") {
			float x,y,z;
			strstm >> x >> y >> z;
			ptBuffer[cur_vp][0] = x;
			ptBuffer[cur_vp][1] = y;
			ptBuffer[cur_vp][2] = z;
			cur_vp ++;
		}
		if (head == "vt") {
			float u, v;
			strstm >> u >> v;
			if (u > 1.0) {
				int ui = u;
				u -= ui;
			}
			if (v > 1.0) {
				int vi = v;
				v -= vi;
			}
			uvBuffer[cur_vtp][0] = u;
			uvBuffer[cur_vtp][1] = v;
			cur_vtp++;
		}
		if (head == "vn") {
			float x, y, z;
			strstm >> x >> y >> z;
			nmBuffer[cur_vnp][0] = x;
			nmBuffer[cur_vnp][1] = y;
			nmBuffer[cur_vnp][2] = z;
			cur_vnp++;
		}
		if (head == "usemtl") {
			std::string name;
			strstm >> name;
			parsePolygon(ifs, name);
		}
	}
}
Object::Object():vNum(0),vtNum(0),vnNum(0),position(0.0,0.0,0.0) {
	ptBuffer = NULL;
	uvBuffer = NULL;
	nmBuffer = NULL;
}
Object::~Object()
{
	if (ptBuffer != NULL) {
		delete[] ptBuffer;
	}
	if (uvBuffer != NULL) {
		delete[] uvBuffer;
	}
	if (nmBuffer != NULL) {
		delete[] nmBuffer;
	}
	for (std::list<SubMesh*>::iterator i = children.begin();i != children.end();i++) {
		delete *i;
	}
}
