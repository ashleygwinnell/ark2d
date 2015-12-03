#ifndef ARK_POINTLIGHT_VERT_GLSL
#define ARK_POINTLIGHT_VERT_GLSL

	struct PointLight 
	{ 
	   vec3 vColor; // Color of that point light
	   vec3 vPosition; 
	    
	   float fAmbient; 

	   float fConstantAtt; 
	   float fLinearAtt; 
	   float fExpAtt; 
	}; 

	vec4 getPointLightColor(const PointLight ptLight, vec3 vWorldPos, vec3 vNormal) 
	{ 
	   vec3 vPosToLight = vWorldPos - ptLight.vPosition; 
	   float distance = length(vPosToLight); 
	   vPosToLight = normalize(vPosToLight); 
	    
	   float diffuse = max(0.0, dot(vNormal, -vPosToLight)); 

	   float attenuation = ptLight.fConstantAtt + ptLight.fLinearAtt*distance + ptLight.fExpAtt*distance*distance; 

	   return vec4(ptLight.vColor, 1.0)*(ptLight.fAmbient+diffuse)/attenuation; 
	}

#endif