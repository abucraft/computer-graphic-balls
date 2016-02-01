#include"skyBox.h"
#include"textureLoader.h"
void Skybox::loadFromFile(const char * filename)
{
	TiXmlDocument doc(filename);
	if (!doc.LoadFile())
	{
		printf("Failed to load skybox file \"%s\"\n", filename);
		return;
	}
	TiXmlHandle hDoc(&doc);
	TiXmlElement* pElem;
	TiXmlHandle hRoot(0);
	pElem = hDoc.FirstChildElement().Element();
	if (!pElem) {
		return;
	}
	hRoot = TiXmlHandle(pElem);
	pElem = hRoot.FirstChild("surface").Element();
	for (int i = 0;i < 6;i++) {
		assert(pElem != 0);
		TiXmlHandle hSurface(pElem);
		TiXmlElement *hBitmap = hSurface.FirstChild("texture").Element();
		if (!LoadTexture(hBitmap->GetText(), texIds[i])) {
			printf("Failed to load texture \"%s\"\n", hBitmap->GetText());
			return;
		}
		printf(" load texture \"%s\"\n", hBitmap->GetText());
		
		TiXmlElement *pVertex = hSurface.FirstChild("vertices").FirstChild().Element();
		for (int j = 0;j < VERTEX_PER_FACE;j++) {
			TiXmlHandle hVertex(pVertex);
			TiXmlElement *pNode = hVertex.FirstChild("position").Element();
			pNode->QueryFloatAttribute("x",&points[i][j][0]);
			pNode->QueryFloatAttribute("y", &points[i][j][1]);
			pNode->QueryFloatAttribute("z", &points[i][j][2]);
			pNode = hVertex.FirstChild("texcoord").Element();
			pNode->QueryFloatAttribute("u", &uv[i][j][0]);
			pNode->QueryFloatAttribute("v", &uv[i][j][1]);
			pVertex = pVertex->NextSiblingElement();
		}
		pElem = pElem->NextSiblingElement();
	}
}

void Skybox::draw(Camera& camera)
{
	//�������ԣ����������Ļ�������������ɫ�ᱻ��պ�Ӱ�죬����ԭ���Ҳ����Ǻ����
	//glPushAttrib(GL_ALL_ATTRIB_BITS);
	glPushMatrix();
	{
		//��պз��������λ��  
		glTranslatef(camera.position.x, camera.position.y, camera.position.z);;
		//���û��  
		glDisable(GL_BLEND);
		//glDisable(GL_CULL_FACE);
		//������Ȳ���  
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_LIGHTING);
		//��ֹ���д��  
		glDepthMask(false);
		glEnable(GL_TEXTURE_2D);
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		// ����Ⱦ����  
		glVertexPointer(3, GL_FLOAT, 0, points);
		glTexCoordPointer(2, GL_FLOAT, 0, uv);
		// ��¼��ǰ�ı�����������Ⱦ���ݻ����е���ʼƫ��  
		int offset = 0;
		glColor3f(1.0, 1.0, 1.0);
		// ��  
		glBindTexture(GL_TEXTURE_2D,texIds[TEX_BACK_IDX]);

		glDrawArrays(GL_TRIANGLE_FAN, offset, VERTEX_PER_FACE);
		offset += VERTEX_PER_FACE;

		// ǰ  
		glBindTexture(GL_TEXTURE_2D, texIds[TEX_FRONT_IDX]);

		glDrawArrays(GL_TRIANGLE_FAN, offset, VERTEX_PER_FACE);
		offset += VERTEX_PER_FACE;

		// ��  
		glBindTexture(GL_TEXTURE_2D, texIds[TEX_RIGHT_IDX]);

		glDrawArrays(GL_TRIANGLE_FAN, offset, VERTEX_PER_FACE);
		offset += VERTEX_PER_FACE;

		// ��  
		glBindTexture(GL_TEXTURE_2D, texIds[TEX_LEFT_IDX]);

		glDrawArrays(GL_TRIANGLE_FAN, offset, VERTEX_PER_FACE);
		offset += VERTEX_PER_FACE;

		// ��  
		glBindTexture(GL_TEXTURE_2D, texIds[TEX_TOP_IDX]);

		glDrawArrays(GL_TRIANGLE_FAN, offset, VERTEX_PER_FACE);
		offset += VERTEX_PER_FACE;

		// �ײ�  
		glBindTexture(GL_TEXTURE_2D, texIds[TEX_BOTTON_IDX]);

		glDrawArrays(GL_TRIANGLE_FAN, offset, VERTEX_PER_FACE);

		//�������ñ�Ҫ״̬  
		glEnable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_TEXTURE_2D);
		glDepthMask(true);
		//glEnable(GL_CULL_FACE);
	}
	glPopMatrix();
	//glPopAttrib();
}
