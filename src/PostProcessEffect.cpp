#include "PostProcessEffect.h"
#include <renderer\graphics.h>

FadeOutEffect::FadeOutEffect(FadeOutEffectSettings *settings) : PostProcessEffect() ,_settings(settings) {
	_rtID = ds::renderer::createRenderTarget(ds::Color(0, 0, 0, 255));
	_shaderID = createFadeShader(0);
	_fadeTimer = 0.0f;
}

// -------------------------------------------------------
// Create a position texture color shader
// -------------------------------------------------------
int FadeOutEffect::createFadeShader(int textureId) {
	const char* g_strBuffer =
		"uniform extern float4x4 gWVP;\r\n"
		"uniform extern texture gTex;\r\n"
		"float timer = 1.0f; \r\n"
		"float4 grayscale = float4(0.2125, 0.7154, 0.0721, 1.0); \r\n"
		"sampler TexS = sampler_state {\r\n"		
		"	Texture = <gTex>;\r\n"
		"	MinFilter = LINEAR;\r\n"
		"	MagFilter = LINEAR;\r\n"
		"	MipFilter = LINEAR;\r\n"
		"	AddressU  = CLAMP;\r\n"
		"	AddressV  = CLAMP;\r\n"
		"};\r\n"
		"struct OutputVS {\r\n"
		"	float4 posH   : POSITION0;\r\n"
		"	float2 tex0   : TEXCOORD0;\r\n"
		"	float4 color0 : COLOR0;\r\n"
		"};\r\n"
		"OutputVS BasicVS(float3 posL : POSITION0,float2 tex0 : TEXCOORD0 , float4 color : COLOR0) {\r\n"
		"	OutputVS outVS = (OutputVS)0;	\r\n"
		"	outVS.posH = mul(float4(posL, 1.0f), gWVP);		\r\n"
		"	outVS.tex0 = tex0;\r\n"
		"	outVS.color0 = color;\r\n"
		"	return outVS;\r\n"
		"}\r\n"
		"float4 BasicPS(OutputVS input) : COLOR {\r\n"
		"	float4 clr = tex2D(TexS, input.tex0);\r\n"
		"	float3 greyscale = dot(clr.rgb, float3(0.30, 0.59, 0.11)); \r\n"
		"	float3 rgb = lerp(clr.rgb, greyscale, timer); \r\n"
		"	return float4(rgb, clr.a);\r\n"
		"}\r\n"
		"technique FadeTech {\r\n"
		"	pass P0 {\r\n"
		"		vertexShader = compile vs_2_0 BasicVS();\r\n"
		"		pixelShader  = compile ps_2_0 BasicPS();\r\n"
		"	}\r\n"
		"}\r\n";
		int ret = ds::renderer::createShaderFromText(g_strBuffer, "FadeTech");
		_shader = ds::renderer::getShader(ret);
		if (textureId != -1) {
			_shader->setTexture("gTex", textureId);
		}
	return ret;
}


void FadeOutEffect::activate() {
	_fadeTimer = 0.0f;
	PostProcessEffect::activate();
}
// ----------------------------------------------
// begin
// ----------------------------------------------
void FadeOutEffect::begin() {
	if (isActive()) {
		ds::renderer::setRenderTarget(_rtID);
		float norm = _fadeTimer / _settings->fadeTTL;
		_shader->setFloat("timer", norm);
		ds::renderer::setCurrentShader(_shaderID);
	}
}

// ----------------------------------------------
// update
// ----------------------------------------------
void FadeOutEffect::update(float dt) {
	if (isActive()) {
		_fadeTimer += dt;
		if (_fadeTimer > _settings->fadeTTL) {
			_fadeTimer = _settings->fadeTTL;
		}
	}
}

// ----------------------------------------------
// end
// ----------------------------------------------
void FadeOutEffect::end() {
	if (isActive()) {
		int sid = ds::renderer::getDefaultShaderID();
		ds::renderer::restoreBackBuffer();
		ds::renderer::draw_render_target_additive(_rtID, _shaderID);
		ds::renderer::setCurrentShader(sid);
	}
}
	