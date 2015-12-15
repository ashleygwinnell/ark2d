#ifndef ARK_DIRECTIONALLIGHT_VERT_GLSL
#define ARK_DIRECTIONALLIGHT_VERT_GLSL

	struct DirectionalLight
	{
		vec3 vColor;
		vec3 vDirection;
		float fAmbientIntensity;
		float fStrength;
	};

	vec4 getDirectionalLightColor(DirectionalLight dirLight, vec3 vNormal)
	{
		float fDiffuseIntensity = max(0.0, dot(normalize(vNormal), -dirLight.vDirection));
		return vec4(dirLight.vColor*(dirLight.fAmbientIntensity+fDiffuseIntensity)*dirLight.fStrength, 1.0);
	}

#endif 