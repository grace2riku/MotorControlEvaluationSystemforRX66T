#pragma once
//=========================================================================//
/*!	@file
	@brief	RX600 グループ A/D 制御
    @author 平松邦仁 (hira@rvf-rc45.net)
	@copyright	Copyright (C) 2017, 2024 Kunihito Hiramatsu @n
				Released under the MIT license @n
				https://github.com/hirakuni45/RX/blob/master/LICENSE
*/
//=========================================================================//
#include "common/renesas.hpp"
#include "common/intr_utils.hpp"
#include "common/vect.h"

namespace device {

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	/*!
		@brief  A/D 制御クラス
		@param[in]	ADCU	A/D チャネル・ユニット
		@param[in]	TASK	割り込みタスク・ファンクタ
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	template <class ADCU, class TASK = utils::null_task>
	class adc_in {
	public:

		typedef ADCU value_type;

	private:

		static inline TASK task_;

		static INTERRUPT_FUNC void adi_task_()
		{
			task_();
		}

		ICU::LEVEL	level_;

		static inline void sleep_() { asm("nop"); }

	public:
		//-----------------------------------------------------------------//
		/*!
			@brief	コンストラクター
		 */
		//-----------------------------------------------------------------//
		adc_in() noexcept : level_(ICU::LEVEL::NONE) { }


		//-----------------------------------------------------------------//
		/*!
			@brief	スタート（シングル）
			@param[in]	ana		アナログ入力チャンネル型
			@param[in]	level	割り込みレベル、０の場合はポーリング
			@return 成功なら「true」
		 */
		//-----------------------------------------------------------------//
		bool start(typename ADCU::ANALOG ana, ICU::LEVEL level = ICU::LEVEL::NONE) noexcept
		{
			level_ = level;

			// 基本変換時間クロック
//			uint32_t n = ADCU::PCLK / ADCU::IN_CONV_TIME_NS / 100;
			uint32_t n = 100;
			n += 5;
			n /= 10;
			// チャネル構成により割り切れるように調整
#if 0
			{
				auto m = n % ADCU::UNIT_NUM;
				if(m > 0) n += ADCU::UNIT_NUM - m;
			}
#endif
			if(n < 12 || n > 255) return false;

			power_mgr::turn(ADCU::PERIPHERAL);

			ADCU::enable(ana);
			ADCU::ADANSA.set(ana);
			ADCU::ADSSTR.set(ana, n);

			return true;
		}


		//-----------------------------------------------------------------//
		/*!
			@brief	スキャン開始
		 */
		//-----------------------------------------------------------------//
		void scan() noexcept
		{
			if(level_ != ICU::LEVEL::NONE) {
				icu_mgr::set_interrupt(ADCU::ADI, adi_task_, level_);
				ADCU::ADCSR = ADCU::ADCSR.ADST.b() | ADCU::ADCSR.ADIE.b();
			} else {
				ADCU::ADCSR = ADCU::ADCSR.ADST.b();
			}
		}


		//-----------------------------------------------------------------//
		/*!
			@brief	変換状態の取得
			@return 変換中なら「true」
		 */
		//-----------------------------------------------------------------//
		bool get_state() const noexcept {
			return ADCU::ADCSR.ADST();
		}


		//-----------------------------------------------------------------//
		/*!
			@brief	読み込み同期
		 */
		//-----------------------------------------------------------------//
		void sync() const noexcept {
			while(ADCU::ADCSR.ADST() != 0) sleep_();
		}


		//-----------------------------------------------------------------//
		/*!
			@brief	A/D 変換結果を取得
			@param[in]	an	変換チャネル
			@return 変換結果（上位１０ビットが有効な値）
		 */
		//-----------------------------------------------------------------//
		uint16_t get(typename ADCU::ANALOG an) const noexcept {
			return ADCU::ADDR(an);
		}
	};
}
