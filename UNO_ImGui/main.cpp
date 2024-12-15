// Dear ImGui: standalone example application for DirectX 11

// Learn about Dear ImGui:
// - FAQ                  https://dearimgui.com/faq
// - Getting Started      https://dearimgui.com/getting-started
// - Documentation        https://dearimgui.com/docs (same as your local docs/ folder).
// - Introduction, links and more at the top of imgui.cpp

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include <d3d11.h>
#include <tchar.h>

// 引入自己的头文件
#include <cstdio>
#include <thread>
#include <mutex>
#include <chrono>
#include "map.h"
#include "client.h"
#include "music.h"
#include "def.h"

#define MUSIC 1

std::mutex mtx;

std::unordered_map<char, std::string> ToString = {
    {'0', "红"},
    {'1', "绿"},
    {'2', "蓝"},
    {'3', "黄"},
    {'4', "Black"},
    {'5', "Skip"},
    {'6', "AddTwo"},
    {'7', "Reverse"},
    {'8', "BlackC"},
    {'9', "Black4"},
    {'n', "Number"},
};
std::unordered_map<char, std::string> ToCard = {
    {'0', "红"},
    {'1', "绿"},
    {'2', "蓝"},
    {'3', "黄"},
    {'4', "黑"},
    {'5', "禁"},
    {'6', "加"},
    {'7', "转"},
    {'8', "调色盘"},
    {'9', "加4"},
};

// Data
int ret = -1;
static ID3D11Device*            g_pd3dDevice = nullptr;
static ID3D11DeviceContext*     g_pd3dDeviceContext = nullptr;
static IDXGISwapChain*          g_pSwapChain = nullptr;
static bool                     g_SwapChainOccluded = false;
static UINT                     g_ResizeWidth = 0, g_ResizeHeight = 0;
static ID3D11RenderTargetView*  g_mainRenderTargetView = nullptr;
// Color
ImVec4 redHovered = ImVec4(0.60f, 0.00f, 0.00f, 1.00f);
ImVec4 greenHovered = ImVec4(0.22f, 0.78f, 0.73f, 1.00f);
ImVec4 blueHovered = ImVec4(0.40f, 0.80f, 1.00f, 1.00f);
ImVec4 yellowHovered = ImVec4(1.00f, 1.00f, 0.00f, 1.00f);
ImVec4 blackHovered = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
ImVec4 redActive = ImVec4(0.60f, 0.00f, 0.00f, 0.40f);
ImVec4 greenActive = ImVec4(0.22f, 0.78f, 0.73f, 0.40f);
ImVec4 blueActive = ImVec4(0.40f, 0.80f, 1.00f, 0.40f);
ImVec4 yellowActive = ImVec4(1.00f, 1.00f, 0.00f, 0.40f);
ImVec4 blackActive = ImVec4(0.00f, 0.00f, 0.00f, 0.40f);
ImVec4 red = ImVec4(0.60f, 0.00f, 0.00f, 0.60f);
ImVec4 green = ImVec4(0.22f, 0.78f, 0.73f, 0.60f);
ImVec4 blue = ImVec4(0.40f, 0.80f, 1.00f, 0.60f);
ImVec4 yellow = ImVec4(1.00f, 1.00f, 0.00f, 0.60f);
ImVec4 black = ImVec4(0.00f, 0.00f, 0.00f, 0.60f);
// My Data
    enum Stage { Waitting, Ready, Racing };
    Stage stage = Stage::Waitting;
    static std::string information = "Welcome!\n";
    std::string p_name;         // 存放玩家昵称
    std::string my_str;         // 存放用户玩家信息
    std::string query_str = ""; // 存放询问服务端结果
    bool isStart = false;       // 判断游戏是否开始
    bool you_turn = false;      // 判断是否为自己的回合
    bool iterAdd = false;       // 判断能否迭代加加
// My Card
    std::string myCard;
    std::string p_msg;
// My bool
    bool showDemoWindow = false;
    bool CardWindow = true;
    bool textWindow = true;
    bool settingsWindow = true;
    bool debugWindow = true;
    bool talkingWindow = true;
    bool showPlayerWindow =false;
    bool showMouse = false;
    bool showYouTurn = false;
    float f = 0.0f;
    int max_player;

    std::string players;
    int selected_player = -1;
    const char* vec_players[4] = {"", "", "", ""};

    #if MUSIC == 1
    Music bgm;
    #endif
