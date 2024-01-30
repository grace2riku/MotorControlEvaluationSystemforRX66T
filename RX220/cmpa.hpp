#pragma once
//=========================================================================//
/*!	@file
	@brief	RX220 グループ・CMPA 定義
    @author 平松邦仁 (hira@rvf-rc45.net)
	@copyright	Copyright (C) 2022, 2024 Kunihito Hiramatsu @n
				Released under the MIT license @n
				https://github.com/hirakuni45/RX/blob/master/LICENSE
*/
//=========================================================================//
#include "common/device.hpp"

namespace device {

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	/*!
		@brief  コンパレータ A（CMPA）
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	struct cmpa_t {

		static constexpr auto PERIPHERAL = peripheral::CMPA;	///< ペリフェラル型
		static constexpr auto LVD1 = ICU::VECTOR::LVD1;			///< 割り込みベクター
		static constexpr auto LVD2 = ICU::VECTOR::LVD2;			///< 割り込みベクター

		//-----------------------------------------------------------------//
		/*!
			@brief  電圧監視 1 回路 / コンパレータ A1 制御レジスタ 1（LVD1CR1）
			@param[in]	base	ベースアドレス
		*/
		//-----------------------------------------------------------------//
		template <uint32_t base>
		struct lvd1cr1_t : public rw8_t<base> {
			typedef rw8_t<base> io_;
			using io_::operator =;
			using io_::operator ();
			using io_::operator |=;
			using io_::operator &=;

			bits_rw_t<io_, bitpos::B0, 2>  LVD1IDTSEL;
			bit_rw_t <io_, bitpos::B2>     LVD1IRQSEL;
		};
		static inline lvd1cr1_t<0x0008'00E0> LVD1CR1;


		//-----------------------------------------------------------------//
		/*!
			@brief  電圧監視 1 回路 / コンパレータ A1 ステータスレジスタ（LVD1SR）
			@param[in]	base	ベースアドレス
		*/
		//-----------------------------------------------------------------//
		template <uint32_t base>
		struct lvd1sr_t : public rw8_t<base> {
			typedef rw8_t<base> io_;
			using io_::operator =;
			using io_::operator ();
			using io_::operator |=;
			using io_::operator &=;

			bit_rw_t <io_, bitpos::B0>     LVD1DET;
			bit_rw_t <io_, bitpos::B1>     LVD1MON;
		};
		static inline lvd1sr_t<0x0008'00E1> LVD1SR;


		//-----------------------------------------------------------------//
		/*!
			@brief  電圧監視 2 回路 / コンパレータ A2 制御レジスタ 1（LVD2CR1）
			@param[in]	base	ベースアドレス
		*/
		//-----------------------------------------------------------------//
		template <uint32_t base>
		struct lvd2cr1_t : public rw8_t<base> {
			typedef rw8_t<base> io_;
			using io_::operator =;
			using io_::operator ();
			using io_::operator |=;
			using io_::operator &=;

			bits_rw_t<io_, bitpos::B0, 2>  LVD2IDTSEL;
			bit_rw_t <io_, bitpos::B2>     LVD2IRQSEL;
		};
		static inline lvd2cr1_t<0x0008'00E2> LVD2CR1;


		//-----------------------------------------------------------------//
		/*!
			@brief  電圧監視 2 回路 / コンパレータ A2 ステータスレジスタ（LVD2SR）
			@param[in]	base	ベースアドレス
		*/
		//-----------------------------------------------------------------//
		template <uint32_t base>
		struct lvd2sr_t : public rw8_t<base> {
			typedef rw8_t<base> io_;
			using io_::operator =;
			using io_::operator ();
			using io_::operator |=;
			using io_::operator &=;

			bit_rw_t <io_, bitpos::B0>     LVD2DET;
			bit_rw_t <io_, bitpos::B1>     LVD2MON;
		};
		static inline lvd2sr_t<0x0008'00E3> LVD2SR;


		//-----------------------------------------------------------------//
		/*!
			@brief  電圧監視回路 / コンパレータ A 制御レジスタ（LVCMPCR）
			@param[in]	base	ベースアドレス
		*/
		//-----------------------------------------------------------------//
		template <uint32_t base>
		struct lvcmpcr_t : public rw8_t<base> {
			typedef rw8_t<base> io_;
			using io_::operator =;
			using io_::operator ();
			using io_::operator |=;
			using io_::operator &=;

			bit_rw_t <io_, bitpos::B0>     EXVREFINP1;
			bit_rw_t <io_, bitpos::B1>     EXVCCINP1;
			bit_rw_t <io_, bitpos::B2>     EXVREFINP2;
			bit_rw_t <io_, bitpos::B3>     EXVCCINP2;

			bit_rw_t <io_, bitpos::B5>     LVD1E;
			bit_rw_t <io_, bitpos::B6>     LVD2E;
		};
		static inline lvcmpcr_t<0x0008'C297> LVCMPCR;


		//-----------------------------------------------------------------//
		/*!
			@brief  電圧監視 1 回路 / コンパレータ A1 制御レジスタ 0（LVD1CR0）
			@param[in]	base	ベースアドレス
		*/
		//-----------------------------------------------------------------//
		template <uint32_t base>
		struct lvd1cr0_t : public rw8_t<base> {
			typedef rw8_t<base> io_;
			using io_::operator =;
			using io_::operator ();
			using io_::operator |=;
			using io_::operator &=;

			bit_rw_t <io_, bitpos::B0>     LVD1RIE;
			bit_rw_t <io_, bitpos::B1>     LVD1DFDIS;
			bit_rw_t <io_, bitpos::B2>     LVD1CMPE;

			bits_rw_t<io_, bitpos::B4, 2>  LVD1FSAMP;
			bit_rw_t <io_, bitpos::B6>     LVD1RI;
			bit_rw_t <io_, bitpos::B7>     LVD1RN;
		};
		static inline lvd1cr0_t<0x0008'C29A> LVD1CR0;


		//-----------------------------------------------------------------//
		/*!
			@brief  電圧監視 2 回路 / コンパレータ A2 制御レジスタ 0（LVD2CR0）
			@param[in]	base	ベースアドレス
		*/
		//-----------------------------------------------------------------//
		template <uint32_t base>
		struct lvd2cr0_t : public rw8_t<base> {
			typedef rw8_t<base> io_;
			using io_::operator =;
			using io_::operator ();
			using io_::operator |=;
			using io_::operator &=;

			bit_rw_t <io_, bitpos::B0>     LVD2RIE;
			bit_rw_t <io_, bitpos::B1>     LVD2DFDIS;
			bit_rw_t <io_, bitpos::B2>     LVD2CMPE;

			bits_rw_t<io_, bitpos::B4, 2>  LVD2FSAMP;
			bit_rw_t <io_, bitpos::B6>     LVD2RI;
			bit_rw_t <io_, bitpos::B7>     LVD2RN;
		};
		static inline lvd2cr0_t<0x0008'C29B> LVD2CR0;
	};
	typedef cmpa_t CMPA;
}
