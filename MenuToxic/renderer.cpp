#include "includes.hpp"
#include "utils.hpp"
#include "Bytes.h"
#pragma comment (lib, "d3dx11.lib")
#pragma comment (lib, "d3d11.lib")

ImFont* LogoFont = nullptr;
ImFont* TabIconFont = nullptr;
ImFont* MainFont = nullptr;
static ID3D11Device           * g_pd3dDevice           = nullptr;
static ID3D11DeviceContext    * g_pd3dDeviceContext    = nullptr;
static IDXGISwapChain         * g_pSwapChain           = nullptr;
static ID3D11RenderTargetView * g_mainRenderTargetView = nullptr;

bool CreateDeviceD3D( HWND );
void CleanupDeviceD3D( );
void CreateRenderTarget( );
void CleanupRenderTarget( );
LRESULT __stdcall WndProc( HWND, UINT , WPARAM, LPARAM );


ImColor MenuColor = ImColor(40, 140, 40);

void Colors()
{
    ImGuiStyle& style = ImGui::GetStyle();
    ImGuiIO& io = ImGui::GetIO();

    style.Colors[ImGuiCol_ChildBg] = ImColor(39, 39, 47);
    style.WindowBorderSize = 1;
    style.Colors[ImGuiCol_Border] = ImColor(40, 140, 40);
    style.Colors[ImGuiCol_BorderShadow] = ImColor(0, 0, 0);
    style.Colors[ImGuiCol_WindowBg] = ImColor(28, 28, 28);
    style.WindowMinSize = ImVec2(750, 550);
    style.FramePadding = ImVec2(0, 0);
    style.ItemSpacing = ImVec2(0, 0);
    style.WindowPadding = ImVec2(0, 0);
    style.Colors[ImGuiCol_ResizeGrip] = ImColor(40, 140, 40, 0);
    style.Colors[ImGuiCol_ResizeGripActive] = ImColor(40, 140, 40, 0);
    style.Colors[ImGuiCol_ResizeGripHovered] = ImColor(40, 140, 40, 0);
    MainFont = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\verdana.ttf", 15);
    LogoFont = io.Fonts->AddFontFromMemoryTTF((void*)logofont, sizeof(logofont), 30);
    TabIconFont = io.Fonts->AddFontFromMemoryTTF((void*)tabfont, sizeof(tabfont), 30);
}

void DrawTopLine(ImColor col)
{
    ImGui::PushStyleColor(ImGuiCol_ChildBg, col.Value);
    ImGui::BeginChild("TopLine", ImVec2(ImGui::GetWindowSize().x, 5), true);
    ImGui::EndChild();
    ImGui::PopStyleColor();
}

int CurrentTab = 0;

bool banana = false;

const char* CheatName = "ToXic";

