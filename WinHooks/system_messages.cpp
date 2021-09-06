#include "pch.h"
#include "system_messages.hpp"


using namespace std;

namespace amped::win
{

const uint64_t system_keys::Return_key = 0x0D;

const uint64_t system_keys::Shift_key = 0x10;

const uint64_t system_keys::Control_key = 0x11;

const uint64_t system_keys::A_key = 0x41;

const uint64_t system_keys::B_key = 0x42;

const uint64_t system_keys::C_key = 0x43;

const uint64_t system_keys::D_key = 0x44;

const uint64_t system_keys::E_key = 0x45;

const uint64_t system_keys::F_key = 0x46;

const uint64_t system_keys::G_key = 0x47;

const uint64_t system_keys::H_key = 0x48;

const uint64_t system_keys::I_key = 0x49;

const uint64_t system_keys::J_key = 0x4A;

const uint64_t system_keys::K_key = 0x4B;

const uint64_t system_keys::L_key = 0x4C;

const uint64_t system_keys::M_key = 0x4D;

const uint64_t system_keys::N_key = 0x4E;

const uint64_t system_keys::O_key = 0x4F;

const uint64_t system_keys::P_key = 0x50;

const uint64_t system_keys::Q_key = 0x51;

const uint64_t system_keys::R_key = 0x52;

const uint64_t system_keys::S_key = 0x53;

const uint64_t system_keys::T_key = 0x54;

const uint64_t system_keys::U_key = 0x55;

const uint64_t system_keys::V_key = 0x56;

const uint64_t system_keys::W_key = 0x57;

const uint64_t system_keys::X_key = 0x58;

const uint64_t system_keys::Y_key = 0x59;

const uint64_t system_keys::Z_key = 0x5A;

#pragma optimize("", off);

send_message_impl::send_message_impl(const handle_t h, const message_t m, const wparam_t w, const lparam_t l) noexcept 
	: m_handle(h), m_message(m), m_param_w(w), m_param_l(l)
{
}

send_message_impl::result_t send_message_impl::operator()() const noexcept
{
	return SendMessage(m_handle, m_message, m_param_w, m_param_l);
}

export_vawe::export_vawe(const send_message_impl::handle_t h) noexcept 
	: send_message(h, { system_keys::Shift_key, system_keys::Control_key, system_keys::R_key })
{
}

send_message_impl::result_t export_vawe::operator()(const wstring_view f_name) const
{
	send_message_impl::result_t _result{ send() };
	return _result;
}

} // namespace amped::win
