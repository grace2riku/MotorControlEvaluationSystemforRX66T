#pragma once
//=====================================================================//
/*!	@file
	@brief	RX64M/RX71M グループ・割り込みマネージャー
    @author 平松邦仁 (hira@rvf-rc45.net)
	@copyright	Copyright (C) 2016, 2022 Kunihito Hiramatsu @n
				Released under the MIT license @n
				https://github.com/hirakuni45/RX/blob/master/LICENSE
*/
//=====================================================================//
#include "common/device.hpp"
#include "RX600/icu_utils.hpp"

namespace device {

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	/*!
		@brief  割り込みマネージャー・クラス
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	template <class _>
	class icu_mgr_ {

		typedef icu_utils::dispatch<ICU::VECTOR_BE0> GROUPBE0_dispatch_t;
		typedef icu_utils::dispatch<ICU::VECTOR_BL0> GROUPBL0_dispatch_t;
		typedef icu_utils::dispatch<ICU::VECTOR_BL1> GROUPBL1_dispatch_t;
		typedef icu_utils::dispatch<ICU::VECTOR_AL0> GROUPAL0_dispatch_t;
		typedef icu_utils::dispatch<ICU::VECTOR_AL1> GROUPAL1_dispatch_t;

		static GROUPBE0_dispatch_t GROUPBE0_dispatch_;
		static GROUPBL0_dispatch_t GROUPBL0_dispatch_;
		static GROUPBL1_dispatch_t GROUPBL1_dispatch_;
		static GROUPAL0_dispatch_t GROUPAL0_dispatch_;
		static GROUPAL1_dispatch_t GROUPAL1_dispatch_;

	public:
		//-----------------------------------------------------------------//
		/*!
			@brief  割り込みタスクを設定
			@param[in]	vec		割り込み要因
			@param[in]	task	割り込みタスク
		*/
		//-----------------------------------------------------------------//
		static void set_task(ICU::VECTOR vec, icu_utils::ITASK task) noexcept {
			set_interrupt_task(task, static_cast<uint32_t>(vec));
		}


		//-----------------------------------------------------------------//
		/*!
			@brief  割り込みレベルを設定する
			@param[in]	vec	通常割り込みベクター型
			@param[in]	lvl	割り込みレベル（０の場合、割り込み禁止）
		*/
		//-----------------------------------------------------------------//
		static void set_level(ICU::VECTOR vec, uint8_t lvl) noexcept
		{
			bool ena = lvl != 0 ? true : false;
			ICU::IER.enable(vec, 0);
			ICU::IPR[vec] = lvl;
			ICU::IER.enable(vec, ena);
		}


		//-----------------------------------------------------------------//
		/*!
			@brief  割り込みレベルを取得する
			@param[in]	vec	割り込み要因
			@return 割り込みレベル
		*/
		//-----------------------------------------------------------------//
		static uint8_t get_level(ICU::VECTOR vec) noexcept { return ICU::IPR[vec]; }


		//-----------------------------------------------------------------//
		/*!
			@brief  割り込み設定（通常ベクター）
			@param[in]	vec		割り込み要因
			@param[in]	task	割り込みタスク
			@param[in]	lvl	割り込みレベル（０の場合、割り込み禁止）
			@return ベクター番号
		*/
		//-----------------------------------------------------------------//
		static ICU::VECTOR set_interrupt(ICU::VECTOR vec, icu_utils::ITASK task, uint8_t lvl) noexcept {
			set_task(vec, task);
			set_level(vec, lvl);
			return vec;
		}


		//-----------------------------------------------------------------//
		/*!
			@brief  割り込み設定（選択Ａベクター）
			@param[in]	sel		割り込み要因
			@param[in]	task	割り込みタスク
			@param[in]	lvl	割り込みレベル（０の場合、割り込み禁止）
			@return 成功なら「true」
		*/
		//-----------------------------------------------------------------//
		static ICU::VECTOR set_interrupt(ICU::VECTOR_SELA sel, icu_utils::ITASK task, uint8_t lvl) noexcept
		{
			return icu_utils::set_interruptSELA<ICU, ICU::VECTOR_SELA, icu_utils::ITASK, 208, 255>(sel, task, lvl);
		}


		//-----------------------------------------------------------------//
		/*!
			@brief  割り込み設定（選択Ｂベクター）
			@param[in]	sel		割り込み要因
			@param[in]	task	割り込みタスク
			@param[in]	lvl	割り込みレベル（０の場合、割り込み禁止）
			@return 成功なら「true」
		*/
		//-----------------------------------------------------------------//
		static ICU::VECTOR set_interrupt(ICU::VECTOR_SELB sel, icu_utils::ITASK task, uint8_t lvl) noexcept
		{
			return icu_utils::set_interruptSELB<ICU, ICU::VECTOR_SELB, icu_utils::ITASK, 144, 207>(sel, task, lvl);
		}


