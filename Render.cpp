// last revising at 03.12.21

#include "stdafx.h"
#include "Render.h"
#include "Log.h"
#include "macros.h"

namespace D3D11Framework
{
	Render::Render(void)
	{
		m_driverType = D3D_DRIVER_TYPE_NULL;
		m_featureLevel = D3D_FEATURE_LEVEL_11_0;
		m_pd3dDevice = nullptr;
		m_pImmediateContext = nullptr;
		m_pSwapChain = nullptr;
		m_pRenderTargetView = nullptr;

		Log::Get()->Debug("Render::Render(): calling of the constructor");
	}

	Render::~Render(void)
	{
		Log::Get()->Debug("Render::~Render(): calling of the destructor");
	}

	bool Render::CreateDevice(HWND hWnd)
	{
		Log::Get()->Debug("Render::CreateDevice(): the beginning");

		HRESULT hr = S_OK;

		RECT rc;
		bool isSuccessClientRect = GetClientRect(hWnd, &rc);
		if (!isSuccessClientRect)
		{
			Log::Get()->Err("Render::CreateDevice(): can't get the client rect");
			return false;
		}

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


		// definition of the swap chain description
		DXGI_SWAP_CHAIN_DESC scd;
		ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

		scd.BufferCount = 1;
		scd.BufferDesc.Width = width;
		scd.BufferDesc.Height = height;
		scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		scd.BufferDesc.RefreshRate.Numerator = 60;
		scd.BufferDesc.RefreshRate.Denominator = 1;
		scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		scd.OutputWindow = hWnd;
		scd.SampleDesc.Count = 1;
		scd.SampleDesc.Quality = 0;
		scd.Windowed = TRUE;

		// creation of the device, device context and swap chain using 
		// the swap chain description
		for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
		{
			m_driverType = driverTypes[driverTypeIndex];

			hr = D3D11CreateDeviceAndSwapChain(NULL,
												m_driverType,
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
				Log::Get()->Debug("Render::CreateDevice(): the swap chain created successfully");
				break;
			}
		}

		if (FAILED(hr))
		{
			Log::Get()->Err("Render::CreateDevice(): can't create the swap chain");
			return false;
		}


		// initialization of the render target view
		ID3D11Texture2D* pBackBuffer = nullptr;
		hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
		if (FAILED(hr))
		{
			_RELEASE(pBackBuffer);
			Log::Get()->Err("Render::CreateDevice(): can't get a buffer from the swap chain");
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


		// Initialization of the viewport
		D3D11_VIEWPORT viewport;
		ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

		viewport.Width = static_cast<FLOAT>(width);
		viewport.Height = static_cast<FLOAT>(height);
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;

		m_pImmediateContext->RSSetViewports(1, &viewport);

		return Init(hWnd);
	}

	void Render::BeginFrame(void)
	{
		FLOAT ColorRGBA[4] = { 0.2f, 0.4f, 0.6f, 1.0f };
		m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView, ColorRGBA);
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
}