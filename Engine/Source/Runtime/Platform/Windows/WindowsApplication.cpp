#include "Application/Application.h"
#ifdef WI_PLATFORM_WIN
#include "WindowsApplication.h"
#include <windowsx.h>
#include "Application/Events/Event.h"
#include "Application/Events/WindowEvents.h"
#include "Application/Events/MouseEvents.h"
#include "Application/Events/KeyEvent.h"

namespace Wi
{
	static KeyCode TranslateKeyCode(WPARAM wParam, LPARAM lParam)
	{
		bool isExtended = ((HIWORD(lParam) & KF_EXTENDED) == KF_EXTENDED);
		int scancode = (HIWORD(lParam) & 0xFF);

		switch (wParam)
		{
			case VK_MENU:
				return isExtended ? KeyCode::RightAlt : KeyCode::LeftAlt;

			case VK_CONTROL:
				return isExtended ? KeyCode::RightControl : KeyCode::LeftControl;

			case VK_SHIFT:
			{
				int leftShiftScancode = MapVirtualKey(VK_LSHIFT, MAPVK_VK_TO_VSC);
				if (scancode == leftShiftScancode)
				{
					return KeyCode::LeftShift;
				}
				return KeyCode::RightShift;
			}

			case VK_LWIN:		return KeyCode::LeftSuper;
			case VK_RWIN:		return KeyCode::RightSuper;
			case VK_APPS:		return KeyCode::Menu;

			case VK_ESCAPE:     return KeyCode::Escape;
			case VK_RETURN:     return isExtended ? KeyCode::KPEnter : KeyCode::Enter;
			case VK_TAB:        return KeyCode::Tab;
			case VK_BACK:       return KeyCode::Backspace;
			case VK_INSERT:     return KeyCode::Insert;
			case VK_DELETE:     return KeyCode::Delete;
			case VK_HOME:       return KeyCode::Home;
			case VK_END:        return KeyCode::End;
			case VK_PRIOR:      return KeyCode::PageUp;
			case VK_NEXT:       return KeyCode::PageDown;
			case VK_UP:         return KeyCode::Up;
			case VK_DOWN:       return KeyCode::Down;
			case VK_LEFT:       return KeyCode::Left;
			case VK_RIGHT:      return KeyCode::Right;
			case VK_CAPITAL:    return KeyCode::CapsLock;
			case VK_SCROLL:     return KeyCode::ScrollLock;
			case VK_NUMLOCK:    return KeyCode::NumLock;
			case VK_SNAPSHOT:   return KeyCode::PrintScreen;
			case VK_PAUSE:      return KeyCode::Pause;
			case VK_SPACE:      return KeyCode::Space;
			case VK_OEM_1:      return KeyCode::Semicolon;
			case VK_OEM_2:      return KeyCode::Slash;
			case VK_OEM_3:      return KeyCode::GraveAccent;
			case VK_OEM_4:      return KeyCode::LeftBracket;
			case VK_OEM_5:      return KeyCode::Backslash;
			case VK_OEM_6:      return KeyCode::RightBracket;
			case VK_OEM_7:      return KeyCode::Apostrophe;
			case VK_OEM_MINUS:  return KeyCode::Minus;
			case VK_OEM_PLUS:   return KeyCode::Equal;
			case VK_OEM_COMMA:  return KeyCode::Comma;
			case VK_OEM_PERIOD: return KeyCode::Period;
			case '0':			return KeyCode::D0;
			case '1':			return KeyCode::D1;
			case '2':			return KeyCode::D2;
			case '3':			return KeyCode::D3;
			case '4':			return KeyCode::D4;
			case '5':			return KeyCode::D5;
			case '6':			return KeyCode::D6;
			case '7':			return KeyCode::D7;
			case '8':			return KeyCode::D8;
			case '9':			return KeyCode::D9;
			case 'A':			return KeyCode::A;
			case 'B':			return KeyCode::B;
			case 'C':			return KeyCode::C;
			case 'D':			return KeyCode::D;
			case 'E':			return KeyCode::E;
			case 'F':			return KeyCode::F;
			case 'G':			return KeyCode::G;
			case 'H':			return KeyCode::H;
			case 'I':			return KeyCode::I;
			case 'J':			return KeyCode::J;
			case 'K':			return KeyCode::K;
			case 'L':			return KeyCode::L;
			case 'M':			return KeyCode::M;
			case 'N':			return KeyCode::N;
			case 'O':			return KeyCode::O;
			case 'P':			return KeyCode::P;
			case 'Q':			return KeyCode::Q;
			case 'R':			return KeyCode::R;
			case 'S':			return KeyCode::S;
			case 'T':			return KeyCode::T;
			case 'U':			return KeyCode::U;
			case 'V':			return KeyCode::V;
			case 'W':			return KeyCode::W;
			case 'X':			return KeyCode::X;
			case 'Y':			return KeyCode::Y;
			case 'Z':			return KeyCode::Z;
			case VK_F1:			return KeyCode::F1;
			case VK_F2:			return KeyCode::F2;
			case VK_F3:			return KeyCode::F3;
			case VK_F4:			return KeyCode::F4;
			case VK_F5:			return KeyCode::F5;
			case VK_F6:			return KeyCode::F6;
			case VK_F7:			return KeyCode::F7;
			case VK_F8:			return KeyCode::F8;
			case VK_F9:			return KeyCode::F9;
			case VK_F10:		return KeyCode::F10;
			case VK_F11:		return KeyCode::F11;
			case VK_F12:		return KeyCode::F12;
			case VK_F13:		return KeyCode::F13;
			case VK_F14:		return KeyCode::F14;
			case VK_F15:		return KeyCode::F15;
			case VK_F16:		return KeyCode::F16;
			case VK_F17:		return KeyCode::F17;
			case VK_F18:		return KeyCode::F18;
			case VK_F19:		return KeyCode::F19;
			case VK_F20:		return KeyCode::F20;
			case VK_F21:		return KeyCode::F21;
			case VK_F22:		return KeyCode::F22;
			case VK_F23:		return KeyCode::F23;
			case VK_F24:		return KeyCode::F24;
			case VK_NUMPAD0:    return KeyCode::KP0;
			case VK_NUMPAD1:    return KeyCode::KP1;
			case VK_NUMPAD2:    return KeyCode::KP2;
			case VK_NUMPAD3:    return KeyCode::KP3;
			case VK_NUMPAD4:    return KeyCode::KP4;
			case VK_NUMPAD5:    return KeyCode::KP5;
			case VK_NUMPAD6:    return KeyCode::KP6;
			case VK_NUMPAD7:    return KeyCode::KP7;
			case VK_NUMPAD8:    return KeyCode::KP8;
			case VK_NUMPAD9:    return KeyCode::KP9;
			case VK_DECIMAL:    return KeyCode::KPDecimal;
			case VK_DIVIDE:     return KeyCode::KPDivide;
			case VK_MULTIPLY:   return KeyCode::KPMultiply;
			case VK_SUBTRACT:   return KeyCode::KPSubtract;
			case VK_ADD:        return KeyCode::KPAdd;
			default:
				return KeyCode::Unknown;
		}
	}

