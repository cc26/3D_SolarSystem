varying vec3 vertex;	// The position of the vertex, in eye space
varying vec3 normal;	// The normal vector of the vertex, in eye space
varying vec3 light;	// The normalized vector from the vertex to the light
varying vec3 eye;

uniform sampler2D tex;
uniform sampler2D tex_normal;
uniform sampler2D tex_cloud;
uniform samplerCube envMap;   

uniform vec4 ambient;		// The ambient channel of the color to reflect
uniform vec4 diffuse;		// The diffuse channel of the color to reflect
uniform vec4 specular;		// The specular channel of the color to reflect

varying vec3 tangentLight;
varying vec3 tangentEye;

void main()
{
    //envMap******/
    vec3 flec = gl_ModelViewMatrixInverse * vec4(reflect(-eye,normal) , 0.0);
    
    //gl_FragColor = textureCube(envMap, flec);
    /*************/
    
    /***cloud map ***/
    
    vec4 cloud = vec4(texture2D(tex_cloud,gl_TexCoord[0].st).xyz , 1.0); 
    
    /****************/
    
    vec4 color = texture2D(tex, gl_TexCoord[0].st);
    
    vec3 tangentNormal = normalize(texture2D(tex_normal, gl_TexCoord[0].st).xyz*2.0 - 1.0);
    
    //Ambeint 
    vec4 ambientLight = gl_LightSource[0].ambient;
    vec4 ambientMaterial = color;
    //vec4 ambientLight = gl_FrontLightProduct[0].ambient;
    
    //Diffuse lighting
    //float kd = max(0.0, dot(normal,light));   
    float kd = max(0.0, dot(tangentNormal,tangentLight));	  
    vec4 diffuseMaterial = color;
    vec4 diffuseLight = gl_LightSource[0].diffuse;
    
    // Specular lighting
    //strange over here... it should be reflect(-light,normal)
    float shiniess = 4.0;
    
    //vec3 r = reflect(light,normal);
    //float ks = pow( max(0.0, dot(eye,r)) , shiniess); 
    
    vec3 halfvec = (tangentEye + tangentLight)/2.0;
    vec3 r = reflect(-tangentLight,tangentNormal);
    float ks = pow( max(0.0, dot(halfvec,r)) , shiniess);	
    vec4 specularLight = gl_LightSource[0].specular;
    
    vec4 finalColor; 
    
    if(kd == 0)
    {
	finalColor = mix( vec4(0.0,0.0,0.0,0.0) ,cloud, 0.15);
	gl_FragColor = mix(finalColor,textureCube(envMap, flec) , 0.1);
    }
    else
    {
	finalColor = mix( diffuseMaterial*diffuseLight*kd +specularLight*ks, cloud , 0.15);
	gl_FragColor = mix(finalColor, textureCube(envMap, flec) , 0.1);
    }
    
    //finalColor = mix( diffuseMaterial*diffuseLight*kd+specularLight*ks, cloud , 0.15);
    
    //gl_FragColor = mix(finalColor, textureCube(envMap, flec) , 0.1);
    
    //gl_FragColor = diffuseMaterial*diffuseLight*kd;
		    //+specularLight*ks ;
    //+ ambientLight*ambientMaterial*.1

 
}