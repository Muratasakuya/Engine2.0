#include "DXShaderCompiler.h"

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/DXClass/DXCommon.h>

//============================================================================*/
//	DXShaderCompiler classMethods
//============================================================================*/

IDxcBlob* DXShaderCompiler::CompileShader(
	const std::wstring& filePath, const wchar_t* profile,
	IDxcUtils* dxcUtils, IDxcCompiler3* dxcCompiler, IDxcIncludeHandler* includeHandler) {

	// hlslファイルを読み込む
	IDxcBlobEncoding* shaderSouce = nullptr;
	HRESULT hr = dxcUtils->LoadFile(filePath.c_str(), nullptr, &shaderSouce);
	// 読めなかったら止める
	assert(SUCCEEDED(hr));
	// 読み込んだファイルの内容を設定する
	DxcBuffer shaderSourceBuffer;
	shaderSourceBuffer.Ptr = shaderSouce->GetBufferPointer();
	shaderSourceBuffer.Size = shaderSouce->GetBufferSize();
	// UTF8の文字コードであることを通知
	shaderSourceBuffer.Encoding = DXC_CP_UTF8;

	LPCWSTR arguments[] = {
		filePath.c_str(),
		L"-E",L"main",
		L"-T",profile,
		L"-Zi",L"-Qembed_debug",
		L"-Od",
		L"-Zpr",
	};
	IDxcResult* shaderResult = nullptr;
	hr = dxcCompiler->Compile(
		&shaderSourceBuffer,
		arguments,
		_countof(arguments),
		includeHandler,
		IID_PPV_ARGS(&shaderResult));
	// コンパイルエラーではなくdxcが起動できないなど致命的な状況
	assert(SUCCEEDED(hr));

	ComPtr<IDxcBlobUtf8> shaderError = nullptr;
	shaderResult->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&shaderError), nullptr);
	if (shaderError != nullptr && shaderError->GetStringLength() != 0) {
		assert(false);
	}

	IDxcBlob* shaderBlob = nullptr;
	hr = shaderResult->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&shaderBlob), nullptr);
	assert(SUCCEEDED(hr));
	// もう使わないリソースを解放
	shaderSouce->Release();
	shaderResult->Release();
	shaderError.Reset();

	return shaderBlob;
}

void DXShaderCompiler::Compile(
	DXCommon* dxCommon, const PostProcessPipelineType& pipelineType) {

	const wchar_t* vsShaderPath = L"./Resources/Engine/Shaders/Fullscreen.VS.hlsl";

	const wchar_t* psShaderPath = nullptr;
	switch (pipelineType) {
	case PostProcessPipelineType::CopyTexture:
		psShaderPath = L"./Resources/Engine/Shaders/CopyImage.PS.hlsl";
		break;
	case PostProcessPipelineType::Vignette:
		psShaderPath = L"./Resources/Engine/Shaders/Vignette.PS.hlsl";
		break;
	case PostProcessPipelineType::RadialBlur:
		psShaderPath = L"./Resources/Engine/Shaders/RadialBlur.PS.hlsl";
		break;
	default:
		assert(false && "Unsupported pipeline type");
		return;
	}

	postProcessVSBlob_[pipelineType] = CompileShader(vsShaderPath, L"vs_6_0",
		dxCommon->GetDxcUtils(), dxCommon->GetDxcCompiler(), dxCommon->GetIncludeHandler());
	assert(postProcessVSBlob_[pipelineType] != nullptr);

	postProcessPSBlob_[pipelineType] = CompileShader(psShaderPath, L"ps_6_0",
		dxCommon->GetDxcUtils(), dxCommon->GetDxcCompiler(), dxCommon->GetIncludeHandler());
	assert(postProcessPSBlob_[pipelineType] != nullptr);

}