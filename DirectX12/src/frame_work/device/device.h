/**
* @file device.h
* @brief 各デバイスを管理するクラス
* @author 石山　悠
* @date 2019/02/08
*/
#pragma once
#include "../common/d3d12.h"
#include "../common/alias.h"
#include <saki/singleton.h>
#include <memory>

/**
* @brief 各デバイスを管理するクラス
*/
class Device :public saki::singleton<Device>
{
	class Impl;
	std::unique_ptr<Impl> pimpl;
public:
	Device();
	~Device()noexcept;

	HRESULT InitDevice(HWND);
	void BeginSceneSet(ComPtr<ID3D12GraphicsCommandList>&);
	HRESULT BeginScene();
	HRESULT EndScene();
	HRESULT Present();
	void SetResourceBarrier
	(D3D12_RESOURCE_STATES, D3D12_RESOURCE_STATES,
		ID3D12Resource*, ComPtr<ID3D12GraphicsCommandList>&);

	//コマンドのセット
	HRESULT SetCommand
	(ComPtr<ID3D12GraphicsCommandList>&,
		ComPtr<ID3D12CommandAllocator>&,
		ID3D12PipelineState*);

	//ゲッタ
	ComPtr<ID3D12Device>& GetDevice();
	ComPtr<ID3D12RootSignature>& GetRootSignature();

	UINT64 GetFrameNum()const;
};