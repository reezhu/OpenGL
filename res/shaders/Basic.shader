#shader vertex
#version 440 core

layout(location=0) in vec4 position;
layout(location=1) in vec2 texCoord;

out vec2 v_TexCoord;

uniform mat4 u_MVP;

void main()
{
	gl_Position = u_MVP * position;//����Ǿ���˷���Ӧ��ֻ����˲��ܷ����ұ�
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
	color = texColor;
	// ������Ϊ�˼��Ϊʲôû�취��ʾlogo��������ʹ��rg����������ӡ�˳������õ���һ�Ŵ�����ͼ��������������Զ���ص�0
	// �������VertexArray�ļ���һ������û�и���element��index����ֵ
	//color = vec4(v_TexCoord[0],v_TexCoord[1],1,1);  

};