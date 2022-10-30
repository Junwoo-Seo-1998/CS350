/* Start Header -------------------------------------------------------
Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: phong_shading.frag
Purpose: shader source for the engine.
Language: C++ MSVC(2022)
Platform: MSVC(2022), GPU needs to support OpenGL 4.6.0, Windows11(x64)
Project: junwoo.seo_cs300_1, junwoo.seo_cs300_2
Author: Junwoo Seo, junwoo.seo, 0055213
Creation date: Sep 05 2022
End Header --------------------------------------------------------*/

in VS_OUT{ 
	vec3 FragPos;
	vec3 NormalVector;
} fs_in; 

uniform LightData Light[MAX_LIGHTS];
uniform int LightNumbers;

uniform AttenuationData Attenuation;

uniform vec3 CameraPosition;
uniform MaterialData Material;

uniform FogData Fog;

out vec4 FragColor;
void main()
{
    vec3 NormalVector=normalize(fs_in.NormalVector);
    vec3 ViewVector=CameraPosition-fs_in.FragPos;
    float ViewDistance=length(ViewVector);
    ViewVector=ViewVector/ViewDistance;
	
    vec3 TotalColor=Material.Emissive;
    for(int i=0; i<LightNumbers; ++i)
    {
        switch (Light[i].LightType)
        {
            case 0:
                TotalColor+=ComputePointLight(Light[i], Attenuation, Material, fs_in.FragPos, NormalVector, ViewVector);
                break;
            case 1:
                TotalColor+=ComputeDirectionLight(Light[i], Attenuation, Material, fs_in.FragPos, NormalVector, ViewVector);
                break;
            case 2:
                TotalColor+=ComputeSpotLight(Light[i], Attenuation, Material, fs_in.FragPos, NormalVector, ViewVector);
                break;
        }
    }
    TotalColor=ComputeFog(Fog, TotalColor, ViewDistance);
	FragColor = vec4(TotalColor, 1.0);
} 