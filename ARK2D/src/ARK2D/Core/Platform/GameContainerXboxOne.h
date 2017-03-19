/*
 * GameContainerWindows.h
 *
 *  Created on: 27 Feb 2011
 *      Author: ashley
 */


#ifndef GAMECONTAINERXBOXONE_H_
#define GAMECONTAINERXBOXONE_H_

#include "../ARK2D.h"

#if defined(ARK2D_XBOXONE) || defined(ARK2D_WINDOWS_STORE)

	#include "../../Windres.h"
	#include "../Graphics/Image.h"

	#include <ppltasks.h>
	#include <wrl.h>
	#include <wrl/client.h>
	#include <dxgi1_4.h>
	#include <d3d11_3.h>
	#include <d2d1_3.h>
	#include <d2d1effects_2.h>
	#include <dwrite_3.h>
	#include <wincodec.h>
	#include <DirectXColors.h>
	#include <DirectXMath.h>
	#include <memory>
	#include <agile.h>
	#include <concrt.h>

	#include <wrl/client.h>
	#if defined(ARK2D_WINDOWS_STORE)
		#include <d3d11_3.h>
	#elif defined(ARK2D_XBOXONE)
		#include <d3d11_x.h>
	#endif

	#include <DirectXMath.h>

	using namespace DirectX;
	using namespace Microsoft::WRL;
	using namespace Windows;
	using namespace concurrency;
	using namespace Windows::ApplicationModel;
	using namespace Windows::ApplicationModel::Core;
	using namespace Windows::ApplicationModel::Activation;
	using namespace Windows::UI::Core;
	using namespace Windows::Graphics::Display;

	namespace ARK {
		namespace Core {

			namespace DisplayMetrics
			{
				// High resolution displays can require a lot of GPU and battery power to render.
				// High resolution phones, for example, may suffer from poor battery life if
				// games attempt to render at 60 frames per second at full fidelity.
				// The decision to render at full fidelity across all platforms and form factors
				// should be deliberate.
				static const bool SupportHighResolutions = false;

				// The default thresholds that define a "high resolution" display. If the thresholds
				// are exceeded and SupportHighResolutions is false, the dimensions will be scaled
				// by 50%.
				static const float DpiThreshold = 192.0f;		// 200% of standard desktop display.
				static const float WidthThreshold = 1920.0f;	// 1080p width.
				static const float HeightThreshold = 1080.0f;	// 1080p height.
			};

			// Constants used to calculate screen rotations
			namespace ScreenRotation
			{
				// 0-degree Z-rotation
				static const XMFLOAT4X4 Rotation0(
					1.0f, 0.0f, 0.0f, 0.0f,
					0.0f, 1.0f, 0.0f, 0.0f,
					0.0f, 0.0f, 1.0f, 0.0f,
					0.0f, 0.0f, 0.0f, 1.0f
					);

				// 90-degree Z-rotation
				static const XMFLOAT4X4 Rotation90(
					0.0f, 1.0f, 0.0f, 0.0f,
					-1.0f, 0.0f, 0.0f, 0.0f,
					0.0f, 0.0f, 1.0f, 0.0f,
					0.0f, 0.0f, 0.0f, 1.0f
					);

				// 180-degree Z-rotation
				static const XMFLOAT4X4 Rotation180(
					-1.0f, 0.0f, 0.0f, 0.0f,
					0.0f, -1.0f, 0.0f, 0.0f,
					0.0f, 0.0f, 1.0f, 0.0f,
					0.0f, 0.0f, 0.0f, 1.0f
					);

				// 270-degree Z-rotation
				static const XMFLOAT4X4 Rotation270(
					0.0f, -1.0f, 0.0f, 0.0f,
					1.0f, 0.0f, 0.0f, 0.0f,
					0.0f, 0.0f, 1.0f, 0.0f,
					0.0f, 0.0f, 0.0f, 1.0f
					);
			};
			// Provides an interface for an application that owns DeviceResources to be notified of the device being lost or created.
			interface IDeviceNotify
			{
				virtual void OnDeviceLost() = 0;
				virtual void OnDeviceRestored() = 0;
			};


			class GameContainer;

			struct ARK2D_API GameContainerARK2DResource {
				void* data;
				unsigned int length;
			};

			class ARK2D_API GameContainerPlatform {
				friend class GameContainer;

				public:
					GameContainer* m_container;

					bool m_vsync_enabled;

					// Direct3D objects.
					Microsoft::WRL::ComPtr<ID3D11Device3> m_device;
					Microsoft::WRL::ComPtr<ID3D11DeviceContext3> m_deviceContext;
					Microsoft::WRL::ComPtr<IDXGISwapChain3> m_swapChain;

					// Direct3D rendering objects. Required for 3D.
					Microsoft::WRL::ComPtr<ID3D11RenderTargetView1> m_renderTargetView;
					Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_depthStencilView;
					Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_depthStencilState;
					D3D11_VIEWPORT m_screenViewport;

					// Rasteriser
					ID3D11RasterizerState* m_rasterStateSolid;
					// ID3D11RasterizerState* m_rasterStateWireframe;

					// Blending
					ID3D11BlendState* m_blendState;
					ID3D11BlendState* m_blendStateAdditive;

					// Direct2D drawing components.
					Microsoft::WRL::ComPtr<ID2D1Factory3>		m_d2dFactory;
					Microsoft::WRL::ComPtr<ID2D1Device2>		m_d2dDevice;
					Microsoft::WRL::ComPtr<ID2D1DeviceContext2>	m_d2dContext;
					Microsoft::WRL::ComPtr<ID2D1Bitmap1>		m_d2dTargetBitmap;

					// DirectWrite drawing components.
					Microsoft::WRL::ComPtr<IDWriteFactory3>		m_dwriteFactory;
					Microsoft::WRL::ComPtr<IWICImagingFactory2>	m_wicFactory;

					// Cached reference to the Window.
					Platform::Agile<Windows::UI::Core::CoreWindow> m_window;

					// Cached device properties.
					D3D_FEATURE_LEVEL								m_d3dFeatureLevel;
					Windows::Foundation::Size						m_d3dRenderTargetSize;
					Windows::Foundation::Size						m_outputSize;
					Windows::Foundation::Size						m_logicalSize;
					Windows::Graphics::Display::DisplayOrientations	m_nativeOrientation;
					Windows::Graphics::Display::DisplayOrientations	m_currentOrientation;
					float											m_dpi;

					// This is the DPI that will be reported back to the app. It takes into account whether the app supports high resolution screens or not.
					float m_effectiveDpi;

					// Transforms used for display orientation.
					D2D1::Matrix3x2F	m_orientationTransform2D;
					DirectX::XMFLOAT4X4	m_orientationTransform3D;

					void init();
					void setTitle(std::string title);
					string getResourcePath() const;

					void SetWindow(Windows::UI::Core::CoreWindow^ window);
					void CreateDeviceIndependentResources();
					void CreateDeviceResources();
					void CreateWindowSizeDependentResources();
					void UpdateRenderTargetSize();

					void HandleDeviceLost();
						void ReleaseResourcesForSuspending();

						void swapBuffers();

					inline float ConvertDipsToPixels(float dips)
					{
						static const float dipsPerInch = 96.0f;
						return floorf(dips * m_dpi / dipsPerInch + 0.5f); // Round to nearest integer.
					}
					inline float ConvertDipsToPixels(float dips, float dpi)
					{
						static const float dipsPerInch = 96.0f;
						return floorf(dips * dpi / dipsPerInch + 0.5f); // Round to nearest integer.
					}

					bool initOpenAL();
					bool deinitOpenAL();

					void beginDXFrame();

					ID3D11Device3*				GetD3DDevice() const			{ return m_device.Get(); }
					ID3D11DeviceContext3*		GetD3DDeviceContext() const		{ return m_deviceContext.Get(); }

					static void* getARK2DResource(int resourceId, int resourceType);
					static GameContainerARK2DResource getARK2DResourceWithLength(int resourceId, int resourceType);


					// This method determines the rotation between the display device's native orientation and the
					// current display orientation.
					DXGI_MODE_ROTATION ComputeDisplayRotation();
					void ValidateDevice();
					void SetLogicalSize(Windows::Foundation::Size logicalSize); // This method is called in the event handler for the SizeChanged event.
					void SetDpi(float dpi);// This method is called in the event handler for the DpiChanged event.
					void SetCurrentOrientation(DisplayOrientations currentOrientation); // This method is called in the event handler for the OrientationChanged event.
					void Trim();
					IDeviceNotify* m_deviceNotify;
					void RegisterDeviceNotify(IDeviceNotify* deviceNotify);
			};

		}
	}

	#if defined(ARK2D_WINDOWS_DLL)
		namespace DX
		{
			inline void ThrowIfFailed(HRESULT hr)
			{
				if (FAILED(hr))
				{
					// Set a breakpoint on this line to catch Win32 API errors.
					throw Platform::Exception::CreateException(hr);
				}
			}

			// Function that reads from a binary file asynchronously.
			inline Concurrency::task<Platform::Array<byte>^> ReadDataAsync(Platform::String^ filename)
			{
				using namespace Windows::Storage;
				using namespace Concurrency;

				auto folder = Windows::ApplicationModel::Package::Current->InstalledLocation;

				return create_task(folder->GetFileAsync(filename)).then([] (StorageFile^ file)
				{
					return file->OpenReadAsync();
				}).then([] (Streams::IRandomAccessStreamWithContentType^ stream)
				{
					unsigned int bufferSize = static_cast<unsigned int>(stream->Size);
					auto fileBuffer = ref new Streams::Buffer(bufferSize);
					return stream->ReadAsync(fileBuffer, bufferSize, Streams::InputStreamOptions::None);
				}).then([] (Streams::IBuffer^ fileBuffer) -> Platform::Array<byte>^
				{
					auto fileData = ref new Platform::Array<byte>(fileBuffer->Length);
					Streams::DataReader::FromBuffer(fileBuffer)->ReadBytes(fileData);
					return fileData;
				});
			}
			#if defined(_DEBUG) || defined(ARK2D_DEBUG)
				// Check for SDK Layer support.
				inline bool SdkLayersAvailable()
				{
					HRESULT hr = D3D11CreateDevice(
						nullptr,
						D3D_DRIVER_TYPE_NULL,       // There is no need to create a real hardware device.
						0,
						D3D11_CREATE_DEVICE_DEBUG,  // Check for the SDK layers.
						nullptr,                    // Any feature level will do.
						0,
						D3D11_SDK_VERSION,          // Always set this to D3D11_SDK_VERSION for Windows Store apps.
						nullptr,                    // No need to keep the D3D device reference.
						nullptr,                    // No need to know the feature level.
						nullptr                     // No need to keep the D3D device context reference.
						);

					return SUCCEEDED(hr);
				}
			#endif
		}
	#endif

#endif

#endif /* GAMECONTAINERWINDOWSPHONE8_H_ */


