#shader vertex
#version 440 core

layout(location=0) in vec4 position;
layout(location=1) in vec2 texCoord;

out vec2 v_TexCoord;

void main()
{
	gl_Position=position;
	v_TexCoord = texCoord;

};

#shader fragment
#version 440 core

layout(location=0) out vec4 color;

in vec2 v_TexCoord;


uniform vec4 u_Color; // ���u_Color��û�б��õ�����˻��ڱ���ʱ�����������ԣ���Ҳ��Ϊʲôֱ�����лᱨ���uniform������
uniform sampler2D u_Texture;

void main()
{

	vec4 texColor = texture(u_Texture, v_TexCoord);
	color = texColor+(u_Color*0.2);//Ϊ����u_Color�����ã��Ҽӵ���ͼƬ�ϲ�������˸��Ч��
	// ������Ϊ�˼��Ϊʲôû�취��ʾlogo��������ʹ��rg����������ӡ�˳������õ���һ�Ŵ�����ͼ��������������Զ���ص�0
	// �������VertexArray�ļ���һ������û�и���element��index����ֵ
	//color = vec4(v_TexCoord[0],v_TexCoord[1],1,1);  

};