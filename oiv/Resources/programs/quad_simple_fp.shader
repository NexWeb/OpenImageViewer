#include "imagecommon.shader"

cbuffer BaseImageData_ : register(b0) 
{
	BaseImageData baseImageData;
};


uniform Texture2D    tex1;
uniform SamplerState samplerState;

#if defined(HLSL) || defined(D3D11)
////////////////////////
///DIRECT3D HLSL FRAGMENT SHADER
///////////////////////
struct ShaderIn
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD;
};

struct ShaderOut
{
    float4 texelOut : SV_Target;
};



float4 GetFinalTexel(float2 i_inputUV)
{
	float2 uvScale = (baseImageData.uImageSize.xy * baseImageData.uScale / baseImageData.uViewportSize.xy );
	float2 offset= baseImageData.uImageOffset.xy / baseImageData.uViewportSize.xy / uvScale;
	float2 uvFixed = i_inputUV / uvScale  - offset;
	
	float4 finalTexelColor;
	
	if (   uvFixed.x >= 0  
		&& uvFixed.x <= 1 
		&& uvFixed.y >= 0 
		&& uvFixed.y <= 1)
        finalTexelColor = tex1.Sample(samplerState, uvFixed);
    else
        finalTexelColor = float4(0, 0, 0, 0);
		
		finalTexelColor.a *= baseImageData.uOpacity;
		return finalTexelColor;
}
void main(in ShaderIn input, out ShaderOut output)
{
    output.texelOut = GetFinalTexel(input.uv);
}
#else
////////////////////////
///OPENGL GLSL FRAGMENT SHADER
///////////////////////
in vec2 coords;
out vec4 outColor;
void main()
{
    outColor = GetFinalTexel(coords, uViewportSize.xy, uImageSize, uvScale, uvOffset, uShowGrid);
}

#endif