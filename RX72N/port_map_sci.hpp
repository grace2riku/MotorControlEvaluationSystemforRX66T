#pragma once
//=========================================================================//
/*!	@file
	@brief	RX66N/RX72N/RX72M グループ・ポート・マッピング (SCIx) 
    @author 平松邦仁 (hira@rvf-rc45.net)
	@copyright	Copyright (C) 2021, 2024 Kunihito Hiramatsu @n
				Released under the MIT license @n
				https://github.com/hirakuni45/RX/blob/master/LICENSE
*/
//=========================================================================//
#if defined(SIG_RX66N)
#include "RX66N/peripheral.hpp"
#include "RX66N/port.hpp"
#include "RX66N/mpc.hpp"
#elif defined(SIG_RX72N)
#include "RX72N/peripheral.hpp"
#include "RX72N/port.hpp"
#include "RX72N/mpc.hpp"
#elif defined(SIG_RX72M)
#include "RX72M/peripheral.hpp"
#include "RX72N/port.hpp"
#include "RX72M/mpc.hpp"
#endif
#include "RX600/port_map_order.hpp"

namespace device {

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	/*!
		@brief  SCI ポート・マッピング
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	class port_map_sci : public port_map_order {
	public:

		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  SCI チャネル型
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		enum class CHANNEL : uint8_t {
			CTS,	///< CTSx#
			RTS,	///< RTSx#
			RXD,	///< RXDx
			SCK,	///< SCKx
			TXD,	///< TXDx
		};

	private:

		static bool sci0_(CHANNEL ch, bool ena, ORDER odr) noexcept
		{
			bool ret = true;
			uint8_t sel = 0;
			switch(ch) {
			case CHANNEL::CTS:
			case CHANNEL::RTS:
				// P23
				// PJ3
				sel = ena ? 0b001011 : 0;
				switch(odr) {
				case ORDER::FIRST:
					PORT2::PMR.B3 = 0;
					MPC::P23PFS.PSEL = sel;
					PORT2::PMR.B3 = ena;
					break;
				case ORDER::SECOND:
					PORTJ::PMR.B3 = 0;
					MPC::PJ3PFS.PSEL = sel;
					PORTJ::PMR.B3 = ena;
					break;
				default:
					ret = false;
					break;
				}
				break;
			case CHANNEL::RXD:
				// P21
				// P33
				sel = ena ? 0b001010 : 0;
				switch(odr) {
				case ORDER::FIRST:
					PORT2::PMR.B1 = 0;
					MPC::P21PFS.PSEL = sel;
					PORT2::PMR.B1 = ena;
					break;
				case ORDER::SECOND:
					PORT3::PMR.B3 = 0;
					MPC::P33PFS.PSEL = sel;
					PORT3::PMR.B3 = ena;
					break;
				default:
					break;
				}
			case CHANNEL::SCK:
				// P22
				// P34
				sel = ena ? 0b001010 : 0;
				switch(odr) {
				case ORDER::FIRST:
					PORT2::PMR.B2 = 0;
					MPC::P22PFS.PSEL = sel;
					PORT2::PMR.B2 = ena;
					break;
				case ORDER::SECOND:
					PORT3::PMR.B4 = 0;
					MPC::P34PFS.PSEL = sel;
					PORT3::PMR.B4 = ena;
					break;
				default:
					break;
				}
			case CHANNEL::TXD:
				// P20
				// P32
				sel = ena ? 0b001010 : 0;
				switch(odr) {
				case ORDER::FIRST:
					PORT2::PMR.B0 = 0;
					MPC::P20PFS.PSEL = sel;
					PORT2::PMR.B0 = ena;
					break;
				case ORDER::SECOND:
					PORT3::PMR.B2 = 0;
					MPC::P32PFS.PSEL = sel;
					PORT3::PMR.B2 = ena;
					break;
				default:
					break;
				}
			default:
				ret = false;
				break;
			}
			return ret;
		}


		static bool sci1_(CHANNEL ch, bool ena, ORDER odr) noexcept
		{
			bool ret = true;
			uint8_t sel = 0;
			switch(ch) {
			case CHANNEL::CTS:
			case CHANNEL::RTS:
				// P14
				// P31
				sel = ena ? 0b001011 : 0;
				switch(odr) {
				case ORDER::FIRST:
					PORT1::PMR.B4 = 0;
					MPC::P14PFS.PSEL = sel;
					PORT1::PMR.B4 = ena;
					break;
				case ORDER::SECOND:
					PORT3::PMR.B1 = 0;
					MPC::P31PFS.PSEL = sel;
					PORT3::PMR.B1 = ena;
					break;
				default:
					ret = false;
					break;
				}
				break;
			case CHANNEL::RXD:
				// P15
				// P30
				// PF2
				sel = ena ? 0b001010 : 0;
				switch(odr) {
				case ORDER::FIRST:
					PORT1::PMR.B5 = 0;
					MPC::P15PFS.PSEL = sel;
					PORT1::PMR.B5 = ena;
					break;
				case ORDER::SECOND:
					PORT3::PMR.B0 = 0;
					MPC::P30PFS.PSEL = sel;
					PORT3::PMR.B0 = ena;
					break;
				case ORDER::THIRD:
					PORTF::PMR.B2 = 0;
					MPC::PF2PFS.PSEL = sel;
					PORTF::PMR.B2 = ena;
					break;
				default:
					break;
				}
			case CHANNEL::SCK:
				// P17
				// P27
				// PF1
				sel = ena ? 0b001010 : 0;
				switch(odr) {
				case ORDER::FIRST:
					PORT1::PMR.B7 = 0;
					MPC::P17PFS.PSEL = sel;
					PORT1::PMR.B7 = ena;
					break;
				case ORDER::SECOND:
					PORT2::PMR.B7 = 0;
					MPC::P27PFS.PSEL = sel;
					PORT2::PMR.B7 = ena;
					break;
				case ORDER::THIRD:
					PORTF::PMR.B1 = 0;
					MPC::PF1PFS.PSEL = sel;
					PORTF::PMR.B1 = ena;
					break;
				default:
					break;
				}
			case CHANNEL::TXD:
				// P16
				// P26
				// PF0
				sel = ena ? 0b001010 : 0;
				switch(odr) {
				case ORDER::FIRST:
					PORT1::PMR.B6 = 0;
					MPC::P16PFS.PSEL = sel;
					PORT1::PMR.B6 = ena;
					break;
				case ORDER::SECOND:
					PORT2::PMR.B6 = 0;
					MPC::P26PFS.PSEL = sel;
					PORT2::PMR.B6 = ena;
					break;
				case ORDER::THIRD:
					PORTF::PMR.B0 = 0;
					MPC::PF0PFS.PSEL = sel;
					PORTF::PMR.B0 = ena;
					break;
				default:
					break;
				}
			default:
				ret = false;
				break;
			}
			return ret;
		}


		static bool sci2_(CHANNEL ch, bool ena, ORDER odr) noexcept
		{
			bool ret = true;
			uint8_t sel = 0;
			switch(ch) {
			case CHANNEL::CTS:
			case CHANNEL::RTS:
				// P54
				// PJ5
				sel = ena ? 0b001011 : 0;
				switch(odr) {
				case ORDER::FIRST:
					PORT5::PMR.B4 = 0;
					MPC::P54PFS.PSEL = sel;
					PORT5::PMR.B4 = ena;
					break;
				case ORDER::SECOND:
					PORTJ::PMR.B5 = 0;
					MPC::PJ5PFS.PSEL = sel;
					PORTJ::PMR.B5 = ena;
					break;
				default:
					ret = false;
					break;
				}
				break;
			case CHANNEL::RXD:
				// P12
				// P52
				sel = ena ? 0b001010 : 0;
				switch(odr) {
				case ORDER::FIRST:
					PORT1::PMR.B2 = 0;
					MPC::P12PFS.PSEL = sel;
					PORT1::PMR.B2 = ena;
					break;
				case ORDER::SECOND:
					PORT5::PMR.B2 = 0;
					MPC::P52PFS.PSEL = sel;
					PORT5::PMR.B2 = ena;
					break;
				default:
					break;
				}
			case CHANNEL::SCK:
				// P11
				// P51
				sel = ena ? 0b001010 : 0;
				switch(odr) {
				case ORDER::FIRST:
					PORT1::PMR.B1 = 0;
					MPC::P11PFS.PSEL = sel;
					PORT1::PMR.B1 = ena;
					break;
				case ORDER::SECOND:
					PORT5::PMR.B1 = 0;
					MPC::P51PFS.PSEL = sel;
					PORT5::PMR.B1 = ena;
					break;
				default:
					break;
				}
			case CHANNEL::TXD:
				// P13
				// P50
				sel = ena ? 0b001010 : 0;
				switch(odr) {
				case ORDER::FIRST:
					PORT1::PMR.B3 = 0;
					MPC::P13PFS.PSEL = sel;
					PORT1::PMR.B3 = ena;
					break;
				case ORDER::SECOND:
					PORT5::PMR.B0 = 0;
					MPC::P50PFS.PSEL = sel;
					PORT5::PMR.B0 = ena;
					break;
				default:
					break;
				}
			default:
				ret = false;
				break;
			}
			return ret;
		}


		static bool sci3_(CHANNEL ch, bool ena, ORDER odr) noexcept
		{
			bool ret = true;
			uint8_t sel = 0;
			switch(ch) {
			case CHANNEL::CTS:
			case CHANNEL::RTS:
				// P26
				sel = ena ? 0b001011 : 0;
				switch(odr) {
				case ORDER::FIRST:
					PORT2::PMR.B6 = 0;
					MPC::P26PFS.PSEL = sel;
					PORT2::PMR.B6 = ena;
					break;
				default:
					ret = false;
					break;
				}
				break;
			case CHANNEL::RXD:
				// P16
				// P25
				sel = ena ? 0b001010 : 0;
				switch(odr) {
				case ORDER::FIRST:
					PORT1::PMR.B6 = 0;
					MPC::P16PFS.PSEL = sel;
					PORT1::PMR.B6 = ena;
					break;
				case ORDER::SECOND:
					PORT2::PMR.B5 = 0;
					MPC::P25PFS.PSEL = sel;
					PORT2::PMR.B5 = ena;
					break;
				default:
					break;
				}
			case CHANNEL::SCK:
				// P15
				// P24
				sel = ena ? 0b001010 : 0;
				switch(odr) {
				case ORDER::FIRST:
					PORT1::PMR.B5 = 0;
					MPC::P15PFS.PSEL = sel;
					PORT1::PMR.B5 = ena;
					break;
				case ORDER::SECOND:
					PORT2::PMR.B4 = 0;
					MPC::P24PFS.PSEL = sel;
					PORT2::PMR.B4 = ena;
					break;
				default:
					break;
				}
			case CHANNEL::TXD:
				// P17
				// P23
				sel = ena ? 0b001010 : 0;
				switch(odr) {
				case ORDER::FIRST:
					PORT1::PMR.B7 = 0;
					MPC::P17PFS.PSEL = sel;
					PORT1::PMR.B7 = ena;
					break;
				case ORDER::SECOND:
					PORT2::PMR.B3 = 0;
					MPC::P23PFS.PSEL = sel;
					PORT2::PMR.B3 = ena;
					break;
				default:
					break;
				}
			default:
				ret = false;
				break;
			}
			return ret;
		}


		static bool sci4_(CHANNEL ch, bool ena, ORDER odr) noexcept
		{
			bool ret = true;
			uint8_t sel = 0;
			switch(ch) {
			case CHANNEL::CTS:
			case CHANNEL::RTS:
				// PB2
				sel = ena ? 0b001011 : 0;
				switch(odr) {
				case ORDER::FIRST:
					PORTB::PMR.B2 = 0;
					MPC::PB2PFS.PSEL = sel;
					PORTB::PMR.B2 = ena;
					break;
				default:
					ret = false;
					break;
				}
				break;
			case CHANNEL::RXD:
				// PB0
				sel = ena ? 0b001010 : 0;
				switch(odr) {
				case ORDER::FIRST:
					PORTB::PMR.B0 = 0;
					MPC::PB0PFS.PSEL = sel;
					PORTB::PMR.B0 = ena;
					break;
				default:
					break;
				}
			case CHANNEL::SCK:
				// PB3
				sel = ena ? 0b001010 : 0;
				switch(odr) {
				case ORDER::FIRST:
					PORTB::PMR.B3 = 0;
					MPC::PB3PFS.PSEL = sel;
					PORTB::PMR.B3 = ena;
					break;
				default:
					break;
				}
			case CHANNEL::TXD:
				// PB1
				sel = ena ? 0b001010 : 0;
				switch(odr) {
				case ORDER::FIRST:
					PORTB::PMR.B1 = 0;
					MPC::PB1PFS.PSEL = sel;
					PORTB::PMR.B1 = ena;
					break;
				default:
					break;
				}
			default:
				ret = false;
				break;
			}
			return ret;
		}


		static bool sci5_(CHANNEL ch, bool ena, ORDER odr) noexcept
		{
			bool ret = true;
			uint8_t sel = 0;
			switch(ch) {
			case CHANNEL::CTS:
			case CHANNEL::RTS:
				// PA6
				// PC0
				sel = ena ? 0b001011 : 0;
				switch(odr) {
				case ORDER::FIRST:
					PORTA::PMR.B6 = 0;
					MPC::PA6PFS.PSEL = sel;
					PORTA::PMR.B6 = ena;
					break;
				case ORDER::SECOND:
					PORTC::PMR.B0 = 0;
					MPC::PC0PFS.PSEL = sel;
					PORTC::PMR.B0 = ena;
					break;
				default:
					ret = false;
					break;
				}
				break;
			case CHANNEL::RXD:
				// PA2
				// PA3
				// PC2
				sel = ena ? 0b001010 : 0;
				switch(odr) {
				case ORDER::FIRST:
					PORTA::PMR.B2 = 0;
					MPC::PA2PFS.PSEL = sel;
					PORTA::PMR.B2 = ena;
					break;
				case ORDER::SECOND:
					PORTA::PMR.B3 = 0;
					MPC::PA3PFS.PSEL = sel;
					PORTA::PMR.B3 = ena;
					break;
				case ORDER::THIRD:
					PORTC::PMR.B2 = 0;
					MPC::PC2PFS.PSEL = sel;
					PORTC::PMR.B2 = ena;
					break;
				default:
					break;
				}
			case CHANNEL::SCK:
				// PA1
				// PC1
				// PC4
				sel = ena ? 0b001010 : 0;
				switch(odr) {
				case ORDER::FIRST:
					PORTA::PMR.B1 = 0;
					MPC::PA1PFS.PSEL = sel;
					PORTA::PMR.B1 = ena;
					break;
				case ORDER::SECOND:
					PORTC::PMR.B1 = 0;
					MPC::PC1PFS.PSEL = sel;
					PORTC::PMR.B1 = ena;
					break;
				case ORDER::THIRD:
					PORTC::PMR.B4 = 0;
					MPC::PC4PFS.PSEL = sel;
					PORTC::PMR.B4 = ena;
					break;
				default:
					break;
				}
			case CHANNEL::TXD:
				// PA4
				// PC3
				sel = ena ? 0b001010 : 0;
				switch(odr) {
				case ORDER::FIRST:
					PORTA::PMR.B4 = 0;
					MPC::PA4PFS.PSEL = sel;
					PORTA::PMR.B4 = ena;
					break;
				case ORDER::SECOND:
					PORTC::PMR.B3 = 0;
					MPC::PC3PFS.PSEL = sel;
					PORTC::PMR.B3 = ena;
					break;
				default:
					break;
				}
			default:
				ret = false;
				break;
			}
			return ret;
		}


		static bool sci6_(CHANNEL ch, bool ena, ORDER odr) noexcept
		{
			bool ret = true;
			uint8_t sel = 0;
			switch(ch) {
			case CHANNEL::CTS:
			case CHANNEL::RTS:
				// PB2
				// PJ3
				sel = ena ? 0b001011 : 0;
				switch(odr) {
				case ORDER::FIRST:
					PORTB::PMR.B2 = 0;
					MPC::PB2PFS.PSEL = sel;
					PORTB::PMR.B2 = ena;
					break;
				case ORDER::SECOND:
					PORTJ::PMR.B3 = 0;
					MPC::PJ3PFS.PSEL = sel;
					PORTJ::PMR.B3 = ena;
					break;
				default:
					ret = false;
					break;
				}
				break;
			case CHANNEL::RXD:
				// P01
				// P33
				// PB0
				sel = ena ? 0b001010 : 0;
				switch(odr) {
				case ORDER::FIRST:
					PORT0::PMR.B1 = 0;
					MPC::P01PFS.PSEL = sel;
					PORT0::PMR.B1 = ena;
					break;
				case ORDER::SECOND:
					PORT3::PMR.B3 = 0;
					MPC::P33PFS.PSEL = sel;
					PORT3::PMR.B3 = ena;
					break;
				case ORDER::THIRD:
					PORTB::PMR.B0 = 0;
					MPC::PB0PFS.PSEL = sel;
					PORTB::PMR.B0 = ena;
					break;
				default:
					break;
				}
			case CHANNEL::SCK:
				// P02
				// P34
				// PB3
				sel = ena ? 0b001010 : 0;
				switch(odr) {
				case ORDER::FIRST:
					PORT0::PMR.B2 = 0;
					MPC::P02PFS.PSEL = sel;
					PORT0::PMR.B2 = ena;
					break;
				case ORDER::SECOND:
					PORT3::PMR.B4 = 0;
					MPC::P34PFS.PSEL = sel;
					PORT3::PMR.B4 = ena;
					break;
				case ORDER::THIRD:
					PORTB::PMR.B3 = 0;
					MPC::PB3PFS.PSEL = sel;
					PORTB::PMR.B3 = ena;
					break;
				default:
					break;
				}
			case CHANNEL::TXD:
				// P00
				// P32
				// PB1
				sel = ena ? 0b001010 : 0;
				switch(odr) {
				case ORDER::FIRST:
					PORT0::PMR.B0 = 0;
					MPC::P00PFS.PSEL = sel;
					PORT0::PMR.B0 = ena;
					break;
				case ORDER::SECOND:
					PORT3::PMR.B2 = 0;
					MPC::P32PFS.PSEL = sel;
					PORT3::PMR.B2 = ena;
					break;
				case ORDER::THIRD:
					PORTB::PMR.B1 = 0;
					MPC::PB1PFS.PSEL = sel;
					PORTB::PMR.B1 = ena;
					break;
				default:
					break;
				}
			default:
				ret = false;
				break;
			}
			return ret;
		}


		static bool sci7_(CHANNEL ch, bool ena, ORDER odr) noexcept
		{
			bool ret = true;
			uint8_t sel = 0;
			switch(ch) {
			case CHANNEL::CTS:
			case CHANNEL::RTS:
				// P93
				// PH3
				sel = ena ? 0b001011 : 0;
				switch(odr) {
				case ORDER::FIRST:
					PORT9::PMR.B3 = 0;
					MPC::P93PFS.PSEL = sel;
					PORT9::PMR.B3 = ena;
					break;
				case ORDER::SECOND:
					PORTH::PMR.B3 = 0;
					MPC::PH3PFS.PSEL = sel;
					PORTH::PMR.B3 = ena;
					break;
				default:
					ret = false;
					break;
				}
				break;
			case CHANNEL::RXD:
				// P57
				// P92
				// PH1
				sel = ena ? 0b001010 : 0;
				switch(odr) {
				case ORDER::FIRST:
					PORT5::PMR.B7 = 0;
					MPC::P57PFS.PSEL = sel;
					PORT5::PMR.B7 = ena;
					break;
				case ORDER::SECOND:
					PORT9::PMR.B2 = 0;
					MPC::P92PFS.PSEL = sel;
					PORT9::PMR.B2 = ena;
					break;
				case ORDER::THIRD:
					PORTH::PMR.B1 = 0;
					MPC::PH1PFS.PSEL = sel;
					PORTH::PMR.B1 = ena;
					break;
				default:
					break;
				}
			case CHANNEL::SCK:
				// P56
				// P91
				// PH0
				sel = ena ? 0b001010 : 0;
				switch(odr) {
				case ORDER::FIRST:
					PORT5::PMR.B6 = 0;
					MPC::P56PFS.PSEL = sel;
					PORT5::PMR.B6 = ena;
					break;
				case ORDER::SECOND:
					PORT9::PMR.B1 = 0;
					MPC::P91PFS.PSEL = sel;
					PORT9::PMR.B1 = ena;
					break;
				case ORDER::THIRD:
					PORTH::PMR.B0 = 0;
					MPC::PH0PFS.PSEL = sel;
					PORTH::PMR.B0 = ena;
					break;
				default:
					break;
				}
			case CHANNEL::TXD:
				// P55
				// P90
				// PH2
				sel = ena ? 0b001010 : 0;
				switch(odr) {
				case ORDER::FIRST:
					PORT5::PMR.B5 = 0;
					MPC::P55PFS.PSEL = sel;
					PORT5::PMR.B5 = ena;
					break;
				case ORDER::SECOND:
					PORT9::PMR.B0 = 0;
					MPC::P90PFS.PSEL = sel;
					PORT9::PMR.B0 = ena;
					break;
				case ORDER::THIRD:
					PORTH::PMR.B2 = 0;
					MPC::PH2PFS.PSEL = sel;
					PORTH::PMR.B2 = ena;
					break;
				default:
					break;
				}
			default:
				ret = false;
				break;
			}
			return ret;
		}


		static bool sci8_(CHANNEL ch, bool ena, ORDER odr) noexcept
		{
			bool ret = true;
			uint8_t sel = 0;
			switch(ch) {
			case CHANNEL::CTS:
				// PC4
				// PK3
				sel = ena ? 0b001011 : 0;
				switch(odr) {
				case ORDER::FIRST:
					PORTC::PMR.B4 = 0;
					MPC::PC4PFS.PSEL = sel;
					PORTC::PMR.B4 = ena;
					break;
				case ORDER::SECOND:
					PORTK::PMR.B3 = 0;
					MPC::PK3PFS.PSEL = sel;
					PORTK::PMR.B3 = ena;
					break;
				default:
					ret = false;
					break;
				}
				break;
			case CHANNEL::RTS:
				// PK3
				// PC5
				sel = ena ? 0b001011 : 0;
				switch(odr) {
				case ORDER::FIRST:
					PORTK::PMR.B3 = 0;
					MPC::PK3PFS.PSEL = sel;
					PORTK::PMR.B3 = ena;
					break;
				case ORDER::SECOND:
					PORTC::PMR.B5 = 0;
					MPC::PC5PFS.PSEL = sel;
					PORTC::PMR.B5 = ena;
					break;
				default:
					ret = false;
					break;
				}
				break;
			case CHANNEL::RXD:
				// PC6
				// PJ1
				// PK1
				sel = ena ? 0b001010 : 0;
				switch(odr) {
				case ORDER::FIRST:
					PORTC::PMR.B6 = 0;
					MPC::PC6PFS.PSEL = sel;
					PORTC::PMR.B6 = ena;
					break;
				case ORDER::SECOND:
					PORTJ::PMR.B1 = 0;
					MPC::PJ1PFS.PSEL = sel;
					PORTJ::PMR.B1 = ena;
					break;
				case ORDER::THIRD:
					PORTK::PMR.B1 = 0;
					MPC::PK1PFS.PSEL = sel;
					PORTK::PMR.B1 = ena;
					break;
				default:
					break;
				}
				break;
			case CHANNEL::SCK:
				// PJ0
				// PK0
				// PC5
				sel = ena ? 0b001010 : 0;
				switch(odr) {
				case ORDER::FIRST:
					PORTJ::PMR.B0 = 0;
					MPC::PJ0PFS.PSEL = sel;
					PORTJ::PMR.B0 = ena;
					break;
				case ORDER::SECOND:
					PORTK::PMR.B0 = 0;
					MPC::PK0PFS.PSEL = sel;
					PORTK::PMR.B0 = ena;
					break;
				case ORDER::THIRD:
					PORTC::PMR.B5 = 0;
					MPC::PC5PFS.PSEL = sel;
					PORTC::PMR.B5 = ena;
					break;
				default:
					break;
				}
				break;
			case CHANNEL::TXD:
				// PC7
				// PJ2
				// PK2
				sel = ena ? 0b001010 : 0;
				switch(odr) {
				case ORDER::FIRST:
					PORTC::PMR.B7 = 0;
					MPC::PC7PFS.PSEL = sel;
					PORTC::PMR.B7 = ena;
					break;
				case ORDER::SECOND:
					PORTJ::PMR.B2 = 0;
					MPC::PJ2PFS.PSEL = sel;
					PORTJ::PMR.B2 = ena;
					break;
				case ORDER::THIRD:
					PORTK::PMR.B2 = 0;
					MPC::PK2PFS.PSEL = sel;
					PORTK::PMR.B2 = ena;
					break;
				default:
					break;
				}
				break;
			default:
				ret = false;
				break;
			}
			return ret;
		}


		static bool sci9_(CHANNEL ch, bool ena, ORDER odr) noexcept
		{
			bool ret = true;
			switch(ch) {
			case CHANNEL::CTS:
				// PB4
				// PL3
				switch(odr) {
				case ORDER::FIRST:
					PORTB::PMR.B4 = 0;
					MPC::PB4PFS.PSEL = ena ? 0b001011 : 0;
					PORTB::PMR.B4 = ena;
					break;
				case ORDER::SECOND:
					PORTL::PMR.B3 = 0;
					MPC::PL3PFS.PSEL = ena ? 0b001011 : 0;
					PORTL::PMR.B3 = ena;
					break;
				default:
					ret = false;
					break;
				}
				break;
			case CHANNEL::RTS:
				// PL3
				// PB5
				switch(odr) {
				case ORDER::FIRST:
					PORTL::PMR.B3 = 0;
					MPC::PL3PFS.PSEL = ena ? 0b001011 : 0;
					PORTL::PMR.B3 = ena;
					break;
				case ORDER::SECOND:
					PORTB::PMR.B5 = 0;
					MPC::PB5PFS.PSEL = ena ? 0b001011 : 0;
					PORTB::PMR.B5 = ena;
					break;
				default:
					ret = false;
					break;
				}
				break;
			case CHANNEL::RXD:
				// PB6
				// PL1
				switch(odr) {
				case ORDER::FIRST:
					PORTB::PMR.B6 = 0;
					MPC::PB6PFS.PSEL = ena ? 0b001010 : 0;
					PORTB::PMR.B6 = ena;
					break;
				case ORDER::SECOND:
					PORTL::PMR.B1 = 0;
					MPC::PL1PFS.PSEL = ena ? 0b001010 : 0;
					PORTL::PMR.B1 = ena;
					break;
				default:
					break;
				}
			case CHANNEL::SCK:
				// PL0
				// PB5
				switch(odr) {
				case ORDER::FIRST:
					PORTL::PMR.B0 = 0;
					MPC::PL0PFS.PSEL = ena ? 0b001010 : 0;
					PORTL::PMR.B0 = ena;
					break;
				case ORDER::SECOND:
					PORTB::PMR.B5 = 0;
					MPC::PB5PFS.PSEL = ena ? 0b001010 : 0;
					PORTB::PMR.B5 = ena;
					break;
				default:
					break;
				}
			case CHANNEL::TXD:
				// PB7
				// PL2
				switch(odr) {
				case ORDER::FIRST:
					PORTB::PMR.B7 = 0;
					MPC::PB7PFS.PSEL = ena ? 0b001010 : 0;
					PORTB::PMR.B7 = ena;
					break;
				case ORDER::SECOND:
					PORTL::PMR.B2 = 0;
					MPC::PL2PFS.PSEL = ena ? 0b001010 : 0;
					PORTL::PMR.B2 = ena;
					break;
				default:
					break;
				}
			default:
				ret = false;
				break;
			}
			return ret;
		}


		static bool sci10_(CHANNEL ch, bool ena, ORDER odr) noexcept
		{
			bool ret = true;
			switch(ch) {
			case CHANNEL::CTS:
				// PC4
				// P83
				// PM3
				switch(odr) {
				case ORDER::FIRST:
					PORTC::PMR.B4 = 0;
					MPC::PC4PFS.PSEL = ena ? 0b100100 : 0;
					PORTC::PMR.B4 = ena;
					break;
				case ORDER::SECOND:
					PORT8::PMR.B3 = 0;
					MPC::P83PFS.PSEL = ena ? 0b001011 : 0;
					PORT8::PMR.B3 = ena;
					break;
				case ORDER::THIRD:
					PORTM::PMR.B3 = 0;
					MPC::PM3PFS.PSEL = ena ? 0b001011 : 0;
					PORTM::PMR.B3 = ena;
					break;
				default:
					ret = false;
					break;
				}
				break;
			case CHANNEL::RTS:
				// PC4
				// PM3
				// P80
				switch(odr) {
				case ORDER::FIRST:
					PORTC::PMR.B4 = 0;
					MPC::PC4PFS.PSEL = ena ? 0b100100 : 0;
					PORTC::PMR.B4 = ena;
					break;
				case ORDER::SECOND:
					PORTM::PMR.B3 = 0;
					MPC::PM3PFS.PSEL = ena ? 0b001011 : 0;
					PORTM::PMR.B3 = ena;
					break;
				case ORDER::THIRD:
					PORT8::PMR.B0 = 0;
					MPC::P80PFS.PSEL = ena ? 0b001011 : 0;
					PORT8::PMR.B0 = ena;
					break;
				default:
					ret = false;
					break;
				}
				break;
			case CHANNEL::RXD:
				// P81
				// P86
				// PC6
				// PM1
				switch(odr) {
				case ORDER::FIRST:
					PORT8::PMR.B1 = 0;
					MPC::P81PFS.PSEL = ena ? 0b001010 : 0;
					PORT8::PMR.B1 = ena;
					break;
				case ORDER::SECOND:
					PORT8::PMR.B6 = 0;
					MPC::P86PFS.PSEL = ena ? 0b001010 : 0;
					PORT8::PMR.B6 = ena;
					break;
				case ORDER::THIRD:
					PORTC::PMR.B6 = 0;
					MPC::PC6PFS.PSEL = ena ? 0b100100 : 0;
					PORTC::PMR.B6 = ena;
					break;
				case ORDER::FOURTH:
					PORTM::PMR.B1 = 0;
					MPC::PM1PFS.PSEL = ena ? 0b001010 : 0;
					PORTM::PMR.B1 = ena;
					break;
				default:
					break;
				}
			case CHANNEL::SCK:
				// PC5
				// PM0
				switch(odr) {
				case ORDER::FIRST:
					PORTC::PMR.B5 = 0;
					MPC::PC5PFS.PSEL = ena ? 0b100100 : 0;
					PORTC::PMR.B5 = ena;
					break;
				case ORDER::SECOND:
					PORTM::PMR.B0 = 0;
					MPC::PM0PFS.PSEL = ena ? 0b001010 : 0;
					PORTM::PMR.B0 = ena;
					break;
				default:
					break;
				}
			case CHANNEL::TXD:
				// P82
				// P87
				// PC7
				// PM2
				switch(odr) {
				case ORDER::FIRST:
					PORT8::PMR.B2 = 0;
					MPC::P82PFS.PSEL = ena ? 0b001010 : 0;
					PORT8::PMR.B2 = ena;
					break;
				case ORDER::SECOND:
					PORT8::PMR.B7 = 0;
					MPC::P87PFS.PSEL = ena ? 0b001010 : 0;
					PORT8::PMR.B7 = ena;
					break;
				case ORDER::THIRD:
					PORTC::PMR.B7 = 0;
					MPC::PC7PFS.PSEL = ena ? 0b100100 : 0;
					PORTC::PMR.B7 = ena;
					break;
				case ORDER::FOURTH:
					PORTM::PMR.B2 = 0;
					MPC::PM2PFS.PSEL = ena ? 0b001010 : 0;
					PORTM::PMR.B2 = ena;
					break;
				default:
					break;
				}
			default:
				ret = false;
				break;
			}
			return ret;
		}


		static bool sci11_(CHANNEL ch, bool ena, ORDER odr) noexcept
		{
			bool ret = true;
			switch(ch) {
			case CHANNEL::CTS:
				// PB4
				// P74
				// PQ3
				switch(odr) {
				case ORDER::FIRST:
					PORTB::PMR.B4 = 0;
					MPC::PB4PFS.PSEL = ena ? 0b100100 : 0;
					PORTB::PMR.B4 = ena;
					break;
				case ORDER::SECOND:
					PORT7::PMR.B4 = 0;
					MPC::P74PFS.PSEL = ena ? 0b001011 : 0;
					PORT7::PMR.B4 = ena;
					break;
				case ORDER::THIRD:
					PORTQ::PMR.B3 = 0;
					MPC::PQ3PFS.PSEL = ena ? 0b001011 : 0;
					PORTQ::PMR.B3 = ena;
					break;
				default:
					ret = false;
					break;
				}
				break;
			case CHANNEL::RTS:
				// PB4
				// PQ3
				// P75
				switch(odr) {
				case ORDER::FIRST:
					PORTB::PMR.B4 = 0;
					MPC::PB4PFS.PSEL = ena ? 0b100100 : 0;
					PORTB::PMR.B4 = ena;
					break;
				case ORDER::SECOND:
					PORTQ::PMR.B3 = 0;
					MPC::PQ3PFS.PSEL = ena ? 0b001011 : 0;
					PORTQ::PMR.B3 = ena;
					break;
				case ORDER::THIRD:
					PORT7::PMR.B5 = 0;
					MPC::P75PFS.PSEL = ena ? 0b001011 : 0;
					PORT7::PMR.B5 = ena;
					break;
				default:
					ret = false;
					break;
				}
				break;
			case CHANNEL::RXD:
				// P76
				// PB6
				// PQ1
				switch(odr) {
				case ORDER::FIRST:
					PORT7::PMR.B6 = 0;
					MPC::P76PFS.PSEL = ena ? 0b001010 : 0;
					PORT7::PMR.B6 = ena;
					break;
				case ORDER::SECOND:
					PORTB::PMR.B6 = 0;
					MPC::PB6PFS.PSEL = ena ? 0b100100 : 0;
					PORTB::PMR.B6 = ena;
					break;
				case ORDER::THIRD:
					PORTQ::PMR.B1 = 0;
					MPC::PQ1PFS.PSEL = ena ? 0b001010 : 0;
					PORTQ::PMR.B1 = ena;
					break;
				default:
					break;
				}
			case CHANNEL::SCK:
				// PB5
				// PQ0
				// P75
				switch(odr) {
				case ORDER::FIRST:
					PORTB::PMR.B5 = 0;
					MPC::PB5PFS.PSEL = ena ? 0b100100 : 0;
					PORTB::PMR.B5 = ena;
					break;
				case ORDER::SECOND:
					PORTQ::PMR.B0 = 0;
					MPC::PQ0PFS.PSEL = ena ? 0b001010 : 0;
					PORTQ::PMR.B0 = ena;
					break;
				case ORDER::THIRD:
					PORT7::PMR.B5 = 0;
					MPC::P75PFS.PSEL = ena ? 0b001010 : 0;
					PORT7::PMR.B5 = ena;
					break;
				default:
					break;
				}
			case CHANNEL::TXD:
				// P77
				// PB7
				// PQ2
				switch(odr) {
				case ORDER::FIRST:
					PORT7::PMR.B7 = 0;
					MPC::P77PFS.PSEL = ena ? 0b001010 : 0;
					PORT7::PMR.B7 = ena;
					break;
				case ORDER::SECOND:
					PORTB::PMR.B7 = 0;
					MPC::PB7PFS.PSEL = ena ? 0b100100 : 0;
					PORTB::PMR.B7 = ena;
					break;
				case ORDER::THIRD:
					PORTQ::PMR.B2 = 0;
					MPC::PQ2PFS.PSEL = ena ? 0b001010 : 0;
					PORTQ::PMR.B2 = ena;
					break;
				default:
					break;
				}
			default:
				ret = false;
				break;
			}
			return ret;
		}


		static bool sci12_(CHANNEL ch, bool ena, ORDER odr) noexcept
		{
			bool ret = true;
			uint8_t sel = ena ? 0b001100 : 0;
			switch(ch) {
			case CHANNEL::CTS:
			case CHANNEL::RTS:
				// PE3
				switch(odr) {
				case ORDER::FIRST:
					PORTE::PMR.B3 = 0;
					MPC::PE3PFS.PSEL = sel;
					PORTE::PMR.B3 = ena;
					break;
				default:
					ret = false;
					break;
				}
				break;
			case CHANNEL::RXD:
				// PE2
				switch(odr) {
				case ORDER::FIRST:
					PORTE::PMR.B2 = 0;
					MPC::PE2PFS.PSEL = sel;
					PORTE::PMR.B2 = ena;
					break;
				default:
					break;
				}
			case CHANNEL::SCK:
				// PE0
				switch(odr) {
				case ORDER::FIRST:
					PORTE::PMR.B0 = 0;
					MPC::PE0PFS.PSEL = sel;
					PORTE::PMR.B0 = ena;
					break;
				default:
					break;
				}
			case CHANNEL::TXD:
				// PE1
				switch(odr) {
				case ORDER::FIRST:
					PORTE::PMR.B1 = 0;
					MPC::PE1PFS.PSEL = sel;
					PORTE::PMR.B1 = ena;
					break;
				default:
					break;
				}
			default:
				ret = false;
				break;
			}
			return ret;
		}

	public:
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  SCIx、チャネル別ポート切り替え
			@param[in]	per		周辺機器タイプ型（SCIx）
			@param[in]	ch		チャネル（CTS、RTS、RXD、TXD、SCK）
			@param[in]	ena		無効にする場合「false」
			@param[in]	order	候補を選択する場合（指定が無い場合「FIRST」）
			@return 無効な周辺機器の場合「false」
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		static bool turn(peripheral per, CHANNEL ch, bool ena = true, ORDER order = ORDER::FIRST) noexcept
		{
			if(order == ORDER::BYPASS) return true;

			MPC::PWPR.B0WI  = 0;	// PWPR 書き込み許可
			MPC::PWPR.PFSWE = 1;	// PxxPFS 書き込み許可

			bool ret = false;
			switch(per) {
			case peripheral::SCI0:
				ret = sci0_(ch, ena, order);
				break;
			case peripheral::SCI1:
				ret = sci1_(ch, ena, order);
				break;
			case peripheral::SCI2:
				ret = sci2_(ch, ena, order);
				break;
			case peripheral::SCI3:
				ret = sci3_(ch, ena, order);
				break;
			case peripheral::SCI4:
				ret = sci4_(ch, ena, order);
				break;
			case peripheral::SCI5:
				ret = sci5_(ch, ena, order);
				break;
			case peripheral::SCI6:
				ret = sci6_(ch, ena, order);
				break;
			case peripheral::SCI7:
				ret = sci7_(ch, ena, order);
				break;
			case peripheral::SCI8:
				ret = sci8_(ch, ena, order);
				break;
			case peripheral::SCI9:
				ret = sci9_(ch, ena, order);
				break;
			case peripheral::SCI10:
				ret = sci10_(ch, ena, order);
				break;
			case peripheral::SCI11:
				ret = sci11_(ch, ena, order);
				break;
			case peripheral::SCI12:
				ret = sci12_(ch, ena, order);
				break;
			default:
				break;
			}

			MPC::PWPR = MPC::PWPR.B0WI.b();

			return ret;
		}
	};
}