	static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		Application* app = Application::InstancePtr();
		switch (msg)
		{
			case WM_CLOSE:
			{
				WindowCloseEvent event;
				app->OnEvent(event);
				break;
			}
			case WM_SIZE:
			{
				i32 width = GET_X_LPARAM(lParam);
				i32 height = GET_Y_LPARAM(lParam);
				WindowResizeEvent event(width, height);
				app->OnEvent(event);
				break;
			}
			case WM_MOUSEMOVE:
			{
				i32 x = GET_X_LPARAM(lParam);
				i32 y = GET_Y_LPARAM(lParam);
				MouseMovedEvent event(x, y);
				app->OnEvent(event);
				break;
			}
			case WM_LBUTTONDOWN:
			case WM_MBUTTONDOWN:
			case WM_RBUTTONDOWN:
			case WM_LBUTTONUP:
			case WM_MBUTTONUP:
			case WM_RBUTTONUP:
			{
				bool pressed = msg == WM_LBUTTONDOWN || msg == WM_RBUTTONDOWN || msg == WM_MBUTTONDOWN;
				MouseButton mouseButton = MouseButton::Unknown;
				switch (msg)
				{
					case WM_LBUTTONDOWN:
					case WM_LBUTTONUP:
					{
						mouseButton = MouseButton::Left;
						break;
					}
					case WM_RBUTTONDOWN:
					case WM_RBUTTONUP:
					{
						mouseButton = MouseButton::Right;
						break;
					}
					case WM_MBUTTONDOWN:
					case WM_MBUTTONUP:
					{
						mouseButton = MouseButton::Middle;
						break;
					}
					default:
						break;
				}
				if (pressed)
				{
					MouseButtonPressedEvent event(mouseButton);
					app->OnEvent(event);
				} else
				{
					MouseButtonReleasedEvent event(mouseButton);
					app->OnEvent(event);
				}
				break;
			}
			case WM_MOUSEWHEEL:
			{
				short zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
				MouseScrolledEvent event(zDelta < 0 ? -1 : 1);
				app->OnEvent(event);
				break;
			}
			case WM_KEYDOWN:
			case WM_SYSKEYDOWN:
			case WM_KEYUP:
			case WM_SYSKEYUP:
			{
				bool pressed = msg == WM_KEYDOWN || msg == WM_SYSKEYDOWN;
				KeyCode key = TranslateKeyCode(wParam, lParam);
				if (pressed)
				{
					KeyPressedEvent event(key, LOWORD(lParam) > 0);
					app->OnEvent(event);
				} else
				{
					KeyReleasedEvent event(key);
					app->OnEvent(event);
				}
				break;
			}
			default:
				break;
		}
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	bool WindowsApplication::Startup()
	{
		m_Instance = GetModuleHandle(nullptr);
		m_Icon = LoadIcon(m_Instance, IDI_APPLICATION);
		m_Cursor = LoadCursor(m_Instance, IDC_ARROW);

		const WNDCLASS wc = {
			.style = CS_DBLCLKS,
			.lpfnWndProc = WndProc,
			.cbClsExtra = 0,
			.cbWndExtra = 0,
			.hInstance = m_Instance,
			.hIcon = m_Icon,
			.hCursor = m_Cursor,
			.hbrBackground = static_cast<HBRUSH>(GetStockObject(COLOR_WINDOW + 1)),
			.lpszMenuName = nullptr,
			.lpszClassName = WindowsWindow::AppWindowClass
		};

		RegisterClass(&wc);

		return true;
	}

	void WindowsApplication::Shutdown()
	{
		
	}

	SharedPtr<IPlatformWindow> WindowsApplication::MakeWindow(const WindowDefinition& config)
	{
		return CreateSharedPtr<WindowsWindow>(config, m_Instance);
	}

	void WindowsApplication::PumpMessages()
	{
		MSG msg;
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
}
#endif
