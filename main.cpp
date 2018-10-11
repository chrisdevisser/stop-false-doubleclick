#include <chrono>
#include <optional>
#include <iostream>

#include <windows.h>

using namespace std::literals;

std::optional<std::chrono::duration<double, std::milli>> g_last_up_time;

auto CALLBACK hook_proc(int code, WPARAM msg, LPARAM data) -> LRESULT {
    const auto& mouse_info = *reinterpret_cast<MSLLHOOKSTRUCT*>(data);
    auto default_return = [=] { return CallNextHookEx(nullptr, code, msg, data); };

    if (code < 0 || (msg != WM_LBUTTONDOWN && msg != WM_LBUTTONUP)) {
        return default_return();
    }

    auto time = std::chrono::milliseconds{mouse_info.time};

    if (msg == WM_LBUTTONUP) {
        g_last_up_time = time;
        return default_return();
    }

    if (not g_last_up_time) {
        return default_return();
    }

    std::cout << "Time: " << (time - *g_last_up_time).count() << " ms\n";
    if (time - *g_last_up_time <= 40ms) {
        std::cout << "Preventing double-click in " << (time - *g_last_up_time).count() << " ms\n";
        return 1;
    }

    return default_return();
}

int main() {
    auto hook = SetWindowsHookEx(WH_MOUSE_LL, hook_proc, GetModuleHandle(nullptr), 0);
    if (not hook) {
        MessageBox(nullptr, L"Couldn't start doubleclick fixer.", nullptr, MB_OK);
        return EXIT_FAILURE;
    }

    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    UnhookWindowsHookEx(hook);
}
