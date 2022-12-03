#pragma once
//=====================================================================//
/*!	@file
	@brief	RX600 グループ・8 ビットタイマ（TMR）定義
    @author 平松邦仁 (hira@rvf-rc45.net)
	@copyright	Copyright (C) 2016, 2022 Kunihito Hiramatsu @n
				Released under the MIT license @n
				https://github.com/hirakuni45/RX/blob/master/LICENSE
*/
//=====================================================================//
#include "common/device.hpp"

namespace device {

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	/*!
		@brief  8 ビットタイマ（TMR）
		@param[in]	base	ベースアドレス
		@param[in]	per		ペリフェラル型
		@param[in]	INT		割り込み型
		@param[in]	cmia	CMIA 型割り込みベクター
		@param[in]	cmib	CMIB 型割り込みベクター
		@param[in]	ovi		OVI 型割り込みベクター
		@param[in]	clk		駆動クロック
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	template <uint32_t base, peripheral per, typename INT, INT cmia, INT cmib, INT ovi, uint32_t clk>
	struct tmr_t {

		static constexpr auto PERIPHERAL = per;	///< ペリフェラル型
		static constexpr auto CMIA_VEC = cmia;	///< CMIA 割り込みベクタ
		static constexpr auto CMIB_VEC = cmib;	///< CMIB 割り込みベクタ
		static constexpr auto OVI_VEC  = ovi;	///< OVI 割り込みベクタ
		static constexpr auto PCLK = clk;		///< PCLK 周波数

		//-----------------------------------------------------------------//
		/*!
			@brie	タイマカウンタ（TCNT）
		*/
		//-----------------------------------------------------------------//
		typedef rw8_t<base + 0x08> TCNT_;
		static  TCNT_ TCNT;


		//-----------------------------------------------------------------//
		/*!
			@brie	タイムコンスタントレジスタ A（TCORA）
		*/
		//-----------------------------------------------------------------//
		typedef rw8_t<base + 0x04> TCORA_;
		static  TCORA_ TCORA;


		//-----------------------------------------------------------------//
		/*!
			@brie	タイムコンスタントレジスタ B（TCORB）
		*/
		//-----------------------------------------------------------------//
		typedef rw8_t<base + 0x06> TCORB_;
		static  TCORB_ TCORB;


		//-----------------------------------------------------------------//
		/*!
			@brief	タイマコントロールレジスタ（TCR）
			@param[in]	ofs	アドレス
		*/
		//-----------------------------------------------------------------//
		template <uint32_t ofs>
		struct tcr_t : public rw8_t<ofs> {
			typedef rw8_t<ofs> io_;
			using io_::operator =;
			using io_::operator ();
			using io_::operator |=;
			using io_::operator &=;

			bits_rw_t<io_, bitpos::B3, 2>  CCLR;
			bit_rw_t <io_, bitpos::B5>     OVIE;
			bit_rw_t <io_, bitpos::B6>     CMIEA;
			bit_rw_t <io_, bitpos::B7>     CMIEB;
		};
		typedef tcr_t<base + 0x00> TCR_;
		static  TCR_ TCR;


		//-----------------------------------------------------------------//
		/*!
			@brief	タイマカウンタコントロールレジスタ（TCCR）
			@param[in]	ofs	アドレス
		*/
		//-----------------------------------------------------------------//
		template <uint32_t ofs>
		struct tccr_t : public rw8_t<ofs> {
			typedef rw8_t<ofs> io_;
			using io_::operator =;
			using io_::operator ();
			using io_::operator |=;
			using io_::operator &=;

