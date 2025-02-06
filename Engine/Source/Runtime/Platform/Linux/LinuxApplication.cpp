#ifdef WI_PLATFORM_LINUX
#include <xcb/xcb.h>
#include <xcb/xcb_keysyms.h>
#include <cstring>

#include "LinuxApplication.hpp"
#include "LinuxWindow.hpp"
#include "Application/Events/WindowEvents.h"
#include "Application/Events/KeyEvent.h"
#include "Application/Events/MouseEvents.h"
#include "Application/Application.hpp"
#include "X11.hpp"

namespace Wi
{
	static KeyCode TranslateKeyCode(X11::KeySym keySim)
	{
		switch (keySim)
		{
			case XK_0:			   return KeyCode::D0;
			case XK_1:			   return KeyCode::D1;
			case XK_2:			   return KeyCode::D2;
			case XK_3:			   return KeyCode::D3;
			case XK_4:			   return KeyCode::D4;
			case XK_5:			   return KeyCode::D5;
			case XK_6:			   return KeyCode::D6;
			case XK_7:			   return KeyCode::D7;
			case XK_8:			   return KeyCode::D8;
			case XK_9:			   return KeyCode::D9;

			case XK_a: case XK_A:  return KeyCode::A;
			case XK_b: case XK_B:  return KeyCode::B;
			case XK_c: case XK_C:  return KeyCode::C;
			case XK_d: case XK_D:  return KeyCode::D;
			case XK_e: case XK_E:  return KeyCode::E;
			case XK_f: case XK_F:  return KeyCode::F;
			case XK_g: case XK_G:  return KeyCode::G;
			case XK_h: case XK_H:  return KeyCode::H;
			case XK_i: case XK_I:  return KeyCode::I;
			case XK_j: case XK_J:  return KeyCode::J;
			case XK_k: case XK_K:  return KeyCode::K;
			case XK_l: case XK_L:  return KeyCode::L;
			case XK_m: case XK_M:  return KeyCode::M;
			case XK_n: case XK_N:  return KeyCode::N;
			case XK_o: case XK_O:  return KeyCode::O;
			case XK_p: case XK_P:  return KeyCode::P;
			case XK_q: case XK_Q:  return KeyCode::Q;
			case XK_r: case XK_R:  return KeyCode::R;
			case XK_s: case XK_S:  return KeyCode::S;
			case XK_t: case XK_T:  return KeyCode::T;
			case XK_u: case XK_U:  return KeyCode::U;
			case XK_v: case XK_V:  return KeyCode::V;
			case XK_w: case XK_W:  return KeyCode::W;
			case XK_x: case XK_X:  return KeyCode::X;
			case XK_y: case XK_Y:  return KeyCode::Y;
			case XK_z: case XK_Z:  return KeyCode::Z;

			case XK_Escape:		   return KeyCode::Escape;
			case XK_Return:		   return KeyCode::Enter;
			case XK_Tab:		   return KeyCode::Tab;
			case XK_BackSpace:	   return KeyCode::Backspace;
			case XK_space:		   return KeyCode::Space;

			case XK_Shift_L:	   return KeyCode::LeftShift;
			case XK_Shift_R:	   return KeyCode::RightShift;
			case XK_Control_L:	   return KeyCode::LeftControl;
			case XK_Control_R:	   return KeyCode::RightControl;
			case XK_Alt_L:		   return KeyCode::LeftAlt;
			case XK_Alt_R:		   return KeyCode::RightAlt;
			case XK_Caps_Lock:	   return KeyCode::CapsLock;

			case XK_Up:			   return KeyCode::Up;
			case XK_Down:		   return KeyCode::Down;
			case XK_Left:		   return KeyCode::Left;
			case XK_Right:		   return KeyCode::Right;

			case XK_F1:			   return KeyCode::F1;
			case XK_F2:			   return KeyCode::F2;
			case XK_F3:			   return KeyCode::F3;
			case XK_F4:			   return KeyCode::F4;
			case XK_F5:			   return KeyCode::F5;
			case XK_F6:			   return KeyCode::F6;
			case XK_F7:			   return KeyCode::F7;
			case XK_F8:			   return KeyCode::F8;
			case XK_F9:			   return KeyCode::F9;
			case XK_F10:		   return KeyCode::F10;
			case XK_F11:		   return KeyCode::F11;
			case XK_F12:		   return KeyCode::F12;
			default:
				return KeyCode::Unknown;
		}
	}

	xcb_atom_t LinuxApplication::s_AtomWmDeleteWindow = 0;
	xcb_atom_t LinuxApplication::s_AtomWmProtocols = 0;