// Forward declarations of helper functions
bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void CreateRenderTarget();
void CleanupRenderTarget();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// My Function
void GetButton(std::string& s){
    for (int i = 0; i < s.size(); i += 3) {
        std::string tmp;
        if (s[i + 2] >= '0' && s[i + 2] <= '9') {
            tmp += ToCard[s[i]] + s[i + 2];
        } else {
            tmp += ToCard[s[i]] + ToCard[s[i + 1]];
        }
        
        if (s[i] == 'x') {
            continue;
        }
        if (i % 9 != 0) {
            ImGui::SameLine(0.0f, 10.0f);
        }
        if (s[i] == '0') {
            ImGui::PushStyleColor(ImGuiCol_Button, red);
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, redActive);
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, redHovered);
            if (ImGui::Button(tmp.c_str(), ImVec2(120, 35))) {
                ret = i / 3 + 1;
            }
            ImGui::PopStyleColor(3);
        } else if (s[i] == '1') {
            ImGui::PushStyleColor(ImGuiCol_Button, green);
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, greenActive);
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, greenHovered);
            if (ImGui::Button(tmp.c_str(), ImVec2(120, 35))) {
                ret = i / 3 + 1;
            }
            ImGui::PopStyleColor(3);
        } else if (s[i] == '2') {
            ImGui::PushStyleColor(ImGuiCol_Button, blue);
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, blueActive);
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, blueHovered);
            if (ImGui::Button(tmp.c_str(), ImVec2(120, 35))) {
                ret = i / 3 + 1;
            }
            ImGui::PopStyleColor(3);
        } else if (s[i] == '3') {
            ImGui::PushStyleColor(ImGuiCol_Button, yellow);
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, yellowActive);
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, yellowHovered);
            if (ImGui::Button(tmp.c_str(), ImVec2(120, 35))) {
                ret = i / 3 + 1;
            }
            ImGui::PopStyleColor(3);
        } else if (s[i] == '4') {
            ImGui::PushStyleColor(ImGuiCol_Button, black);
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, blackActive);
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, blackHovered);
            if (ImGui::Button(tmp.c_str(), ImVec2(120, 35))) {
                ret = i / 3 + 1;
            }
            ImGui::PopStyleColor(3);
        }
    }
}

