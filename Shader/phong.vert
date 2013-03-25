varying vec3 vertex;	// The position of the vertex, in eye space
varying vec3 normal;	// The normal vector of the vertex, in eye space
varying vec3 light;	// The normalized vector from the vertex to the light
varying vec3 eye;
void main()
{
    gl_Position = ftransform();
    
    vertex = (gl_ModelViewMatrix * gl_Vertex).xyz;
    normal = normalize(gl_NormalMatrix*gl_Normal).xyz;
    light = normalize(gl_LightSource[0].position.xyz - vertex);
    eye = normalize(-vertex);
    
    gl_TexCoord[0] = gl_MultiTexCoord0;
}