		//-----------------------------------------------------------------//
		/*!
			@brief  DMAC 要因の設定
			@param[in]	dma_per	DMAC ペリフェラル
			@param[in]	target	DMA 要因のベクター番号
			@return 成功なら「true」
		*/
		//-----------------------------------------------------------------//
		static bool set_dmac(peripheral dma_per, ICU::VECTOR target) noexcept
		{
			switch(dma_per) {
			case peripheral::DMAC0:
				ICU::DMRSR0 = static_cast<uint8_t>(target);
				break;
			case peripheral::DMAC1:
				ICU::DMRSR1 = static_cast<uint8_t>(target);
				break;
			case peripheral::DMAC2:
				ICU::DMRSR2 = static_cast<uint8_t>(target);
				break;
			case peripheral::DMAC3:
				ICU::DMRSR3 = static_cast<uint8_t>(target);
				break;
			case peripheral::DMAC4:
				ICU::DMRSR4 = static_cast<uint8_t>(target);
				break;
			case peripheral::DMAC5:
				ICU::DMRSR5 = static_cast<uint8_t>(target);
				break;
			case peripheral::DMAC6:
				ICU::DMRSR6 = static_cast<uint8_t>(target);
				break;
			case peripheral::DMAC7:
				ICU::DMRSR7 = static_cast<uint8_t>(target);
				break;
			default:
				return false;
				break;
			}
			return true;
		}


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  通常割り込みに対するベクタを返す（ダミー）
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		static ICU::VECTOR get_group_vector(ICU::VECTOR vec) noexcept {
			return ICU::VECTOR::NONE;
		}


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  グループ割り込み BE0 に対するベクタを返す
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		static ICU::VECTOR get_group_vector(ICU::VECTOR_BE0 vec) noexcept {
			return ICU::VECTOR::GROUPBE0;
		}


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  グループ割り込み BL0 に対するベクタを返す
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		static ICU::VECTOR get_group_vector(ICU::VECTOR_BL0 vec) noexcept {
			return ICU::VECTOR::GROUPBL0;
		}


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  グループ割り込み BL1 に対するベクタを返す
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		static ICU::VECTOR get_group_vector(ICU::VECTOR_BL1 vec) noexcept {
			return ICU::VECTOR::GROUPBL1;
		}


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  グループ割り込み AL0 に対するベクタを返す
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		static ICU::VECTOR get_group_vector(ICU::VECTOR_AL0 vec) noexcept {
			return ICU::VECTOR::GROUPAL0;
		}


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  グループ割り込み AL1 に対するベクタを返す
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		static ICU::VECTOR get_group_vector(ICU::VECTOR_AL1 vec) noexcept {
			return ICU::VECTOR::GROUPAL1;
		}


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  グループ割り込み・ハンドラ GROUPBE0（エッジ割り込み）
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		static INTERRUPT_FUNC void group_be0_handler_() noexcept
		{
			GROUPBE0_dispatch_.run<ICU::GCRBE0_>(ICU::GCRBE0, ICU::GRPBE0());
		}


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  グループ割り込み・ハンドラ GROUPBL0（レベル割り込み）
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		static INTERRUPT_FUNC void group_bl0_handler_() noexcept
		{
			GROUPBL0_dispatch_.run(ICU::GRPBL0());
		}


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  グループ割り込み・ハンドラ GROUPBL1（レベル割り込み）
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		static INTERRUPT_FUNC void group_bl1_handler_() noexcept
		{
			GROUPBL1_dispatch_.run(ICU::GRPBL1());
		}


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  グループ割り込み・ハンドラ GROUPAL0（レベル割り込み）
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		static INTERRUPT_FUNC void group_al0_handler_() noexcept
		{
			GROUPAL0_dispatch_.run(ICU::GRPAL0());
		}


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  グループ割り込み・ハンドラ GROUPAL1（レベル割り込み）
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		static INTERRUPT_FUNC void group_al1_handler_() noexcept
		{
			GROUPAL1_dispatch_.run(ICU::GRPAL1());
		}


		//-----------------------------------------------------------------//
		/*!
			@brief  グループ割り込み設定のダミー
			@param[in]	vec		割り込みベクター型
			@param[in]	task	割り込みタスク（※nullptr なら無効）
			@return グループ割り込み以外なら「false」
		*/
		//-----------------------------------------------------------------//
		static bool install_group_task(ICU::VECTOR vec, icu_utils::GTASK task) noexcept
		{
			return false;
		}


