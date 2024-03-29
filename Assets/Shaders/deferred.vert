layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 UV;

out VS_OUT{ 
	vec3 FragPos;
	vec3 NormalVector;
	vec2 UV;
} vs_out; 

uniform MatrixData Matrix;

void main()
{
	gl_Position = Matrix.Projection*Matrix.View*Matrix.Model*vec4(aPos.x, aPos.y, aPos.z, 1.0);

	vs_out.FragPos = vec3(Matrix.Model*vec4(aPos,1.0));
	vs_out.NormalVector=mat3(Matrix.Normal) * aNormal;
	vs_out.UV=UV;
}