			bits_rw_t<io_, bitpos::B0, 3>  CKS;
			bits_rw_t<io_, bitpos::B3, 2>  CSS;
			bit_rw_t <io_, bitpos::B7>     TMRIS;
		};
		typedef tccr_t<base + 0x0A> TCCR_;
		static  TCCR_ TCCR;
	};
	template <uint32_t base, peripheral per, typename INT, INT cmia, INT cmib, INT ovi, uint32_t clk>
		typename tmr_t<base, per, INT, cmia, cmib, ovi, clk>::TCNT_ tmr_t<base, per, INT, cmia, cmib, ovi, clk>::TCNT;
	template <uint32_t base, peripheral per, typename INT, INT cmia, INT cmib, INT ovi, uint32_t clk>
		typename tmr_t<base, per, INT, cmia, cmib, ovi, clk>::TCORA_ tmr_t<base, per, INT, cmia, cmib, ovi, clk>::TCORA;
	template <uint32_t base, peripheral per, typename INT, INT cmia, INT cmib, INT ovi, uint32_t clk>
		typename tmr_t<base, per, INT, cmia, cmib, ovi, clk>::TCORB_ tmr_t<base, per, INT, cmia, cmib, ovi, clk>::TCORB;
	template <uint32_t base, peripheral per, typename INT, INT cmia, INT cmib, INT ovi, uint32_t clk>
		typename tmr_t<base, per, INT, cmia, cmib, ovi, clk>::TCR_ tmr_t<base, per, INT, cmia, cmib, ovi, clk>::TCR;
	template <uint32_t base, peripheral per, typename INT, INT cmia, INT cmib, INT ovi, uint32_t clk>
		typename tmr_t<base, per, INT, cmia, cmib, ovi, clk>::TCCR_ tmr_t<base, per, INT, cmia, cmib, ovi, clk>::TCCR;


	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	/*!
		@brief  8 ビットタイマ（TMR）偶数版
		@param[in]	base	ベースアドレス
		@param[in]	per		ペリフェラル型
		@param[in]	INT		割り込み型
		@param[in]	cmia	CMIA 型割り込みベクター
		@param[in]	cmib	CMIB 型割り込みベクター
		@param[in]	ovi		OVI 型割り込みベクター
		@param[in]	clk		駆動クロック
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	template <uint32_t base, peripheral per, typename INT, INT cmia, INT cmib, INT ovi, uint32_t clk>
	struct tmr0246_t : public tmr_t<base, per, INT, cmia, cmib, ovi, clk> {

		//-----------------------------------------------------------------//
		/*!
			@brie	タイマカウンタ（TCNTW）
		*/
		//-----------------------------------------------------------------//
		typedef rw16_t<base + 0x08> TCNT16_;
		static  TCNT16_ TCNT16;


		//-----------------------------------------------------------------//
		/*!
			@brie	タイムコンスタントレジスタ A（TCORA）
		*/
		//-----------------------------------------------------------------//
		typedef rw16_t<base + 0x04> TCORA16_;
		static  TCORA16_ TCORA16;


		//-----------------------------------------------------------------//
		/*!
			@brie	タイムコンスタントレジスタ B（TCORB）
		*/
		//-----------------------------------------------------------------//
		typedef rw16_t<base + 0x06> TCORB16_;
		static  TCORB16_ TCORB16;


		//-----------------------------------------------------------------//
		/*!
			@brief	タイマコントロール／ステータスレジスタ（TCSR）
			@param[in]	ofs	アドレス
		*/
		//-----------------------------------------------------------------//
		template <uint32_t ofs>
		struct tcsr_t : public rw8_t<ofs> {
			typedef rw8_t<ofs> io_;
			using io_::operator =;
			using io_::operator ();
			using io_::operator |=;
			using io_::operator &=;

			bits_rw_t<io_, bitpos::B0, 2>  OSA;
			bits_rw_t<io_, bitpos::B2, 2>  OSB;
			bit_rw_t <io_, bitpos::B4>     ADTE;
		};
		typedef tcsr_t<base + 0x02> TCSR_;
		static  TCSR_ TCSR;
	};
	template <uint32_t base, peripheral per, typename INT, INT cmia, INT cmib, INT ovi, uint32_t clk>
		typename tmr0246_t<base, per, INT, cmia, cmib, ovi, clk>::TCNT16_ tmr0246_t<base, per, INT, cmia, cmib, ovi, clk>::TCNT16;
	template <uint32_t base, peripheral per, typename INT, INT cmia, INT cmib, INT ovi, uint32_t clk>
		typename tmr0246_t<base, per, INT, cmia, cmib, ovi, clk>::TCORA16_ tmr0246_t<base, per, INT, cmia, cmib, ovi, clk>::TCORA16;
	template <uint32_t base, peripheral per, typename INT, INT cmia, INT cmib, INT ovi, uint32_t clk>
		typename tmr0246_t<base, per, INT, cmia, cmib, ovi, clk>::TCORB16_ tmr0246_t<base, per, INT, cmia, cmib, ovi, clk>::TCORB16;
	template <uint32_t base, peripheral per, typename INT, INT cmia, INT cmib, INT ovi, uint32_t clk>
		typename tmr0246_t<base, per, INT, cmia, cmib, ovi, clk>::TCSR_ tmr0246_t<base, per, INT, cmia, cmib, ovi, clk>::TCSR;


	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	/*!
		@brief  8 ビットタイマ（TMR）奇数版
		@param[in]	base	ベースアドレス
		@param[in]	per		ペリフェラル型
		@param[in]	INT		割り込み型
		@param[in]	cmia	CMIA 型割り込みベクター
		@param[in]	cmib	CMIB 型割り込みベクター
		@param[in]	ovi		OVI 型割り込みベクター
		@param[in]	clk		駆動クロック
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	template <uint32_t base, peripheral per, typename INT, INT cmia, INT cmib, INT ovi, uint32_t clk>
	struct tmr1357_t : public tmr_t<base, per, INT, cmia, cmib, ovi, clk> {

		//-----------------------------------------------------------------//
		/*!
			@brief	タイマコントロール／ステータスレジスタ（TCSR）
			@param[in]	ofs	アドレス
		*/
		//-----------------------------------------------------------------//
		template <uint32_t ofs>
		struct tcsr_t : public rw8_t<ofs> {
			typedef rw8_t<ofs> io_;
			using io_::operator =;
			using io_::operator ();
			using io_::operator |=;
			using io_::operator &=;

			bits_rw_t<io_, bitpos::B0, 2>  OSA;
			bits_rw_t<io_, bitpos::B2, 2>  OSB;
		};
		typedef tcsr_t<base + 0x02> TCSR_;
		static  TCSR_ TCSR;
	};
	template <uint32_t base, peripheral per, typename INT, INT cmia, INT cmib, INT ovi, uint32_t clk>
		typename tmr1357_t<base, per, INT, cmia, cmib, ovi, clk>::TCSR_ tmr1357_t<base, per, INT, cmia, cmib, ovi, clk>::TCSR;