int StartRendering( ) 
{   
    const auto title = utils::random_string( );
    utils::spoof_exe( false );

    WNDCLASSEX wc = 
    { 
        sizeof( WNDCLASSEX ),
        CS_CLASSDC,
        WndProc, 
        0, 
        0, 
        0, 
        0, 
        0, 
        0, 
        0, 
        title.c_str(),
        0
    };

    RegisterClassExA( &wc );
    HWND hwnd = CreateWindowExA(0, wc.lpszClassName , title.c_str() , WS_SYSMENU , 100 , 100 , 1280 , 800 , 0 , 0 , wc.hInstance , 0 );

    if ( !CreateDeviceD3D( hwnd ) )
    {
        CleanupDeviceD3D( );
        UnregisterClassA( wc.lpszClassName , wc.hInstance );

        return 1;
    }

    ShowWindow( GetConsoleWindow( ) , SW_HIDE );
    ShowWindow( hwnd , SW_SHOWDEFAULT );
    UpdateWindow( hwnd );

    ImGui::CreateContext( );

    ImGuiIO& io = ImGui::GetIO( ); 

    io.IniFilename = nullptr;
    io.LogFilename = nullptr;

    io.WantSaveIniSettings = false;

    Colors();

    ImGui_ImplWin32_Init( hwnd );
    ImGui_ImplDX11_Init( g_pd3dDevice , g_pd3dDeviceContext );

    const auto clear_color = ImVec4( 0.45f , 0.55f , 0.60f , 1.00f );
    const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };

    while ( true )
    {
        MSG msg;

        while ( PeekMessageA( &msg , NULL , 0U , 0U , PM_REMOVE ) )
        {
            TranslateMessage( &msg );
            DispatchMessageA( &msg );

            if ( msg.message == WM_QUIT )
                break;
        }

        ImGui_ImplDX11_NewFrame( );
        ImGui_ImplWin32_NewFrame( );
        ImGui::NewFrame( );

        ImGui::Begin("Toxic", nullptr, ImGuiWindowFlags_NoTitleBar);
        {
            ImGui::PushFont(LogoFont);
            ImVec2 LogoTextSize = ImGui::CalcTextSize("ToXic");
            ImGui::SetCursorPos(ImVec2(10, 10));
            ImGui::BeginChild("LogoChild", ImVec2(LogoTextSize.x + 20, 50), true);
            {
                ImGui::SetCursorPos(ImVec2(10, 25 - (ImGui::CalcTextSize(CheatName).y) / 2));
                ImGui::TextColored(ImColor(255, 255, 255), CheatName);
            }
            ImGui::EndChild();
            ImGui::PopFont();

            ImGui::SetCursorPos(ImVec2(LogoTextSize.x + 40, 10));
            ImGui::BeginChild("TabsChild", ImVec2(ImGui::GetContentRegionAvail().x - 10, 50), true);
            {
                int TabX = ImGui::GetContentRegionAvail().x / 5;
                if (ImGui::Tab("LegitBot", "A", 0 == CurrentTab, TabX))
                    CurrentTab = 0;
                ImGui::SameLine();
                if (ImGui::Tab("ESP", "B", 1 == CurrentTab, TabX))
                    CurrentTab = 1;
                ImGui::SameLine();
                if (ImGui::Tab("Visuals", "C", 2 == CurrentTab, TabX))
                    CurrentTab = 2;
                ImGui::SameLine();
                if (ImGui::Tab("Misc", "D", 3 == CurrentTab, TabX))
                    CurrentTab = 3;
                ImGui::SameLine();
                if (ImGui::Tab("Changer", "E", 4 == CurrentTab, TabX))
                    CurrentTab = 4;
                ImGui::SameLine();
                if (ImGui::Tab("Config", "F", 5 == CurrentTab, TabX))
                    CurrentTab = 5;
            }
            ImGui::EndChild();

            ImGui::SetCursorPos(ImVec2(10, 70));
            ImGui::BeginChild("mf", ImVec2(ImGui::GetContentRegionAvail().x - 10, ImGui::GetContentRegionAvail().y - 10), true);
            {
                ImGui::SetCursorPos(ImVec2(20, 20));
                ImGui::BeginFeaturesChild("Sla", true, ImVec2((ImGui::GetWindowSize().x / 2) - 10, ImGui::GetWindowSize().y - 60));
                {
                    ImGui::Checkbox("Bananna", &banana);
                }
                ImGui::EndChild();
            }
            ImGui::EndChild();
        }
        ImGui::End();

        ImGui::Render( );
       
        g_pd3dDeviceContext->OMSetRenderTargets( 1 , &g_mainRenderTargetView , NULL );
        g_pd3dDeviceContext->ClearRenderTargetView( g_mainRenderTargetView , clear_color_with_alpha );
       
        ImGui_ImplDX11_RenderDrawData( ImGui::GetDrawData( ) );

        g_pSwapChain->Present(0, 0); 
    }

    ImGui_ImplDX11_Shutdown( );
    ImGui_ImplWin32_Shutdown( );
    ImGui::DestroyContext( );

    CleanupDeviceD3D( );
    DestroyWindow( hwnd );
    UnregisterClassA( wc.lpszClassName , wc.hInstance );

    return 0;
}


bool CreateDeviceD3D( HWND hWnd )
{
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory( &sd , sizeof( sd ) );
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;

    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };

    if ( D3D11CreateDeviceAndSwapChain( NULL , D3D_DRIVER_TYPE_HARDWARE , NULL , createDeviceFlags , featureLevelArray , 2 , D3D11_SDK_VERSION , &sd , &g_pSwapChain , &g_pd3dDevice , &featureLevel , &g_pd3dDeviceContext ) != S_OK )
        return false;

    CreateRenderTarget( );
    return true;
}

void CleanupDeviceD3D( )
{
    CleanupRenderTarget( );
    if ( g_pSwapChain ) { g_pSwapChain->Release( ); g_pSwapChain = NULL; }
    if ( g_pd3dDeviceContext ) { g_pd3dDeviceContext->Release( ); g_pd3dDeviceContext = NULL; }
    if ( g_pd3dDevice ) { g_pd3dDevice->Release( ); g_pd3dDevice = NULL; }
}

void CreateRenderTarget( )
{
    ID3D11Texture2D* pBackBuffer;
    g_pSwapChain->GetBuffer( 0 , IID_PPV_ARGS( &pBackBuffer ) );
    g_pd3dDevice->CreateRenderTargetView( pBackBuffer , NULL , &g_mainRenderTargetView );
    pBackBuffer->Release( );
}

void CleanupRenderTarget( )
{
    if ( g_mainRenderTargetView ) { g_mainRenderTargetView->Release( ); g_mainRenderTargetView = NULL; }
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler( HWND hWnd , UINT msg , WPARAM wParam , LPARAM lParam );

LRESULT WINAPI WndProc( HWND hWnd , UINT msg , WPARAM wParam , LPARAM lParam )
{
    if ( ImGui_ImplWin32_WndProcHandler( hWnd , msg , wParam , lParam ) )
        return true;

    switch ( msg )
    {
    case WM_SIZE:
        if ( g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED )
        {
            CleanupRenderTarget( );
            g_pSwapChain->ResizeBuffers( 0 , (UINT)LOWORD( lParam ) , (UINT)HIWORD( lParam ) , DXGI_FORMAT_UNKNOWN , 0 );
            CreateRenderTarget( );
        }
        return 0;
    case WM_SYSCOMMAND:
        if ( ( wParam & 0xfff0 ) == SC_KEYMENU )
            return 0;
        break;
    case WM_DESTROY:
        PostQuitMessage( 0 );
        return 0;
    }
    return ::DefWindowProc( hWnd , msg , wParam , lParam );
}