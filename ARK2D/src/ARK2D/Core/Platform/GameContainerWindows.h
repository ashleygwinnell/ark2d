/*
 * GameContainerWindows.h
 *
 *  Created on: 27 Feb 2011
 *      Author: ashley
 */


#ifndef GAMECONTAINERWINDOWS_H_
#define GAMECONTAINERWINDOWS_H_

#include "../../Includes.h"
#include "../../ARK2D.h"

	#ifdef ARK2D_WINDOWS

		#ifdef __GNUC__
			#define MAPVK_VK_TO_VSC 0
			#define MAPVK_VSC_TO_VK 1
			#define MAPVK_VK_TO_CHAR 2
			#define MAPVK_VSC_TO_VK_EX 3
		#endif

		

			#include "../../Windres.h"
			#include "../../Includes.h"
			#include "../../Graphics/Image.h"

 			#include <Winsock2.h>
			#include <windows.h>
			#include <string>
			#include <vector>

	 			// https://github.com/GameTechDev/Windows-Desktop-Sensors/blob/master/WindowsSensors.cpp
				//
				// Screen auto-rotation defines
				// We load the "SetDisplayAutoRotationPreferences" function with GetProcAddress so one
				// binary will run in both Windows 7 and Windows 8
				//
				//#if (WINVER <= 0x0601 /* _WIN32_WINNT_WIN7 */ )
				typedef enum MY_ORIENTATION_PREFERENCE {

				    MY_ORIENTATION_PREFERENCE_NONE              = 0x0,

					MY_ORIENTATION_PREFERENCE_LANDSCAPE = 0x1,

					MY_ORIENTATION_PREFERENCE_PORTRAIT = 0x2,

					MY_ORIENTATION_PREFERENCE_LANDSCAPE_FLIPPED = 0x4,

					MY_ORIENTATION_PREFERENCE_PORTRAIT_FLIPPED = 0x8

				} MY_ORIENTATION_PREFERENCE;
				typedef BOOL(WINAPI *pSDARP)(MY_ORIENTATION_PREFERENCE orientation);
				typedef BOOL(WINAPI *pSDARP2)(MY_ORIENTATION_PREFERENCE* orientation);
				typedef BOOL(WINAPI *pSDARP3)(AR_STATE* orientation);

				static pSDARP MySetDisplayAutoRotationPreferences = NULL;
				static pSDARP2 MyGetDisplayAutoRotationPreferences = NULL;
				static pSDARP3 MyGetAutoRotationState = NULL;
				//#endif // #if (WINVER <= 0x0601)

				typedef BOOL(WINAPI *pSDARP4)(HWND window, ULONG flags);
				static pSDARP4 MyRegisterTouchWindow = NULL;

				typedef BOOL (WINAPI* pGetTouchInputInfo)(HTOUCHINPUT, UINT, PTOUCHINPUT, int);
				static pGetTouchInputInfo MyGetTouchInputInfo = NULL;

				typedef bool (WINAPI* pCloseTouchInputHandle)(HTOUCHINPUT hTouchInput);
				static pCloseTouchInputHandle MyCloseTouchInputHandle = NULL;


			using namespace std;

			namespace ARK {
				namespace Core {
				class GameContainer;

					struct ARK2D_API GameContainerDisplayMode {
						unsigned int width;
						unsigned int height;
						unsigned int bpp;
					};

					struct ARK2D_API GameContainerARK2DResource {
						void* data;
						unsigned int length;
					};

					class ARK2D_API GameContainerPlatform {
							friend class GameContainer;
					public:
						GameContainer* m_container;
						HINSTANCE m_hInstance;
						WNDCLASSEX m_windowClass;
						RECT m_windowRect;
						RECT m_screenResolutionRect; // the size of the user's screen.
						HDC m_hDeviceContext;
						HWND m_hWindow;

						static const unsigned int TWOINONESTATE_UNKNOWN = 0; 
						static const unsigned int TWOINONESTATE_DESKTOP = 1; 
						static const unsigned int TWOINONESTATE_TABLET = 2; 
						unsigned int m_twoInOneState;
						void switchToTabletMode(bool doCallback);
						void switchToDesktopMode(bool doCallback);
						
						bool isAutoRotationAvailable();
						bool isAutoRotationEnabled();

						void enableAutoRotation();
						void disableAutoRotation(); // fixes to landscape
						void lockAutoRotation(); // stops it changing.

						unsigned int m_screenStartX; // windows 8 docking offsets.
						unsigned int m_screenStartY;
						void refreshScreenResolutionRect();
						
						#if defined(ARK2D_RENDERER_OPENGL)
							HGLRC m_hRenderingContext;
						#elif defined(ARK2D_RENDERER_DIRECTX)
							
							// Direct2D drawing components.
							ID2D1Factory1* 			m_d2dFactory;
							ID2D1Device* 			m_d2dDevice;
							ID2D1DeviceContext* 	m_d2dContext;
							ID2D1Bitmap1* 			m_d2dTargetBitmap;

							// DirectWrite drawing components.
							IDWriteFactory1*		m_dwriteFactory;
							//IWICImagingFactory*		m_wicFactory;

							ID2D1HwndRenderTarget* m_pRenderTarget;
							ID2D1SolidColorBrush* m_pLightSlateGrayBrush;
							ID2D1SolidColorBrush* m_pCornflowerBlueBrush;

							bool m_vsync_enabled;
							int m_videoCardMemory;
							char m_videoCardDescription[128];
							IDXGISwapChain* m_swapChain;
							ID3D11Device* m_device;
							ID3D11DeviceContext* m_deviceContext;
							ID3D11RenderTargetView* m_renderTargetView;
							ID3D11Texture2D* m_depthStencilBuffer;
							ID3D11DepthStencilState* m_depthStencilState;
							ID3D11DepthStencilView* m_depthStencilView; 
							ID3D11RasterizerState* m_rasterState;

							ID3D11DepthStencilState* m_depthDisabledStencilState;
							//DirectX::XMMATRIX m_projectionMatrix; // D3DXMATRIX
							///DirectX::XMMATRIX m_worldMatrix; // D3DXMATRIX
							//DirectX::XMMATRIX m_orthoMatrix; // D3DXMATRIX // XMFLOAT4X4

							string getD3DFeatureLevelString(D3D_FEATURE_LEVEL l) {
								if (l == 37120) {
									return "D3D_FEATURE_LEVEL_9_1";
								} else if (l == 37376) {
									return "D3D_FEATURE_LEVEL_9_2";
								} else if (l == 37632) {
									return "D3D_FEATURE_LEVEL_9_3";
								} else if (l == 40960) {
									return "D3D_FEATURE_LEVEL_10_0";
								} else if (l == 41216) {
									return "D3D_FEATURE_LEVEL_10_1";
								} else if (l == 45056) {
									return "D3D_FEATURE_LEVEL_11_0";
								} else if (l == 45312) {
									return "D3D_FEATURE_LEVEL_11_1";
								}
								return "D3D_FEATURE_LEVEL_UNKNOWN";
							}
						#endif


						float m_lastTime;

						DEVMODE m_dmScreenSettings; // device mode contains fullscreen, bitmode. something else...
						string m_iconpath;
						BOOL perf_flag;        // Timer Selection Flag
						double time_factor;    // Time Scaling Factor
						LONGLONG last_time;    // Previous timer value

						vector<GameContainerDisplayMode> m_availableDisplayModes;
						GameContainerDisplayMode* findDisplayMode(unsigned int w, unsigned int h, unsigned int bpp);

						LRESULT CALLBACK WndProc(HWND hWindow, UINT msg, WPARAM wParam, LPARAM lParam);
						
						#define MAXPOINTS 10
						int __GetContactIndex(int dwId);
						int idLookup[MAXPOINTS];

						void getWindowFlags(DWORD* windowflags_return, DWORD* exstyle_return, bool undecorated, bool child_window);
						int getWindowFlags2();

						int getSX();
						int getSY();
						HWND doCreateWindow();

						void setTitle(std::string title);
						string getResourcePath() const;

						void enableOpenGL(HWND hWnd, HDC* hDC, HGLRC* hRC);
						void disableOpenGL(HWND hWnd, HDC hDC, HGLRC hRC);

						HRESULT initDXDeviceIndependentResources();
						bool initDXDeviceResources();
						void deinitDXDeviceResources();
						void beginDXFrame();
						void endDXFrame();
						string getDXVideoCardInfo();
						void turnZBufferOnDX();
						void turnZBufferOffDX();

						bool initOpenAL();
						bool deinitOpenAL();

						void setFullscreen_ScreenMode(bool fullscreen);
						void setFullscreen_Fake(bool fullscreen);

						bool isRetina();

						bool mySetDisplayAutoRotationPreferences(MY_ORIENTATION_PREFERENCE pref);
						bool myGetDisplayAutoRotationPreferences(MY_ORIENTATION_PREFERENCE* pref);
						bool myGetAutoRotationState(AR_STATE* state);
						bool myRegisterTouchWindow(HWND wnd, ULONG flags);
						bool myGetTouchInputInfo(HTOUCHINPUT hTouchInput, UINT cInputs, PTOUCHINPUT pInputs, int cbSize);
						bool myCloseTouchInputHandle(HTOUCHINPUT hTouchInput);

						static void* getARK2DResource(int resourceId, int resourceType);
						static GameContainerARK2DResource getARK2DResourceWithLength(int resourceId, int resourceType);

					};

				}
			}

	#endif

#endif /* GAMECONTAINERWINDOWS_H_ */


