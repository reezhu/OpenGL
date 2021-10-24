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


uniform vec4 u_Color; // 这个u_Color并没有被用到，因此会在编译时被编译器忽略，这也是为什么直接运行会报这个uniform不存在
uniform sampler2D u_Texture;

void main()
{

	vec4 texColor = texture(u_Texture, v_TexCoord);
	color = texColor+(u_Color*0.2);//为了让u_Color能有用，我加到了图片上产生了闪烁的效果
	// 这里我为了检查为什么没办法显示logo，把坐标使用rg两个参数打印了出来，得到了一张纯蓝的图，告诉我坐标永远返回的0
	// 最后发现在VertexArray文件中一个参数没有根据element的index来赋值
	//color = vec4(v_TexCoord[0],v_TexCoord[1],1,1);  

};