void drawWindow(bool& drawDone, bool& close, ImVec4& clear_color, ImGuiIO& io){
    while (!drawDone)
    {
        // Poll and handle messages (inputs, window resize, etc.)
        // See the WndProc() function below for our to dispatch events to the Win32 backend.
        MSG msg;
        while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE)){
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT){
                close = true;
            }
        }
        if (close){
            break;
        }

        // Handle window being minimized or screen locked
        if (g_SwapChainOccluded && g_pSwapChain->Present(0, DXGI_PRESENT_TEST) == DXGI_STATUS_OCCLUDED)
        {
            ::Sleep(10);
            continue;
        }
        g_SwapChainOccluded = false;

        // Handle window resize (we don't resize directly in the WM_SIZE handler)
        if (g_ResizeWidth != 0 && g_ResizeHeight != 0)
        {
            CleanupRenderTarget();
            g_pSwapChain->ResizeBuffers(0, g_ResizeWidth, g_ResizeHeight, DXGI_FORMAT_UNKNOWN, 0);
            g_ResizeWidth = g_ResizeHeight = 0;
            CreateRenderTarget();
        }

        // Start the Dear ImGui frame
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        // -----------------------------------------------------------------------1
        // 绘制窗口
        
        if (showDemoWindow){
            ImGui::ShowDemoWindow(&showDemoWindow);
        }

        if (CardWindow) {
            ImGui::Begin(u8"UNO Gaming", &CardWindow, ImGuiWindowFlags_NoMove);
            ImGui::TextColored(ImColor(102, 204, 255, 255), u8"欢迎您！");
            ImGui::Checkbox(u8"更改设置", &settingsWindow);
            ImGui::Checkbox(u8"查看玩家", &showPlayerWindow);

            ImGui::SliderFloat(u8"无用的条", &f, 0.0f, 1.0f);


            ImGui::PushStyleColor(ImGuiCol_CheckMark, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
            ImGui::Checkbox(u8"到", &showYouTurn);
            ImGui::SameLine(0.0f, 10.0f);
            ImGui::Checkbox(u8"你", &showYouTurn);
            ImGui::SameLine(0.0f, 10.0f);
            ImGui::Checkbox(u8"了", &showYouTurn);
            ImGui::SameLine(0.0f, 10.0f);
            ImGui::Checkbox(u8"兵", &showYouTurn);
            ImGui::SameLine(0.0f, 10.0f);
            ImGui::Checkbox(u8"贵", &showYouTurn);
            ImGui::SameLine(0.0f, 10.0f);
            ImGui::Checkbox(u8"神", &showYouTurn);
            ImGui::SameLine(0.0f, 10.0f);
            ImGui::Checkbox(u8"速", &showYouTurn);
            ImGui::PopStyleColor();

            ImGui::PushStyleColor(ImGuiCol_Button, black);
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, blueActive);
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, blueHovered);
            if (ImGui::Button(u8"洛神", ImVec2(250, 35))) {
                ret = 0;
            }
            ImGui::PopStyleColor(3);
            
            GetButton(myCard);
        }
        if (debugWindow) {
            ImGui::Begin(u8"界面调试器", &debugWindow);
            ImGui::TextColored(ImColor(102, 204, 255, 255), u8"欢迎您！");
            ImGui::Checkbox(u8"打开调试器", &showDemoWindow);
            ImGui::Checkbox(u8"显示鼠标位置", &showMouse);

            ImGui::End();
        }
        if (showMouse) {
                // 获取当前鼠标位置
                ImVec2 mouse_pos = ImGui::GetMousePos();
                // 显示工具提示，但不与任何元素关联
                if (ImGui::BeginTooltip()) {
                    ImGui::SetNextWindowPos(mouse_pos);
                    ImGui::Text(u8"I am following you around.");
                    ImGui::EndTooltip();
                }
                if (ImGui::Begin(" ", nullptr, ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoInputs)) {
                    ImGui::End();
                }
        }
        
        if (talkingWindow) {
            ImGui::Begin(u8"聊天界面", &talkingWindow);
            ImGui::TextColored(ImColor(102, 204, 255, 255), u8"Fairy提醒您，请友善沟通交流！");
            static char inputBuffer[600];
            ImGui::InputText(u8"输入框", inputBuffer, IM_ARRAYSIZE(inputBuffer));
            ImGui::PushStyleColor(ImGuiCol_Button, blue);
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, blueActive);
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, blueHovered);
            if (ImGui::Button(u8"发送！")) {
                p_msg = inputBuffer;
                memset(inputBuffer, 0, sizeof(inputBuffer));
                to_call(information, p_name, p_msg);
            }
            ImGui::PopStyleColor(3);
            ImGui::End();
        }

        if (settingsWindow) {
            ImGui::Begin(u8"设置", &settingsWindow);
            ImGui::ColorEdit4(u8"自定义背景", (float*)&clear_color, ImGuiColorEditFlags_::ImGuiColorEditFlags_AlphaBar);
            if (ImGui::Button(u8"关闭设置")) {
                settingsWindow = false;
            }
            ImGui::End();
        }
        if (showPlayerWindow) {
            ImGui::Begin(u8"玩家列表", &showPlayerWindow);
            ImGui::TextColored(ImColor(102, 204, 255, 255), u8"玩家列表（别输的太难看bushi）");
            if (ImGui::ListBox(" ", &selected_player, vec_players, max_player, 4)) {}
            ImGui::TextColored(ImColor(102, 204, 255, 255), "Selected: %s", vec_players[selected_player]);
            if (ImGui::Button(u8"关闭列表")){
                showPlayerWindow = false;
            }
            ImGui::End();
        }

        if (textWindow) {
            ImGui::Begin(u8"文本窗口", &textWindow, ImGuiWindowFlags_NoMove);
            ImGui::BeginChild(u8"ScrollingRegion", ImVec2(0, -ImGui::GetItemsLineHeightWithSpacing()), false, ImGuiWindowFlags_HorizontalScrollbar);
            ImGui::TextUnformatted(information.c_str());
            ImGui::SetScrollHereY(1.0f);
            ImGui::EndChild();
            ImGui::End();
        }

        if (ret != -1) {
            myCard[(ret-1)*3] = 'x';
            f = 0.0f;
            if (!you_turn) {
                ret = -1;
            }
        }
        ImGui::End();
        // -----------------------------------------------------------------------2

        // Rendering
        ImGui::Render();
        const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
        g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, nullptr);
        g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        // Update and Render additional Platform Windows
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
        }

        // Present
        HRESULT hr = g_pSwapChain->Present(1, 0);   // Present with vsync
        //HRESULT hr = g_pSwapChain->Present(0, 0); // Present without vsync
        g_SwapChainOccluded = (hr == DXGI_STATUS_OCCLUDED);
    }
}