#if defined(SIG_RX220) 
	typedef tmr0246_t<0x0008'8200, peripheral::TMR0, ICU::VECTOR,
		ICU::VECTOR::CMIA0, ICU::VECTOR::CMIB0, ICU::VECTOR::OVI0, clock_profile::PCLKB> TMR0;
	typedef tmr1357_t<0x0008'8201, peripheral::TMR1, ICU::VECTOR,
		ICU::VECTOR::CMIA1, ICU::VECTOR::CMIB1, ICU::VECTOR::OVI1, clock_profile::PCLKB> TMR1;
	typedef tmr0246_t<0x0008'8210, peripheral::TMR2, ICU::VECTOR,
		ICU::VECTOR::CMIA2, ICU::VECTOR::CMIB2, ICU::VECTOR::OVI2, clock_profile::PCLKB> TMR2;
	typedef tmr1357_t<0x0008'8211, peripheral::TMR3, ICU::VECTOR,
		ICU::VECTOR::CMIA3, ICU::VECTOR::CMIB3, ICU::VECTOR::OVI3, clock_profile::PCLKB> TMR3;
#elif defined(SIG_RX621) || defined(SIG_RX62N)
	typedef tmr0246_t<0x0008'8200, peripheral::TMR0, ICU::VECTOR,
		ICU::VECTOR::CMIA0, ICU::VECTOR::CMIB0, ICU::VECTOR::OVI0, clock_profile::PCLK> TMR0;
	typedef tmr1357_t<0x0008'8201, peripheral::TMR1, ICU::VECTOR,
		ICU::VECTOR::CMIA1, ICU::VECTOR::CMIB1, ICU::VECTOR::OVI1, clock_profile::PCLK> TMR1;
	typedef tmr0246_t<0x0008'8210, peripheral::TMR2, ICU::VECTOR,
		ICU::VECTOR::CMIA2, ICU::VECTOR::CMIB2, ICU::VECTOR::OVI2, clock_profile::PCLK> TMR2;
	typedef tmr1357_t<0x0008'8211, peripheral::TMR3, ICU::VECTOR,
		ICU::VECTOR::CMIA3, ICU::VECTOR::CMIB3, ICU::VECTOR::OVI3, clock_profile::PCLK> TMR3;
