// last revising at 11.11.21

#include "stdafx.h"
#include "Render.h"
#include "Log.h"
#include "macros.h"

namespace D3D11Framework
{
	//-------------------------------------------------------------------

	Render::Render(void)
	{
		m_driverType = D3D_DRIVER_TYPE_NULL;
		m_featureLevel = D3D_FEATURE_LEVEL_11_0;
		m_pd3dDevice = nullptr;
		m_pImmediateContext = nullptr;
		m_pSwapChain = nullptr;
		m_pRenderTargetView = nullptr;
	}

	Render::~Render(void)
	{

	}

	bool Render::CreateDevice(HWND hWnd)
	{
		Log::Get()->Debug("Render::CreateDevice()");
		HRESULT hr = S_OK;

		RECT rc;
		GetClientRect(hWnd, &rc);

		UINT width = rc.right - rc.left;
		UINT height = rc.bottom - rc.top;


		UINT createDeviceFlags = 0;
#ifdef _DEBUG
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		D3D_DRIVER_TYPE driverTypes[] =
		{
			D3D_DRIVER_TYPE_HARDWARE,
			D3D_DRIVER_TYPE_WARP,
			D3D_DRIVER_TYPE_REFERENCE,
		};

		UINT numDriverTypes = ARRAYSIZE(driverTypes);


		D3D_FEATURE_LEVEL featureLevels[] =
		{
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
		};

		UINT numFeatureLevels = ARRAYSIZE(featureLevels);




		// --------------------------------------------
		// Filling in of the swap chain description

		DXGI_SWAP_CHAIN_DESC scd;
		ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

		scd.BufferCount = 1;
		scd.BufferDesc.Width = width;
		scd.BufferDesc.Height = height;
		scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		scd.BufferDesc.RefreshRate.Numerator = 60;
		scd.BufferDesc.RefreshRate.Denominator = 0;
		scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		scd.OutputWindow = hWnd;
		scd.SampleDesc.Count = 1;
		scd.SampleDesc.Quality = 0;
		scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
		scd.Windowed = TRUE;


		// -----------------------------------------------
		// Creation of the swap chain, device and device context
		// using the swap chain description

		D3D_DRIVER_TYPE driverType;

		for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
		{
			driverType = driverTypes[driverTypeIndex];

			hr = D3D11CreateDeviceAndSwapChain(NULL,
				driverType,
				NULL,
				createDeviceFlags,
				featureLevels,
				numFeatureLevels,
				D3D11_SDK_VERSION,
				&scd,
				&m_pSwapChain,
				&m_pd3dDevice,
				&m_featureLevel,
				&m_pImmediateContext);

			if (SUCCEEDED(hr))
			{
				Log::Get()->Debug("Render::CreateDevice(): swap chain, "
					"device and device context were created successfully");
				break;
			}
		}

		if (FAILED(hr))
		{
			Log::Get()->Err("Render::CreateDevice(): error during execution "
				"of D3D11CreateSwapChainAndDevice()");
			return false;
		}

		// ------------------------------------------------------------
		// Initialization of the render target view

		ID3D11Texture2D* pBackBuffer = nullptr;
		hr = m_pSwapChain->GetBuffer(0, _uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
		if (FAILED(hr))
		{
			Log::Get()->Err("Render::CreateDevice(): can't get buffer from the swap chain");
			return false;
		}

		hr = m_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_pRenderTargetView);
		_RELEASE(pBackBuffer);
		if (FAILED(hr))
		{
			Log::Get()->Err("Render::CreateDevice(): can't create the render target view");
			return false;
		}

		m_pImmediateContext->OMSetRenderTargets(1, &m_pRenderTargetView, nullptr);




		// ------------------------------------------------------------
		// Initialization of the viewport

		D3D11_VIEWPORT viewPort;
		ZeroMemory(&viewPort, sizeof(D3D11_VIEWPORT));

		viewPort.Width = width;
		viewPort.Height = height;
		viewPort.MinDepth = 0.0f;
		viewPort.MaxDepth = 1.0f;
		viewPort.TopLeftX = 0;
		viewPort.TopLeftY = 0;

		m_pImmediateContext->RSSetViewports(1, &viewPort);

		Log::Get()->Debug("Render::CreateDevice(): was runned successfully");

		return Init(hWnd);
	}


	void Render::BeginFrame(void)
	{
		float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f };
		m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView, ClearColor);
	}

	void Render::EndFrame(void)
	{
		m_pSwapChain->Present(0, 0);
	}

	void Render::Shutdown()
	{
		Close();

		if (m_pImmediateContext)
			m_pImmediateContext->ClearState();

		_RELEASE(m_pRenderTargetView);
		_RELEASE(m_pSwapChain);
		_RELEASE(m_pImmediateContext);
		_RELEASE(m_pd3dDevice);
	}
//-------------------------------------------------------------------
}