	bool LinuxApplication::Startup()
	{
		m_Connection = xcb_connect(nullptr, nullptr);

		if (m_Connection == nullptr)
		{
			return false;
		}

		xcb_intern_atom_cookie_t wmDeleteCookie = xcb_intern_atom(
			m_Connection,
			0,
			strlen("WM_DELETE_WINDOW"),
			"WM_DELETE_WINDOW"
		);
		xcb_intern_atom_cookie_t wmProtocolsCookie = xcb_intern_atom(
			m_Connection,
			1,
			strlen("WM_PROTOCOLS"),
			"WM_PROTOCOLS"
		);
		xcb_intern_atom_reply_t* wmDeleteReply = xcb_intern_atom_reply(
			m_Connection,
			wmDeleteCookie,
			nullptr
		);
		xcb_intern_atom_reply_t* wmProtocolsReply = xcb_intern_atom_reply(
			m_Connection,
			wmProtocolsCookie,
			nullptr
		);

		s_AtomWmDeleteWindow = wmDeleteReply->atom;
		s_AtomWmProtocols = wmProtocolsReply->atom;

		free(wmProtocolsReply);
		free(wmDeleteReply);

		return true;
	}

	SharedPtr<IPlatformWindow> LinuxApplication::MakeWindow(const WindowDefinition& config)
	{
		const auto window = CreateSharedPtr<LinuxWindow>(config, m_Connection);
		xcb_window_t id = window->m_WindowHandle;
		m_Windows[id] = window;
		return window;
	}

	void LinuxApplication::PumpMessages()
	{
		Application* app = Application::InstancePtr();
		xcb_generic_event_t* event = nullptr;
		while ((event = xcb_poll_for_event(m_Connection)) != nullptr)
		{
			switch (event->response_type & ~0x80)
			{
				case XCB_EXPOSE:
				{
					xcb_flush(m_Connection);
					break;
				}
				case XCB_CONFIGURE_NOTIFY:
				{
					auto* cfg = reinterpret_cast<xcb_configure_notify_event_t*>(event);
					u16 width = cfg->width;
					u16 height = cfg->height;
					if (m_Windows.contains(cfg->window)) {
						const auto window = m_Windows[cfg->window];
						if (window->GetWidth() != width || window->GetHeight() != height)
						{
							WindowResizeEvent e(width, height);
							app->OnEvent(e);
						}
					}
					break;
				}
				case XCB_CLIENT_MESSAGE:
				{
					auto* msg = reinterpret_cast<xcb_client_message_event_t*>(event);
					if (msg->data.data32[0] == s_AtomWmDeleteWindow)
					{
						WindowCloseEvent e;
						app->OnEvent(e);
					}
					break;
				}
				case XCB_MOTION_NOTIFY:
				{
					auto* motion = reinterpret_cast<xcb_motion_notify_event_t*>(event);
					i16 x = motion->event_x;
					i16 y = motion->event_y;
					MouseMovedEvent e(x, y);
					app->OnEvent(e);
					break;
				}
				case XCB_BUTTON_PRESS:
				case XCB_BUTTON_RELEASE:
				{
					auto* bp = reinterpret_cast<xcb_button_press_event_t*>(event);
					bool pressed = event->response_type == XCB_BUTTON_PRESS;
					MouseButton btn = MouseButton::Unknown;
					switch (bp->detail)
					{
						case XCB_BUTTON_INDEX_1: btn = MouseButton::Left;   break;
						case XCB_BUTTON_INDEX_2: btn = MouseButton::Middle; break;
						case XCB_BUTTON_INDEX_3: btn = MouseButton::Right;  break;
						case 4:
						{
							MouseScrolledEvent e(+1.0f);
							app->OnEvent(e);
							return;
						}
						case 5:
						{
							MouseScrolledEvent e(-1.0f);
							app->OnEvent(e);
							return;
						}
						default: break;
					}
					if (btn != MouseButton::Unknown)
					{
						if (pressed)
						{
							MouseButtonPressedEvent e(btn);
							app->OnEvent(e);
						} else
						{
							MouseButtonReleasedEvent e(btn);
							app->OnEvent(e);
						}
					}
					break;
				}
				case XCB_KEY_PRESS:
				case XCB_KEY_RELEASE:
				{
					bool pressed = event->response_type == XCB_KEY_PRESS;
					auto* kpEvent = reinterpret_cast<xcb_key_press_event_t*>(event);
					xcb_keycode_t keycode = kpEvent->detail;
					// TODO: Fix memory leak
					static xcb_key_symbols_t* keySymbols = xcb_key_symbols_alloc(m_Connection);
					X11::KeySym keySym = xcb_key_symbols_get_keysym(keySymbols, keycode, 0);
					KeyCode code = TranslateKeyCode(keySym);
					if (code != KeyCode::Unknown)
					{
						if (pressed)
						{
							KeyPressedEvent e(code, false);
							app->OnEvent(e);
						} else
						{
							KeyReleasedEvent e(code);
							app->OnEvent(e);
						}
					}
					break;	
				}
				default:
					break;
			}

			free(event);
		}
	}

	void LinuxApplication::Shutdown()
	{
		xcb_disconnect(m_Connection);
	}
}

#endif
