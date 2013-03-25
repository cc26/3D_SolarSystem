varying vec3 vertex;	// The position of the vertex, in eye space
varying vec3 normal;	// The normal vector of the vertex, in eye space
varying vec3 light;	// The normalized vector from the vertex to the light
varying vec3 eye;

varying vec3 tangentLight;
varying vec3 tangentEye;

void main()
{
    gl_Position = ftransform();
    
    vertex = (gl_ModelViewMatrix * gl_Vertex).xyz;
    normal = normalize(gl_NormalMatrix*gl_Normal).xyz;
    light = normalize(gl_LightSource[0].position.xyz - vertex);
    eye = normalize(-vertex);
    
    gl_TexCoord[0] = gl_MultiTexCoord0;

    // calculate tangent
    vec3 tangent;
    vec3 v1 = cross(gl_Normal, vec3(0.0,0.0,1.0));
    vec3 v2 = cross(gl_Normal, vec3(0.0,1.0,0.0));
    
    if(length(v1) > length(v2))
	tangent = v1;
    else
	tangent = v2;
    
    vec3 N = normalize(gl_NormalMatrix*gl_Normal);
    vec3 T = normalize(gl_NormalMatrix*tangent);
    vec3 B = cross(N,T);
    
    //tanfrom matrix : eyespace-> tagent space
    //mat3 et_matrix = mat3(T.x,B.x,N.x,
			  //T.y,B.y,N.y,
			  //T.z,B.z,N.z);
    mat3 et_matrix = mat3(T,B,N);
    
    tangentLight = normalize(light*et_matrix);
    
    
    tangentEye = normalize(eye*et_matrix);
    
}