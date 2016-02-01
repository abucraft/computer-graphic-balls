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
	//保存属性，不这样做的话后面的物体的颜色会被天空盒影响，具体原因我并不是很清楚
	//glPushAttrib(GL_ALL_ATTRIB_BITS);
	glPushMatrix();
	{
		//天空盒放在相机的位置  
		glTranslatef(camera.position.x, camera.position.y, camera.position.z);;
		//禁用混合  
		glDisable(GL_BLEND);
		//glDisable(GL_CULL_FACE);
		//禁用深度测试  
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_LIGHTING);
		//禁止深度写入  
		glDepthMask(false);
		glEnable(GL_TEXTURE_2D);
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		// 绑定渲染数据  
		glVertexPointer(3, GL_FLOAT, 0, points);
		glTexCoordPointer(2, GL_FLOAT, 0, uv);
		// 记录当前四边面在整体渲染数据缓存中的起始偏移  
		int offset = 0;
		glColor3f(1.0, 1.0, 1.0);
		// 后  
		glBindTexture(GL_TEXTURE_2D,texIds[TEX_BACK_IDX]);

		glDrawArrays(GL_TRIANGLE_FAN, offset, VERTEX_PER_FACE);
		offset += VERTEX_PER_FACE;

		// 前  
		glBindTexture(GL_TEXTURE_2D, texIds[TEX_FRONT_IDX]);

		glDrawArrays(GL_TRIANGLE_FAN, offset, VERTEX_PER_FACE);
		offset += VERTEX_PER_FACE;

		// 右  
		glBindTexture(GL_TEXTURE_2D, texIds[TEX_RIGHT_IDX]);

		glDrawArrays(GL_TRIANGLE_FAN, offset, VERTEX_PER_FACE);
		offset += VERTEX_PER_FACE;

		// 左  
		glBindTexture(GL_TEXTURE_2D, texIds[TEX_LEFT_IDX]);

		glDrawArrays(GL_TRIANGLE_FAN, offset, VERTEX_PER_FACE);
		offset += VERTEX_PER_FACE;

		// 顶  
		glBindTexture(GL_TEXTURE_2D, texIds[TEX_TOP_IDX]);

		glDrawArrays(GL_TRIANGLE_FAN, offset, VERTEX_PER_FACE);
		offset += VERTEX_PER_FACE;

		// 底部  
		glBindTexture(GL_TEXTURE_2D, texIds[TEX_BOTTON_IDX]);

		glDrawArrays(GL_TRIANGLE_FAN, offset, VERTEX_PER_FACE);

		//重新启用必要状态  
		glEnable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_TEXTURE_2D);
		glDepthMask(true);
		//glEnable(GL_CULL_FACE);
	}
	glPopMatrix();
	//glPopAttrib();
}
