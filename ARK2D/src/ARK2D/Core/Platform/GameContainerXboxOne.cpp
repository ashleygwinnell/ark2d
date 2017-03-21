/*
 * GameContainerWindows.cpp
 *
 *  Created on: 27 Feb 2011
 *      Author: ashley
 */

#include "GameContainerXboxOne.h"
#include "../GameContainer.h"
#include "../ARK2D.h"
#include "../../Common/Audio.h"
#include "../../Audio/Sound.h"

#include "../../Namespaces.h"

#include "../Geometry/Shape.h"
#include "../Geometry/Circle.h"
#include "../Geometry/Line.h"
#include "../Geometry/Rectangle.h"


#if (defined(ARK2D_XBOXONE) || defined(ARK2D_WINDOWS_STORE))

	#include "../Camera.h"
	#include "../Math/Random.h"

	#include "../../Windres.h"
	#include "../Graphics/Image.h"
	#include "../Log.h"

	namespace ARK {
		namespace Core {

			GameContainer::GameContainer(Game& g, int width, int height, int bpp, bool fullscreen):
				m_timer(),
				m_game(g),
				m_input(),
				m_graphics(),
				m_gamepads(),
				scene(NULL),
				m_originalWidth(width),
				m_originalHeight(height),
				m_width(width),
				m_height(height),
				m_screenWidth(0),
				m_screenHeight(0),
				m_scaleLock(false),
				m_scale(1.0f),
				m_scaleX(1.0f),
				m_scaleY(1.0f),
				m_translateX(0),
				m_translateY(0),
				m_bpp(bpp),
				m_fullscreen(fullscreen),
				m_resizable(false),
				m_scaleToWindow(true),
				m_touchMode(false),
				m_screenOrientationPrevious(ORIENTATION_DUMMY),
				m_orientationInverted(false),
				m_2in1enabled(false),
				m_bRunning(false),
				m_clearColor(Color::black),
				m_resizeBehaviour(RESIZE_BEHAVIOUR_SCALE),
				m_showingFPS(false),
				m_willLoadDefaultFont(true),
				m_platformSpecific()
			{
				m_platformSpecific.m_container = this;
				m_platformSpecific.init();

				m_input.setGameContainer(this);

				ARK2D::s_container = this;
				ARK2D::s_game = &m_game;
				ARK2D::s_graphics = &m_graphics;
				ARK2D::s_input = &m_input;
				ARK2D::s_camera = new ARK::Core::Camera();
				ARK2D::s_camera->setViewport(0, 0, m_width, m_height);
				ARK2D::s_log = ARK::Core::Log::getInstance();
				scene = ARK2D::getScene();

				ARK2D::getRenderer()->preinit();
			}


			void GameContainer::setSize(int width, int height, bool docallback) {

				if (width == (signed int) m_width && height == (signed int) m_height) { return; }

				m_screenWidth = width;
				m_screenHeight = height;

				resizeBehaviour(width, height);
				resizeWindowToFitViewport();
			}

			void GameContainerPlatform::init() {
				m_d3dFeatureLevel = D3D_FEATURE_LEVEL_11_0;
				m_nativeOrientation = DisplayOrientations::None;
				m_currentOrientation = DisplayOrientations::None;
				m_dpi = -1.0f;
				m_effectiveDpi = -1.0f;

				CreateDeviceIndependentResources();
				CreateDeviceResources();
			}

			void GameContainer::setFullscreen(bool fullscreen) {
				this->m_fullscreen = fullscreen;
			}


			void GameContainer::setIcon(const std::string& path) {

			}

			void GameContainer::initGamepads() {

			}

			void GameContainer::setCursorVisible(bool b) {

			}

			void GameContainer::resizeWindowToFitViewport() {

			}

			void GameContainer::processGamepadInput() {
				//ARK2D::getLog()->e("GameContainer::processGamepadInput");
				for( unsigned int i = 0; i < m_gamepads.size(); ++i) {
					m_gamepads.at(i)->update();
				}
			}

			void GameContainer::start() {

				// Seed the random
				Random::init();

				// populate the gamepads.
				ARK2D::getLog()->i("Initialising Gamepads... ");
				initGamepads();
				ARK2D::getLog()->i("done.");

				// Enable OpenGL
				ARK2D::getLog()->i("Initialising OpenGL... ");
				ARK2D::getRenderer()->init();

				this->enable2D();
				ARK2D::getLog()->i("done.");

				// Load default Font - relies on Image so must be done after rendering is initted.
				if (m_willLoadDefaultFont) {
					ARK::Core::Font::BMFont* fnt = Resource::get("ark2d/fonts/default.fnt")->asFont()->asBMFont();
					fnt->scale(0.5f);
					m_graphics.m_DefaultFont = fnt;
					m_graphics.m_Font = fnt;
				} else {
					m_graphics.m_DefaultFont = NULL;
					m_graphics.m_Font = NULL;
				}

				// Enable OpenAL
				Sound::initialiseXAudio();
				ARK2D::getLog()->i("done.");

				ARK2D::getLog()->i("Initialising Log");
				ARK2D::s_log->init();

				ARK2D::getLog()->i("Initialising Localisations");
				initLocalisation();

				ARK2D::getLog()->i("Initialising ");
				ARK2D::getLog()->i(m_game.getTitle());
				ARK2D::getLog()->i("...");
				m_game.init(this);

				ARK2D::getLog()->i("Initialised ");
				ARK2D::getLog()->i(m_game.getTitle());
				ARK2D::getLog()->i("...");

				m_bRunning = true;
				return;
			}



			void GameContainer::close() const {
				ARK2D::getContainer()->m_bRunning = false;
				Windows::ApplicationModel::Core::CoreApplication::Exit();

				//Windows::Phone::UI::Input::BackPressedEventArgs^ args = ref new Windows::Phone::UI::Input::BackPressedEventArgs();
				//m_platformSpecific.m_nativeGame->OnBackButtonPressed(NULL, args);
			}

			void GameContainerPlatform::swapBuffers() {

				// The application may optionally specify "dirty" or "scroll"
				// rects to improve efficiency in certain scenarios.
				DXGI_PRESENT_PARAMETERS parameters = {0};
				// parameters.DirtyRectsCount = 0;
				// parameters.pDirtyRects = nullptr;
				// parameters.pScrollRect = nullptr;
				// parameters.pScrollOffset = nullptr;

				// The first argument instructs DXGI to block until VSync, putting the application
				// to sleep until the next VSync. This ensures we don't waste any cycles rendering
				// frames that will never be displayed to the screen.
				HRESULT hr;// = m_swapChain->Present(1, 0);

				// if(m_vsync_enabled)
				// {
					// Lock to screen refresh rate.
					hr = m_swapChain->Present1(1, 0, &parameters);
					if (FAILED(hr)) {
						ErrorDialog::createAndShow(StringUtil::append("DirectX 11 SwapChain::Present failed: ", DX_GetError(hr)));
						exit(0);
					}
				// }
				// else
				// {
				// 	// Present as fast as possible.
				// 	hr = m_swapChain->Present1(0, 0, &parameters);
				// 	if (FAILED(hr)) {
				// 		ErrorDialog::createAndShow(StringUtil::append("DirectX 11 SwapChain::Present failed: ", DX_GetError(hr)));
				// 		exit(0);
				// 	}
				// }


				// Discard the contents of the render target.
				// This is a valid operation only when the existing contents will be entirely
				// overwritten. If dirty or scroll rects are used, this call should be removed.

				m_deviceContext->DiscardView1(m_renderTargetView.Get(), nullptr, 0);

				// Discard the contents of the depth stencil.
				m_deviceContext->DiscardView1(m_depthStencilView.Get(), nullptr, 0);

				// If the device was removed either by a disconnect or a driver upgrade, we
				// must recreate all device resources.
				if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
				{
					HandleDeviceLost();
				}
				else
				{
					DX::ThrowIfFailed(hr);
				}

			}

			void GameContainer::swapBuffers() {
				m_platformSpecific.swapBuffers();
			}

			int GameContainer::getGlobalMouseX() const {
				//DWORD mousepos = GetMessagePos();
				//POINTS mouseXY = MAKEPOINTS(mousepos);
				//return mouseXY.x;
				ARK2D::getLog()->w("getGlobalMouseX not implemented");
				return 0;
			}
			int GameContainer::getGlobalMouseY() const {
				//DWORD mousepos = GetMessagePos();
				//POINTS mouseXY = MAKEPOINTS(mousepos);
				//return mouseXY.y;
				ARK2D::getLog()->w("getGlobalMouseY not implemented");
				return 0;
			}

			// Disable OpenGL
			//void GameContainerPlatform::disableOpenGL(HWND hWnd, HDC hDC, HGLRC hRC)
			//{
				//wglMakeCurrent( NULL, NULL );
				//wglDeleteContext( hRC );
				//ReleaseDC( hWnd, hDC );
			//}

			void GameContainerPlatform::SetWindow(Windows::UI::Core::CoreWindow^ window)
			{
				DisplayInformation^ currentDisplayInformation = DisplayInformation::GetForCurrentView();

				m_window = window;
				m_swapChain = nullptr;
				m_logicalSize = Windows::Foundation::Size(window->Bounds.Width, window->Bounds.Height);
				m_nativeOrientation = currentDisplayInformation->NativeOrientation;
				m_currentOrientation = currentDisplayInformation->CurrentOrientation;
				m_dpi = currentDisplayInformation->LogicalDpi;
				m_d2dContext->SetDpi(m_dpi, m_dpi);

				CreateWindowSizeDependentResources();
			}


			// Configures resources that don't depend on the Direct3D device.
			void GameContainerPlatform::CreateDeviceIndependentResources()
			{
				ARK2D::getLog()->i("CreateDeviceIndependentResources");
				// Initialize Direct2D resources.
				D2D1_FACTORY_OPTIONS options;
				ZeroMemory(&options, sizeof(D2D1_FACTORY_OPTIONS));

				#if defined(_DEBUG)
					// If the project is in a debug build, enable Direct2D debugging via SDK Layers.
					options.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
				#endif

				// Initialize the Direct2D Factory.
				DX::ThrowIfFailed(
					D2D1CreateFactory(
						D2D1_FACTORY_TYPE_SINGLE_THREADED,
						__uuidof(ID2D1Factory3),
						&options,
						&m_d2dFactory
						)
					);

				// Initialize the DirectWrite Factory.
				DX::ThrowIfFailed(
					DWriteCreateFactory(
						DWRITE_FACTORY_TYPE_SHARED,
						__uuidof(IDWriteFactory3),
						&m_dwriteFactory
						)
					);

				// Initialize the Windows Imaging Component (WIC) Factory.
				DX::ThrowIfFailed(
					CoCreateInstance(
						CLSID_WICImagingFactory2,
						nullptr,
						CLSCTX_INPROC_SERVER,
						IID_PPV_ARGS(&m_wicFactory)
						)
					);
			}

			// These are the resources that depend on the device.
			void GameContainerPlatform::CreateDeviceResources()
			{
				ARK2D::getLog()->i("CreateDeviceResources");// This flag adds support for surfaces with a different color channel ordering
				// than the API default. It is required for compatibility with Direct2D.
				UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

				#if defined(_DEBUG) || defined(ARK2D_DEBUG)
					if (DX::SdkLayersAvailable()) {
						ARK2D::getLog()->w("SdkLayersAvailable. Making debug DX Device.");
						// If the project is in a debug build, enable debugging via SDK Layers with this flag.
						creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
					}
				#endif

				// This array defines the set of DirectX hardware feature levels this app will support.
				// Note the ordering should be preserved.
				// Don't forget to declare your application's minimum required feature level in its
				// description.  All applications are assumed to support 9.1 unless otherwise stated.
				D3D_FEATURE_LEVEL featureLevels[] =
				{
					D3D_FEATURE_LEVEL_12_1,
					D3D_FEATURE_LEVEL_12_0,
					D3D_FEATURE_LEVEL_11_1,
					D3D_FEATURE_LEVEL_11_0,
					D3D_FEATURE_LEVEL_10_1,
					D3D_FEATURE_LEVEL_10_0,
					D3D_FEATURE_LEVEL_9_3
				};

				// Create the Direct3D 11 API device object and a corresponding context.
				ComPtr<ID3D11Device> device;
				ComPtr<ID3D11DeviceContext> context;
				HRESULT hr = D3D11CreateDevice(
					nullptr, // Specify nullptr to use the default adapter.
					D3D_DRIVER_TYPE_HARDWARE,
					0,
					creationFlags, // Set set debug and Direct2D compatibility flags.
					featureLevels, // List of feature levels this app can support.
					ARRAYSIZE(featureLevels),
					D3D11_SDK_VERSION, // Always set this to D3D11_SDK_VERSION.
					&device, // Returns the Direct3D device created.
					&m_d3dFeatureLevel, // Returns feature level of device created.
					&context // Returns the device immediate context.

				);
				if (FAILED(hr)) {
					ErrorDialog::createAndShow("ERROR Could not create DX 11 context.");
					//exit(0);
					DX::ThrowIfFailed(
						D3D11CreateDevice(
							nullptr,
							D3D_DRIVER_TYPE_WARP, // Create a WARP device instead of a hardware device.
							0,
							creationFlags,
							featureLevels,
							ARRAYSIZE(featureLevels),
							D3D11_SDK_VERSION,
							&device,
							&m_d3dFeatureLevel,
							&context
							)
						);
				}

				// Store pointers to the Direct3D 11.3 API device and immediate context.
				DX::ThrowIfFailed(
					device.As(&m_device)
					);

				DX::ThrowIfFailed(
					context.As(&m_deviceContext)
					);

				m_vsync_enabled = false;

				// Create the Direct2D device object and a corresponding context.
				ComPtr<IDXGIDevice3> dxgiDevice;
				DX::ThrowIfFailed(
					m_device.As(&dxgiDevice)
					);

				DX::ThrowIfFailed(
					m_d2dFactory->CreateDevice(dxgiDevice.Get(), &m_d2dDevice)
					);

				DX::ThrowIfFailed(
					m_d2dDevice->CreateDeviceContext(
						D2D1_DEVICE_CONTEXT_OPTIONS_NONE,
						&m_d2dContext
						)
					);


			}

			void GameContainerPlatform::UpdateRenderTargetSize() {
				ARK2D::getLog()->i("UpdateRenderTargetSize");

				m_effectiveDpi = m_dpi;

				// To improve battery life on high resolution devices, render to a smaller render target
				// and allow the GPU to scale the output when it is presented.
				if (!DisplayMetrics::SupportHighResolutions && m_dpi > DisplayMetrics::DpiThreshold)
				{
					float width = ConvertDipsToPixels(m_logicalSize.Width, m_dpi);
					float height = ConvertDipsToPixels(m_logicalSize.Height, m_dpi);

					// When the device is in portrait orientation, height > width. Compare the
					// larger dimension against the width threshold and the smaller dimension
					// against the height threshold.
					if (max(width, height) > DisplayMetrics::WidthThreshold && min(width, height) > DisplayMetrics::HeightThreshold)
					{
						// To scale the app we change the effective DPI. Logical size does not change.
						m_effectiveDpi /= 2.0f;
					}
				}

				// Calculate the necessary render target size in pixels.
				m_outputSize.Width = ConvertDipsToPixels(m_logicalSize.Width, m_effectiveDpi);
				m_outputSize.Height = ConvertDipsToPixels(m_logicalSize.Height, m_effectiveDpi);

				// Prevent zero size DirectX content from being created.
				m_outputSize.Width = max(m_outputSize.Width, 1.0f);
				m_outputSize.Height = max(m_outputSize.Height, 1.0f);

				//m_renderTargetSize.Width = convertDipsToPixels(m_windowBounds.Width);
				//m_renderTargetSize.Height = convertDipsToPixels(m_windowBounds.Height);
			}

			void GameContainerPlatform::CreateWindowSizeDependentResources() {
				ARK2D::getLog()->i("CreateWindowSizeDependentResources");

				// Clear the previous window size specific context.
				ID3D11RenderTargetView* nullViews[] = {nullptr};
				m_deviceContext->OMSetRenderTargets(ARRAYSIZE(nullViews), nullViews, nullptr);
				m_renderTargetView = nullptr;
				m_d2dContext->SetTarget(nullptr);
				m_d2dTargetBitmap = nullptr;
				m_depthStencilView = nullptr;
				m_deviceContext->Flush1(D3D11_CONTEXT_TYPE_ALL, nullptr);

				UpdateRenderTargetSize();

				// The width and height of the swap chain must be based on the window's
				// natively-oriented width and height. If the window is not in the native
				// orientation, the dimensions must be reversed.
				DXGI_MODE_ROTATION displayRotation = ComputeDisplayRotation();

				bool swapDimensions = displayRotation == DXGI_MODE_ROTATION_ROTATE90 || displayRotation == DXGI_MODE_ROTATION_ROTATE270;
				m_d3dRenderTargetSize.Width = swapDimensions ? m_outputSize.Height : m_outputSize.Width;
				m_d3dRenderTargetSize.Height = swapDimensions ? m_outputSize.Width : m_outputSize.Height;

				// Calculate the necessary swap chain and render target size in pixels.
				if (m_swapChain != nullptr) {
					// If the swap chain already exists, resize it.
					HRESULT hr = m_swapChain->ResizeBuffers(
						2, // Double-buffered swap chain.
						lround(m_d3dRenderTargetSize.Width),
						lround(m_d3dRenderTargetSize.Height),
						DXGI_FORMAT_B8G8R8A8_UNORM,
						0
						);

					if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET) {
						// If the device was removed for any reason, a new device and swap chain will need to be created.
						HandleDeviceLost();

						// Everything is set up now. Do not continue execution of this method. HandleDeviceLost will reenter this method
						// and correctly set up the new device.
						return;
					}
					else
					{
						DX::ThrowIfFailed(hr);
					}
				}
				else
				{
					// Otherwise, create a new one using the same adapter as the existing Direct3D device.
					DXGI_SCALING scaling = DisplayMetrics::SupportHighResolutions ? DXGI_SCALING_NONE : DXGI_SCALING_STRETCH;
					DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {0};

					swapChainDesc.Width = lround(m_d3dRenderTargetSize.Width);		// Match the size of the window.
					swapChainDesc.Height = lround(m_d3dRenderTargetSize.Height);
					swapChainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;				// This is the most common swap chain format.
					swapChainDesc.Stereo = false;
					swapChainDesc.SampleDesc.Count = 1;								// Don't use multi-sampling.
					swapChainDesc.SampleDesc.Quality = 0;
					swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
					swapChainDesc.BufferCount = 2;									// Use double-buffering to minimize latency.
					swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;	// All Windows Store apps must use this SwapEffect.
					swapChainDesc.Flags = 0;
					swapChainDesc.Scaling = scaling;
					swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_IGNORE;

					// This sequence obtains the DXGI factory that was used to create the Direct3D device above.
					ComPtr<IDXGIDevice3> dxgiDevice;
					DX::ThrowIfFailed(
						m_device.As(&dxgiDevice)
					);

					ComPtr<IDXGIAdapter> dxgiAdapter;
					DX::ThrowIfFailed(
						dxgiDevice->GetAdapter(&dxgiAdapter)
					);

					ComPtr<IDXGIFactory4> dxgiFactory;
					DX::ThrowIfFailed(
						dxgiAdapter->GetParent(IID_PPV_ARGS(&dxgiFactory))
					);

					ComPtr<IDXGISwapChain1> swapChain;
					DX::ThrowIfFailed(
						dxgiFactory->CreateSwapChainForCoreWindow(
							m_device.Get(),
							reinterpret_cast<IUnknown*>(m_window.Get()),
							&swapChainDesc,
							nullptr,
							&swapChain
							)
						);
					DX::ThrowIfFailed(
						swapChain.As(&m_swapChain)
						);

					// Ensure that DXGI does not queue more than one frame at a time. This both reduces latency and
					// ensures that the application will only render after each VSync, minimizing power consumption.
					DX::ThrowIfFailed(
						dxgiDevice->SetMaximumFrameLatency(1)
					);
				}


				// Create a render target view of the swap chain back buffer.
				ComPtr<ID3D11Texture2D1> backBuffer;
				DX::ThrowIfFailed(
					m_swapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer) )
				);

				DX::ThrowIfFailed(
					m_device->CreateRenderTargetView1(
						backBuffer.Get(),
						nullptr,
						&m_renderTargetView
						)
					);

				// Create a depth stencil view.
				CD3D11_TEXTURE2D_DESC1 depthStencilDesc(
					DXGI_FORMAT_D24_UNORM_S8_UINT,
					lround(m_d3dRenderTargetSize.Width),
					lround(m_d3dRenderTargetSize.Height),
					1,
					1,
					D3D11_BIND_DEPTH_STENCIL
					);

				ComPtr<ID3D11Texture2D1> depthStencil;
				DX::ThrowIfFailed(
					m_device->CreateTexture2D1(
						&depthStencilDesc,
						nullptr,
						&depthStencil
						)
					);



				D3D11_DEPTH_STENCIL_DESC actualDepthStencilDesc;
				ZeroMemory(&actualDepthStencilDesc, sizeof(actualDepthStencilDesc));

				// Set up the description of the stencil state.
				actualDepthStencilDesc.DepthEnable = false;
				actualDepthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
				actualDepthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

				actualDepthStencilDesc.StencilEnable = false;
				actualDepthStencilDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;// 0xFF;
				actualDepthStencilDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;// 0xFF;

				// Stencil operations if pixel is front-facing.
				actualDepthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
				actualDepthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;// D3D11_STENCIL_OP_INCR;
				actualDepthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;// D3D11_STENCIL_OP_KEEP;
				actualDepthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

				// Stencil operations if pixel is back-facing.
				actualDepthStencilDesc.BackFace = actualDepthStencilDesc.FrontFace;
				//actualDepthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
				//actualDepthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
				//actualDepthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
				//actualDepthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

				// Create the depth stencil state.
				DX::ThrowIfFailed(m_device->CreateDepthStencilState(&actualDepthStencilDesc, &m_depthStencilState));
				m_deviceContext->OMSetDepthStencilState(m_depthStencilState.Get(), 0); // 1);*/

				// Depth stencil view..?
				CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D);
				depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

				DX::ThrowIfFailed(
					m_device->CreateDepthStencilView(
						depthStencil.Get(),
						&depthStencilViewDesc,
						&m_depthStencilView
						)
					);

				// alpha blend state
				D3D11_BLEND_DESC blendStateDesc;
				ZeroMemory(&blendStateDesc, sizeof(D3D11_BLEND_DESC));
				blendStateDesc.AlphaToCoverageEnable = false;
				blendStateDesc.IndependentBlendEnable = false;
				blendStateDesc.RenderTarget[0].BlendEnable = TRUE;
				blendStateDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
				blendStateDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
				blendStateDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
				blendStateDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE; // D3D11_BLEND_SRC_ALPHA;// D3D11_BLEND_INV_DEST_ALPHA;//D3D11_BLEND_SRC_ALPHA;
				blendStateDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO; //D3D11_BLEND_DEST_ALPHA;// D3D11_BLEND_ONE;//D3D11_BLEND_DEST_ALPHA;
				blendStateDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;//D3D11_BLEND_OP_ADD;
				blendStateDesc.RenderTarget[0].RenderTargetWriteMask = 0x0f;//D3D11_COLOR_WRITE_ENABLE_ALL;

				HRESULT rs = m_device->CreateBlendState(&blendStateDesc, &m_blendState);
				if (FAILED(rs)) {
					ARK2D::getLog()->e("Failed To Create Blend State");
					exit(0);
				}
				float blendFactor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
				m_deviceContext->OMSetBlendState(m_blendState, blendFactor, 0xFFFFFFFF);

				// alpha blend state
				D3D11_BLEND_DESC blendStateDesc2;
				ZeroMemory(&blendStateDesc2, sizeof(D3D11_BLEND_DESC));
				blendStateDesc2.AlphaToCoverageEnable = false;
				blendStateDesc2.IndependentBlendEnable = false;
				blendStateDesc2.RenderTarget[0].BlendEnable = TRUE;
				blendStateDesc2.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
				blendStateDesc2.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
				blendStateDesc2.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
				blendStateDesc2.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE; // D3D11_BLEND_SRC_ALPHA;// D3D11_BLEND_INV_DEST_ALPHA;//D3D11_BLEND_SRC_ALPHA;
				blendStateDesc2.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO; //D3D11_BLEND_DEST_ALPHA;// D3D11_BLEND_ONE;//D3D11_BLEND_DEST_ALPHA;
				blendStateDesc2.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;//D3D11_BLEND_OP_ADD;
				blendStateDesc2.RenderTarget[0].RenderTargetWriteMask = 0x0f;//D3D11_COLOR_WRITE_ENABLE_ALL;

				HRESULT rs2 = m_device->CreateBlendState(&blendStateDesc2, &m_blendStateAdditive);
				if (FAILED(rs2)) {
					ARK2D::getLog()->e("Failed To Create (Additive) Blend State");
					exit(0);
				}


				// render things that are backfacing.
				D3D11_RASTERIZER_DESC rasterDesc;
				rasterDesc.FillMode = D3D11_FILL_SOLID;
				rasterDesc.CullMode = D3D11_CULL_NONE;
				rasterDesc.FrontCounterClockwise = false;
				rasterDesc.DepthBias = 0;
				rasterDesc.DepthBiasClamp = 0.0f;
				rasterDesc.SlopeScaledDepthBias = 0.0f;
				rasterDesc.DepthClipEnable = false;
				rasterDesc.ScissorEnable = false;
				rasterDesc.MultisampleEnable = false;
				rasterDesc.AntialiasedLineEnable = false;

				DX::ThrowIfFailed(
					m_device->CreateRasterizerState(
						&rasterDesc,
						&m_rasterStateSolid
					)
				);

				m_deviceContext->RSSetState(m_rasterStateSolid);

				// Set the rendering viewport to target the entire window.
				m_screenViewport = CD3D11_VIEWPORT(
					0.0f,
					0.0f,
					m_d3dRenderTargetSize.Width,
					m_d3dRenderTargetSize.Height
				);

				m_deviceContext->RSSetViewports(1, &m_screenViewport);

				// Create a Direct2D target bitmap associated with the
				// swap chain back buffer and set it as the current target.
				D2D1_BITMAP_PROPERTIES1 bitmapProperties =
					D2D1::BitmapProperties1(
						D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
						D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED),
						m_dpi,
						m_dpi
						);

				ComPtr<IDXGISurface2> dxgiBackBuffer;
				DX::ThrowIfFailed(
					m_swapChain->GetBuffer(0, IID_PPV_ARGS(&dxgiBackBuffer))
					);

				DX::ThrowIfFailed(
					m_d2dContext->CreateBitmapFromDxgiSurface(
						dxgiBackBuffer.Get(),
						&bitmapProperties,
						&m_d2dTargetBitmap
						)
					);

				m_d2dContext->SetTarget(m_d2dTargetBitmap.Get());
				m_d2dContext->SetDpi(m_effectiveDpi, m_effectiveDpi);

				// Grayscale text anti-aliasing is recommended for all Windows Store apps.
				m_d2dContext->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_GRAYSCALE);
			}

			void GameContainerPlatform::beginDXFrame() {

				// Reset the viewport to target the whole screen.
				auto viewport = m_screenViewport;
				m_deviceContext->RSSetViewports(1, &viewport);

				ID3D11RenderTargetView *const targets[1] = { m_renderTargetView.Get() };
				m_deviceContext->OMSetRenderTargets(1, targets, m_depthStencilView.Get());

				const float midnightBlue[] = { m_container->m_clearColor.getRedf(), m_container->m_clearColor.getGreenf(), m_container->m_clearColor.getBluef(), m_container->m_clearColor.getAlphaf() };
				m_deviceContext->ClearRenderTargetView( m_renderTargetView.Get(), midnightBlue );
				m_deviceContext->ClearDepthStencilView(
					m_depthStencilView.Get(),
					D3D11_CLEAR_DEPTH,// | D3D11_CLEAR_STENCIL,
					1.0f,
					0
				);
			}

			// Recreate all device resources and set them back to the current state.
			void GameContainerPlatform::HandleDeviceLost()
			{
				ARK2D::getLog()->i("HandleDeviceLost");
				// Reset these member variables to ensure that UpdateForWindowSizeChange recreates all resources.
				m_swapChain = nullptr;

				if (m_deviceNotify != nullptr)
				{
					m_deviceNotify->OnDeviceLost();
				}

				CreateDeviceResources();
				m_d2dContext->SetDpi(m_dpi, m_dpi);
				CreateWindowSizeDependentResources();

				if (m_deviceNotify != nullptr)
				{
					m_deviceNotify->OnDeviceRestored();
				}
			}

			void GameContainerPlatform::ReleaseResourcesForSuspending()
			{
				// Phone applications operate in a memory-constrained environment, so when entering
				// the background it is a good idea to free memory-intensive objects that will be
				// easy to restore upon reactivation. The swapchain and backbuffer are good candidates
				// here, as they consume a large amount of memory and can be reinitialized quickly.
				m_swapChain = nullptr;
				m_renderTargetView = nullptr;
				m_depthStencilView = nullptr;
			}





			bool GameContainerPlatform::initOpenAL() {
				return false;
			}
			bool GameContainerPlatform::deinitOpenAL() {
				return true;
			}

			void GameContainerPlatform::setTitle(std::string title) {
				//SetWindowTextA(m_hWindow, title.c_str());
			}

			string GameContainerPlatform::getResourcePath() const {
				return "./data/";
			}

			GameContainerARK2DResource GameContainerPlatform::getARK2DResourceWithLength(int resourceId, int resourceType) {
				ARK2D::getLog()->e("GameContainerPlatform::getARK2DResourceWithLength not implemented.");
				GameContainerARK2DResource retval;
				retval.data = NULL;
				retval.length = 0;
				return retval;
			}

			void* GameContainerPlatform::getARK2DResource(int resourceId, int resourceType) {
				GameContainerARK2DResource resource = getARK2DResourceWithLength(resourceId, resourceType);
				return resource.data;
			}


			// This method determines the rotation between the display device's native orientation and the
			// current display orientation.
			DXGI_MODE_ROTATION GameContainerPlatform::ComputeDisplayRotation()
			{
				DXGI_MODE_ROTATION rotation = DXGI_MODE_ROTATION_UNSPECIFIED;

				// Note: NativeOrientation can only be Landscape or Portrait even though
				// the DisplayOrientations enum has other values.
				switch (m_nativeOrientation)
				{
				case DisplayOrientations::Landscape:
					switch (m_currentOrientation)
					{
					case DisplayOrientations::Landscape:
						rotation = DXGI_MODE_ROTATION_IDENTITY;
						break;

					case DisplayOrientations::Portrait:
						rotation = DXGI_MODE_ROTATION_ROTATE270;
						break;

					case DisplayOrientations::LandscapeFlipped:
						rotation = DXGI_MODE_ROTATION_ROTATE180;
						break;

					case DisplayOrientations::PortraitFlipped:
						rotation = DXGI_MODE_ROTATION_ROTATE90;
						break;
					}
					break;

				case DisplayOrientations::Portrait:
					switch (m_currentOrientation)
					{
					case DisplayOrientations::Landscape:
						rotation = DXGI_MODE_ROTATION_ROTATE90;
						break;

					case DisplayOrientations::Portrait:
						rotation = DXGI_MODE_ROTATION_IDENTITY;
						break;

					case DisplayOrientations::LandscapeFlipped:
						rotation = DXGI_MODE_ROTATION_ROTATE270;
						break;

					case DisplayOrientations::PortraitFlipped:
						rotation = DXGI_MODE_ROTATION_ROTATE180;
						break;
					}
					break;
				}
				return rotation;
			}
			void GameContainerPlatform::ValidateDevice() {
				// The D3D Device is no longer valid if the default adapter changed since the device
				// was created or if the device has been removed.

				// First, get the information for the default adapter from when the device was created.

				ComPtr<IDXGIDevice3> dxgiDevice;
				DX::ThrowIfFailed(m_device.As(&dxgiDevice));

				ComPtr<IDXGIAdapter> deviceAdapter;
				DX::ThrowIfFailed(dxgiDevice->GetAdapter(&deviceAdapter));

				ComPtr<IDXGIFactory4> deviceFactory;
				DX::ThrowIfFailed(deviceAdapter->GetParent(IID_PPV_ARGS(&deviceFactory)));

				ComPtr<IDXGIAdapter1> previousDefaultAdapter;
				DX::ThrowIfFailed(deviceFactory->EnumAdapters1(0, &previousDefaultAdapter));

				DXGI_ADAPTER_DESC1 previousDesc;
				DX::ThrowIfFailed(previousDefaultAdapter->GetDesc1(&previousDesc));

				// Next, get the information for the current default adapter.

				ComPtr<IDXGIFactory4> currentFactory;
				DX::ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(&currentFactory)));

				ComPtr<IDXGIAdapter1> currentDefaultAdapter;
				DX::ThrowIfFailed(currentFactory->EnumAdapters1(0, &currentDefaultAdapter));

				DXGI_ADAPTER_DESC1 currentDesc;
				DX::ThrowIfFailed(currentDefaultAdapter->GetDesc1(&currentDesc));

				// If the adapter LUIDs don't match, or if the device reports that it has been removed,
				// a new D3D device must be created.

				if (previousDesc.AdapterLuid.LowPart != currentDesc.AdapterLuid.LowPart ||
					previousDesc.AdapterLuid.HighPart != currentDesc.AdapterLuid.HighPart ||
					FAILED(m_device->GetDeviceRemovedReason()))
				{
					// Release references to resources related to the old device.
					dxgiDevice = nullptr;
					deviceAdapter = nullptr;
					deviceFactory = nullptr;
					previousDefaultAdapter = nullptr;

					// Create a new device and swap chain.
					HandleDeviceLost();
				}
			}

			// This method is called in the event handler for the SizeChanged event.
			void GameContainerPlatform::SetLogicalSize(Windows::Foundation::Size logicalSize)
			{
				if (m_logicalSize != logicalSize)
				{
					ARK2D::getContainer()->setSize(logicalSize.Width, logicalSize.Height);

					m_logicalSize = logicalSize;
					CreateWindowSizeDependentResources();
				}
			}

			// This method is called in the event handler for the DpiChanged event.
			void GameContainerPlatform::SetDpi(float dpi)
			{
				if (dpi != m_dpi)
				{
					m_dpi = dpi;

					// When the display DPI changes, the logical size of the window (measured in Dips) also changes and needs to be updated.
					m_logicalSize = Windows::Foundation::Size(m_window->Bounds.Width, m_window->Bounds.Height);

					m_d2dContext->SetDpi(m_dpi, m_dpi);
					CreateWindowSizeDependentResources();
				}
			}

			// This method is called in the event handler for the OrientationChanged event.
			void GameContainerPlatform::SetCurrentOrientation(DisplayOrientations currentOrientation)
			{
				if (m_currentOrientation != currentOrientation)
				{
					m_currentOrientation = currentOrientation;
					CreateWindowSizeDependentResources();
				}
			}
			// Call this method when the app suspends. It provides a hint to the driver that the app
			// is entering an idle state and that temporary buffers can be reclaimed for use by other apps.
			void GameContainerPlatform::Trim()
			{
				ComPtr<IDXGIDevice3> dxgiDevice;
				m_device.As(&dxgiDevice);

				dxgiDevice->Trim();
			}
			void GameContainerPlatform::RegisterDeviceNotify(IDeviceNotify* deviceNotify) {
				m_deviceNotify = deviceNotify;
			}



		}
	}

#endif
