/*
 * GameContainerWindows.h
 *
 *  Created on: 27 Feb 2011
 *      Author: ashley
 */


#ifndef GAMECONTAINERWINDOWSPHONE8_H_
#define GAMECONTAINERWINDOWSPHONE8_H_

#include "../../ARK2D.h"

	#if defined(ARK2D_WINDOWS_PHONE_8)

			#include "../../Windres.h"
			#include "../../Includes.h"
			#include "../../Graphics/Image.h"

			#include <windows.h>
			
 			#include <wrl/client.h>
			#include <d3d11_1.h>
			#include <DirectXMath.h>

 			using namespace DirectX;
 			using namespace Microsoft::WRL;
			using namespace Windows;
 			using namespace Windows::UI::Core;
 			using namespace Windows::Foundation;
			using namespace Windows::Graphics::Display;
			using namespace Windows::Foundation;


			namespace ARK {
				namespace Core {
				class GameContainer; 

					struct ARK2D_API GameContainerARK2DResource {
						void* data;
						unsigned int length;
					};
 
					class ARK2D_API GameContainerPlatform {
						friend class GameContainer;
						
						public:
							GameContainer* m_container;

							ID3D11Device* m_device;
							ID3D11DeviceContext* m_deviceContext;
							IDXGISwapChain1* m_swapChain;
							ID3D11RenderTargetView* m_renderTargetView;
							ID3D11DepthStencilView* m_depthStencilView;
							ID3D11RasterizerState* m_rasterState;
							ID3D11RasterizerState* m_rasterStateSolid;
							ID3D11RasterizerState* m_rasterStateWireframe;
							ID3D11DepthStencilState* m_depthStencilState;
							ID3D11BlendState* m_blendState;
							ID3D11BlendState* m_blendStateAdditive;

							D3D_FEATURE_LEVEL m_featureLevel;
							Windows::Foundation::Size m_renderTargetSize;
							Windows::Foundation::Rect m_windowBounds;
							Platform::Agile<Windows::UI::Core::CoreWindow> m_window;
							Platform::Agile<Windows::ApplicationModel::Core::IFrameworkView> m_nativeGame;

							Platform::Agile<Windows::UI::Core::CoreDispatcher> m_uiDispatcher; 
							
							void setTitle(std::string title);
							string getResourcePath() const;

							//void enableOpenGL(HWND hWnd, HDC* hDC, HGLRC* hRC);
							//void disableOpenGL(HWND hWnd, HDC hDC, HGLRC hRC);

							void initialize(Windows::UI::Core::CoreWindow^ window, Windows::ApplicationModel::Core::IFrameworkView^ nativeGame);
							void createDeviceResources();
							void createWindowSizeDependentResources();

							void updateForWindowSizeChange(); // This method is called in the event handler for the SizeChanged event.
 							void handleDeviceLost();
 							void releaseResourcesForSuspending();

 							void updateAndRender();
 							void swapBuffers();

							float convertDipsToPixels(float dips);

							bool initOpenAL();
							bool deinitOpenAL();

							void beginDXFrame();
							

							static void* getARK2DResource(int resourceId, int resourceType);
							static GameContainerARK2DResource getARK2DResourceWithLength(int resourceId, int resourceType);

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
				}
			#endif

	#endif

#endif /* GAMECONTAINERWINDOWSPHONE8_H_ */