		//-----------------------------------------------------------------//
		/*!
			@brief  GROUPBE0 割り込みタスクを登録する @n
					※ここで登録するタスクは「割り込みアトリビュート」無しの関数を登録する事
			@param[in]	grpv	グループベクター型
			@param[in]	task	割り込みタスク（nullptr なら不許可にする）
		*/
		//-----------------------------------------------------------------//
		static void install_group_task(ICU::VECTOR_BE0 grpv, icu_utils::GTASK task) noexcept
		{
			ICU::GENBE0.set(grpv, false);
			set_task(get_group_vector(grpv), group_be0_handler_);
			GROUPBE0_dispatch_.set_task(grpv, task);
			if(task != nullptr) {
				ICU::GENBE0.set(grpv);
			}
		}


		//-----------------------------------------------------------------//
		/*!
			@brief  GROUPBL0 割り込みタスクを登録する @n
					※ここで登録するタスクは「割り込みアトリビュート」無しの関数を登録する事
			@param[in]	grpv	グループベクター型
			@param[in]	task	割り込みタスク（nullptr なら不許可にする）
		*/
		//-----------------------------------------------------------------//
		static void install_group_task(ICU::VECTOR_BL0 grpv, icu_utils::GTASK task) noexcept
		{
			ICU::GENBL0.set(grpv, false);
			set_task(get_group_vector(grpv), group_bl0_handler_);
			GROUPBL0_dispatch_.set_task(grpv, task);
			if(task != nullptr) {
				ICU::GENBL0.set(grpv);
			}
		}


		//-----------------------------------------------------------------//
		/*!
			@brief  GROUPBL1 割り込みタスクを登録する @n
					※ここで登録するタスクは「割り込みアトリビュート」無しの関数を登録する事
			@param[in]	grpv	グループベクター型
			@param[in]	task	割り込みタスク（nullptr なら不許可にする）
		*/
		//-----------------------------------------------------------------//
		static void install_group_task(ICU::VECTOR_BL1 grpv, icu_utils::GTASK task) noexcept
		{
			ICU::GENBL1.set(grpv, false);
			set_task(get_group_vector(grpv), group_bl1_handler_);
			GROUPBL1_dispatch_.set_task(grpv, task);
			if(task != nullptr) {
				ICU::GENBL1.set(grpv);
			}
		}


		//-----------------------------------------------------------------//
		/*!
			@brief  GROUPAL0 割り込みタスクを登録する @n
					※ここで登録するタスクは「割り込みアトリビュート」無しの関数を登録する事
			@param[in]	grpv	グループベクター型
			@param[in]	task	割り込みタスク（nullptr なら不許可にする）
			@return グループ割り込み以外なら「false」
		*/
		//-----------------------------------------------------------------//
		static void install_group_task(ICU::VECTOR_AL0 grpv, icu_utils::GTASK task) noexcept
		{
			ICU::GENAL0.set(grpv, false);
			set_task(get_group_vector(grpv), group_al0_handler_);
			GROUPAL0_dispatch_.set_task(grpv, task);
			if(task != nullptr) {
				ICU::GENAL0.set(grpv);
			}
		}


		//-----------------------------------------------------------------//
		/*!
			@brief  GROUPAL1 割り込みタスクを登録する @n
					※ここで登録するタスクは「割り込みアトリビュート」無しの関数を登録する事
			@param[in]	grpv	グループベクター型
			@param[in]	task	割り込みタスク（nullptr なら不許可にする）
		*/
		//-----------------------------------------------------------------//
		static void install_group_task(ICU::VECTOR_AL1 grpv, icu_utils::GTASK task) noexcept
		{
			ICU::GENAL1.set(grpv, false);
			set_task(get_group_vector(grpv), group_al1_handler_);
			GROUPAL1_dispatch_.set_task(grpv, task);
			if(task != nullptr) {
				ICU::GENAL1.set(grpv);
			}
		}


		//-----------------------------------------------------------------//
		/*!
			@brief  割り込み設定（グループ BE0）
			@param[in]	sel		割り込み要因
			@param[in]	task	割り込みタスク @n
								※ここで登録するタスクは「割り込みアトリビュート」無しの関数を登録する事
			@param[in]	lvl		割り込みレベル @n
								※グループ割り込みレベルが、設定レベルより高い場合に設定される。
			@return ベクター番号
		*/
		//-----------------------------------------------------------------//
		static ICU::VECTOR set_interrupt(ICU::VECTOR_BE0 sel, icu_utils::GTASK task, uint8_t lvl) noexcept
		{
			install_group_task(sel, task);
			if(get_level(ICU::VECTOR::GROUPBE0) < lvl) {
				set_level(ICU::VECTOR::GROUPBE0, lvl);
			}
			return ICU::VECTOR::GROUPBE0;
		}


