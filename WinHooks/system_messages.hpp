#ifndef _SYSTEM_MESSAGES_HPP_
#define _SYSTEM_MESSAGES_HPP_


#include "pch.h"

#include "hooks_headers.h"


using namespace std;

namespace amped::win
{

namespace en = amped::win::enumerate;

struct system_keys
{
	static const uint64_t Return_key;
	static const uint64_t Shift_key;
	static const uint64_t Control_key;
	static const uint64_t A_key;
	static const uint64_t B_key;
	static const uint64_t C_key;
	static const uint64_t D_key;
	static const uint64_t E_key;
	static const uint64_t F_key;
	static const uint64_t G_key;
	static const uint64_t H_key;
	static const uint64_t I_key;
	static const uint64_t J_key;
	static const uint64_t K_key;
	static const uint64_t L_key;
	static const uint64_t M_key;
	static const uint64_t N_key;
	static const uint64_t O_key;
	static const uint64_t P_key;
	static const uint64_t Q_key;
	static const uint64_t R_key;
	static const uint64_t S_key;
	static const uint64_t T_key;
	static const uint64_t U_key;
	static const uint64_t V_key;
	static const uint64_t W_key;
	static const uint64_t X_key;
	static const uint64_t Y_key;
	static const uint64_t Z_key;
}; // struct system_keys

enum class type_send_message : uint8_t
{
	mouse,
	keyboard,
	clipboard,
	scroll
}; // class enum type_send_message

struct send_message_impl
{
	using handle_t = en::handle_window_t;
	using message_t = UINT;
	using wparam_t = WPARAM;
	using lparam_t = LPARAM;
	using result_t = LRESULT;

	send_message_impl() = delete;

	explicit send_message_impl(const handle_t h, const message_t m, const wparam_t w, const lparam_t l) noexcept;

	result_t operator()() const noexcept;

	handle_t m_handle;
	message_t m_message;
	wparam_t m_param_w;
	lparam_t m_param_l;
}; // struct send_message_impl

template <type_send_message _Type = type_send_message::mouse>
struct send_message
{
	using array_msg_t = array<send_message_impl::message_t, 2>;
	using cord_t = tuple<int16_t, int16_t>;

	explicit send_message() noexcept
	{}

	explicit send_message(const send_message_impl::handle_t h, const cord_t& cord) noexcept
		: m_handle(h), m_cord(cord)
	{
		auto&& [x, y] = m_cord;
		m_param_l = y;
		m_param_l = m_param_l << 16 | x;
	}

	send_message_impl::result_t operator()() const
	{
		return send();
	}
protected:
	send_message_impl::result_t send() const
	{
		send_message_impl::result_t _result{ 0 };
		for (auto&& it : m_message)
		{
			_result |= send_message_impl(m_handle, it, m_param_w, m_param_l)();
		}
		return _result;
	}
private:
	send_message_impl::handle_t m_handle{ HWND_BROADCAST };
	array_msg_t m_message{ WM_LBUTTONDOWN, WM_LBUTTONUP };
	send_message_impl::wparam_t m_param_w{ 0 };
	send_message_impl::lparam_t m_param_l{ 0 };
	cord_t m_cord{ 0, 0 };
}; // struct send_message<type_send_message::mouse>

template <>
struct send_message<type_send_message::keyboard>
{
	using list_key_t = list<send_message_impl::wparam_t>;
	using array_msg_t = array<send_message_impl::message_t, 2>;

	explicit send_message() noexcept 
	{}

	explicit send_message(const send_message_impl::handle_t h, const list_key_t& list) noexcept
		: m_handle(h), m_keys(list)
	{
		m_keys.sort();
	}

	send_message_impl::result_t operator()() const
	{
		return send();
	}
protected:
	send_message_impl::result_t send() const
	{
		SetActiveWindow(m_handle);
		SetFocus(m_handle);
		send_message_impl::result_t _result{ 0 };
		for (auto&& it : m_message)
		{
			for (auto&& itt : m_keys)
			{
				_result |= send_message_impl(m_handle, it, itt, m_param_l)();
			}
		}
		return _result; 
	}
private:
	send_message_impl::handle_t m_handle{ HWND_BROADCAST };
	array_msg_t m_message{ WM_KEYDOWN, WM_KEYUP };
	list_key_t m_keys{};
	send_message_impl::lparam_t m_param_l{ 0 };
}; // struct send_message<type_send_message::keyboard>

// https://docs.microsoft.com/en-us/windows/win32/dataxchg/wm-paste
template <>
struct send_message<type_send_message::clipboard>
{
	explicit send_message() noexcept
	{}

	explicit send_message(const send_message_impl::handle_t& handle, const int32_t delta, const tuple<uint32_t, uint32_t> coord) noexcept
	{}

	send_message_impl::result_t operator()() const
	{
		return send();
	}
protected:
	send_message_impl::result_t send() const
	{
		send_message_impl::result_t _result{ 0 };
		return _result;
	}
private:
}; // struct send_message<type_send_message::clipboard>

template <>
struct send_message<type_send_message::scroll>
{
	explicit send_message() noexcept
	{}

	explicit send_message(const send_message_impl::handle_t& handle, const int32_t delta, const tuple<uint32_t, uint32_t> coord) noexcept
		: m_handle(handle), m_wparam(delta << 16)
	{
		auto&& [_x, _y] = coord;
		m_lparam = (_y << 16) | _x;
	}

	send_message_impl::result_t operator()() const
	{
		return send();
	}
protected:
	send_message_impl::result_t send() const
	{
		SetActiveWindow(m_handle);
		return send_message_impl(m_handle, m_message, m_wparam, m_lparam)();
	}
private:
	send_message_impl::handle_t m_handle{ HWND_BROADCAST };
	send_message_impl::message_t m_message{ WM_MOUSEHWHEEL };
	send_message_impl::wparam_t m_wparam{ 0 };
	send_message_impl::lparam_t m_lparam{ 0 };
}; // struct send_message<type_send_message::scroll>

// Export Audio/Video ( Ctrl + Shift + R )
struct export_vawe : public send_message<type_send_message::keyboard>
{
	export_vawe() = delete;

	explicit export_vawe(const send_message_impl::handle_t h) noexcept;

	send_message_impl::result_t operator()(const wstring_view f_name) const;
}; // struct export_vawe

} // namespace amped::win


#endif // _SYSTEM_MESSAGES_HPP_
