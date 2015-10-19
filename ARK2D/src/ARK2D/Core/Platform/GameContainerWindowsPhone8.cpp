/*
 * GameContainerWindows.cpp
 *
 *  Created on: 27 Feb 2011
 *      Author: ashley
 */ 
  
#include "GameContainerWindowsPhone8.h"
#include "../GameContainer.h" 
#include "../../ARK2D.h"

#include "../../Namespaces.h"
#include "../../Includes.h"

#include "../../Geometry/Shape.h"
#include "../../Geometry/Circle.h"
#include "../../Geometry/Line.h"
#include "../../Geometry/Rectangle.h"

 
#if defined(ARK2D_WINDOWS_PHONE_8)

	#include "../../Geometry/GigaRectangle.h"

	#include "../../Windres.h"
	#include "../../Graphics/Image.h" 
	#include "../../Util/Log.h"

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
				m_touchMode(true),
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

				m_input.setGameContainer(this);

				//m_platformSpecific.m_hInstance = GetModuleHandle(NULL);

				ARK2D::s_container = this;
				ARK2D::s_game = &m_game;
				ARK2D::s_graphics = &m_graphics;
				ARK2D::s_input = &m_input;
				ARK2D::s_log = ARK::Util::Log::getInstance();
				scene = new Scene();
				scene->addChild(ARK2D::s_game);
				scene->addChild(ARK2D::s_log);



				ARK2D::getRenderer()->preinit();
			}


			void GameContainer::setSize(int width, int height) {
				
				if (width == (signed int) m_width && height == (signed int) m_height) { return; }

				m_screenWidth = width;
				m_screenHeight = height;  

				resizeBehaviour(width, height);

				resizeWindowToFitViewport();
			}

			void GameContainer::setFullscreen(bool fullscreen) {
				this->m_fullscreen = fullscreen;
			}


			void GameContainer::setIcon(const std::string& path) {
				//m_platformSpecific.m_iconpath = path;
			}

			void GameContainer::initGamepads() {

			}

			void GameContainer::setCursorVisible(bool b) {
				//ShowCursor(b);
			}

			void GameContainer::resizeWindowToFitViewport() {
				ARK2D::getRenderer()->setScissorTestEnabled(false);
			}
			// Enable OpenGL
		//	void GameContainerPlatform::enableOpenGL(HWND hWnd, HDC* hDC, HGLRC* hRC)
		//	{
		//		
		//
		//	}

			void GameContainer::processGamepadInput() {

			}

			void GameContainer::start() {

				
				// Seed the random
				MathUtil::seedRandom();


				// populate the gamepads.
				ARK2D::getLog()->i("Initialising Gamepads... ");
				initGamepads(); 
				ARK2D::getLog()->i("done.");

				// Initialise platform...
				//m_platformSpecific.initialize();

				// Enable OpenGL
				ARK2D::getLog()->i("Initialising OpenGL... ");
				ARK2D::getRenderer()->init();
				//gl2dxInit(m_platformSpecific.m_device, m_platformSpecific.m_deviceContext);

				//m_platformSpecific.enableOpenGL(m_platformSpecific.m_hWindow, &m_platformSpecific.m_hDeviceContext, &m_platformSpecific.m_hRenderingContext);
				this->enable2D();
				ARK2D::getLog()->i("done.");

				// Load default Font - relies on Image so must be done after OpenGL is initted.
				//BMFont* fnt = new BMFont("data/fonts/default.fnt", "data/fonts/default.png");
				//Image* fntImg = new Image((unsigned int) ARK2D_FONT_PNG, ARK2D_RESOURCE_TYPE_PNG);
				if (m_willLoadDefaultFont) { 
					//ARK::Font::BMFont* fnt = new ARK::Font::BMFont(ARK2D_FONT_FNT, ARK2D_FONT_PNG, ARK2D_RESOURCE_TYPE_PNG);
					ARK::Font::BMFont* fnt = Resource::get("ark2d/fonts/default.fnt")->asFont()->asBMFont();
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



				ARK2D::getLog()->i("Initialising Window... ");
				//ShowWindow(m_platformSpecific.m_hWindow, SW_SHOWNORMAL);
				//UpdateWindow(m_platformSpecific.m_hWindow);
				//ClipCursor(&m_windowRect);
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

				//Image::showAnyGlErrorAndExit();

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
				// The first argument instructs DXGI to block until VSync, putting the application
				// to sleep until the next VSync. This ensures we don't waste any cycles rendering
				// frames that will never be displayed to the screen.
				HRESULT hr = m_swapChain->Present(1, 0);

				// Discard the contents of the render target.
				// This is a valid operation only when the existing contents will be entirely
				// overwritten. If dirty or scroll rects are used, this call should be removed.
				
				//m_deviceContext->DiscardView(m_renderTargetView);

				// Discard the contents of the depth stencil.
				
				//m_deviceContext->DiscardView(m_depthStencilView);

				// If the device was removed either by a disconnect or a driver upgrade, we 
				// must recreate all device resources.
				if (hr == DXGI_ERROR_DEVICE_REMOVED)
				{
					handleDeviceLost();
				}
				else
				{
					DX::ThrowIfFailed(hr);
				}

			}

			void GameContainer::swapBuffers() {
				//SwapBuffers(m_platformSpecific.m_hDeviceContext);
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

			void GameContainerPlatform::initialize(
				Windows::UI::Core::CoreWindow^ window,
				Windows::ApplicationModel::Core::IFrameworkView^ nativeGame) 
			{
				m_window = window;
				m_nativeGame = m_nativeGame;

				createDeviceResources();
				createWindowSizeDependentResources();

				//gl2dxInit(*(&device), *(&context));
			}
			// These are the resources that depend on the device.
			void GameContainerPlatform::createDeviceResources()
			{
				// This flag adds support for surfaces with a different color channel ordering
				// than the API default. It is required for compatibility with Direct2D.
				UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

			#if defined(_DEBUG) || defined(ARK2D_DEBUG)
				// If the project is in a debug build, enable debugging via SDK Layers with this flag.
				creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
			#endif

				// This array defines the set of DirectX hardware feature levels this app will support.
				// Note the ordering should be preserved.
				// Don't forget to declare your application's minimum required feature level in its
				// description.  All applications are assumed to support 9.1 unless otherwise stated.
				D3D_FEATURE_LEVEL featureLevels[] = 
				{
					//D3D_FEATURE_LEVEL_11_1,
					//D3D_FEATURE_LEVEL_11_0,
					//D3D_FEATURE_LEVEL_10_1,
					//D3D_FEATURE_LEVEL_10_0,
					D3D_FEATURE_LEVEL_9_3
				};

				// Create the Direct3D 11 API device object and a corresponding context.
				//ComPtr<ID3D11Device> device;
				//ComPtr<ID3D11DeviceContext> context;
				DX::ThrowIfFailed(
					D3D11CreateDevice(
						nullptr, // Specify nullptr to use the default adapter.
						D3D_DRIVER_TYPE_HARDWARE,
						nullptr,
						creationFlags, // Set set debug and Direct2D compatibility flags.
						featureLevels, // List of feature levels this app can support.
						ARRAYSIZE(featureLevels),
						D3D11_SDK_VERSION, // Always set this to D3D11_SDK_VERSION.
						&m_device, // Returns the Direct3D device created.
						&m_featureLevel, // Returns feature level of device created.
						&m_deviceContext // Returns the device immediate context.
						)
					);

				// Get the Direct3D 11.1 API device and context interfaces.

				//ComPtr<ID3D11Device1> d3dDevice;
				//ComPtr<ID3D11DeviceContext1> d3dContext;

				/*DX::ThrowIfFailed(
					device.As(&m_device)
					);

				DX::ThrowIfFailed(
					context.As(&m_deviceContext)
					);*/

				//m_d3dDevice = d3dDevice.Get();  
				//m_d3dContext = d3dContext.Get();

				
			}

			void GameContainerPlatform::createWindowSizeDependentResources() {
				m_windowBounds = m_window->Bounds;

				// Calculate the necessary swap chain and render target size in pixels.
				m_renderTargetSize.Width = convertDipsToPixels(m_windowBounds.Width);
				m_renderTargetSize.Height = convertDipsToPixels(m_windowBounds.Height);

				DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {0};
				swapChainDesc.Width = static_cast<UINT>(m_renderTargetSize.Width); // Match the size of the window.
				swapChainDesc.Height = static_cast<UINT>(m_renderTargetSize.Height);
				swapChainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM; // This is the most common swap chain format.
				swapChainDesc.Stereo = false;
				swapChainDesc.SampleDesc.Count = 1; // Don't use multi-sampling.
				swapChainDesc.SampleDesc.Quality = 0;
				swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
				swapChainDesc.BufferCount = 1; // On phone, only single buffering is supported.
				swapChainDesc.Scaling = DXGI_SCALING_STRETCH; // On phone, only stretch and aspect-ratio stretch scaling are allowed.
				swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD; // On phone, no swap effects are supported.
				swapChainDesc.Flags = 0;

				IDXGIDevice1* dxgiDevice = NULL;
				HRESULT hr = m_device->QueryInterface(__uuidof(IDXGIDevice1), (void**)&dxgiDevice);
				if (FAILED(hr)) {
					ARK2D::getLog()->e("Could not QueryInterface for IDXGIDevice1...");
					exit(0);
				}
				
				IDXGIAdapter* dxgiAdapter;
				DX::ThrowIfFailed(
					dxgiDevice->GetAdapter(&dxgiAdapter)
					);

				ComPtr<IDXGIFactory2> dxgiFactory;
				DX::ThrowIfFailed(
					dxgiAdapter->GetParent(
						__uuidof(IDXGIFactory2), 
						&dxgiFactory
						)
					);

				Windows::UI::Core::CoreWindow^ window = m_window.Get();
				DX::ThrowIfFailed(
					dxgiFactory->CreateSwapChainForCoreWindow(
						m_device,
						reinterpret_cast<IUnknown*>(window),
						&swapChainDesc,
						nullptr, // Allow on all displays.
						&m_swapChain
						)
					);
					
				// Ensure that DXGI does not queue more than one frame at a time. This both reduces latency and
				// ensures that the application will only render after each VSync, minimizing power consumption.
				DX::ThrowIfFailed(
					dxgiDevice->SetMaximumFrameLatency(1)
					);

				// Create a render target view of the swap chain back buffer.
				ComPtr<ID3D11Texture2D> backBuffer;
				DX::ThrowIfFailed(
					m_swapChain->GetBuffer(
						0,
						__uuidof(ID3D11Texture2D),
						&backBuffer
						)
					);

				DX::ThrowIfFailed(
					m_device->CreateRenderTargetView(
						backBuffer.Get(),
						nullptr,
						&m_renderTargetView
						)
					);

				// Create a depth stencil view.
				CD3D11_TEXTURE2D_DESC depthStencilDesc(
					DXGI_FORMAT_D24_UNORM_S8_UINT,
					static_cast<UINT>(m_renderTargetSize.Width),
					static_cast<UINT>(m_renderTargetSize.Height),
					1,
					1,
					D3D11_BIND_DEPTH_STENCIL
					);

				ComPtr<ID3D11Texture2D> depthStencil;
				DX::ThrowIfFailed(
					m_device->CreateTexture2D(
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
				m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 0); // 1);

				// Depth stencil view..?
				CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D);
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
				rasterDesc.AntialiasedLineEnable = true;
				rasterDesc.CullMode = D3D11_CULL_NONE;
				rasterDesc.DepthBias = 0;
				rasterDesc.DepthBiasClamp = false;
				rasterDesc.FillMode = D3D11_FILL_SOLID;
				rasterDesc.FrontCounterClockwise = false;
				rasterDesc.ScissorEnable = true;
				rasterDesc.SlopeScaledDepthBias = 0.0f;

				DX::ThrowIfFailed(
					m_device->CreateRasterizerState(
						&rasterDesc, 
						&m_rasterStateSolid
					)
				);

				// render things that are backfacing.
				//D3D11_RASTERIZER_DESC rasterDesc;
				rasterDesc.AntialiasedLineEnable = true;
				rasterDesc.CullMode = D3D11_CULL_NONE;
				rasterDesc.DepthBias = 0;
				rasterDesc.DepthBiasClamp = false;
				rasterDesc.FillMode = D3D11_FILL_WIREFRAME; //D3D11_FILL_SOLID;
				rasterDesc.FrontCounterClockwise = false;
				rasterDesc.ScissorEnable = true;
				rasterDesc.SlopeScaledDepthBias = 0.0f;

				DX::ThrowIfFailed(
					m_device->CreateRasterizerState(
					&rasterDesc,
					&m_rasterStateWireframe
					)
				);


				m_deviceContext->RSSetState(m_rasterStateSolid);

				// Set the rendering viewport to target the entire window.
				CD3D11_VIEWPORT viewport(
					0.0f,
					0.0f,
					//m_container->m_width,
					//m_container->m_height
					m_renderTargetSize.Width,
					m_renderTargetSize.Height
					);

				m_deviceContext->RSSetViewports(1, &viewport);

				
				//m_device->SetRenderState(D3DRS_LIGHTING, FALSE);    // turn off the 3D lighting
				//d3ddev->SetRenderState(D3DRS_ZENABLE, TRUE);
				//m_deviceContext->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);


				ID3D11RenderTargetView *const targets[1] = { m_renderTargetView };
				m_deviceContext->OMSetRenderTargets(1, targets, NULL);// m_depthStencilView);
			}

			void GameContainerPlatform::updateAndRender() {

				ARK2D::getLog()->w("Platform::updateAndRender"); 
				if (m_container->m_bRunning) {  
					ARK2D::getLog()->w("Still loading..."); 
					return; 
				}

				
				m_container->m_timer.tick();
				m_container->m_game.preUpdate(m_container, &m_container->m_timer);
				m_container->m_game.update(m_container, &m_container->m_timer);
				m_container->m_game.postUpdate(m_container, &m_container->m_timer);
				ARK2D::getInput()->clearKeyPressedRecord();

				//arklog->i("native render");
				//fillRect(100,100,10,10);
				RendererStats::reset();
				Renderer* r = ARK2D::getRenderer();
			//	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

				beginDXFrame();

				m_container->m_game.preRender(m_container, r);
				m_container->m_game.render(m_container, r);
				m_container->m_game.postRender(m_container, r);
				if (m_container->isShowingFPS()) { m_container->renderFPS(); }
				ARK2D::getLog()->render(m_container, r);

				ARK2D::getRenderer()->finish(); 
			}

			void GameContainerPlatform::beginDXFrame() {
				//const float midnightBlue[] = { 0.098f, 0.098f, 0.439f, 1.000f };
				const float midnightBlue[] = { m_container->m_clearColor.getRedf(), m_container->m_clearColor.getGreenf(), m_container->m_clearColor.getBluef(), m_container->m_clearColor.getAlphaf() };
				m_deviceContext->ClearRenderTargetView(
					m_renderTargetView,
					midnightBlue
				);
				

				m_deviceContext->ClearDepthStencilView(
					m_depthStencilView,
					D3D11_CLEAR_DEPTH,
					1.0f,
					0
				);
			}


			// This method is called in the event handler for the SizeChanged event.
			void GameContainerPlatform::updateForWindowSizeChange()
			{
				//std::cout << "Window. Width: " << m_window->Bounds.Width << ". Height: " << m_window->Bounds.Height << ".";

				//MessageBox.Show("lol");
				if (m_window->Bounds.Width  != m_windowBounds.Width ||
					m_window->Bounds.Height != m_windowBounds.Height)
				{
					ID3D11RenderTargetView* nullViews[] = {nullptr};
					m_deviceContext->OMSetRenderTargets(ARRAYSIZE(nullViews), nullViews, nullptr);
					m_renderTargetView = nullptr;
					m_depthStencilView = nullptr;
					m_deviceContext->Flush();
					createWindowSizeDependentResources();
				}
			}


			// Recreate all device resources and set them back to the current state.
			void GameContainerPlatform::handleDeviceLost()
			{
				// Reset these member variables to ensure that UpdateForWindowSizeChange recreates all resources.
				m_windowBounds.Width = 0;
				m_windowBounds.Height = 0;
				m_swapChain = nullptr;

				createDeviceResources();
				updateForWindowSizeChange();
			}

			void GameContainerPlatform::releaseResourcesForSuspending()
			{
				// Phone applications operate in a memory-constrained environment, so when entering
				// the background it is a good idea to free memory-intensive objects that will be
				// easy to restore upon reactivation. The swapchain and backbuffer are good candidates
				// here, as they consume a large amount of memory and can be reinitialized quickly.
				m_swapChain = nullptr;
				m_renderTargetView = nullptr;
				m_depthStencilView = nullptr;
			}



			// Method to convert a length in device-independent pixels (DIPs) to a length in physical pixels.
			float GameContainerPlatform::convertDipsToPixels(float dips)
			{
				static const float dipsPerInch = 96.0f;   
				#ifdef NTDDI_PHONE8
					return floor(dips * DisplayProperties::LogicalDpi / dipsPerInch + 0.5f); // Round to nearest integer.
				#elif defined(NTDDI_WINBLUE) 
					return floor(dips * DisplayInformation::GetForCurrentView()->LogicalDpi / dipsPerInch + 0.5f); // Round to nearest integer.
				#endif
				//return dips;
			}
 
			bool GameContainerPlatform::initOpenAL() {
				//IXAudio2* pXAudio2 = NULL;
				//HRESULT hr;
				//if ( FAILED(hr = XAudio2Create( &pXAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR ) ) )
    			//	return hr;
				//return hr; 
				return false;
			}
			bool GameContainerPlatform::deinitOpenAL() {
				return true;
			}

			void GameContainerPlatform::setTitle(std::string title) {
				//SetWindowTextA(m_hWindow, title.c_str());
			}

			string GameContainerPlatform::getResourcePath() const {
				//#ifdef ARK2D_WINDOWS_VS
				//	return "data/"; 
				//#else 
				return "./data/";
				//#endif
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

		}
	}

#endif