		//-----------------------------------------------------------------//
		/*!
			@brief  割り込み設定（グループ BL0）
			@param[in]	sel		割り込み要因
			@param[in]	task	割り込みタスク @n
								※ここで登録するタスクは「割り込みアトリビュート」無しの関数を登録する事
			@param[in]	lvl		割り込みレベル @n
								※グループ割り込みレベルが、設定レベルより高い場合に設定される。
			@return ベクター番号
		*/
		//-----------------------------------------------------------------//
		static ICU::VECTOR set_interrupt(ICU::VECTOR_BL0 sel, icu_utils::GTASK task, uint8_t lvl) noexcept
		{
			install_group_task(sel, task);
			if(get_level(ICU::VECTOR::GROUPBL0) < lvl) {
				set_level(ICU::VECTOR::GROUPBL0, lvl);
			}
			return ICU::VECTOR::GROUPBL0;
		}


		//-----------------------------------------------------------------//
		/*!
			@brief  割り込み設定（グループ BL1）
			@param[in]	sel		割り込み要因
			@param[in]	task	割り込みタスク @n
								※ここで登録するタスクは「割り込みアトリビュート」無しの関数を登録する事
			@param[in]	lvl		割り込みレベル @n
								※グループ割り込みレベルが、設定レベルより高い場合に設定される。
			@return ベクター番号
		*/
		//-----------------------------------------------------------------//
		static ICU::VECTOR set_interrupt(ICU::VECTOR_BL1 sel, icu_utils::GTASK task, uint8_t lvl) noexcept
		{
			install_group_task(sel, task);
			if(get_level(ICU::VECTOR::GROUPBL1) < lvl) {
				set_level(ICU::VECTOR::GROUPBL1, lvl);
			}
			return ICU::VECTOR::GROUPBL1;
		}


		//-----------------------------------------------------------------//
		/*!
			@brief  割り込み設定（グループ AL0）
			@param[in]	sel		割り込み要因
			@param[in]	task	割り込みタスク @n
								※ここで登録するタスクは「割り込みアトリビュート」無しの関数を登録する事
			@param[in]	lvl		割り込みレベル @n
								※グループ割り込みレベルが、設定レベルより高い場合に設定される。
			@return ベクター番号
		*/
		//-----------------------------------------------------------------//
		static ICU::VECTOR set_interrupt(ICU::VECTOR_AL0 sel, icu_utils::GTASK task, uint8_t lvl) noexcept
		{
			install_group_task(sel, task);
			if(get_level(ICU::VECTOR::GROUPAL0) < lvl) {
				set_level(ICU::VECTOR::GROUPAL0, lvl);
			}
			return ICU::VECTOR::GROUPBL0;
		}


		//-----------------------------------------------------------------//
		/*!
			@brief  割り込み設定（グループ AL1）
			@param[in]	sel		割り込み要因
			@param[in]	task	割り込みタスク @n
								※ここで登録するタスクは「割り込みアトリビュート」無しの関数を登録する事
			@param[in]	lvl		割り込みレベル @n
								※グループ割り込みレベルが、設定レベルより高い場合に設定される。
			@return ベクター番号
		*/
		//-----------------------------------------------------------------//
		static ICU::VECTOR set_interrupt(ICU::VECTOR_AL1 sel, icu_utils::GTASK task, uint8_t lvl) noexcept
		{
			install_group_task(sel, task);
			if(get_level(ICU::VECTOR::GROUPAL1) < lvl) {
				set_level(ICU::VECTOR::GROUPAL1, lvl);
			}
			return ICU::VECTOR::GROUPAL1;
		}


		//-----------------------------------------------------------------//
		/*!
			@brief  高速割り込み設定
			@param[in]	vec		割り込み要因
			@param[in]	ena		不許可にする場合「false」
		*/
		//-----------------------------------------------------------------//
		static void enable_fast_interrupt(ICU::VECTOR vec, bool ena = true) noexcept
		{
			if(ena) {
				ICU::FIR = ICU::FIR.FVCT.b(static_cast<uint8_t>(vec)) | ICU::FIR.FIEN.b();
			} else {
				ICU::FIR = 0;
			}
		}
	};
	template <class _> typename icu_mgr_<_>::GROUPBE0_dispatch_t icu_mgr_<_>::GROUPBE0_dispatch_;
	template <class _> typename icu_mgr_<_>::GROUPBL0_dispatch_t icu_mgr_<_>::GROUPBL0_dispatch_;
	template <class _> typename icu_mgr_<_>::GROUPBL1_dispatch_t icu_mgr_<_>::GROUPBL1_dispatch_;
	template <class _> typename icu_mgr_<_>::GROUPAL0_dispatch_t icu_mgr_<_>::GROUPAL0_dispatch_;
	template <class _> typename icu_mgr_<_>::GROUPAL1_dispatch_t icu_mgr_<_>::GROUPAL1_dispatch_;

	typedef icu_mgr_<void> icu_mgr;
}