#elif defined(SIG_RX64M) || defined(SIG_RX71M) || defined(SIG_RX65N) || defined(SIG_RX72M) || defined(SIG_RX72N)
	typedef tmr0246_t<0x0008'8200, peripheral::TMR0, ICU::VECTOR_SELB,
		ICU::VECTOR_SELB::CMIA0, ICU::VECTOR_SELB::CMIB0, ICU::VECTOR_SELB::OVI0, clock_profile::PCLKB> TMR0;
	typedef tmr1357_t<0x0008'8201, peripheral::TMR1, ICU::VECTOR_SELB,
		ICU::VECTOR_SELB::CMIA1, ICU::VECTOR_SELB::CMIB1, ICU::VECTOR_SELB::OVI1, clock_profile::PCLKB> TMR1;
	typedef tmr0246_t<0x0008'8210, peripheral::TMR2, ICU::VECTOR_SELB,
		ICU::VECTOR_SELB::CMIA2, ICU::VECTOR_SELB::CMIB2, ICU::VECTOR_SELB::OVI2, clock_profile::PCLKB> TMR2;
	typedef tmr1357_t<0x0008'8211, peripheral::TMR3, ICU::VECTOR_SELB,
		ICU::VECTOR_SELB::CMIA3, ICU::VECTOR_SELB::CMIB3, ICU::VECTOR_SELB::OVI3, clock_profile::PCLKB> TMR3;
#elif defined(SIG_RX24T) || defined(SIG_RX66T) || defined(SIG_RX72T)
	typedef tmr0246_t<0x0008'8200, peripheral::TMR0, ICU::VECTOR,
		ICU::VECTOR::CMIA0, ICU::VECTOR::CMIB0, ICU::VECTOR::OVI0, clock_profile::PCLKB> TMR0;
	typedef tmr1357_t<0x0008'8201, peripheral::TMR1, ICU::VECTOR,
		ICU::VECTOR::CMIA1, ICU::VECTOR::CMIB1, ICU::VECTOR::OVI1, clock_profile::PCLKB> TMR1;
	typedef tmr0246_t<0x0008'8210, peripheral::TMR2, ICU::VECTOR,
		ICU::VECTOR::CMIA2, ICU::VECTOR::CMIB2, ICU::VECTOR::OVI2, clock_profile::PCLKB> TMR2;
	typedef tmr1357_t<0x0008'8211, peripheral::TMR3, ICU::VECTOR,
		ICU::VECTOR::CMIA3, ICU::VECTOR::CMIB3, ICU::VECTOR::OVI3, clock_profile::PCLKB> TMR3;
	typedef tmr0246_t<0x0008'8220, peripheral::TMR4, ICU::VECTOR,
		ICU::VECTOR::CMIA4, ICU::VECTOR::CMIB4, ICU::VECTOR::OVI4, clock_profile::PCLKB> TMR4;
	typedef tmr1357_t<0x0008'8221, peripheral::TMR5, ICU::VECTOR,
		ICU::VECTOR::CMIA5, ICU::VECTOR::CMIB5, ICU::VECTOR::OVI5, clock_profile::PCLKB> TMR5;
	typedef tmr0246_t<0x0008'8230, peripheral::TMR6, ICU::VECTOR,
		ICU::VECTOR::CMIA6, ICU::VECTOR::CMIB6, ICU::VECTOR::OVI6, clock_profile::PCLKB> TMR6;
	typedef tmr1357_t<0x0008'8231, peripheral::TMR7, ICU::VECTOR,
		ICU::VECTOR::CMIA7, ICU::VECTOR::CMIB7, ICU::VECTOR::OVI7, clock_profile::PCLKB> TMR7;
#endif
}