void update(bool& close){
    #if MUSIC == 1
    Music mic;
    #endif
    while (!close) {
        if (stage == Stage::Waitting) {
            // 询问游戏是否开始
            to_get_start(information, isStart);
            if (isStart) {
                stage = Stage::Ready;
                #if MUSIC == 1
                mic.music_start();
                #endif
                // 获取玩家信息
                to_get_players(information, players);
                int j = 0;
                // char* tmp = (char*)players.c_str();
                char* token = strtok((char*)players.c_str(), " ");
                while (token != nullptr) {
                    vec_players[j++] = token;
                    token = strtok(nullptr, " ");
                }
                max_player = j;
                // 获取第一张牌
                to_get_first(information);
            }
        }
        if (isStart) {
            std::lock_guard<std::mutex> lock(mtx);
            // 询问并更新是否为自己的回合
            to_query_turn(information, my_str, you_turn);
            showYouTurn = you_turn;
            // 获取人机的输出状态
            to_computer_update(information, my_str);
            if (you_turn) {
                to_query_add(information, query_str, you_turn, iterAdd);
                if (!query_str.empty()) {
                    if (iterAdd) {
                        to_show_card(information, myCard);
                        #if MUSIC == 1
                        mic.music_add2();
                        #endif
                        while (ret == -1) {}
                    } else {
                        #if MUSIC == 1
                        mic.music_beadd();
                        #endif
                    }
                    to_add(information, myCard, iterAdd, ret);
                } else {
                    to_show_card(information, myCard);
                    while (ret == -1) {}
                    #if MUSIC == 1
                    mic.music_play();
                    #endif
                    to_updata(information, myCard, ret);
                }
                // 获取玩家信息
                to_get_players(information, players);
                int j = 0;
                // char* tmp = (char*)players.c_str();
                char* token = strtok((char*)players.c_str(), " ");
                while (token != nullptr) {
                    vec_players[j++] = token;
                    token = strtok(nullptr, " ");
                }
                query_str.clear();                 // 归零
                you_turn = false;                  // 不是自己的回合
            }
        }
        // std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

// Main code
int main(int, char**) {
    system("chcp 65001");

    // Create application window
    //ImGui_ImplWin32_EnableDpiAwareness();
    WNDCLASSEXW wc = { sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, L"ImGui Example", nullptr };
    ::RegisterClassExW(&wc);
    HWND hwnd = ::CreateWindowW(wc.lpszClassName, L"Dear ImGui DirectX11 UNO Gaming", WS_OVERLAPPEDWINDOW, 100, 100, 1280, 720, nullptr, nullptr, wc.hInstance, nullptr);

    // Initialize Direct3D
    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    // Show the window
    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;
    //io.ConfigViewportsNoDefaultParent = true;
    //io.ConfigDockingAlwaysTabBar = true;
    //io.ConfigDockingTransparentPayload = true;
    //io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleFonts;     // FIXME-DPI: Experimental. THIS CURRENTLY DOESN'T WORK AS EXPECTED. DON'T USE IN USER APP!
    //io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleViewports; // FIXME-DPI: Experimental.

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();
    //ImGui::StyleColorsClassic();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

    // Fonts
    ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\simsun.ttc", 18.0f, nullptr, io.Fonts->GetGlyphRangesChineseFull());
    IM_ASSERT(font != nullptr);

    // Our state
    ImVec4 clear_color = ImVec4(0.40f, 0.80f, 1.00f, 1.00f);

    // 第一次握手
    to_hello(information);
    // 登入服务端
    login_to_server(information, p_name, my_str);
    // 入场音乐 :)
    #if MUSIC == 1
    bgm.music_qiu();
    #endif

    // Main loop
    bool drawDone = false;
    bool close = false;
    while (!close) {
        std::thread updating(&update, std::ref(close));
        drawWindow(std::ref(drawDone), std::ref(close), std::ref(clear_color), std::ref(io));
        updating.join();
    }
    // --------------------------------------------------------------------------------------0

    
    // Cleanup
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClassW(wc.lpszClassName, wc.hInstance);

    return 0;
}

// Helper functions
bool CreateDeviceD3D(HWND hWnd)
{
    // Setup swap chain
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
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
    //createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
    HRESULT res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (res == DXGI_ERROR_UNSUPPORTED) // Try high-performance WARP software driver if hardware is not available.
        res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (res != S_OK)
        return false;

    CreateRenderTarget();
    return true;
}

void CleanupDeviceD3D()
{
    CleanupRenderTarget();
    if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = nullptr; }
    if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = nullptr; }
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = nullptr; }
}

void CreateRenderTarget()
{
    ID3D11Texture2D* pBackBuffer;
    g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_mainRenderTargetView);
    pBackBuffer->Release();
}

void CleanupRenderTarget()
{
    if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = nullptr; }
}

#ifndef WM_DPICHANGED
#define WM_DPICHANGED 0x02E0 // From Windows SDK 8.1+ headers
#endif

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Win32 message handler
// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (wParam == SIZE_MINIMIZED)
            return 0;
        g_ResizeWidth = (UINT)LOWORD(lParam); // Queue resize
        g_ResizeHeight = (UINT)HIWORD(lParam);
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    case WM_DPICHANGED:
        if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DpiEnableScaleViewports)
        {
            //const int dpi = HIWORD(wParam);
            //printf("WM_DPICHANGED to %d (%.0f%%)\n", dpi, (float)dpi / 96.0f * 100.0f);
            const RECT* suggested_rect = (RECT*)lParam;
            ::SetWindowPos(hWnd, nullptr, suggested_rect->left, suggested_rect->top, suggested_rect->right - suggested_rect->left, suggested_rect->bottom - suggested_rect->top, SWP_NOZORDER | SWP_NOACTIVATE);
        }
        break;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}
